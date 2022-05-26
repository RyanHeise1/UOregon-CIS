/* 
 * Author: Ryan Heise
 * Usage: ./dtsv3
 *
 * Program Description: building a Distributed Time Server (DTS);
 *    develop the first version of the DTS such that it can echo 
 *    back the requests it receives from the client program supplied 
 *    in the starting archive.  DTS v3 will perform the following 
 *    steps: 
 *   1. Print out information about each event when it has 
 *      elapsed
 *       a. printf(“Event fired: %lu|%s|%s|%u\n”, <clid>, <host>, <service>, <port>); 
 *   2. Use a Protity Queue where priority is the time since the Epoch
 *       a. Then harvest all entries in the Pqueue whose time is <= current time
 *   3. When you store an event in the queue, you will associate with that event a unique 
 *      server id
 *   4. Since you cannot remove anything from a priority queue except the 
 *      minimum value, you will likely need another data structure that will permit you to find 
 *      the event data structure associated with a particular server id
 *   5. since you cannot remove it from the priority queue, you will likely need a member in the 
 *      request structure that is true if the event has been cancelled
 *   6. When your periodic timer indicates it is time to harvest requests from the priority 
 *      queue, you will likely store the event structures in a Queue. It is reasonable to process 
 *      this queue in another thread
 *   7. The return value to the client for OneShot and Repeat requests should be formatted 
 *      sprintf(response, "1%08lu", svid); if Cancel is invoked, the svid argument will also be 
 *      formatted as "%08lu".
 *
 */
#include "BXP/bxp.h" 
#include <valgrind/valgrind.h>
#include "ADTs/prioqueue.h"
#include "ADTs/arraylist.h"
#include "ADTs/hashmap.h"
#include <assert.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <pthread.h>
#include <time.h>

#define UNUSED __attribute__((unused)) 
#define PORT 19999
#define SERVICE "DTS"
#define SHIFT 31L
#define INTERVAL 10  // TODO: Does this number need to be bigger?

/*    Globals    */
BXPService bxps; 
const PrioQueue *pq = NULL;
const Map *m;
const ArrayList *al = NULL;
int localID = 0;

/*  Structs    */
typedef struct Event{
    // Name of the event
    char *event; 
    char *service;
    char *host;
    int intervals;
    int repeats; // if = 0 then it needs to go on FOREVER
    int local_id; 
    long unsigned int client_id; 
    unsigned short int port;
    bool cancel;
}Event;

int cmp(UNUSED void *p1, UNUSED void *p2){
    struct timeval *s1 = (struct timeval *)p1;
    struct timeval *s2 = (struct timeval *)p2;
    int c;
    if((s1->tv_sec) < (s2->tv_sec))
        c = -1;
    else if((s1->tv_sec) > (s2->tv_sec))
        c = 1;
    else
        c = 0;

    return c;
}

int hash_cmp(void *v1, void *v2){
    return strcmp((char *)v1, (char *)v2);
}

long hash(void *value, long N) {
    char *sp = (char *)value;
    unsigned long ans;
    for ( ; *sp != '\0'; sp++)
        ans = SHIFT * ans + (unsigned long)*sp;
    return (long)(ans % N);
}

void oneShot(const ArrayList *al, Event *e){
    struct timeval *tm = (struct timeval *) malloc (sizeof(struct timeval));

    al->get(al, 0, (void **)&e->event);
    al->get(al, 1, (void **)&e->client_id);
    al->get(al, 2, (void **)&tm->tv_sec);
    al->get(al, 3, (void **)&tm->tv_usec);
    al->get(al, 4, (void **)&e->host);
    al->get(al, 5, (void **)&e->service);
    al->get(al, 6, (void **)&e->port);

    e->intervals = 0;
    e->local_id = localID++;
    e->cancel = 0;
    
}
void repeat(const ArrayList *al, Event *e){
    struct timeval *tm = (struct timeval *) malloc (sizeof(struct timeval));
    long msecs;
    long usecs;
    long secs = 0;

    al->get(al, 2, (void **)&msecs);
    usecs = msecs * 1000;
    tm->tv_usec = usecs;
    while (usecs > 1000000){
        usecs -= 1000000;
        secs += 1;
    }
    tm->tv_sec = secs;


    al->get(al, 0, (void **)&e->event);
    al->get(al, 1, (void **)&e->client_id);
    al->get(al, 2, (void **)&e->intervals);
    al->get(al, 3, (void **)&e->repeats);
    al->get(al, 4, (void **)&e->host);
    al->get(al, 5, (void **)&e->service);
    al->get(al, 6, (void **)&e->port);

    e->local_id = localID++;
    e->cancel = 0;

    pq->insert(pq, tm, (void *)e);
    m->putUnique(m, (void *)&localID, (void *)e);
}

void cancel(const ArrayList *al){
    Event *e;
    unsigned long svid;
    al->get(al, 1, (void **)&svid);
    m->get(m, (void *)svid, (void **)&e);
    e->cancel = 1;
}

void *timer(UNUSED void *args){
    return EXIT_SUCCESS;
}

void *svcFxn(UNUSED void *args) {
    char *query = (char *)malloc(BUFSIZ);
    char *response = (char *)malloc(BUFSIZ);
    Event *e = (Event *)malloc(sizeof(Event));
    BXPEndpoint ep;
    unsigned qlen, rlen; 
    unsigned short port = PORT;
 
    // initialize BXP system and bind to port 5000
    assert(bxp_init(port, 1)); 
    // offer service named echo in this process
    // assert that it was successful
    if((bxps = bxp_offer(SERVICE)) == NULL){
        printf("Error Offering bxps\n");
        free(query);
        free(response);
        exit(EXIT_FAILURE);
    } 

    while ((qlen = bxp_query(bxps, &ep, query, 10000)) > 0) { 

        // fill ArrayList
        char *tmp = strtok(query, "|");
        while(tmp != NULL){
            printf("%s\n", tmp);
            al->add(al, tmp);
            tmp = strtok(NULL, "|");
        }
        int len = al->size(al);
        char *word;
        al->get(al, 0, (void **)&word);
        
        if ((strcmp(word, "OneShot") == 0) && len == 7){ //secs as priority
            sprintf(response, "1%s", query);
            oneShot(al, e);
        }else if ((strcmp(word, "Repeat") == 0) && len == 7){
            sprintf(response, "1%s", query);
            repeat(al, e);
        }else if((strcmp(word, "Cancel") == 0) && len == 2){
            sprintf(response, "1%s", query);
            cancel(al);
        }else{
            sprintf(response, "0\n");
        }
        al->clear(al);

        rlen = strlen(response) + 1;
        bxp_response(bxps, &ep, response, rlen); 
    } 
    free(query);
    free(response);
    al->destroy(al);
    return EXIT_SUCCESS;
}
 
int main(UNUSED int argc, UNUSED char *argv[]) { 

    pthread_t svcThread; 
    pthread_t timerThread; 

    if ((pq = PrioQueue_create(cmp, doNothing, free)) == NULL) {
        fprintf(stderr, "Unable to create priority queue\n");
        exit(EXIT_FAILURE);
    }
    if ((m = HashMap(1024L, 2.0, hash, hash_cmp, free, free)) == NULL) {
        fprintf(stderr, "Unable to create Map to hold (key, value) pairs\n");
        exit(EXIT_FAILURE);
    }
    if ((al = ArrayList_create(0L, doNothing)) == NULL){
        fprintf(stderr, "Unable to create ArrayList\n");
        exit(EXIT_FAILURE);
    }
    // Create a thread that will receive requests from client applications
    if (pthread_create(&svcThread, NULL, svcFxn, (void *)bxps) != 0){
        fprintf(stderr, "Can't start timer thread\n");
        exit(EXIT_FAILURE);
    }

    if(pthread_create(&timerThread, NULL, timer, NULL) != 0) {
        fprintf(stderr, "Can't start timer thread\n");
        exit(EXIT_FAILURE);
    }
    pthread_join(svcThread, NULL);
    pthread_join(timerThread, NULL);
    // VALGRIND_MONITOR_COMMAND("leak_check summary");
    return 0; 
} 
