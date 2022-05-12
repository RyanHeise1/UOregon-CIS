/*
.L10:
	movq (%rax), %rcx 		// store value of a[i][j] into t1
	movq (%rdx), %rsi 		// store value of a[j][i] into t2
	movq %rsi, (%rax) 		// get value of a[i][j] and store into t2
	movq %rcx, (%rdx) 		// get value of a[j][i] and store into t1
	addq $8, %rax  			// Move memory location to the next row in the array
	addq $32, %rdx 			// Move memory location to the next column in the array
	cmpq %r9, %rax 			// compare i and j
	jne.L10 				// Jump if i and j are not equal


Copy the optimized x86-64 code above (compiled with -O1) into a C file as a comment.  
Annotate each line of the x86-64code in terms of N,a,i,j,t1, and t2 from the original 
C code. Write a new C version of transpose that implements these optimizations using 
pointer arithmetic and dereferencing as demonstrated by the optimized assembly.

In other words, write an optimized C version of transpose the produces x86-64 code 
equivalent to the optimized code above when you compile itwith -Og.

*/

#include <stdio.h>
#define N 4

typedef long t_array[N][N];

void transpose(t_array a){
	long *cp;
	long *rp;
	long t1;
	long t2;
	long i;
	long j;

	for(i = 0; i < N; ++i){
		rp = &a[i][0];
		cp = &a[0][i];
		for (j = 0; j < i; ++j){
			t1 = *rp;
			t2 = *cp;
			*rp = t2;
			*cp = t1;
			rp += 1;
			cp += N;
		}
	}
}

int main(){
	int i;
	int j;
	t_array array = {
		{1,2,3,4},
		{5,6,7,8},
		{9,10,11,12},
		{13,14,15,16}
	};
	transpose(array);
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			printf("%ld ", array[i][j]);
		}
		printf("\n");
	}
}
