#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "sort.c"
#include "extractSS.h"
#include "ADTs/stringADT.h"
#include "ADTs/hashcskmap.h"

#define UNUSED __attribute__((unused))
#define USAGE  "USAGE: %s [-1 | -3] FILE . . .\n"
#define FERROR "Unable to open file: %s\n"
#define STRING_ERROR "Unable to create String ADT\n"
#define MERROR "%s: ERROR - Unable to create CSKmap\n"

void freeValue(void *elem){
	if (elem != NULL){
		const ArrayList *al = (const ArrayList *)elem;
		al->destroy(al);
	}
}

void processFile(FILE *fd, const CSKMap *map){
	char buf[BUFSIZ];
	const ArrayList *al;
	// const String *st = String_create("");

	while(fgets(buf, BUFSIZ, fd) != NULL){
		char ss[3];
		// get the key (Hint: extractSS())
		extractSS(buf, ss);
		//printf("%s\n", ss);
		// if key not in map
		if(! map->containsKey(map, ss)){
			// create an arrayList (Hint: ArrayList(al, free))
			al = ArrayList_create(0L, free);
			// insert (key, arrayList) into the map
			map->putUnique(map, ss, (void **)al);
		}
		map->get(map, ss, (void **)&al);
		al->add(al, strdup(buf));
	}
	//st->destroy(st); 
	//al->destroy(al);
}

void processStdin(FILE *stdin, const CSKMap *map, bool if1, UNUSED bool if3){
	const String *st = String_create("");
	const ArrayList *al;
	char buf[BUFSIZ];
	char ss[3]; 
	while(fgets(buf, BUFSIZ, stdin) != NULL){
		strcpy(ss, buf);
		// clear string ADT and append new SS into the ADT
		st->clear(st); st->append(st, ss); 
		// remove the \n character
		st->strip(st);
		map->get(map, st->convert(st), (void **)&al);
		char *addr;
		long i, N = al->size(al);
		for (i = 0L; i < N; i++){
			al->get(al, i, (void **)&addr);
			if(if1)
				printf("%s", addr);
			else{
				st->clear(st); st->append(st, addr);
				const ArrayList *tmp = st->split(st, "|");
				long size = tmp->size(tmp);
				long j;
				for(j = 0L; j < size; j++){
					char *s;
					tmp->get(tmp, j, (void **)&s);
					printf("%s%s", (j == 0L) ? "" : "\n", s);
				}
				tmp->destroy(tmp);
			}
		}
	}
	st->destroy(st);
}

int main(int argc, char *argv[]){
	char *filename;
	int i, opt, nArgs= 0 ;
	bool if1 = false, if3;

	const CSKMap *map = HashCSKMap(0L, 0.0, freeValue);

	while((opt = getopt(argc, argv, "13")) != -1){
		switch(opt){
			case '1': if1 = true; if3  = false; nArgs++; break;
			case '3': nArgs++; break;
			default:
				fprintf(stderr, USAGE, argv[0]);
				return EXIT_FAILURE;
		}
	}
	if(map == NULL){
		fprintf(stderr, MERROR, argv[0]);
	}

	/*
	 * --- Two pass loop: First loop to open files, Second to send them to outer function ----
	*/
	for(i = optind; i < argc; i++){
		filename = argv[i];
		FILE *fd = fopen(filename, "r");
		if (fd == NULL){
			fprintf(stderr, FERROR, filename);
			return EXIT_FAILURE;
		}
		fclose(fd);
	}
	// ---- Second loop to open file and send it to sliptLine ----
	for(i = optind; i < argc; i++){
		filename = argv[i];
		FILE *fd = fopen(filename, "r");
		//printf("Processing File Now:\n");
		processFile(fd, map);
		fclose(fd);
	}
	//printf("Processing Stdin Now:\n");
	processStdin(stdin, map, if1, if3);
	map->destroy(map);
}