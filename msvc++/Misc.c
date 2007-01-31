#include "md.h"

void ExitWithCode (int Code)
{
	printf ("(Exit %d): %s\n", Code, ErrorCodes[Code]);
	fflush (stdout);
	fprintf (fp_rep, "(Exit %d): %s\n", Code, ErrorCodes[Code]);
	fflush (fp_rep);
	exit (Code);
}



int indexed_binary_search (int value, int lo, int hi, int *List, int *index)
{

	while (value != List[index[(hi+lo)/2]]) {
		if ((hi - lo) <= 1)
			return (-1);
			
		if (value > List[index[(hi+lo)/2]])
			lo = (hi+lo)/2;
		else
			hi = (hi+lo)/2;
	}
	
	return ((hi+lo)/2);
}


int binary_search (double value, int lo, int hi, double *List)
{
// return the index [i] within List for which value > List[i] and value <= List[i+1]
	int i;

	if (List[hi] > 0.0000001) {
		while ((value < List[(hi+lo)/2] || value >= List[((hi+lo)/2) + 1]) && (hi - lo >= 1)) {
			if (value > List[(hi+lo)/2])
				lo = (hi+lo)/2;
			else
				hi = (hi+lo)/2;
		}
		i = lo;
		while (value >= List[i+1])
			i++;

		return (i+1);
	}
	else {
		return (-1);
	}
	
	return (-1);
}


int pct (int j, int k)
{
// return j as a percent of k in percentage units.

	return ((int)((100.0*j/((double)k+1.0)) + 0.5));
}

/*
void addRAM (char *function, int size)
{
	int avail;

	Memory += size;
	AvailMemory -= size;

	if (fp_ram != NULL && Ini->RAM_DEBUG_LEVEL > 0) {
		avail = avail_ram();

		fprintf (fp_ram, "%s: adding %.3f MB, total= %.3f MB, avail= %.3f MB, AvailMemory= %.3f MB, system= %.3f MB, difference= %d\n", function, size/(1024.0*1024.0), Memory/(1024.0*1024.0), avail/(1024.0*1024.0), AvailMemory/(1024.0*1024.0), (avail + Memory)/(1024.0*1024.0), (AvailMemory - avail));
		fflush (fp_ram);
	}
}


void relRAM (char *function, int size)
{
	int avail;

	Memory -= size;
	AvailMemory += size;

	if (fp_ram != NULL && Ini->RAM_DEBUG_LEVEL > 0) {
		avail = avail_ram();
		
		fprintf (fp_ram, "%s: releasing %.3f MB, total= %.3f MB, avail= %.3f MB, AvailMemory= %.3f MB, system= %.3f MB, difference= %d\n", function, size/(1024.0*1024.0), Memory/(1024.0*1024.0), avail/(1024.0*1024.0), AvailMemory/(1024.0*1024.0), (avail + Memory)/(1024.0*1024.0), (AvailMemory - avail));
		fflush (fp_ram);
	}
}
*/