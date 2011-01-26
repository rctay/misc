#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "testfuncs.h"
#include "htable.h"
#include "isort.h"


/* !!! IMPT NOTE !!!
Change these 2 constants as you wish. 
They are for PROFILING purposes using gprof, called in the 
profileSort() function.

PROF_MAX_ELEM is an upper bound on our random numbers generation.
PROF_ARR_SZ is the array size */
#define PROF_MAX_ELEM 100000
#define PROF_ARR_SZ 100000



/* Tests the insertion sort functions */
void testSort()
{
	enum tsError ret;
	ret = rtestSort();

	switch(ret){
		case 0:
			fprintf(stderr, "Test passed.\n");
			break;

		case GEN_ERR:
			fprintf(stderr, "Error generating random number file.\n");
			break;

		case FILE_ERR:
			fprintf(stderr, "Error opening file for reading in "
			"random numbers.\n");
			break;

		case ELEM_SZ_ERR:
			fprintf(stderr, "Incorrect number of elements read.\n");
			break;

		case HASH_ERR:
			fprintf(stderr, "Array and hash table entries do not match.\n");
			break;

		case MEM_ERR:
			fprintf(stderr, "Memory allocation error.\n");
			break;

		case FIRST_SORT_ERR:
			fprintf(stderr, "Conventional insertion sort failed.\n");
			break;

		case SEC_SORT_ERR:
			fprintf(stderr, "Modified insertion sort failed.\n");
			break;
		
		case DIFF_ERR:
			fprintf(stderr, "The result of the 2 sorts are different.\n");
			break;

		default:
			fprintf(stderr, "Unknown error. This is not supposed to "
			"happen.\n");
			break;
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


/* Sort function to be used for profiling.
Remember to change ARR_SZ and MY_MAX #defines at the top!
*/
void profileSort()
{
	int i, flag;
	int *arr1;
	int *arr2;

	arr1 = malloc(PROF_ARR_SZ * sizeof(int));
	if(arr1 == NULL){
		fprintf(stderr, "Unable to allocate memory.\n");
		return;
	}

	arr2 = malloc(PROF_ARR_SZ * sizeof(int));
	if(arr2 == NULL){
		fprintf(stderr, "Memory allocation failed.\n");
		free(arr1);
		return;
	}

	for(i = 0; i < PROF_ARR_SZ; ++i){
		arr1[i] = rand() % PROF_MAX_ELEM;
		arr2[i] = arr1[i];
	}

	insertionSortSwap(arr1, PROF_ARR_SZ);
	insertionSort(arr2, PROF_ARR_SZ);

	free(arr1);
	free(arr2);
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


