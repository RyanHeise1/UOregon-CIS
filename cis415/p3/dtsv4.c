/* 
 * Author: Ryan Heise
 * Usage: ./dtsv1
 *
 * Program Description: building a Distributed Time Server (DTS);
 *    develop the first version of the DTS such that it can echo 
 *    back the requests it receives from the client program supplied 
 *    in the starting archive.  DTS v1 will perform the following 
 *    steps: 
 *   1. Initialize the BXP runtime so that it can create and accept encrypted connection 
 *      requests.
 *   2. Create a thread that will receive requests from client applications
 *   3. Respond to each such request by echoing back the received request along with a 
 *      status byte; the first byte of the response is ‘1’ for success, ‘0’ for failure. 
 *      Thus, if the request is in the character array query, the response to send back 
 *      would be created by sprintf(response, "1%s", query); for this version of DTS, 
 *      you will never respond with a failure indication
 *   4. Except for initialization failure, DTSv1 should never exit
 */
#include "BXP/bxp.h" 
#include <valgrind/valgrind.h>
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

void *svcFxn(UNUSED void *args) {
    
    char *query = (char *)malloc(BUFSIZ);
    char *response = (char *)malloc(BUFSIZ);
    unsigned qlen, rlen; 
    BXPEndpoint ep; 

    while ((qlen = bxp_query(bxps, &ep, query, 10000)) > 0) { 
        // Respond to each such request by echoing back the received request 
        // along with a status byte
        sprintf(response, "1%s", query); 
        rlen = strlen(response) + 1; 
        assert(bxp_response(bxps, &ep, response, rlen)); 
    } 
    return EXIT_SUCCESS;
}
 
int main(UNUSED int argc, UNUSED char *argv[]) { 

    pthread_t svcThread; 
    int ifEncrypt = 1;
 
    // initialize BXP system and bind to port 5000
    assert(bxp_init(PORT, ifEncrypt)); 
    // offer service named echo in this process
    // assert that it was successful
    assert((bxps = bxp_offer(SERVICE))); 
    // Create a thread that will receive requests from client applications
    assert(! pthread_create(&svcThread, NULL, svcFxn, (void *)bxps));
    pthread_join(svcThread, NULL);
    // TODO Enter valgrind statement here
    VALGRIND_MONITOR_COMMAND("leak_check summary");
    return 0; 
} 
