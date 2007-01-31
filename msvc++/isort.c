/*
 *  isort uses the c library qsort() function to return an
 *  array of integer pointers (*index[]) sorted based on the values of
 *  the integer data array (idata[]);
 */
#include "md.h"

#define OFFSET 0		/* set OFFSET to 1 to index from 1 for fortran calling routines, 0 otherwise */

void isort (int *idata, int *index, int elements)
{
	int __cdecl i_compare(const void *, const void *);

	int **indexP, i;

	indexP = (int **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, elements*sizeof(int *));
//	addRAM ("isort", elements*sizeof(int *));


	for (i=0; i < elements; i++)
		indexP[i] = idata+i;

	qsort (indexP, elements, sizeof(int *), i_compare);
        
	for (i=0; i < elements; i++)
		*(index+i) = (int) (indexP[i] - idata + OFFSET);

	HeapFree (heapHandle, 0, indexP);
//	relRAM ("isort", elements*sizeof(int *));
}


int __cdecl i_compare (const void *arg1, const void *arg2)
{

	int **a = (int **)arg1;
	int **b = (int **)arg2;

	return (**a - **b);
}
