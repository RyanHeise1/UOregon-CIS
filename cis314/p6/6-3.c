#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


#define GET_TIME(__TIME_now)								\
	{														\
		struct timeval t;									\
		gettimeofday(&t, NULL);								\
		__TIME_now = (double)(t.tv_sec + (t.tv_usec/1e6));	\
	}

float f(float *a, int n) {
	float prod = 1.0f;
	for (int i = 0; i < n; ++i) {
		if (a[i] != 0.0f) {
			prod *= a[i];
		}
	}return prod;
}

// dont preform zero check (same as f() otherwise)
float g(float *a, int n) {
	float prod = 1.0f;
	for (int i = 0; i < n; ++i) {
		if (a[i] != 0.0f) {
			prod *= a[i];
		}
	}return prod;
}

float* createArray(int size) {
	float *a = (float *)malloc(size * sizeof(float));
	for (int i = 0; i < size; ++i) {
		// 50% chance that a[i] is 0.0f, random value on the range
		// [0.76, 1.26] otherwise.
		float r = rand()/(float)RAND_MAX;
		a[i] = r < 0.5f ? 0.0f : r + 0.26f;
	}return a;
}


int main(int argc, char **argv){
	int size = 10000;
	float *a = createArray(size);

	float *b = (float *) malloc(size * sizeof(float));
	for (int i = 0; i < size; ++i){
		b[i] = a[i];
		if (b[i] == 0.0f){
			b[i] = 1.0f;
		}
	}
	int count = 0;
	for (int i = 0; i < size; ++i){
		if (a[i] != 0.0f){
			count++;
		}
	}
	printf("!= 0.0 count: %d\n", count);

	float *c = (float *) malloc(count * sizeof(float));
	int place_at = 0;
	for (int i = 0; i < size; ++i){
		if (a[i] != 0.0){
			c[place_at++] = a[i];
		}
	}

	float retval = 0.0f;
	double start, end;
	double time_GA, time_GB, time_GC;


	/*
	f(a, size) takes the longest time to run becuase it is 
	doing a 0 check on the i value a[i] whereas g is not.
	*/
	GET_TIME(start);
	retval = f(a, size);
	GET_TIME(end);
	time_GA = end - start;
	printf("f(a, size) == %f\n", retval);
	printf("f(a, size) time: %lf sec.\n", time_GA);

	free(a);

	/*
	g(b, size) has a faster run time becuase we are not doing
	the 0 check in this function. This means our program has
	less things to compare, thus running faster.
	*/
	GET_TIME(start);
	retval = g(b, size);
	GET_TIME(end);
	time_GB = end - start;
	printf("g(b, size) == %f\n", retval);
	printf("g(b, size) time: %lf sec.\n", time_GB);

	free(b);


	/*
	g(c, count) funs faster than any of the above tests becuase
	the variable "count" has less numbers to itterate through.
	this is becuase count is only the count of numbers that are
	!= 0.0f.
	*/
	GET_TIME(start);
	retval = g(c, count);
	GET_TIME(end);
	time_GC = end - start;
	printf("g(c, count) == %f\n", retval);
	printf("g(c, count) time: %lf sec.\n", time_GC);

	free(c);

	return 0;

}


