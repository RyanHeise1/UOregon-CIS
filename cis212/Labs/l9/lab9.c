#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "sort.h"
#include "ADTs/hashcskmap.h"
#include "ADTs/stringADT.h"

#define USAGE  "USAGE: ./%s [files]\n"
#define MERROR "%s: ERROR - Unable to create CSKmap\n"
#define SERROR "%s: ERROR - Unable to create string instance\n"
#define FERROR "Unable to open file: %s !\n"

void freeValue(void *adt){
	if (adt != NULL){
		const CSKMap *map = (const CSKMap *)adt;
		map->destroy(map);
	}
}

int keycmp(void *x1, void *x2){
	return strcmp((char*)x1, (char*)x2);
}

int valuecmp(void *x1, void *x2){
	MEntry *m1 = (MEntry *)x1;
	MEntry *m2 = (MEntry *)x2;
	return *(int *)m1->value <= *(int *)m2->value;
}

void printMap(const CSKMap *map){
	printf("Most frequently used words\n");
	long N, K, i;
	char **keys = map->keyArray(map, &N);
	sort((void **)keys, N, keycmp);
	for (i = 0; i < N; i++){
		const CSKMap *imap;
		(void) map->get(map, keys[i], (void **)&imap);
		MEntry **words = imap->entryArray(imap, &K);
		sort((void **)words, K, valuecmp);
		printf("%s: %s - %d\n", keys[i], words[0]->key, *(int *)words[0]->value);
		free(words);
	}
	free(keys);
}

void processFile(FILE *fd, const CSKMap *map){
	char buf[BUFSIZ], *junk[] = {"[:cntrl:]", "[:punct:]", "[:digit:]"};
	const String *st = String_create("");
	int j;

	while(fgets(buf, BUFSIZ, fd) != NULL){
		st->clear(st); st->append(st, buf);
		st->strip(st); st->lower(st);

		for (j = 0; j < 3; j++){
			st->translate(st, junk[j], ' ');
		}
		if(st->len(st) > 0){
			const ArrayList *al = st->split(st, " ");
			const Iterator *it = al->itCreate(al);
			while(it->hasNext(it)){
				char *tmp;
				(void)it->next(it, (void **)&tmp);
				if(strcmp(tmp, "") != 0){
					char letter[2] = {tmp[0], '\0'};
					if(!map->containsKey(map, letter)){
						const CSKMap *imap = HashCSKMap(0L, 0.0, free);
						(void)map->putUnique(map, letter, (void *)imap);
					}
					const CSKMap *inner = NULL;
					(void)map->get(map, letter, (void **)&inner);

					if(!inner->containsKey(inner, tmp)){
						int *value = (int *)malloc(sizeof(int));
						*value = 1;
						(void)inner->putUnique(inner, tmp, (void *)value);
					}else{
						int *value;
						(void)inner->get(inner, tmp, (void **)&value);
						*value += 1;
					}
				}
			}
			it->destroy(it);
			al->destroy(al);
		}
	}
	st->destroy(st);
}

int main(int argc, char *argv[]){
	const CSKMap *map = NULL;
	int exitStatus = EXIT_FAILURE;
	FILE *fd = NULL;
	extern int opterr;
	int i, option;

	opterr = 0;
	while((option = getopt(argc, argv, "")) != -1) {
		switch(option) {
			default:
				fprintf(stderr, USAGE, argv[0]);
			       	goto cleanup;	
		}
	}
	//create the map
	map = HashCSKMap(0L, 0.0, freeValue);		
	if(map == NULL){
		fprintf(stderr, MERROR, argv[0]);
		goto cleanup;
	}

	if(argc <  2){
		fprintf(stderr, USAGE, argv[0]);
		goto cleanup;
	} else {
		for(i = optind; i < argc; i++) {
			fd = fopen(argv[i], "r");
			if(fd == NULL){
				fprintf(stderr, FERROR, argv[i]);
				goto cleanup;
			}
			processFile(fd, map);
			fclose(fd);
		}
		printMap(map);
		exitStatus = EXIT_SUCCESS;
	}
	

	cleanup:
		//cleanup memory
		if(map != NULL){
			map->destroy(map);
		}
	return exitStatus;
}
