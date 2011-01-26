#include <string.h>

#include "isort.h"

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
