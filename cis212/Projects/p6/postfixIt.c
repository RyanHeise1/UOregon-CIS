#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "ADTs/stack.h"

void evalLine(char *line){
	const Stack *st = Stack_create(NULL);

	int i;
	long value;
	long digit;
	//printf("Line: %s", line);

	if (!st){
		fprintf(stderr, "Error making Stack ADT");
		exit(1);
	}
	for (i = 0; line[i] != '\n'; ++i){
		if(line[i] == ' ')
			continue;
		else if(isdigit(line[i])){
			value = 0;
			while(isdigit(line[i])){
				value = value * 10 + (long)(line[i]-'0');
				i ++;
			}
			i --;
			st->push(st, (void *)value);
			//printf("Pushed: %ld\n", value);
		}else {
			long val1;
			long val2;
			st->pop(st, (void **)&val1);
			st->pop(st, (void **)&val2);

			if(line[i] == '+'){
				st->push(st, (void *)(val2 + val1));
				//printf("Adding %ld + %ld\n", val2, val1);
			}
			if(line[i] == '-'){
				st->push(st, (void *)(val2 - val1));
				//printf("Sub: %ld\n", val2-val1);
			}
			if(line[i] == '*'){
				st->push(st, (void *)(val2 * val1));
			}
			if(line[i] == '/'){
				st->push(st, (void *)(val2 / val1));
			}
		}
	}
	st->pop(st, (void **)&digit);
	printf("%ld\n", digit);
	st->destroy(st);
}

void readFile(FILE *fp){
	char str[BUFSIZ];
	fgets(str, BUFSIZ, fp); // reads first line ans stores it into str
	long loopLines = atoi(str); // change the str to an int
	char line[BUFSIZ]; // stores the line to look at
	long i; // itterator

	for (i = 1L; i <= loopLines; i++){
		fgets(line, BUFSIZ, fp);
		//printf("%s", line);
		evalLine(line);
	}
}

int main(int argc, char *argv[]){
	int i = 0;

	if (argc == 1){
		fprintf(stderr, "%s: illegal usage\n", argv[0]);
		fprintf(stderr, "usage: ./postfixIt FILE\n");
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