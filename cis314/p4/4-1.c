/*
loop:						// Initialize
	movq  %rsi, %rcx		// moving b to a new register
	movl  $1, %eax			// setting long mask = 1
	movl  $0, %edx			// sets result = 0
.L2:						// for loop
	testq  %rax, %rax		// Test to see if rax = 0
	je     .L4				// jump to L4 if rax = 0
	movq  %rax, %r8			// move the mask value to a new register
	andq  %rdi, %r8			// stores the a & mask result
	orq   %r8, %rdx			// stores the value of result |= %r8
	salq  %cl, %rax			// mask <<= b
	jmp   .L2				// Jump back up the loop
.L4:						// return statement
	movq  %rdx, %rax		// moving result to the return value
	ret 					// return the value from result
*/

/*
Annotate each line of the x86-64 code in terms of x, y, result, and 
mask. Assume that this x86-64 code follows the register usage conventions
outlined in B&O’H section 3.7.5(it does).
*/

#include <stdio.h>

long loop(long a, long b) {
	long result = 0;
	for (long mask = 1; mask != 0; mask <<= b) {
		result |= (a & mask);
	}
	return result;
}

int main(){
	printf("loop(1, 5): %ld\n", loop(1, 5)); // 1
	printf("loop(2, 4): %ld\n", loop(2, 4)); // 0
	printf("loop(3, 3): %ld\n", loop(3, 3)); // 1
	printf("loop(4, 2): %ld\n", loop(4, 2)); // 4
	printf("loop(5, 1): %ld\n", loop(5, 1)); // 5
}
