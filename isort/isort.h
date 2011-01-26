#ifndef ISORT_H
#define ISORT_H

void insertionSort(int arr[], int sz);
void insertionSortSwap(int arr[], int sz);

// We inline the swap to make it faster
#define SWAP(p, q, type) type tmp = *p; *p = *q; *q = tmp



#endif
