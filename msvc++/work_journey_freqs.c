#include "md.h"


void work_jrny_freqs (struct journey_attribs *JourneyAttribs, short **hh_freqs, short ***hh_pers_freqs, int **hh2id)
{

	int i, k, hh;
	int max_hh, next;
	int hh_index, pers_index;


// get max hh number to dimension correspndence array
	max_hh  = 0;
	for (k=0; k < Ini->NUMBER_JOURNEYS; k++)
		if (JourneyAttribs->hh[k] > max_hh)
			max_hh = JourneyAttribs->hh[k];

// allocate memory for hh to id correspondence array.
	(*hh2id) = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (max_hh+1)*sizeof(int));

// generate correspondence
	next = 0;
	for (k=0; k < Ini->NUMBER_JOURNEYS; k++) {
		hh = JourneyAttribs->hh[k];
		if ((*hh2id)[hh] == 0) {
			(*hh2id)[hh] = next;
			next++;
		}
	}


// Memory allocations
	(*hh_freqs) = (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, next*sizeof(short));
	(*hh_pers_freqs) = (short **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 1*sizeof(short *));
	for (i=0; i < 1; i++)
		(*hh_pers_freqs)[i] = (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 10*(next+1)*sizeof(short));


// tally up at-work journeys per person and school journeys per household
	for (k=0; k < Ini->NUMBER_JOURNEYS; k++) {
		hh_index = (*hh2id)[JourneyAttribs->hh[k]];
		pers_index = 10*hh_index + JourneyAttribs->persno[k];
		if (JourneyAttribs->purpose[k] == 4)
			(*hh_freqs)[hh_index]++;
		if (JourneyAttribs->purpose[k] == 7)
			(*hh_pers_freqs)[0][pers_index]++;
	}


//	addRAM ("work_jrny_freqs", (max_hh+1)*sizeof(int) + next*sizeof(short) + 10*(next+1)*sizeof(short));

}


