#include <stdio.h>

int oddBit(unsigned int x){
	/*
	Return 1 if x has at least one bit with a value of 1 at an odd index
	and 0 otherwise

	isolate odd bits
	*/
	unsigned int even  = 0xAAAAAAAA; // All even bits
	// Removes all of the even bits
	x = x & even; 

	// Since we removed all of the even bits above, we can use the !!
	// operator to output a 1 or 0.
	// If we were to only have "!x" it would print out the inverse of 
	// what we want
	return !!x;
}

int main(){
	printf("%X\n", oddBit(0x1)); // 0
	printf("%X\n", oddBit(0x2)); // 1
	printf("%X\n", oddBit(0x3)); // 1
	printf("%X\n", oddBit(0x4)); // 0
	printf("%X\n", oddBit(0xFFFFFFFF)); // 1
	printf("%X\n", oddBit(0xAAAAAAAA)); // 1
	printf("%X\n", oddBit(0x55555555)); // 0
}
