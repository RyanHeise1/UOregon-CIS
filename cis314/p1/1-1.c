#include <stdio.h>

unsigned int combine(unsigned int x, unsigned int y) {
	/*
	returns unsigned int consisting of bytes 1-3 from x
		and 0 from y\
	*/
	// Creating a mask for the x value, leaving the last 8 bits open
	unsigned int mask_x = x & 0xFFFFFF00;
	// Creating a mask for the y value, only leaving the last 8 bits
	unsigned int mask_y = y & 0x000000FF;
	// Take the new x and y values and combine them with the "or" 
	// opperator
	return mask_x | mask_y;
}	

int main(){
	/*
	Testing the combine function
	*/
	printf("%X\n", combine(0x12345678, 0xABCDEF00)); // 0x12345600
	printf("%X\n", combine(0xABCDEF00, 0x12345678)); // 0xABCDEF78
	// return;
}
