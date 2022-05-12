#include <time.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct ColorPoint {
 	long a;
	long r;
 	long g;
	long b;
}ColorPoint;

long f(struct ColorPoint **points, int n) {
	long sum = 0;
 	for (int i = 0; i < n; i++) {
 		for (int j = 0; j < n; j++) {
 			sum += points[j][i].a;
 			sum += points[j][i].r;
 			sum += points[j][i].g;
 			sum += points[j][i].b;
	 	}
	 }
	return sum;
}

long g(struct ColorPoint **points, int n) {
	long sum = 0;
 	for (int i = 0; i < n; i++) {
 		for (int j = 0; j < n; j++) {
 			sum += points[i][j].a;
 			sum += points[i][j].r;
 			sum += points[i][j].g;
 			sum += points[i][j].b;
 		}
 	}
	return sum;
}

struct ColorPoint** create2DArray(int n) {
	 // Array to hold a pointer to the beginning of each row
 	struct ColorPoint **points = (struct ColorPoint **)malloc(n * sizeof(struct ColorPoint *));
 	for (int i = 0; i < n; ++i) {
 		// Array to hold each row
 		points[i] =
			 (struct ColorPoint *)malloc(n * sizeof(struct ColorPoint));
 		for (int j = 0; j < n; ++j) {
			 // Init the ColorPoint struct
 			points[i][j].a = rand();
 			points[i][j].r = rand();
 			points[i][j].g = rand();
			points[i][j].b = rand();
 		}
 	}
	return points;
}

void free2DArray(struct ColorPoint** points, int n) {
 	for (int i = 0; i < n; ++i) {
 		free(points[i]);
	}
 	free(points);
}

/*
f() is slow because it has to swap between rows which means we have to reload 
the cache. This in turn, makes f() run slower than g()

g() is the faster function becuase it takes advantage of our memory being row
major. That allows g() to iterate through the columns before the rows. Which
whill lead to the best results.
*/
#define size 2048

int main(){
	struct ColorPoint** x = create2DArray(size);

	// f() time test
	clock_t t1 = clock();
	printf("f(x, size): %ld\n", f(x, size));
	t1 = clock() - t1;
	double time1 = ((double) t1)/CLOCKS_PER_SEC;
	printf("time: %f\n", time1);

	//g() time test
	clock_t t2 = clock();
	printf("g(x, size): %ld\n", g(x, size));
	t2 = clock() - t2;
	double time2 = ((double) t2)/CLOCKS_PER_SEC;
	printf("time: %f\n", time2);

	free2DArray(x, size);
}
