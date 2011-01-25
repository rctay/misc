#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* CREDITS to rc who made me think of spending a little time
doing this. I probably learnt more things in this process.
*/


/* This program has 2 insertion sort functions:

1. Traditional Insertion Sort which swaps adjacent elements.
2. Insertion Sort which does not swap adjacent elements

WARNING: The 2nd version of insertion sort is not meant for pure
Java programmers. By this, I mean people who think that pointers
and raw memory are a bad bad thing and that pure OOP is the right
way to go.



Profiling shows that the 2nd version is faster.

Results (Timings in s):

n         Traditional        Modified
100,000    27.08 (71.2%)      10.95 (28.8%)
1,000,000  2747.14 (70.6%)    1143.80 (29.4%)

Yea I spent one hell of a time waiting for 1,000,000 ops...


This performance difference is despite the use of a macro for 
swapping in our conventional insertion sort function.

Using a macro instead of a function effectively changes the 
SWAP to inline code at compilation, which saves the
overhead of > hundreds of thousands of function calls for smth
this trivial. 

Just imagine how much longer poor me will have to wait if 
I did not use a macro.

*/




/*
Change these 2 constants as you wish. They are for PROFILING
purposes using gprof, called in the profileSort() function.

MY_MAX is an upper bound on our random numbers generation.
ARR_SZ is the array size */
#define MY_MAX 1000000
#define ARR_SZ 1000000


void printArray(int arr[], int limit);
void insertionSort(int arr[], int sz);
void insertionSortSwap(int arr[], int sz);

// We inline the swap to make it faster
#define SWAP(p, q, type) type tmp = *p; *p = *q; *q = tmp


/* 
testSort: Used on small array to see the sort works.
profSort: Used on large arrays for profiling purposes.
*/

void testSort();
void profileSort();



int main(void)
{
	/* Uncomment the line below this section to use the test function
	which show the 2 insertion sorts working on small arrays.*/

	//testSort();


	/* Uncomment the line below this section for profiling purposes.
	Compile this program with -pg flags, eg.

	$ gcc -o insertionSort insertionSort.c -pg
	$ ./insertionSort

	At this point, a gmon.out file will be in your current directory.
	Then do the following to see the result of profiling:
	
	$ gprof insertionSort gmon.out 
	*/

	//profileSort();

	return 0;
}


void printArray(int arr[], int limit)
{
	int i;
	for(i = 0; i < limit; ++i){
		printf("%d ", arr[i]);
	}
	printf("\n\n");
}



/* Conventional insertion sort which swaps adjacent elems */
void insertionSortSwap(int arr[], int sz)
{
	int i, j, key;
	for(i = 1; i < sz; ++i){
		j = i;
		key = arr[i];

		while(j > 0 && arr[j] < arr[j-1]){
			SWAP(&arr[j], &arr[j-1], int);
			--j;
		}

		arr[j] = key;
	}
}


/* ALERT: Java Programmers please DO NOT proceed!!! 
Do so at your own caution!!!

Insertion sort without swapping adjacent elements.

Instead of swapping adjacent elements one by one after each 
comparison, just make use of the comparisons to find out the final 
position of our key in question, starting from the second element 
of the array.

Instead of starting from the element in question and bubbling it
to its final position of the list when we encounter a "larger"
predecessor, just find out it's final position and use memmove
to shift the affected region at one go.


Now show me how you do memmove or copy raw memory using Java
without using additional buffers.
*/

void insertionSort(int arr[], int sz)
{
	int i, j;
	int key;
	for(i = 1; i < sz; ++i){
		key = arr[i];
		j = i-1;
		while(j >= 0 && key < arr[j]){
			--j;
		}
		
		/* Final position of key is j+1

		Shift arr[j+1 ... i-1] to arr[j+2 ... i] since 
		j+1 is our key's final position and i WAS it's 
		original pos. Hence, this subarray covers the
		affected region.

		Total of i-1-(j+1)+1 = (i-j-1) to be shifted.

		If this looks ugly despite comments, one can use 
		some additional variables to store the data.
		*/
		
		memmove(arr+j+2, arr+j+1, (i-j-1) * sizeof(int));
		arr[j+1] = key;
	}
}
#define SAMP_SZ 10

void testSort()
{
	int arr1[SAMP_SZ] = {7, 1, 13, 5, 9, 2, 64, 8, 11, 1};
	int arr2[SAMP_SZ] = {7, 1, 13, 5, 9, 2, 64, 8, 11, 1};

	insertionSortSwap(arr1, SAMP_SZ);
	insertionSort(arr2, SAMP_SZ);

	printf("First array:\n");
	printArray(arr1, SAMP_SZ);

	printf("Second array:\n");
	printArray(arr2, SAMP_SZ);
}



/* Sort function to be used for profiling.
Remember to change ARR_SZ and MY_MAX #defines at the top!
*/
void profileSort()
{
	int i, flag;
	int *arr1;
	int *arr2;

	arr1 = malloc(ARR_SZ * sizeof(int));
	if(arr1 == NULL){
		printf("Unable to allocate memory.\n");
		return;
	}

	arr2 = malloc(ARR_SZ * sizeof(int));
	if(arr2 == NULL){
		printf("Memory allocation failed.\n");
		free(arr1);
		return;
	}

	for(i = 0; i < ARR_SZ; ++i){
		arr1[i] = rand() % MY_MAX;
		arr2[i] = arr1[i];
	}

	insertionSortSwap(arr1, ARR_SZ);
	insertionSort(arr2, ARR_SZ);

	free(arr1);
	free(arr2);
}
