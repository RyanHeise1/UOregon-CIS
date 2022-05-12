/*
For each line of C code responsible for a push or pop operation 
(either directly or as a result of a call or return), add a comment 
describing the stack operationand its purpose.
*/
#include <stdio.h>

long fact(long x) {
	if (x <= 1) {
		return 1; // Pops value from stack and returns it
	}

	long px= x; // Pushes px to stack and sets it equal to x
	long fx= fact(x -1); // Push the value of fact(x-1) onto the stack
	return px* fx;  // pop value off of stack and return it. 
					// Also pops the fx variable off of stack
}

int main(){
	printf("fact(1): %ld\n", fact(1));
	printf("fact(2): %ld\n", fact(2));
	printf("fact(3): %ld\n", fact(3));
	printf("fact(4): %ld\n", fact(4));
	printf("fact(5): %ld\n", fact(5));
}
