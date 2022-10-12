/* 
 * Author: Ryan Heise
 * Usage: ./dtsv2
 *
 * Program Description: building a Distributed Time Server (DTS);
 *    develop the first version of the DTS such that it can echo 
 *    back the requests it receives from the client program supplied 
 *    in the starting archive.  DTSv2 will perform the following 
 *    steps: 
 *   1. will set the status byte appropriately if it is a legal request or 
 *      not. 
 *      a. OneShot|<clid>|<secs>|<usecs>|<host>|<service>|<port> 
 *      b. Repeat|<clid>|<msecs>|<repeats>|<host>|<service>|<port> 
 *      c. Cancel|<svid>
 *   2. Need to check that the first word in each request is one of the 
 *       three legal strings ("OneShot", "Repeat", "Cancel") 
 *   3. that the number of arguments is correct.
 *   4. If both are true, sprintf(response, "1%s", query); otherwise,  
 *       sprintf(response, "0");
 */
#include "BXP/bxp.h" 
#include "ADTs/stringADT.h"
#include "ADTs/arraylist.h"
#include <valgrind/valgrind.h>
#include <assert.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <pthread.h>

#define UNUSED __attribute__((unused)) 
#define PORT 19999
#define SERVICE "DTS"

/*    Globals    */
BXPService bxps; 

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
    VALGRIND_MONITOR_COMMAND("leak_check summary");
    return 0; 
} 
