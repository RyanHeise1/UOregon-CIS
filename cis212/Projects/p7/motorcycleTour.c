#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ADTs/queue.h"

void readFile(FILE *fp){
	const Queue *q = Queue_create(NULL);

	long rLieters, mtStation, i, N;
	int totalL; // Total lieters, Mile coverage
	char buf[BUFSIZ]; // Holds the line

	(void)fgets(buf, BUFSIZ, fp); // Get first line
	(void)sscanf(buf, "%ld", &N); // Get loop lines from frist line

	for(i = 0L; i < N; i++){
		fgets(buf, BUFSIZ, fp);
		sscanf(buf, "%ld %ld", &rLieters, &mtStation);
		totalL += rLieters;
		if(totalL * 10 >= mtStation){
			totalL = ((totalL * 10) - mtStation) / 10;
			//printf("Line %ld: \t\t Total L: %d\n", i, totalL);
			q->enqueue(q, (void *)i);
		}else{
			q->clear(q);
			totalL = 0;
		}
	}
	long head;
	q->front(q, (void **)&head);
	printf("%ld\n", head);
	q->destroy(q);
}

int main(int argc, char *argv[]){
	int i = 0;

	if (argc != 2){
		fprintf(stderr, "%s: illegal usage\n", argv[0]);
		fprintf(stderr, "usage: ./motorcycleTour FILE\n");
		return EXIT_FAILURE;
	}else{
		for (i = 1; i < argc; i++){
			char *filename = argv[i];
			FILE *fp = fopen(filename, "r");
			if (fp == NULL){
				fprintf(stderr, "%s: fopen(%s, \"r\") failed\n", argv[0], filename);
				exit(1);
			}else{
				readFile(fp);
				fclose(fp);
			}
		}
	}
}