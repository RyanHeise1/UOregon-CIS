#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

void prefixCat(FILE *fd, bool ifI, bool ifF) {
	int i = 0;
	char line[BUFSIZ];
	char *quit =  "q\n";
	char prefix[50] = "";
	
	if(!ifF) { printf(">>> "); }
	while(fgets(line, BUFSIZ, fd) != NULL) {
		if(strcmp(quit, line) == 0) {
			break;
		} else {
			if(ifI) {
				sprintf(prefix, "%4d: ", i);
				i++;
			}
			printf("%s%s\n", prefix, line);
		}
	if(!ifF) { printf(">>> "); }
	}
	printf("Goodbye! ^.^\n");
}

int main(int argc, char *argv[]) {
	int option;
	extern int opterr;
	bool ifI, ifF;
	bool start = true;
	int exitStatus;
	char *filename;


	//parse the argument from command line
	opterr = 0;
	ifI = ifF = false;
	while((option = getopt(argc, argv, "f:i")) != -1 ) {
		switch(option) {
			case 'i': ifI = true; break; 
			case 'f': ifF = true; filename = optarg; break;
			default: 
				 printf("Unknown flag: -%c\n", optopt);
				 start = false;
				 break;
		}
	}

	//open file/call prefixCat
	if(start) {
		if(ifF) {
			FILE *fd = fopen(filename, "r");
			if (fd != NULL){
				prefixCat(fd, ifI, ifF);
			}else{ 
				exitStatus = EXIT_FAILURE;
			}	
			fclose(fd);
		} else {
			prefixCat(stdin, ifI, ifF);
		}
	
		exitStatus = EXIT_SUCCESS;
	} else { exitStatus = EXIT_FAILURE; }

	return exitStatus;

}
