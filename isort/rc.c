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

void run_sort(int *to_sort, int *buf, int size, void (*sort_func)(int to_sort[], int size)) {
	copy_array(to_sort, buf, size);
	(*sort_func)(buf, size);
	print_array(buf, size);
}

int main() {
	int *input, *buf, size;
	int ret = 0;
	if ((ret = read_loop(&input)) < 0) {
		fprintf(stderr, "read loop failed\n");
		goto abort_read_loop;
	}

	size = ret;
	buf = malloc(size * sizeof(int));
	if (!buf) {
		fprintf(stderr, "malloc failed\n");
		ret = -1;
		goto abort_buf;
	}

	run_sort(input, buf, size, insertionSortSwap);
	run_sort(input, buf, size, insertionSort);

	free(buf);
abort_buf:
	free(input);
abort_read_loop:
	return ret;
}
