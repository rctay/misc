#ifndef TEST_FUNCS_H
#define TEST_FUNCS_H

#include "htable.h"

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

testSort - our test driver function
rtestSort - the real test function
profSort - Used on large arrays for profiling purposes.
compareTable - Compares an array to a hash table
compareArrays - Compares 2 arrays elem wise up to cnt elems

*/

void testSort();
enum tsError rtestSort();
void profileSort();
int compareTable(struct HTable *t, int *arr, int sz);
int compareArrays(int *first, int *sec, int sz);

#endif
