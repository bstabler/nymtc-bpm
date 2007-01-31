/*
 *	At work journey mode choice logsums.
 *
 *  Read the three files written when running work journeys for each of the three income groups.
 *  These files contain household id, person id, and destination taz of the work place.
 *  Match these records by household/person to the at-work journeys.  The work destinations
 *  become origins for the at-work journeys.  The at-work-mode is 1 if drive alone, 0 otherwise.
 */

#include "md.h"

void atwork_linkages (FILE *fp[], struct journey_attribs *JourneyAttribs)
{
//	int hh, persno, i, k, work_journeys, atwork_hh_pers, temp_len, no_link=0;
	int hh, persno, i, k, work_journeys, atwork_hh_pers, no_link=0;
	int *hh_pers, *index;
	short *dest, *mode;
//	char temp[256];


	hh_pers = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->MAX_WORK_JOURNEYS*sizeof(int));
	index =   (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->MAX_WORK_JOURNEYS*sizeof(int));
	dest =  (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->MAX_WORK_JOURNEYS*sizeof(short));
	mode =  (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->MAX_WORK_JOURNEYS*sizeof(short));
//	addRAM ("atwork_linkages", 2*Ini->MAX_WORK_JOURNEYS*sizeof(int) + 2*Ini->MAX_WORK_JOURNEYS*sizeof(short));

	// read low income work journeys.
	k = 0;
	printf ("reading low income work journey destinations...\n");
	while ((fscanf (fp[0], "%d %d %d %d", &hh, &persno, &dest[k], &mode[k])) != EOF) {
		hh_pers[k] = hh*MAX_PERSONS_PER_HH + persno;
		index[k] = k;
		k++;
	}

	// read medium income work journeys.
	printf ("reading medium income work journey destinations...\n");
	while ((fscanf (fp[1], "%d %d %d %d", &hh, &persno, &dest[k], &mode[k])) != EOF) {
		hh_pers[k] = hh*MAX_PERSONS_PER_HH + persno;
		k++;
	}

	// read high income work journeys.
	printf ("reading high income work journey destinations...\n");
	while ((fscanf (fp[2], "%d %d %d %d", &hh, &persno, &dest[k], &mode[k])) != EOF) {
		hh_pers[k] = hh*MAX_PERSONS_PER_HH + persno;
		k++;
	}
	work_journeys = k;


	// sort all work journeys by household id and person number
	isort (hh_pers, index, work_journeys);


	// go through atwork journeys, lookup corresponding work journey, assign dest and mode.
	for (i=0; i < Ini->NUMBER_JOURNEYS; i++) {
		// i is the at-work journey number
		// k is the work journey number

		atwork_hh_pers = JourneyAttribs->hh[i]*MAX_PERSONS_PER_HH + JourneyAttribs->persno[i];
		k = indexed_binary_search (atwork_hh_pers, 0, work_journeys, hh_pers, index);
		if (k < 0) {
			// k is negative if corresponding work journey was not found
			no_link++;
			JourneyAttribs->at_work_mode[i] = -1;
		}
		else {
			JourneyAttribs->orig[i] = dest[k];
			JourneyAttribs->at_work_mode[i] = mode[k];

			// corresponding work journey is unfrozen by its presence in the SUBAREA_ATWORK_**_FILE file
			// so this atwork journey should also be unfrozen regardless of where its orig and dest are located.
			if (Ini->SUBAREA_ANALYSIS == 1) {
				JourneyAttribs->frozen[i] = 0;
			}
		}

//		sprintf (temp, "at-work journey %d linkages %3d%% complete", i, 100*i/Ini->NUMBER_JOURNEYS);
//		temp_len = (int)strlen(temp);
//		for (k=0; k < temp_len; k++)
//			sprintf (&temp[temp_len + k], "\b");
//		printf ("%s", temp);
	}
	printf ("at-work journey %d linkages complete\n", i);
	printf ("%d at-work journeys had no corresponding work journey\n", no_link);
	
	
	HeapFree (heapHandle, 0, hh_pers);
	HeapFree (heapHandle, 0, index);
	HeapFree (heapHandle, 0, dest);
	HeapFree (heapHandle, 0, mode);
//	relRAM ("atwork_linkages", 2*Ini->MAX_WORK_JOURNEYS*sizeof(int) + 2*Ini->MAX_WORK_JOURNEYS*sizeof(short));
}
