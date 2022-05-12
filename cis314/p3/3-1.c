#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>


struct IntArray {
	int length;
	int *dataPtr;
};

struct IntArray* mallocIntArray(int length){
	/*
	allocates, initializes,and returnsa pointer to a new struct IntArray.
	Need two malloc calls, one for the instanceand one for the instance’s
	dataPtr

	answer in lab3_2.c
	*/
	struct IntArray *arrayPtr;
	arrayPtr->length = length;
	arrayPtr = malloc(length *sizeof(struct IntArray));
	arrayPtr->dataPtr = (int *)malloc(length *sizeof(long));

	return arrayPtr;
}

void freeIntArray(struct IntArray *arrayPtr){
	/*
	frees the instance’s dataPtr and frees the instance
	*/
	arrayPtr->length = 0;
	free(arrayPtr->dataPtr);
	free(arrayPtr);
}

void readIntArray(struct IntArray *arrayPtr){
	/*
	prompts and reads positive (>0) ints from the user to fill the array
	(i.e., read one int for each array index).  Your program should 
	print an error message and prompt again if the user enters a value
	that cannot be parsed as apositiveint.
	*/
	char string[50];
	int i = 0;
	long int final; // long so it's 0-some big number
	while(i < arrayPtr->length){
		printf("Enter Int: ");
		fgets(string, sizeof string, stdin);
		final = strtol(string, NULL, 10);
		if(final != 0){
			arrayPtr->dataPtr[i] = (long)final;
			i++;
		}else {
			printf("Invalid input\n");
		}
	}
}

void swap(int *xp, int *yp){
	/*
	swaps the int values stored at the xp and yp pointers

	Look in class slides
	Took This from class slides
	*/
	int tmp = *xp;
	*xp = *yp;
	*yp = tmp;
}

void sortIntArray(struct IntArray *array){
	/*
	sorts the input array in ascending order using BubbleSort

	Source: 
		https://www.youtube.com/watch?v=xli_FI7CuzA
	*/
	int size = array->length; 
	for(int i = 0; i < size - 1; i++){
		for(int j = 0; j < size - i - 1; j++){
			if (array->dataPtr[j] > array->dataPtr[j + 1]){
				swap(&array->dataPtr[j], &array->dataPtr[j + 1]);
			}
		}
	}
}

void printIntArray(struct IntArray *array){
	/*
	prints the array (e.g., “[ 1, 3, 5, 7 ]”)
	*/
	int i = 0;
	// printf("got here");
	printf("[");
	while(i < array->length){
		printf("%ld", array->dataPtr[i]);
		if(array->dataPtr[i + 1])
			printf(", ");
		i++;
	}
	printf("]\n");
}

int main(){
	/*
	prompt the user to input a positive(>0) int length from the user
	(print an error message and prompt again if the input cannot 
	be parsed as a positive int), call malloc IntArray to create 
	an array, call read IntArray to prompt the user to fill the array, 
	call sort IntArray to sort it, call print Array to print the resulting 
	sorted array, then call freeIntArray to free the heap memory used 
	by the array. 
	*/
	int entry = 0;
	char line[50];

	while (entry <= 0){
		printf("Enter length: ");
		fgets(line, 50, stdin);
		entry = strtol(line, NULL, 10);
		if(entry <= 0){
			printf("Invalid input\n");
		}
	}
	
	struct IntArray *array = NULL;
	array = mallocIntArray(entry);
	if(array != NULL){
		readIntArray(array);
		sortIntArray(array);
		printIntArray(array);
		freeIntArray(array);
	}
}
