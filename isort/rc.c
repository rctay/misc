#include <stdio.h>
#include <stdlib.h>

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

int main() {
	int *input, size;
	int ret = 0;
	if ((ret = read_loop(&input)) < 0) {
		fprintf(stderr, "read loop failed\n");
		return ret;
	}

	size = ret;
	return 0;
}
