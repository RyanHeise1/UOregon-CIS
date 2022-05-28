/* 
 * Author: Ryan Heise
 * Usage: ./dtsv
 *
 * Does Not Work
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
const ArrayList *al = NULL;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//useconds_t musecs = USECS;
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

    pq->insert(pq, tm, (void *)e);
    m->putUnique(m, (void *)&localID, (void *)e);
    
}

void repeat(const ArrayList *al, Event *e){
    long msecs;
    long usecs;
    long secs = 0;

    struct timeval *tm = (struct timeval *) malloc (sizeof(struct timeval));

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
    struct timeval now;
    struct timeval *time;
    struct timeval *new_time = (struct timeval *) malloc (sizeof(struct timeval));
    const Queue *ready_queue = Queue_create(NULL);
    const Queue *repeat_queue = Queue_create(NULL);
    Event *e;

    for (;;){
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
        while(ready_queue->dequeue(ready_queue, (void **)&time)){
            if ((strcmp(e->event, "Cancel") == 0)){ // check if event is canceled, fire if not
                printf("Event fired: %lu|%s|%s|%u\n", e->client_id, e->host, e->service, e->port);
            }
            else if((strcmp(e->event, "Repeat") == 0) && e->repeats > 0){
                printf("Event fired: %lu|%s|%s|%u\n", e->client_id, e->host, e->service, e->port);
                if (e->repeats > 0){ // if there are still repeats, move to repeat queue
                    repeat_queue->enqueue(repeat_queue, (void *)e);
                }else{ // no repeats left
                    m->remove(m, (void *)&e);
                    free(e);
                }
                e->repeats -= 1;
            }else{ // event is canceled
                m->remove(m, (void *)&e);
                free(e);
            }
        }
        pthread_mutex_unlock(&mutex);
        

        pthread_mutex_lock(&mutex);
        while(repeat_queue->dequeue(repeat_queue, (void **)&e)){
            gettimeofday(new_time, NULL);
            pq->insert(pq,(void *)&new_time, (void *)&e);
            free(new_time);
        }
        pthread_mutex_unlock(&mutex);
    }
    ready_queue->destroy(ready_queue);
    repeat_queue->destroy(repeat_queue);
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
