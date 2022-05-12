#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define _GNU_SOURCE
char *strcasestr(const char *haystack, const char *needle); // from stack overflow

#define UNUSED __attribute__((unused))


void grepFile(FILE *fd, char *pattern, char *filename, int fCount, bool ifI,  bool ifV, bool ifC){
	char buf[BUFSIZ];


	if (ifI && ifV && ifC){ // if ALL flags selected
		int matchCt = 0; // counter for matching lines
		while (fgets(buf, sizeof buf, fd) != NULL){
			if(strcasestr(buf, pattern) == NULL){
				matchCt += 1;
			}
		}
		if(fCount > 1){
			printf("%s:%d\n", filename, matchCt);
		}else{
			printf("%d\n", matchCt);
		}
	}


	if (ifI && !ifV && !ifC){ // if ONLY I flag selected
		while (fgets(buf, sizeof buf, fd) != NULL){
			if(strcasestr(buf, pattern) != NULL){
				if (fCount > 1){
					printf("%s:%s", filename, buf);
				}else{ // there is only one file/ no files at all
					printf("%s", buf);
				}
			}
		}
	}


	if (!ifI && !ifV && ifC){ // if ONLY C flag selected
		int matchCt = 0; // counter for matching lines
		while (fgets(buf, sizeof buf, fd) != NULL){
			if(strstr(buf, pattern) != NULL){
				matchCt += 1;
			}
		}
		if(fCount > 1){
			printf("%s:%d\n", filename, matchCt);
		}else{
			printf("%d\n", matchCt);
		}
	}


	if (ifI &&ifV && !ifC){ // if I and V flag are selected
		while (fgets(buf, sizeof buf, fd) != NULL){
			if(strcasestr(buf, pattern) != NULL){
				if (fCount > 1){
					printf("%s:%s\n", filename, buf);
				}else{
					printf("%s", buf);
				}
			}
		}
	}


	if (!ifI && ifV && ifC){ // if V and C flag are selected
		int matchCt = 0; // counter for matching lines
		while (fgets(buf, sizeof buf, fd) != NULL){
			if(strstr(buf, pattern) == NULL){
				matchCt += 1;
			}
		}
		if(fCount > 1){
			printf("%s:%d", filename, matchCt);
		}else{
			printf("%d\n", matchCt);
		}
	}


	if (ifI && !ifV && ifC){ // if ONLY I and C flag selected
		int matchCt = 0; // counter for matching lines
		while (fgets(buf, sizeof buf, fd) != NULL){
			if(strcasestr(buf, pattern) != NULL){
				matchCt += 1;
			}
		}
		if(fCount > 1){
			printf("%s:%d", filename, matchCt);
		}else{
			printf("%d\n", matchCt);
		}
	}


	if(ifV && !ifI && !ifC){// if ONLY V flag selected
		while (fgets(buf, sizeof buf, fd) != NULL){
			if(strstr(buf, pattern) == NULL){// passes if pattern in NOT in the line
				if (fCount > 1){
					printf("%s:%s", filename, buf);
				}else{ // there is only one file/ no files at all
					printf("%s", buf);
				}
			}

		}
	}


	if(!ifV && !ifI && !ifC){ // if NO flags selected
		while (fgets(buf, sizeof buf, fd) != NULL){
			if(strstr(buf, pattern) != NULL){
				if (fCount > 1){
					printf("%s:%s", filename, buf);
				}else{ // there is only one file/ no files at all
					printf("%s", buf);
				}
			}
		}
	}
}

int main(int argc, char *argv[]){
	int opt;
	bool ifI, ifV, ifC;
	int nFlags = 0;
	char *pattern; // gets pattern to use in search
	char *filename = "";
	int fCount = 0; // counts number of files
	int i;

	ifI = ifV = ifC = false;
	opterr = 0;
	while((opt = getopt(argc, argv, "ivc")) != -1){
		switch(opt){
			case 'i':  nFlags++; ifI = true; break;
			case 'v': nFlags++; ifV = true; break;
			case 'c': nFlags++; ifC = true; break;
			default:
				fprintf(stderr, "%s: illegal option '-%c'\n", argv[0], optopt);
				fprintf(stderr, "usage: %s [-ivc] STRING [FILE]\n", argv[0]);
				return EXIT_FAILURE;
		}
	}


	/*
	---- Checks to see if the STRING is present ----
			If it is no, it will produce an error
			If there is a string, it will set the pattern to the location of the word
	*/
	if(argv[optind] == NULL){
		fprintf(stderr, "%s: no string provided\n", argv[0]);
		fprintf(stderr, "usage: %s [-ivc] STRING [FILE]\n", argv[0]);
		return EXIT_FAILURE;
	}else{
		pattern = argv[optind];
	}
	
	
	/* 
	---- Checks to see if FILE is present -----
			Loops through first time to make sure all files are there
			If not, it will default to stdin
	*/
	if (argv[optind + 1] == NULL){
		//No file specified. Processing stdin
		grepFile(stdin, pattern, filename, fCount, ifI, ifV, ifC);
	}else{
		//---- First loop to catch wrong filename(s) and count files ----
		for(i = optind + 1; i < argc; i++){
			filename = argv[i];
			FILE *fd = fopen(filename, "r");
			if (fd == NULL){
				fprintf(stderr, "%s: fopen(%s, \"r\") failed\n", argv[0], filename);
				exit(1);
				return EXIT_FAILURE;
			}else{
				fCount += 1;
				fclose(fd);
			}
		}

		i = 0;
		// ---- Second loop to open file and send it to uniqFile ----
		for(i = optind + 1; i < argc; i++){
			filename = argv[i];
			FILE *fd = fopen(filename, "r");
			grepFile(fd, pattern, filename, fCount, ifI, ifV, ifC);
			fclose(fd);
		} 
	}

	
	return EXIT_SUCCESS;
}