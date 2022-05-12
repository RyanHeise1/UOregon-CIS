#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "ADTs/llistprioqueue.h"

#define USAGE "USAGE: %s filename\n"
#define ERROR1 "%s: ERROR - Unable to create priority que\n"
#define ERROR2 "Unable to open file: %s !\n"
#define ERROR3 "Error inserting line %ld into priorite que\n"

int cmp(void *a, void *b){
    return (*(long*)a - *(long*)b);
}

void processFile(FILE *fd, const PrioQueue *prioQ, char *filename){
    char buf[BUFSIZ], word[BUFSIZ];
    long i, N = 0;

    fgets(buf, BUFSIZ, fd);
    sscanf(buf, "%ld", &N);
    long pArray[N];

    for (i = 0; i < N; i++){
        long prio;
        fgets(buf, BUFSIZ, fd);
        sscanf(buf, "%ld %s", &prio, word);
        pArray[i] = prio;
        if (!prioQ->insert(prioQ, (void *)&pArray[i], (void *)strdup(word))){
            fprintf(stderr, ERROR3, i);
            return;
        }
    }
    fprintf(stdout, "Top 10 least frequesntly used words in file: %s\n", filename);
    for (i = 1; i <= 10L; i++){
        long *p;
        char *w;
        if(!prioQ->removeMin(prioQ, (void **)&p, (void **)&w)){
            fprintf(stderr, "ERROR - unable to pop min elem from PQ.\n");
            return;
        }
        fprintf(stdout, "%ld: (%ld - %s)\n", i, *p, w);
        free(w);
    }
}


int main(int argc, char *argv[]){
    const PrioQueue *pq = NULL;
    int exitStatus = EXIT_FAILURE;
    FILE *fd = NULL;

    pq = PrioQueue_create(cmp, NULL, free);
    if (pq == NULL){
        fprintf(stderr, ERROR1, argv[0]);
        goto cleanup;
    }
    if (argc != 2){
        fprintf(stderr, USAGE, argv[0]);
        goto cleanup;
    }
    fd = fopen(argv[1], "r");

    if (fd == NULL){
        fprintf(stderr, ERROR2, argv[1]);
        goto cleanup;
    }else{
        processFile(fd, pq, argv[1]);
        exitStatus = EXIT_SUCCESS;
    }
cleanup:
    if (fd != NULL)
        fclose(fd);
    if(pq != NULL)
        pq->destroy(pq);
    return exitStatus;

}