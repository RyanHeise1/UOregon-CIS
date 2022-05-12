#include <stdio.h>

unsigned int mask(int n){
	/*
	We first must shift the hex value to the left n times. This will create the 
	mask that we are looking for. For example, when we shift 0x1 << 5, we would
	get the binary value 0b100000 or 0x20. For this question we are going to return
	the bit mask containing 1s for the least-significanat n bits. Subtracting 1 from
	the binary value would change it from 0b100000 -> 0b011111.
	*/
	unsigned int x = (0x1 << n) - 1;

	return x;
}

int main(){
	printf("0x%X\n", mask(1));	// 0x1
	printf("0x%X\n", mask(2));	// 0x3
	printf("0x%X\n", mask(3));	// 0x7
	printf("0x%X\n", mask(5));	// 0x1F
	printf("0x%X\n", mask(8));	// 0xFF
	printf("0x%X\n", mask(16));	// 0xFFFF
	printf("0x%X\n", mask(31));	// 0x7FFFFFFF
}
