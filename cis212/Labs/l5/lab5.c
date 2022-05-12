#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>

#include "ADTs/stringADT.h"
#include "ADTs/arraylist.h"

#define USAGE "usage: %s [-s] [FILE] ...\n"


void getStats(FILE *fds[], int nFiles, bool doStats){
	int i;
	for (i = 0; i < nFiles; i++){
		const ArrayList *csv = ArrayList_create(0L, NULL); 
		const String *st = String_create("");
		char buf[BUFSIZ];

		while(fgets(buf, BUFSIZ, fds[i]) != NULL){
			st->clear(st);
			st->append(st, buf);
			const ArrayList *al = st->split(st, ",");
			const Iterator *it = al->itCreate(al);

			while(it->hasNext(it)){
				char *temp;
				(void) it->next(it,(void**)&temp);
				(void)csv->add(csv,temp);
			}
			it->destroy(it);
		}
		st->destroy(st);
		if(doStats){
			long sum = 0;
			double diff = 0.0;
			double mean = 0.0;
			double stdev = 0.0;
			const Iterator *it2 = csv->itCreate(csv);

			while(it2->hasNext(it2)){
				char *temp;
				(void) it2->next(it2, (void**)&temp);
				sum+= atoi(temp);
			}
			it2->destroy(it2);

			mean = (double)sum/csv->size(csv);
			int i;
			for (i=0;  i < csv->size(csv); i++){
				char *temp;
				(void) csv->get(csv, i, (void **)&temp);
				diff += pow((atoi(temp) - mean), 2);
			}
			stdev = sqrt((double)diff/csv->size(csv));
			printf("mean: %.2f\n", mean);
			printf("Stdev: %.2f\n", stdev);
		}
		csv->destroy(csv);
	}
}


int main(int argc, char *argv[]){
	int opt;
	int nFiles = 0;
	FILE *fds[50];
	bool doStats = false;
	extern int opterr;
	int exitStatus = EXIT_FAILURE;

	opterr = 0;
	while((opt = getopt(argc, argv, "s")) != -1){
		switch(opt){
			case 's':
				doStats = true;
				break;
			default:
				fprintf(stderr, "%s: invalid option '-%c'\n", argv[0], optopt);
				fprintf(stderr, USAGE, argv[0]);
				goto cleanup;
		}
	}
	if((argc - optind) == 0){
		fprintf(stderr, USAGE, argv[0]);
		goto cleanup;
	}
	else{
		int i;
		for(i = optind; i < argc; i++){
			FILE *fd = fopen(argv[i], "r");
			if(fd == NULL){
				fprintf(stderr, "%s: unable to open file: %s\n", argv[0], argv[i]);
				goto cleanup;
			}
			fds[nFiles++] = fd;
		}
		getStats(fds, nFiles, doStats);
		exitStatus = EXIT_SUCCESS;
	}
	cleanup:
		for(int i = 0; i < nFiles; i++){
			fclose(fds[i]);
		}
		return exitStatus;
}