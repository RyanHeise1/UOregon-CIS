#include "stdio.h"
#include "stdlib.h"
#include "sys/time.h"

#define GET_TIME(__TIME_now)								\
	{														\
		struct timeval t;									\
		gettimeofday(&t, NULL);								\
		__TIME_now = (double)(t.tv_sec + (t.tv_usec/1e6));	\
	}

void inner(float *u, float *v, int length, float *dest){
	int i;
	float sum = 0.0f;
	for (i = 0; i < length; ++i){
		sum += u[i] * v[i];
	}
	*dest = sum;
}

void inner2(float *u, float *v, int length, float *dest){
	int i;
	float sum0 = 0.0f;
	float sum1 = 0.0f;
	float sum2 = 0.0f;
	float sum3 = 0.0f;
	// loop over every 4 elems in a chunk so we conditional jump 4x less
	for (i = 0; i < (length - 3); i+=4){
		sum0 += u[i] * v[i];
		sum1 += u[i + 1] * v[i + 1];
		sum2 += u[i + 2] * v[i + 2];
		sum3 += u[i + 3] * v[i + 3];
	} 
	// in clase 'length' != a multiple of 4
	for(; i < length; ++i){
		sum1 += u[i] * v[i];
	}

	*dest = sum0 + sum1 + sum2 + sum3;
}

int main(int argc, char **argv){
	// timer values for inner()
	double t_inner_start;
	double t_inner_end;

	// start and end values for inner2()
	double t_inner2_start;
	double t_inner2_end;

	int n = 1e5;
	float *u = (float *) malloc(n * sizeof(float));
	float *v = (float *) malloc(n * sizeof(float));
	for (int i = 0; i < n; ++i){
		u[i] = rand() / (float)RAND_MAX;
		v[i] = rand() / (float)RAND_MAX;
	}
	float dest;
	int iteration_max = 20000;


	printf("Testing inner()...\n");
	GET_TIME(t_inner_start);
	for (int i = 0; i < iteration_max; ++i){
		inner(u, v, n, &dest);
	}
	GET_TIME(t_inner_end);


	printf("Testing inner2()...\n");
	GET_TIME(t_inner2_start);
	for (int i = 0; i < iteration_max; ++i){
		inner2(u, v, n, &dest);
	}
	GET_TIME(t_inner2_end);


	// ----
	printf("inner(c, v, n, &dest) * %d iterations ...: %.8lf seconds\n",
		iteration_max, (t_inner_end - t_inner_start));
	printf("inner2(c, v, n, &dest) * %d iterations ...: %.8lf seconds\n",
		iteration_max, (t_inner2_end - t_inner2_start));

	printf("done");

	free(u);
	free(v);
	return 0;
}