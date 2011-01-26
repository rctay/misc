#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "genrandom.h"
#include "testfuncs.h"
#include "isort.h"

/* CREDITS to rc who made me think of spending a little time
doing this. I probably learnt more things in this process.
*/


/* 

------------------ INSTRUCTIONS ---------------------------
Makefile is now available!

Just:
$make




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


