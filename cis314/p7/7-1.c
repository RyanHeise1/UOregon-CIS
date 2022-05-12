#include <time.h>
#include <stdlib.h>
#include <stdio.h>

// Returns the byte offset of the address within its cache block
unsigned int getOffset(unsigned int x){
	// Isolate the last two digits in the hex value "x" that is passed in
	return x & 0x000000ff;
}

// Returns the cache set for the address
unsigned int getSet(unsigned int x){
	// Isolate the 3rd bit in the hex value "x" that is passed into the fcn
	x = x & 0x00000f00;
	// Shift that value to the rightmost position
	return x >> 8;
}

// Returns the cache tag for the address
unsigned int getTag(unsigned int x){
	// Shift last 3 numbers from the value passed in
	// This leaves the last 5 numbbers
	return x >> 12;
}

// Function to test your functions
int main(){
	//Test offset of our two tests
	printf("0x12345678 Offset: %X - Tag: %X - Set: %X\n", getOffset(0x12345678), getTag(0x12345678), getSet(0x12345678));
	printf("0x87654321 Offset: %X - Tag: %X - Set: %X\n", getOffset(0x87654321), getTag(0x87654321), getSet(0x87654321));
}
