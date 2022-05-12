#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "ADTs/stack.h"

void printStack(const Stack *st, FILE *fp){

	char str[60]; // used to get the first line
	char line[128]; // stores the line to look at
	long len; // value stored by st->toArray()
	long element; // stores value to be pushed
	long popElem; // stores the value that it popped when we call st->pop()
	fgets(str, 60, fp); // reads first line ans stores it into str
	int loopLines = atoi(str); // change the str to an int
	int i; // itterator
	long j; // itterator

	for (i = 1; i <= loopLines; i++){
		fgets(line, 128, fp);

		if (sscanf(line, "push %ld\n", &element) == 1){
			st->push(st, (void *)element);
		}

		if (strstr(line, "pop\n")){
			if(st->size(st) == 0){
				printf("StackError\n");
				continue;
			}
			st->pop(st, (void **)&popElem);
			printf("%ld\n", popElem);
		}

		if(strstr(line, "print\n")){
			if (st->size(st) == 0)
				printf("Empty\n");
			else{
				long *array = (long *)st->toArray(st, &len);
				for(j = 0L; j < len; j++){
					printf("%ld%s", array[j], (j == len - 1) ? "" : " ");
				}
				printf("\n");
				free(array);
			}
		}
	}	
	st->destroy(st);
}

int main(int argc, char *argv[]){
	const Stack *st = Stack_create(NULL);

	if (!st){
		fprintf(stderr, "Error making Stack ADT");
	}

	int i = 0;

	if (argc == 1){
		fprintf(stderr, "%s: illegal usage\n", argv[0]);
		fprintf(stderr, "usage: ./exerciseStack FILE\n");
	}else{
		for (i = 1; i < argc; i++){
			char *filename = argv[i];
			FILE *fp = fopen(filename, "r");
			if (fp == NULL){
				fprintf(stderr, "%s: fopen(%s, \"r\") failed\n", argv[0], filename);
				exit(1);
			}else{
				printStack(st, fp);
				fclose(fp);
			}
		}
	}
}