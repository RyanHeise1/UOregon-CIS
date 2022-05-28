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
#include "ADTs/queue.h"
#include "ADTs/arraylist.h"
#include "ADTs/hashmap.h"
#include <assert.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>

#define UNUSED __attribute__((unused)) 
#define PORT 19999
#define SERVICE "DTS"
#define SHIFT 31L
#define USECS (10 * 1000)

/*    Globals    */
BXPService bxps; 
const PrioQueue *pq = NULL;
const Map *m;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//useconds_t musecs = USECS;
int localID = 0;

/*  Structs    */
typedef struct Event{
    // Name of the event
    int event; // 1 = OneShot: 2 = Repeat: 3 = Cancel 
    char *service;
    char *host;
    int intervals;
    int repeats; // if = 0 then it needs to go on FOREVER
    int local_id; 
    long unsigned int client_id; 
    unsigned int port;
    bool cancel;
}Event;

int cmp(void *p1, void *p2){
    struct timeval *s1 = (struct timeval *)p1;
    struct timeval *s2 = (struct timeval *)p2;
    int c;
    if((s1->tv_sec) < (s2->tv_sec))
        c = -1;
    else if((s1->tv_sec) > (s2->tv_sec))
        c = 1;
    else{
        if((s1->tv_usec) < (s2->tv_usec))
            c = -1;
        else if((s1->tv_usec) > (s2->tv_usec))
            c = 1;
        else
            c = 0;
    }
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

void oneShot(char **word, Event *e){
    struct timeval *tm = (struct timeval *) malloc (sizeof(struct timeval));

    e->event = 1;
    sscanf(word[1], "%lu", &(e->client_id));
    sscanf(word[2], "%ld", &(tm->tv_sec));
    sscanf(word[3], "%ld", &(tm->tv_usec));
    e->host = strdup(word[4]);
    e->service = strdup(word[5]);
    sscanf(word[6], "%u", &(e->port));

    e->intervals = 0;
    e->local_id = localID++;
    e->cancel = 0;

    pq->insert(pq, (void *)tm, (void *)e);
    m->putUnique(m, (void *)&localID, (void *)e);
}

void repeat(char **word, Event *e){    
    long secs = 0;


    struct timeval *tm = (struct timeval *) malloc (sizeof(struct timeval));
    gettimeofday(tm, NULL);

    e->event = 2;
    sscanf(word[1], "%lu", &(e->client_id));
    sscanf(word[2], "%d", &(e->intervals));
    sscanf(word[3], "%d", &(e->repeats));
    e->host = strdup(word[4]);
    e->service = strdup(word[5]);
    sscanf(word[6], "%u", &(e->port));

    e->local_id = localID++;
    e->cancel = 0;

    tm->tv_usec = e->intervals * 1000;
    while (tm->tv_usec > 1000000){
        tm->tv_usec -= 1000000;
        tm->tv_sec += 1;
    }
    tm->tv_sec = secs;

    pq->insert(pq, tm, (void *)e);
    m->putUnique(m, (void *)&e->local_id, (void *)e);
}

void cancel(char **word){
    Event *e;
    unsigned long svid;

    sscanf(word[1], "%lu", &(svid));
    m->get(m, (void *)svid, (void **)&e);
    e->cancel = 1;
}

void *timer(UNUSED void *args){
    struct timeval now;
    struct timeval *time;
    struct timeval *new_time = (struct timeval *) malloc (sizeof(struct timeval));
    
    Event *e;

    for (;;){
        const Queue *ready_queue = Queue_create(NULL);
        const Queue *repeat_queue = Queue_create(NULL);

        usleep(USECS);
        gettimeofday(&now, NULL);

        pthread_mutex_lock(&mutex);
        while(pq->min(pq, (void **)&time, (void **)&e)){
            if (cmp((void *)time, (void *)&now) <= 0){
                pq->removeMin(pq, (void **)&time, (void **)&e);
                ready_queue->enqueue(ready_queue, e);
            } else{
                break;
            }
        }
        pthread_mutex_unlock(&mutex);


        pthread_mutex_lock(&mutex);
        while(ready_queue->dequeue(ready_queue, (void **)&e)){
            printf("LENGTH: %ld\n", ready_queue->size(ready_queue));
            if((e->event == 2) && e->repeats > 0){
                printf("Event fired: %lu|%s|%s|%u\n", e->client_id, e->host, e->service, e->port);
                if (e->repeats > 0){ // if there are still repeats, move to repeat queue
                    repeat_queue->enqueue(repeat_queue, (void *)e);
                }else{ // no repeats left
                    m->remove(m, (void *)&e);
                    free(e);
                }
                e->repeats -= 1;
            }
            else if (e->cancel == 0){ // check if event is canceled, fire if not
                printf("Event fired: %lu|%s|%s|%u\n", e->client_id, e->host, e->service, e->port);
            }
            else{ // event is canceled
                m->remove(m, (void *)&e);
                free(e);
            }
        }
        pthread_mutex_unlock(&mutex);
        

        pthread_mutex_lock(&mutex);
        while(repeat_queue->dequeue(repeat_queue, (void **)&e)){
            gettimeofday(new_time, NULL);
            pq->insert(pq,(void *)new_time, (void *)&e);
            free(new_time);
        }
        pthread_mutex_unlock(&mutex);

        ready_queue->destroy(ready_queue);
        repeat_queue->destroy(repeat_queue);
    }
}

int extractWords(char *query, char **word){
    int size = 0;
    char *tmp = strtok(query, "|");
    while(tmp != NULL){
        word[size] = tmp;
        tmp = strtok(NULL, "|");
        size++;
    }
    word[size] = NULL;
    return size;
}

void *svcFxn(UNUSED void *args) {
    char *query = (char *)malloc(BUFSIZ);
    char *response = (char *)malloc(BUFSIZ);
    char *word[100];
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
        int len = extractWords(query, word);
        // fill ArrayList
                
        if ((strcmp(word[0], "OneShot") == 0) && len == 7){ //secs as priority
            sprintf(response, "1%s", query);
            oneShot(word, e);
        }else if ((strcmp(word[0], "Repeat") == 0) && len == 7){
            sprintf(response, "1%s", query);
            repeat(word, e);
        }else if((strcmp(word[0], "Cancel") == 0) && len == 2){
            sprintf(response, "1%s", query);
            cancel(word);
        }else{
            sprintf(response, "0\n");
        }

        rlen = strlen(response) + 1;
        bxp_response(bxps, &ep, response, rlen); 
    } 
    free(query);
    free(response);
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
