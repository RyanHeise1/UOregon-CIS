#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

/*
tutorialspoint.com/getopt-function-in-c to help with printing filename
10:00AM Lab Apr 17
*/

int main(int argc, char *argv[]){
	bool cFlag, dFlag, iFlag, uFlag;
	int opt;

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
				nOptions++;	
				break;
		}

	}
	if (opterr == 1){
		fprintf(stderr, "%s: illegal options '-%c'\n", argv[0], optopt);
		fprintf(stderr, "usage: %s [-cdiu] [FILE]\n", argv[0]);
		return EXIT_FAILURE;
		exit(1);
	}
	if (nOptions == 0){
		printf("Default processing\n");
	}
	if (cFlag == true){
		printf("Print count, blank, then the line\n");
	}
	if (dFlag == true){
		printf("Print only duplicate lines\n");
	}
	if (iFlag == true){
		printf("Perform case-insensitive matching\n");
	}
	if (uFlag == true){
		printf("Print only unique lines\n");
	}
	if (optind == argc){
		printf("Processing standard input\n");
	}
	if(optind < argc) { 		/*when file is passed in*/
		printf("Processing file\n"); 		/* TODO Not Sure How To Process File*/
	} 
	return EXIT_SUCCESS;
}