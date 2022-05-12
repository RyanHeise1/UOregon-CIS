/*
int sum(int from, int to) {
	int result = 0;
	start:
		result += from;
		++from;
		if(from <= to){
			goto start
		}
	return result;
}
*/
#include <stdio.h>

long sum(long from, long to) {
// Declar and initialize result var –*do not modify*.
long result = 0;
// Ensure that argument *from* is in %rdi, 
// argument *to* is in %rsi, *result* is in %rax -*do not 
// modify*.
__asm__ ("movq %0, %%rdi # from in rdi;" :: "r" ( from )); 
__asm__ ("movq %0, %%rsi # to in rsi;" :: "r" ( to )); 
__asm__ ("movq %0, %%rax # result in rax;" :: "r" ( result )); 
// Your x86-64 code goes below -comment each instruction...
__asm__(
	// TODO -Replace the two lines below with add, compare, 
	// jump instructions, labels, etc as necessary to implement
	// the loop.
	".L2:"
		"addq %rdi, %rax\n" 	// result = result + from
		"addq $1, %rdi\n"		// add 1 tp from
		"cmpq %rsi, %rdi\n"		// compare from <= to
		"jle .L2\n"				// Just to the start are true
	);
// Ensure that *result* is in %rax for return -*do not modify*.
__asm__ ("movq %%rax, %0 #result in rax;" : "=r" ( result )); return result;}

int main(){
	printf("sum(1, 6): %d\n", sum(1, 6)); // 21
	printf("sum(3, 5): %d\n", sum(3, 5)); // 12
	printf("sum(5, 3): %d\n", sum(5, 3)); // 5
}