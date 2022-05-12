#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED __attribute__((unused))

int main(UNUSED int argc, UNUSED char *argv[]) {
	
	//read file
	int returnStatus = EXIT_FAILURE;
	char buf[BUFSIZ];
	while(fgets(buf,BUFSIZ, stdin) != NULL) {
		printf("%s", buf);
	}
	returnStatus = EXIT_SUCCESS;
	return returnStatus;
}