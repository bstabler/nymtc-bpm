#include "md.h"

void get_journey_data (FILE *fp_work[], struct zone_data *ZonalData, struct journey_attribs *JourneyAttribs, int **TotProds)
{

	FILE *fp;


	
	// open and read journey records file.
	printf ("Reading journey records input file:\n");
	if (strcmp(Ini->INJOURNEYDATA, "")) {
		if ((fp = fopen(Ini->INJOURNEYDATA, "rt")) == NULL)
			ExitWithCode(5);
	}
	else
		ExitWithCode (33);


	// count journey records so we know how large to make the journey_attribs data structure
	printf ("counting the number of %s journey records...\n", PurposeLabels[Ini->PURPOSE_TO_PROCESS]);
	count_kpmg_journey_records (fp);
	rewind (fp);


	// dimension actual data structure to NUMBER_JOURNEYS determined from counting journey records file.
//	printf ("     allocating final space for %d journeys...\n", Ini->NUMBER_JOURNEYS);
	(*JourneyAttribs).persno		= (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(short));
	(*JourneyAttribs).orig			= (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(short));
	(*JourneyAttribs).purpose		= (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(short));
	(*JourneyAttribs).autos			= (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(short));
	(*JourneyAttribs).workers		= (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(short));
	(*JourneyAttribs).children		= (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(short));
	(*JourneyAttribs).nwas			= (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(short));
	(*JourneyAttribs).income		= (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(short));
	(*JourneyAttribs).person_type	= (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(short));
	(*JourneyAttribs).walk_orig		= (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(short));
	(*JourneyAttribs).aa			= (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(short));
	(*JourneyAttribs).ak			= (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(short));
	(*JourneyAttribs).at_work_mode	= (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(short));
	(*JourneyAttribs).frozen		= (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(short));
	(*JourneyAttribs).hh			= (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(int));
	(*JourneyAttribs).packet		= (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(int));
	(*JourneyAttribs).haj			= (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(int));
//	addRAM ("get_journey_data", 3*Ini->NUMBER_JOURNEYS*sizeof(int) + 13*Ini->NUMBER_JOURNEYS*sizeof(short));


	// read journey records
	printf ("reading journey attributes...\n");
	read_kpmg_journey_records (fp, fp_work, ZonalData, JourneyAttribs, TotProds);
	fclose (fp);

	printf ("done reading %d journey records from HAJ output file.\n", Ini->NUMBER_JOURNEYS);
}
