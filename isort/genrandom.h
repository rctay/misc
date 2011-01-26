#ifndef GEN_RANDOM_H
#define GEN_RANDOM_H

/* Given a filename, generates sz integers in the range of 
0 to max_elem - 1 in the file

Returns -1 on failure, 0 on success.
*/
int genRandom(const char *filename, int sz, int max_elem);

#endif
