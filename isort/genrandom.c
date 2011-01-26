#include "genrandom.h"
#include <stdio.h>
#include <stdlib.h>

/* Opens a file supplied by file name and generates sz integers 
in range of 0 to max_elem - 1 in the file 

Returns -1 on error, 0 on success*/

int genRandom(const char *fileName, int sz, int max_elem)
{
	srand(time(0));
	FILE *outfile = fopen(fileName, "w");

	if(outfile == NULL){
		return -1;
	}

	int i, randNum;
	size_t elemSz = sizeof(int);
	for(i = 0; i < sz; ++i){
		randNum = rand() % max_elem;
		fwrite(&randNum, elemSz, 1, outfile);
	}

	fclose(outfile);
	return 0;
}
