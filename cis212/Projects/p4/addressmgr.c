#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "ADTs/stringADT.h"

#define UNUSED __attribute__((unused))

// -x translates St. => Street and so on
// -s only processes addresses that are in the state defined by 'SS'
void splitLline(FILE *fd, char *pattern, bool ifX, bool ifS){
	char buf[BUFSIZ];
	const String *st = String_create("");

	if (st == NULL){
		fprintf(stderr, "Unable to create String ADT\n");
	}
	while(fgets(buf, BUFSIZ, fd) != NULL){
		const ArrayList *al;
		long i, N;

		st->clear(st);
		st->append(st,buf);

		if(ifX){
			int replaceSt = st->replace(st, "St.", "Street");
			if (replaceSt == 0)
				continue;

			int replaceDr = st->replace(st, "Dr.", "Drive");
			if (replaceDr == 0)
				continue;

			int replaceRd = st->replace(st, "Rd.", "Road");
			if (replaceRd == 0)
				continue;

			int replaceAve = st->replace(st, "Ave.", "Avenue");
			if (replaceAve == 0)
				continue;

			int replacePkway = st->replace(st, "Pkwy.", "Parkway");
			if (replacePkway == 0)
				continue;
		}
		st->rStrip(st);
		al = st->split(st,"|");

		if (al == NULL)
			continue;

		N = al->size(al);
		for (i = 0L; i < N; i++){ // Loop from StringADT video
			char *s;
			/*
			 * --- Deals with s flag and gets the 3rd item containing the pattern
			*/
			if(ifS){
				al->get(al, 2, (void **)&s); // get the 3rd item in the array (one that holds pattern)
				if(strstr(s, pattern) != NULL){ // if the pattern is  there break (contains)
					al->get(al, i, (void **)&s);
					printf("%s\n",s);
				}else{
					break; 
				}
			}else{// No s flag
				al->get(al, i, (void **)&s);
				printf("%s\n",s);
			}
			//al->destroy(al);
		}
		al->destroy(al);
		//st->destroy(st);
	}
	st->destroy(st);
}



int main(int argc, char *argv[]){
	int opt;
	char *pattern = ""; // holds the pattern in the -s flag. If false fattern is ""
	char *filename = ""; // holds the name of the file "" if no file
	int i = 0; // Counter
	//char buf[BUFSIZ];
	bool ifX, ifS;

	ifX = ifS = false;
	opterr = 0;
	while((opt = getopt(argc, argv, "xs:")) != -1){
		switch(opt){
			case 'x': ifX = true; break;
			case 's': ifS = true;  pattern = optarg; break;
			default:
				fprintf(stderr, "%s: illegal option '-%c'\n", argv[0], optopt);
				fprintf(stderr, "usage: %s [-x] [-s SS] [FILE] . . .\n", argv[0]);
				return EXIT_FAILURE;
		}
	}

	/*
 	 * ---- Deal with stdin ----
	*/
	if (argv[optind] == NULL){
		splitLline(stdin, pattern, ifX, ifS);
		return EXIT_SUCCESS;
	}

	/*
	 * --- Two pass loop: First loop to open files, Second to send them to outer function ----
	*/
	for(i = optind; i < argc; i++){
		filename = argv[i];
		FILE *fd = fopen(filename, "r");
		if (fd == NULL){
			fprintf(stderr, "%s: fopen(%s, \"r\") failed\n", argv[0], filename);
			exit(1);
			return EXIT_FAILURE;
		}
	}
	// ---- Second loop to open file and send it to sliptLine ----
	for(i = optind; i < argc; i++){
		filename = argv[i];
		FILE *fd = fopen(filename, "r");
		splitLline(fd, pattern, ifX, ifS);
		fclose(fd);
	} 
}