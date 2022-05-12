#include <stdio.h>

void printBytes(unsigned char *start, int len) {
	for (int i = 0; i < len; ++i){
		printf(" %.2x", start[i]);
	}
	printf("\n");
}

void printInt(int x) {
	printBytes((unsigned char *) &x, sizeof(int));
}

void printFloat(float x) {
	printBytes((unsigned char *) &x, sizeof(float));
}

void printShort(short x) {
	printBytes((unsigned char *) &x, sizeof(short));
}

void printLong(long x) {
	printBytes((unsigned char *) &x, sizeof(long));
}

void printLongLong(long long x) {
	printBytes((unsigned char *) &x, sizeof(long long));
}

void printDouble(double x) {
	printBytes((unsigned char *) &x, sizeof(double));
}

int main(){
	/*
	These two functions look to be ordering the sequence of bits in different
	orders. More specifically, I think that this program is putting these int's
	in little and big euclidian form. 

	printing the ints in hex form. Ints and floats are also treated differnet
	in programming. 1.0 is not the same as 1. This would explain why the output
	is different between the two functions. One set is taking the int of x and 
	the other is taking the float of x. printInt(0) makes sence beciase it will
	be 00 00 00 00 regardless becuase 0 is treated as 0b0 in binary and 0x0 in 
	hex. 
	*/
	printInt(0);
	printInt(8);
	printInt(16);
	printInt(24);
	printf("\n");
	printFloat(0);
	printFloat(8);
	printFloat(16);
	printFloat(24);
	printf("\n");
	printShort(0);
	printShort(8);
	printShort(16);
	printShort(24);
	printf("\n");
	printf("\n");
	printLong(0);
	printLong(8);
	printLong(16);
	printLong(24);
	printf("\n");
	printLongLong(0);
	printLongLong(8);
	printLongLong(16);
	printLongLong(24);
	printf("\n");
	printDouble(0);
	printDouble(8);
	printDouble(16);
	printDouble(24);
}
