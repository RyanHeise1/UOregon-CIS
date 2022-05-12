#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

void printLine(char line[], int matchCt, bool cFlag, bool dFlag, bool uFlag){ // need help with the logic here
	if (cFlag){
		if (uFlag){
			if (matchCt == 1){
				printf("%7d %s", matchCt, line);
			}
		}
		else if(dFlag){
			if (matchCt > 1){
				printf("%7d %s", matchCt, line);
			}
		}
		else{
			printf("%7d %s", matchCt, line);
		}
	}
	else if(dFlag){
		if (matchCt > 1) {
			printf("%s", line);
		}
	}
	else if(uFlag){
		if(matchCt == 1){
			printf("%s", line);
		}
	}
	else{
		printf("%s", line);
	}
} // printLine end


void uniqFile(FILE *fd, bool cFlag, bool dFlag, bool iFlag, bool uFlag){
	char buf[BUFSIZ];
    char last[BUFSIZ] = "";    
    bool first = true;
    int matchCt = 1;

    while(fgets(buf, sizeof buf, fd) != NULL){
    	bool matches;
    	if (iFlag){
    		matches = strcasecmp(last, buf) == 0;
    	}else {
    		matches = strcmp(last,buf) == 0;
    	}
    	if (! matches) {                                       /* NO match if(matches) to test if they match*/
            if (! first)                                              /* not first line, print last */
                printLine(last, matchCt, cFlag, dFlag, uFlag);
    		else                                                      /* was first line */                 
    			first = false;
            strcpy(last, buf);                                 /* save line for next compare */
            matchCt = 1;                                       /* reset counter */
    	} else{                                                      /*if match increase counter for that word*/
            matchCt ++;
        }
    }
    if (! first)                                                        /* write out the last line */         
    	printLine(last, matchCt, cFlag, dFlag, uFlag);
} // end of fcn




int main(int argc, char *argv[]){
	bool cFlag, dFlag, iFlag, uFlag;
	int opt;
	char buf[BUFSIZ];
    char last[BUFSIZ] = "";   
    bool first = true;     
    int matchCt = 1;  
    FILE *fd;

	cFlag = iFlag = uFlag = dFlag = false;
	int nOptions = 0; /* Count number of options */
	opterr = 0;  /* Tells getopt to NOT print an illegal option error message */

	while(( opt = getopt(argc, argv, "cdiu")) != -1){
		switch(opt){
			case 'c': 				/* Print count, blank, then line*/
				cFlag = true;
				nOptions++;
				break;
			case 'd':				/* Print only duplicate lines*/
				dFlag = true;
				nOptions++;
				break;
			case 'i':				/* Preform case-insensitive matching*/
				iFlag = true;
				nOptions++;
				break;
			case 'u':				/* Print only unique lines*/
				uFlag = true;
				nOptions++;
				break;
			case '?':				/* Catch errors*/
				opterr = 1;
				break;
			default:				/* Default processing*/
				// nOptions++;	
				break;
		}
	}

	if (opterr == 1){
		fprintf(stderr, "%s: illegal options '-%c'\n", argv[0], optopt);
		fprintf(stderr, "usage: %s [-cdiu] [FILE]\n", argv[0]);
		return EXIT_FAILURE;
		exit(1);
	}

	if(uFlag && dFlag){// if u and d flag are selected, there is nothing to print
		return EXIT_FAILURE;
	}

	if (optind == argc){ //stdin
		while(fgets(buf, sizeof buf, stdin) != NULL){
			bool matches;
			if(iFlag){
				matches = strcasecmp(last, buf) == 0;
			}else{
				matches = strcmp(last, buf) == 0;
			}
			if (! matches){
				if (! first){
					printLine(last, matchCt, cFlag, dFlag, uFlag);
				}
				else
					first = false;
				strcpy(last, buf);
				matchCt = 1;
			}
			else {
				matchCt += 1;
			}
		}
		if (! first){
			printLine(last, matchCt, cFlag, dFlag, uFlag);
		}
	}


	if(optind < argc) { //Processing file
		fd = fopen(argv[argc - 1], "r"); // finds file
		if (fd == NULL){ // If file does not exist
			fprintf(stderr, "%s: fopen(%s, \"r\") failed\n", argv[0], argv[argc - 1]);
			fclose(fd);
			return EXIT_FAILURE;
		}else{
			fd = fopen(argv[argc - 1], "r"); // finds file
			uniqFile(fd, cFlag, dFlag, iFlag, uFlag);
			fclose(fd);
		}
	} 	
	return EXIT_SUCCESS;
}
