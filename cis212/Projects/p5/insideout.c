#include "addresslist.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ADTs/stringADT.h"

#define UNUSED __attribute__((unused))


int main(UNUSED int argc, UNUSED char *argv[]){
	//create an Address List
	const AddressList *al = AddressList_create(0L);
	char buf[BUFSIZ];
	int status;
	long i;
	long N;


	//if failure
	if (al == NULL){
		//print error on stderr
		fprintf(stderr, "Unable to create ArrayList ADT\n");
		//terminate program
		return EXIT_FAILURE;
	}
	//while there is a next line
	while(fgets(buf, sizeof buf, stdin) != NULL){
		//duplicate the line on the heap
		char *sp = strdup(buf);
		//insert the line into the middle of the AddressList (Code given)
		long index = (al->size(al)) / 2;
		//if insert fails
		status = al->insert(al, index, sp);
		if(status == 0){
			//print error
			fprintf(stderr, "Unable to insert line at index: %ld", index);
			//cleanup and terminate
			al->destroy(al);
			return EXIT_FAILURE;
		}
	}
		
	//set N to the number of strings in the AddressList
	N = al->size(al);
	//for i = N-1 down to 0
	for(i = N - 1; i > -1; i--) {
		char *s;
		//get address at index i
		al->get(al, i, &s);
		//print address
		printf("%s", s);
	}
	//destroy the AddressList
	al->destroy(al);
	return EXIT_SUCCESS;
}
