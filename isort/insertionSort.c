#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"
#include "genrandom.h"

/* CREDITS to rc who made me think of spending a little time
doing this. I probably learnt more things in this process.
*/


/* 

------------------ INSTRUCTIONS ---------------------------
Now updated with hash table and test functions.

Compile like this:

$gcc -o [whateverfile] insertionSort.c htable.c genrandom.c




------------------ SUMMARY -------------------------------
This program has 2 insertion sort functions:

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


/* tsError, for errors in rtestSort() function */
enum tsError{
	GEN_ERR = 1, // Error generating random number file
	FILE_ERR = 2, // Error opening file
	ELEM_SZ_ERR = 4, // Wrong number of random numbers read in
	HASH_ERR = 8, // Error comparing hash table to first array
	MEM_ERR = 16, // Error allocating memory
	FIRST_SORT_ERR = 32, // First insertion sort fails
	SEC_SORT_ERR = 64, // Second insertion sort fails
	DIFF_ERR = 128 
};


/* Testing functions:

testSort: our test driver function
rtestSort: the real test function
profSort: Used on large arrays for profiling purposes.
compareTable: Compares an array to a hash table
compareArrays: Compares 2 arrays elem wise up to cnt elems
*/

void testSort();
enum tsError rtestSort();
void profileSort();
int compareTable(struct HTable *t, int *arr, int sz);
int compareArrays(int *first, int *sec, int sz);


int main(void)
{
	/* Uncomment the line below this section to use the test function
	which show the 2 insertion sorts working on small arrays.*/

	testSort();


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



/* Tests the insertion sort functions */
void testSort()
{
	enum tsError ret;
	ret = rtestSort();

	if(ret == 0){
		printf("Test passed.\n");
		return;
	}

	if(ret & GEN_ERR){
		printf("Error generating random number file.\n");
		return;
	}

	if(ret & FILE_ERR){
		printf("Error opening file for reading in random numbers.\n");
		return;
	}

	if(ret & ELEM_SZ_ERR){
		printf("Incorrect number of random numbers read in.\n");
		return;
	}

	if(ret & HASH_ERR){
		printf("Comparison between array and hash table after "
		"reading in the infile failed.\n");
		return;
	}

	if(ret & MEM_ERR){
		printf("Error allocating memory for secondary array.\n");
		return;
	}

	if(ret & FIRST_SORT_ERR){
		printf("First insertion sort failed.\n");
		return;
	}

	if(ret & SEC_SORT_ERR){
		printf("Second insertion sort failed.\n");
		return;
	}

	if(ret & DIFF_ERR){
		printf("The result of the 2 sorts are different.\n");
		return;
	}
}


/* Tests our insertion sort algorithms */
enum tsError rtestSort()
{
	/* TEST_NUM_ELEMS - number of random numbers we will generate.
	TEST_MAX_ELEM - Maximum size of elements 
	HTABLE_SZ - size of hash table */
	#define TEST_NUM_ELEMS 10000
	#define TEST_MAX_ELEM 10000
	#define HTABLE_SZ 4567

	int i, elem, cnt;
	size_t elemSz;
	enum tsError ret;
	memset(&ret, 0, sizeof(enum tsError));

	const char *inputFile = "randNums.txt";
	if(genRandom(inputFile, TEST_NUM_ELEMS, TEST_MAX_ELEM) != 0){
		ret |= GEN_ERR;
		return ret;
	}

	FILE *infile = fopen(inputFile, "r");

	if(infile == NULL){
		fprintf(stderr, "Unable to open %s for reading.\n", inputFile);
		ret |= FILE_ERR;
		return ret;
	}


	/* read the TABLE_SZ random numbers from randNums.txt into 
	myArr and the hash table.
	
	The hash table is used for verification purposes */

	int myArr[TEST_NUM_ELEMS];
	int *copyArr = NULL;
	struct HTable *myTable;

	myTable = HashNew(HTABLE_SZ, HashFn);
	if(myTable == NULL){
		ret |= MEM_ERR;
		return ret;
	}


	cnt = 0;
	elemSz = sizeof(int);
	while(fread(&elem, elemSz, 1, infile) > 0 && cnt < TEST_NUM_ELEMS){
		HashInsert(myTable, elem);
		myArr[cnt++] = elem;
	}

	
	/* Incorrect number of random numbers read in */
	if(cnt != TEST_NUM_ELEMS){
		ret |= ELEM_SZ_ERR;
		goto cleanup;
	}

	/* Missing entries in hash table (Shouldn't happen) */
	if(compareTable(myTable, myArr, cnt) != 0){
		ret |= HASH_ERR;
		goto cleanup;
	}

	copyArr = malloc(cnt * sizeof(int));
	if(copyArr == NULL){
		ret |= MEM_ERR;
		goto cleanup;
	}

	/* Initialize copy array */
	for(i = 0; i < cnt; ++i){
		copyArr[i] = myArr[i];
	}

	/* First insertion sort algorithm */
	insertionSortSwap(myArr, cnt);
	if(compareTable(myTable, myArr, cnt) != 0){
		ret |= FIRST_SORT_ERR;
		goto cleanup;
	}

	/* Second insertion sort algorithm */
	insertionSort(copyArr, cnt);
	if(compareTable(myTable, copyArr, cnt) != 0){
		ret |= SEC_SORT_ERR;
		goto cleanup;
	}


	/* Compare the 2 arrays */
	if(compareArrays(myArr, copyArr, cnt) != 0){
		ret |= DIFF_ERR;
	}


cleanup:
	/* I hope the Velociraptor doesn't come */
	if(copyArr){
		free(copyArr);
	}

	HashFree(myTable);
	fclose(infile);
	return ret;
}


/* Compares an array against a hash table
This is assuming the hash table only contains the array's entries */
int compareTable(struct HTable *t, int *arr, int sz)
{
	int i, ret;
	ret = 0;

	for(i = 0; i < sz; ++i){
		/* NOTE: using HashUpdateSearch here to update the counter 
		field inside each node for search hits */
		if(HashUpdateSearch(t, arr[i]) != 0){
			ret = -1;
			goto cleanup;
		}
	}

	/* Checks whether counter == total for all nodes in table */
	if(HashCheck(t) != 0){
		ret = -1;
		goto cleanup;
	}

cleanup:
	HashCntReset(t);
	return ret;
}


/* Does a pairwise comparison of first and sec arrays up to sz elems.
Returns 0 on success, -1 on failure */
int compareArrays(int *first, int *sec, int sz)
{
	int i;
	for(i = 0; i < sz; ++i){
		if(first[i] != sec[i]){
			return -1;
		}
	}

	return 0;
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
