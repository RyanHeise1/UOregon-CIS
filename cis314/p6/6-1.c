#include <stdio.h>

int f(int a, int b, int c, int d, int n){
	int result = 0;
	int ab = a * b;
	int cd = c * d;
	int icd = 0;
	int tmp;

	for (int i = 0; i < n; ++i){
		for (int j = 0; j < n; ++j){
			tmp = j;
			while(tmp--)
				icd += cd;
			result += ab + icd + j;
			icd = 0;
		}
	}
	return result;
}

int main(){
	printf("f(1, 2, 3, 4, 5): %d\n", f(1, 2, 3, 4, 5));  // 700
	printf("f(2, 3, 4, 5, 6): %d\n", f(2, 3, 4, 5, 6));  // 2106
	printf("f(6, 5, 4, 3, 2): %d\n", f(6, 5, 4, 3, 2));  // 146
	printf("f(5, 4, 3, 2, 1): %d\n", f(5, 4, 3, 2, 1));  // 20
}
