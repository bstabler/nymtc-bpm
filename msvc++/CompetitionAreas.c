#include "md.h"

void competition_areas (struct zone_data *ZonalData)
{

	FILE *fp, *fp2;

	int h, i, j;
	int orig, dest;
	int rowTot, colTot, subareaTot;
	int **journeys;
	int **journeys_t;
	int freq[2][2][4];
	double relevance;


	// initialize array for counting frequency of zones (Os and Ds) in each subarea (4)
	for (h=0; h < 2; h++)
		for (i=0; i < 2; i++)
			for (j=0; j < 4; j++)
				freq[h][i][j] = 0;


	journeys = (int **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(int *));
	journeys_t = (int **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(int *));
	for (i=0; i < Ini->MAX_TAZS+1; i++) {
		journeys[i] = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(int));
		journeys_t[i] = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(int));
	}





// open MDSC output journeys file, the file sent to David Rhoden, for reading MDSC output journey orig and dest
	if (strcmp(Ini->MDSCOUTPUTJOURNEYS, "")) {
		if ((fp = fopen(Ini->MDSCOUTPUTJOURNEYS, "rt")) == NULL)
			ExitWithCode(121);
	}
	else
		ExitWithCode (122);


// read journey orig and dest, create o/d matrix of journeys and its transpose
	while ((fscanf(fp, "%*d %d %d %*d %*d %*d %*d", &orig, &dest)) != EOF) {
		journeys[orig][dest] ++;
		journeys_t[dest][orig] ++;
	}
	fclose (fp);



// calculate relevance for origins of outbound journeys outside the subarea using o/d journeys
	for (i=1; i <= Ini->MAX_TAZS; i++) {

		// for zones originating outside subarea
		if (ZonalData->OrigSubAreaOB[i] == 0) {

			// sum journeys originating from zone i
			rowTot = 0;
			for (j=1; j <= Ini->MAX_TAZS; j++)
				rowTot += journeys[i][j];

			// sum journeys originating from zone i, destined to zones j in the subarea
			subareaTot = 0;
			for (j=1; j <= Ini->MAX_TAZS; j++) {
				if (ZonalData->DestSubAreaOB[j] == 1)		// is a destination subarea
					subareaTot += journeys[i][j];
			}

			if (rowTot > 0)
				relevance = (double)subareaTot/rowTot;
			else
				relevance = 0.0;

			if (relevance >= Ini->DEGREE_OF_RELEVANCE)
				ZonalData->OrigSubAreaOB[i] = 2;
			else if ((relevance < Ini->DEGREE_OF_RELEVANCE) && (relevance > 0.0))
				ZonalData->OrigSubAreaOB[i] = 3;
			else
				ZonalData->OrigSubAreaOB[i] = 0;
		}
	}


// calculate relevance for origins of inbound journeys outside the subarea using o/d journeys transpose
	for (i=1; i <= Ini->MAX_TAZS; i++) {

		// for zones originating outside subarea
		if (ZonalData->OrigSubAreaIB[i] == 0) {

			// sum journeys originating from zone i
			rowTot = 0;
			for (j=1; j <= Ini->MAX_TAZS; j++)
				rowTot += journeys_t[i][j];

			// sum journeys originating from zone i, destined to zones j in the subarea
			subareaTot = 0;
			for (j=1; j <= Ini->MAX_TAZS; j++) {
				if (ZonalData->DestSubAreaIB[j] == 1)		// is a destination subarea
					subareaTot += journeys_t[i][j];
			}

			if (rowTot > 0)
				relevance = (double)subareaTot/rowTot;
			else
				relevance = 0.0;

			if (relevance >= Ini->DEGREE_OF_RELEVANCE)
				ZonalData->OrigSubAreaIB[i] = 2;
			else if ((relevance < Ini->DEGREE_OF_RELEVANCE) && (relevance > 0.0))
				ZonalData->OrigSubAreaIB[i] = 3;
			else
				ZonalData->OrigSubAreaIB[i] = 0;
		}
	}





// calculate relevance for destinations of outbound journeys outside subarea using o/d journeys
	for (j=1; j <= Ini->MAX_TAZS; j++) {

		// for zones destined outside subarea
		if (ZonalData->DestSubAreaOB[j] == 0) {

			// sum journeys destined for zone j
			colTot = 0;
			for (i=1; i <= Ini->MAX_TAZS; i++)
				colTot += journeys[i][j];

			// sum journeys destined for zone j, originating from the subarea
			subareaTot = 0;
			for (i=1; i <= Ini->MAX_TAZS; i++) {
				if (ZonalData->OrigSubAreaOB[i] == 1)		// is an origin subarea
					subareaTot += journeys[i][j];
			}

			if (colTot > 0)
				relevance = (double)subareaTot/colTot;
			else
				relevance = 0.0;

			if (relevance >= Ini->DEGREE_OF_RELEVANCE)
				ZonalData->DestSubAreaOB[j] = 2;
			else if ((relevance < Ini->DEGREE_OF_RELEVANCE) && (relevance > 0.0))
				ZonalData->DestSubAreaOB[j] = 3;
			else
				ZonalData->DestSubAreaOB[j] = 0;
		}
	}


// calculate relevance for destinations of outbound journeys outside subarea using o/d journeys transpose
	for (j=1; j <= Ini->MAX_TAZS; j++) {

		// for zones destined outside subarea
		if (ZonalData->DestSubAreaIB[j] == 0) {

			// sum journeys destined for zone j
			colTot = 0;
			for (i=1; i <= Ini->MAX_TAZS; i++)
				colTot += journeys_t[i][j];

			// sum journeys destined for zone j, originating from the subarea
			subareaTot = 0;
			for (i=1; i <= Ini->MAX_TAZS; i++) {
				if (ZonalData->OrigSubAreaIB[i] == 1)		// is an origin subarea
					subareaTot += journeys_t[i][j];
			}

			if (colTot > 0)
				relevance = (double)subareaTot/colTot;
			else
				relevance = 0.0;

			if (relevance >= Ini->DEGREE_OF_RELEVANCE)
				ZonalData->DestSubAreaIB[j] = 2;
			else if ((relevance < Ini->DEGREE_OF_RELEVANCE) && (relevance > 0.0))
				ZonalData->DestSubAreaIB[j] = 3;
			else
				ZonalData->DestSubAreaIB[j] = 0;
		}
	}



	// initialize array for counting frequency of zones (Os and Ds) in each subarea (4)
	for (h=0; h < 2; h++)
		for (i=0; i < 2; i++)
			for (j=0; j < 4; j++)
				freq[h][i][j] = 0;


	// tabulate the frequencies
	for (i=1; i <= Ini->MAX_TAZS; i++) {
		freq[0][0][ZonalData->OrigSubAreaOB[i]] ++;
		freq[1][0][ZonalData->OrigSubAreaIB[i]] ++;
		freq[0][1][ZonalData->DestSubAreaOB[i]] ++;
		freq[1][1][ZonalData->DestSubAreaIB[i]] ++;
	}


	// report the frequencies
//	printf ("\nFrequency of TAZs in each Outbound Sub-area\n");
//	printf ("Sub-area     No. Origin TAZs     No. Destination TAZs\n");
//	printf ("-----------------------------------------------------\n");
//	printf ("In Sub-area      %11d %24d\n", freq[0][0][1], freq[0][1][1]);
//	printf ("Hi Competition   %11d %24d\n", freq[0][0][2], freq[0][1][2]);
//	printf ("Lo Competition   %11d %24d\n", freq[0][0][3], freq[0][1][3]);
//	printf ("No Consideration %11d %24d\n", freq[0][0][0], freq[0][1][0]);
//	printf ("-----------------------------------------------------\n");
//	printf ("Total            %11d %24d\n\n", freq[0][0][0]+freq[0][0][1]+freq[0][0][2]+freq[0][0][3], freq[0][1][0]+freq[0][1][1]+freq[0][1][2]+freq[0][1][3]);
	
//	printf ("\nFrequency of TAZs in each Inbound Sub-area\n");
//	printf ("Sub-area     No. Origin TAZs     No. Destination TAZs\n");
//	printf ("-----------------------------------------------------\n");
//	printf ("In Sub-area      %11d %24d\n", freq[1][0][1], freq[1][1][1]);
//	printf ("Hi Competition   %11d %24d\n", freq[1][0][2], freq[1][1][2]);
//	printf ("Lo Competition   %11d %24d\n", freq[1][0][3], freq[1][1][3]);
//	printf ("No Consideration %11d %24d\n", freq[1][0][0], freq[1][1][0]);
//	printf ("-----------------------------------------------------\n");
//	printf ("Total            %11d %24d\n\n", freq[1][0][0]+freq[1][0][1]+freq[1][0][2]+freq[1][0][3], freq[1][1][0]+freq[1][1][1]+freq[1][1][2]+freq[1][1][3]);

	fprintf (fp_rep, "\nFrequency of TAZs in each Outbound Sub-area\n");
	fprintf (fp_rep, "Sub-area     No. Origin TAZs     No. Destination TAZs\n");
	fprintf (fp_rep, "-----------------------------------------------------\n");
	fprintf (fp_rep, "In Sub-area      %11d %24d\n", freq[0][0][1], freq[0][1][1]);
	fprintf (fp_rep, "Hi Competition   %11d %24d\n", freq[0][0][2], freq[0][1][2]);
	fprintf (fp_rep, "Lo Competition   %11d %24d\n", freq[0][0][3], freq[0][1][3]);
	fprintf (fp_rep, "No Consideration %11d %24d\n", freq[0][0][0], freq[0][1][0]);
	fprintf (fp_rep, "-----------------------------------------------------\n");
	fprintf (fp_rep, "Total            %11d %24d\n\n", freq[0][0][0]+freq[0][0][1]+freq[0][0][2]+freq[0][0][3], freq[0][1][0]+freq[0][1][1]+freq[0][1][2]+freq[0][1][3]);
	
	fprintf (fp_rep, "\nFrequency of TAZs in each Inbound Sub-area\n");
	fprintf (fp_rep, "Sub-area     No. Origin TAZs     No. Destination TAZs\n");
	fprintf (fp_rep, "-----------------------------------------------------\n");
	fprintf (fp_rep, "In Sub-area      %11d %24d\n", freq[1][0][1], freq[1][1][1]);
	fprintf (fp_rep, "Hi Competition   %11d %24d\n", freq[1][0][2], freq[1][1][2]);
	fprintf (fp_rep, "Lo Competition   %11d %24d\n", freq[1][0][3], freq[1][1][3]);
	fprintf (fp_rep, "No Consideration %11d %24d\n", freq[1][0][0], freq[1][1][0]);
	fprintf (fp_rep, "-----------------------------------------------------\n");
	fprintf (fp_rep, "Total            %11d %24d\n\n", freq[1][0][0]+freq[1][0][1]+freq[1][0][2]+freq[1][0][3], freq[1][1][0]+freq[1][1][1]+freq[1][1][2]+freq[1][1][3]);

	//fflush(stdout);
	fflush(fp_rep);



	// if file specified in .ini, open and write TAZs with codes
	//		1=in subarea, 2=in hi competition area, 3=in low competition area, 0=outside consideration
	if (strcmp(Ini->SUBAREA_OUTPUT, "")) {

		// just open it.
		// no need to check since if the file could not be opened, the program would have bombed in get_input_data().
		fp2 = fopen(Ini->SUBAREADATA, "rt");

		if ((fp = fopen(Ini->SUBAREA_OUTPUT, "wt")) == NULL)
			ExitWithCode(139);

		// first echo contents of subarea definitions file
		echo_ini (fp2, fp, Ini->SUBAREADATA);
		fprintf (fp, "\n\n\n");

		// print subarea codes by TAZ
		fprintf (fp, "      TAZ  OB_Orig  OB_Dest  IB_Orig  IB_Dest   (1=in subarea, 2=high competition, 3=low competition, 0=not considered)\n");
		for (i=1; i <= Ini->MAX_TAZS; i++)
			fprintf (fp, "%9d %8d %8d %8d %8d\n", i, ZonalData->OrigSubAreaOB[i], ZonalData->DestSubAreaOB[i],
				ZonalData->OrigSubAreaIB[i], ZonalData->DestSubAreaIB[i]);
	}
	fclose (fp);
	fclose (fp2);


	HeapFree (heapHandle, 0, journeys);
	HeapFree (heapHandle, 0, journeys_t);
}