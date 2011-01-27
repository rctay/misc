#include <stdio.h>
#include <stdlib.h>

#include "isort.h"

int read_loop(int **input) {
	int i = 0,
		size,
		read;

	scanf("%d", &read);
	size = i = read;

	*input = malloc(read * sizeof(int));
	if (!*input) {
		fprintf(stderr, "malloc failed\n");
		return -1;
	}

	while (i && scanf("%d", &read) == 1) {
		switch (read) {
		case '\r':
		case '\n':
			continue;
		default:
			*((*input)++) = read;
			i--;
		}
	}

	/* rewind */
	*input -= size;

	return size;
}

void copy_array(const int *from, int *dest, int size) {
	int i;

	for (i = 0; i < size; i++)
		*(dest++) = *(from++);
}

void print_array(int *input, int size) {
	int i;
	for (i = 0; i < size; i++)
		printf("%s%d", i ? "," : "", *(input++));
	printf("\n");
}

int main() {
	int *input, *to_sort, size;
	int ret = 0;
	if ((ret = read_loop(&input)) < 0) {
		fprintf(stderr, "read loop failed\n");
		goto abort_read_loop;
	}

	size = ret;
	to_sort = malloc(size * sizeof(int));
	if (!to_sort) {
		fprintf(stderr, "malloc failed\n");
		ret = -1;
		goto abort_to_sort;
	}

	copy_array(input, to_sort, size);
	insertionSortSwap(to_sort, size);
	print_array(to_sort, size);

	free(to_sort);
abort_to_sort:
	free(input);
abort_read_loop:
	return ret;
}
