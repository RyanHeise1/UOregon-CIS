#include "stdio.h"

long f(long x, long y, long z){
    /*
    x = %rsi
    y = %rsi
    z = %rdx
    new long a = %rax
    */
	// addq  %rsi, %rdx 
    y = y + z;
    // imulq %rdx, %rdi
    x = x * y;
    // movq  %rdi, %rax
    long a = y;
    // salq  $63, %rdx
    // sarq  $63, %rdx
    // xorq  %rdx, %rax
    a = (((a << 63) >> 63) ^ x);
    return a;
}

int main(){
	printf("f(1, 2, 4): %d\n", f(1, 2, 4)); // 6
	printf("f(3, 5, 7): %d\n", f(3, 5, 7)); // 36
	printf("f(10, 20, 30): %d\n", f(10, 20, 30)); // 500
}
