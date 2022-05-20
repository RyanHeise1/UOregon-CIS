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
#include <assert.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <pthread.h>

#define UNUSED __attribute__((unused)) 
#define PORT 5000
#define SERVICE "Echo"

/*    Globals    */
BXPService bxps; 
const PrioQueue *pq = NULL;

/*  Structs    */
typedef struct Event{
    char *event;
    bool cancel;
    int intervals;
    int repeats;
    int client_id;
    int local_id;
    int service; // might not need
    int port; // might not need
}Event;

void *svcFxn(UNUSED void *args) {
    char *query = (char *)malloc(BUFSIZ);
    char *response = (char *)malloc(BUFSIZ);
    unsigned qlen, rlen; 
    int ifEncrypt = 1;
    unsigned short port = PORT;
    BXPEndpoint ep;
 
    // initialize BXP system and bind to port 19999
    assert(bxp_init(port, ifEncrypt)); 
    // offer service named echo in this process
    // assert that it was successful
    if((bxps = bxp_offer(SERVICE)) == NULL){
        printf("Error Offering bxps\n");
        free(query);
        free(response);
        exit(EXIT_FAILURE);
    } 

    while ((qlen = bxp_query(bxps, &ep, query, 10000)) > 0) { 
        int len = 0;
        // store fist word in query
        char *tmp = strtok(query, "|");
        char *word = tmp;
        // get length of arguments
        while(tmp != NULL){
            len += 1;
            tmp = strtok(NULL, "|");
        }

        if ((strcmp(word, "OneShot") == 0) && len == 7){
            sprintf(response, "1%s\n", query);
        }else if ((strcmp(word, "Repeat") == 0) && len == 7){
            sprintf(response, "1%s\n", query);
        }else if((strcmp(word, "Cancel") == 0) && len == 2){
            sprintf(response, "1%s\n", query);
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
    
    // Create a thread that will receive requests from client applications
    pthread_create(&svcThread, NULL, svcFxn, (void *)bxps);
    pthread_join(svcThread, NULL);
    // VALGRIND_MONITOR_COMMAND("leak_check summary");
    return 0; 
} 
