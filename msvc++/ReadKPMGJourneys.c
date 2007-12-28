#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md.h"

#define JOURNEY_RECORD_LENGTH 44+2
#define SEQ_START 1
#define SEQ_LENGTH 10
#define CLUSTER_START 11
#define CLUSTER_LENGTH 10
#define OTAZ_START 21
#define OTAZ_LENGTH 6
#define WALK_ORIG_START 27
#define WALK_ORIG_LENGTH 2
#define INCOME_START 29
#define INCOME_LENGTH 2
#define PERSONS_START 31
#define PERSONS_LENGTH 2
#define WORKERS_START 33
#define WORKERS_LENGTH 2
#define CHILDREN_START 35
#define CHILDREN_LENGTH 2
#define NWADULTS_START 37
#define NWADULTS_LENGTH 2
#define AUTOS_START 39
#define AUTOS_LENGTH 2
#define HH_JRNYS_START 41
#define HH_JRNYS_LENGTH 4
#define PERSNO_START 1
#define PERSNO_LENGTH 2
#define PERSON_TYPE_START 3
#define PERSON_TYPE_LENGTH 2
#define JRNY_TYPE_START 5
#define JRNY_TYPE_LENGTH 2




void read_kpmg_journey_records (FILE *fp, FILE *fp_work[], struct zone_data *ZonalData, struct journey_attribs *JourneyAttribs, int **TotProds)
{

	FILE *fp1, *fp2;

	int dummy=0;


	int h, i, j, k, seq, orig, dest, walk_orig, income, workers, children, nwas, autos, persno, hh;
	int haj, purpose, hh_jrnys, person_type, jrny_type;
	int numAutosRestricted;
	int num_journeys=0, num_households=0;
	int start_k, adult_maint, adult_discr, child_maint, child_discr;
	int max_seq, max_pers, mdsc_journeys, input_journeys;
	int work_journeys, work_journeys_unfrozen;
	int linked_work_mode, linked_work_dest, not_linked=0;
	int *Os, *Ds;
	int *hh_pers, *hh_pers_unfrozen;
	int *index, *index_unfrozen;
	int freq[2][2][4];
	short *work_dest, *work_mode, *work_dest_unfrozen, *work_mode_unfrozen;
	char JourneyRecord[JOURNEY_RECORD_LENGTH+1];
	char temp[LRECL];



	// check that only one household auto restriction policy is being evaluated.
	// must be full hh restriction or individual car restriction, or neither.  Cannot be both.
	if ( Ini->PCT_HHS_WITH_FULL_AUTO_RESTRICTION > 0.0 && Ini->PCT_LP_RESTRICTION > 0.0 ) {

			fprintf (fp_rep, "only one retriction of household autos should be specified in one model run.\n");
			fprintf (fp_rep, "PCT_HHS_WITH_FULL_AUTO_RESTRICTION = %.5f and PCT_LP_RESTRICTION = %.5f.\n", Ini->PCT_HHS_WITH_FULL_AUTO_RESTRICTION, Ini->PCT_LP_RESTRICTION);
			fprintf (fp_rep, "only 1 of the two, or neither should be > 0.0 for a model run.\n");
			fprintf (fp_rep, "exit (-234).\n\n");
			fflush (fp_rep);
			printf ("only one retriction of household autos should be specified in one model run.\n");
			printf ("PCT_HHS_WITH_FULL_AUTO_RESTRICTION = %.5f and PCT_LP_RESTRICTION = %.5f.\n", Ini->PCT_HHS_WITH_FULL_AUTO_RESTRICTION, Ini->PCT_LP_RESTRICTION);
			printf ("only 1 of the two, or neither should be > 0.0 for a model run.\n");
			printf ("exit (-234).\n\n");
			fflush (stdout);
			exit (-234);

	}




	max_seq=0;
	max_pers=0;

	// Open journey sequence file for debugging purposes.  Optional file.
	fp2 = NULL;
	if (Ini->SUBAREA_ANALYSIS == 1) {
		if (strcmp(Ini->SUBAREA_JOURNEYSEQUENCE, "")) {
			if ((fp2 = fopen(Ini->SUBAREA_JOURNEYSEQUENCE, "w")) == NULL)
				ExitWithCode(140);

			fprintf (fp2, "%15s %9s %9s %9s %9s %5s\n", "input_journeys", "k", "haj", "frozen", "seq", "i");
		}
	}
	else {
		if (strcmp(Ini->JOURNEYSEQUENCE, "")) {
			if ((fp2 = fopen(Ini->JOURNEYSEQUENCE, "w")) == NULL)
				ExitWithCode(141);

			fprintf (fp2, "%15s %9s %9s %9s %9s %5s\n", "input_journeys", "k", "haj", "frozen", "seq", "i");
		}
	}



	if (Ini->SUBAREA_ANALYSIS == 1) {
	
		Os = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_JOURNEYS+1)*sizeof(int));
		Ds = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_JOURNEYS+1)*sizeof(int));


		// open MDSC output journeys file, the file sent to David Roden, for reading MDSC output journey orig and dest
		if (strcmp(Ini->MDSCOUTPUTJOURNEYS, "")) {
			if ((fp1 = fopen(Ini->MDSCOUTPUTJOURNEYS, "rt")) == NULL)
				ExitWithCode(121);
		}
		else
			ExitWithCode (122);


		// read journey orig and dest
		i = 0;
		while ((fscanf(fp1, "%d %d %d %*d %*d %*d %*d", &k, &orig, &dest)) != EOF) {
			Os[k] = orig;
			Ds[k] = dest;
			i++;
		}
		fclose (fp1);
		mdsc_journeys = i;

	}
	

	

	if (Ini->PURPOSE_TO_PROCESS == 7) {

		hh_pers = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->MAX_WORK_JOURNEYS*sizeof(int));
		index = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->MAX_WORK_JOURNEYS*sizeof(int));
		work_dest =  (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->MAX_WORK_JOURNEYS*sizeof(short));
		work_mode =  (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->MAX_WORK_JOURNEYS*sizeof(short));

		// read work destinations for complete set of work journeys
		// read low income work journeys.
		k = 0;
		printf ("reading low income work journey destinations...\n");
		while ((fscanf (fp_work[0], "%d %d %d %d", &hh, &persno, &work_dest[k], &work_mode[k])) != EOF) {
			hh_pers[k] = hh*MAX_PERSONS_PER_HH + persno;
			index[k] = k;
			k++;
		}

		// read medium income work journeys.
		printf ("reading medium income work journey destinations...\n");
		while ((fscanf (fp_work[1], "%d %d %d %d", &hh, &persno, &work_dest[k], &work_mode[k])) != EOF) {
			hh_pers[k] = hh*MAX_PERSONS_PER_HH + persno;
			k++;
		}

		// read high income work journeys.
		printf ("reading high income work journey destinations...\n");
		while ((fscanf (fp_work[2], "%d %d %d %d", &hh, &persno, &work_dest[k], &work_mode[k])) != EOF) {
			hh_pers[k] = hh*MAX_PERSONS_PER_HH + persno;
			k++;
		}
		work_journeys = k;

		// sort all work journeys by household id and person number
		printf ("sorting combined set of all work journey destinations...\n");
		isort (hh_pers, index, work_journeys);



		if (Ini->SUBAREA_ANALYSIS == 1) {

			hh_pers_unfrozen = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->MAX_WORK_JOURNEYS*sizeof(int));
			index_unfrozen = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->MAX_WORK_JOURNEYS*sizeof(int));
			work_dest_unfrozen =  (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->MAX_WORK_JOURNEYS*sizeof(short));
			work_mode_unfrozen =  (short *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->MAX_WORK_JOURNEYS*sizeof(short));

			// read work destinations for unfrozen set of work journeys
			// read unfrozen low income work journeys.
			k = 0;
			printf ("reading unfrozen low income work journey destinations...\n");
			while ((fscanf (fp_work[3], "%d %d %d %d", &hh, &persno, &work_dest_unfrozen[k], &work_mode_unfrozen[k])) != EOF) {
				hh_pers_unfrozen[k] = hh*MAX_PERSONS_PER_HH + persno;
				index_unfrozen[k] = k;
				k++;
			}

			// read unfrozen medium income work journeys.
			printf ("reading unfrozen medium income work journey destinations...\n");
			while ((fscanf (fp_work[4], "%d %d %d %d", &hh, &persno, &work_dest_unfrozen[k], &work_mode_unfrozen[k])) != EOF) {
				hh_pers_unfrozen[k] = hh*MAX_PERSONS_PER_HH + persno;
				k++;
			}

			// read unfrozen high income work journeys.
			printf ("reading unfrozen high income work journey destinations...\n");
			while ((fscanf (fp_work[5], "%d %d %d %d", &hh, &persno, &work_dest_unfrozen[k], &work_mode_unfrozen[k])) != EOF) {
				hh_pers_unfrozen[k] = hh*MAX_PERSONS_PER_HH + persno;
				k++;
			}
			work_journeys_unfrozen = k;

			// sort all work journeys by household id and person number
			printf ("sorting combined set of all unfrozen work journey destinations...\n");
			isort (hh_pers_unfrozen, index_unfrozen, work_journeys_unfrozen);

		}
	}
	
	

	for (i=0; i <= Ini->MAX_TAZS; i++)
		for (j=0; j < PURPOSES; j++)
			TotProds[i][j] = 0;

// Read journey household records.  The first is a header record with date/time stamp to be written to reportfile.
	k = 0;
	input_journeys = 0;
	fgets (JourneyRecord, JOURNEY_RECORD_LENGTH+1, fp);
	haj = 1;
	fprintf (fp_rep, "\nHAJ Journey file header record:\n%s\n\n", JourneyRecord);
	while ((fgets(JourneyRecord, JOURNEY_RECORD_LENGTH+1, fp)) != NULL && k < Ini->NUMBER_JOURNEYS) {
		haj++;

		strncpy (temp, &JourneyRecord[OTAZ_START-1], OTAZ_LENGTH);
		temp[OTAZ_LENGTH] = '\0';
		orig = atoi(temp);


		if (orig != STATUE_OF_LIBERTY_TAZ) {


			strncpy (temp, &JourneyRecord[SEQ_START-1], SEQ_LENGTH);
			temp[SEQ_LENGTH] = '\0';
			seq = atoi(temp);

			strncpy (temp, &JourneyRecord[WALK_ORIG_START-1], WALK_ORIG_LENGTH);
			temp[WALK_ORIG_LENGTH] = '\0';
			walk_orig = atoi(temp) - 1;	// input records coded as no=1, yes=2
		
			strncpy (temp, &JourneyRecord[INCOME_START-1], INCOME_LENGTH);
			temp[INCOME_LENGTH] = '\0';
			income = atoi(temp);

			strncpy (temp, &JourneyRecord[WORKERS_START-1], WORKERS_LENGTH);
			temp[WORKERS_LENGTH] = '\0';
			workers = atoi(temp);

			strncpy (temp, &JourneyRecord[CHILDREN_START-1], CHILDREN_LENGTH);
			temp[CHILDREN_LENGTH] = '\0';
			children = atoi(temp);

			strncpy (temp, &JourneyRecord[NWADULTS_START-1], NWADULTS_LENGTH);		// non-working adults in HH
			temp[NWADULTS_LENGTH] = '\0';
			nwas = atoi(temp);

			strncpy (temp, &JourneyRecord[AUTOS_START-1], AUTOS_LENGTH);
			temp[AUTOS_LENGTH] = '\0';
			autos = atoi(temp) - 1;		// input records coded as auto ownership groups (1-4)

			strncpy (temp, &JourneyRecord[HH_JRNYS_START-1], HH_JRNYS_LENGTH);
			temp[HH_JRNYS_LENGTH] = '\0';
			hh_jrnys = atoi(temp);
		


			numAutosRestricted = getNumAutosRestricted (autos);


			num_households ++;

			// read the journeys for this household.
			start_k = k;
			adult_maint = 0;
			adult_discr = 0;
			child_maint = 0;
			child_discr = 0;
			for (i=0; i < hh_jrnys; i++) {
				if (k < Ini->NUMBER_JOURNEYS) {

					fgets(JourneyRecord, JOURNEY_RECORD_LENGTH+2, fp);
					haj++;

					strncpy (temp, &JourneyRecord[PERSNO_START-1], PERSNO_LENGTH);
					temp[PERSNO_LENGTH] = '\0';
					persno = atoi(temp);
	
					strncpy (temp, &JourneyRecord[PERSON_TYPE_START-1], PERSON_TYPE_LENGTH);
					temp[PERSON_TYPE_LENGTH] = '\0';
					person_type = atoi(temp);
	
					strncpy (temp, &JourneyRecord[JRNY_TYPE_START-1], JRNY_TYPE_LENGTH);
					temp[JRNY_TYPE_LENGTH] = '\0';
					jrny_type = atoi(temp);

					num_journeys ++;
			
			// define purpose codes
					switch (jrny_type) {
						case (6):					
							purpose = income - 1;	// work (0,1,2)

							break;
						case (1):
						case (2):
						case (3):
							purpose = 3;			// school (3)
							break;
						case (4):
						case (5):
							purpose = 4;			// university (4)
							break;
						case (7):
						case (9):
						case (10):
							purpose = 5;			// maintenance (5)
							if (person_type == 1)
								child_maint++;
							else
								adult_maint++;
							break;
						case (11):
						case (12):
						case (13):
							purpose = 6;			// discretionary (6)
							if (person_type == 1)
								child_discr++;
							else
								adult_discr++;
							break;
						case (8):
							purpose = 7;			// at work (7)
							break;
					}
					TotProds[0][purpose]++;
					TotProds[orig][purpose]++;


					if (purpose == Ini->PURPOSE_TO_PROCESS) {

						if (Ini->SUBAREA_ANALYSIS == 1) {
							if (Ini->PURPOSE_TO_PROCESS != 7) {		// check for purposes other than at-work
								if (orig != Os[input_journeys]) {
									printf ("\nError in read_kpmg_journey_records() for sub-area analysis.\n");
									printf ("While reading journey index=%d, orig was determined to be=%d\n", input_journeys, orig);
									printf ("but the origin of the corresponding MDSC output record was=%d.\n", Os[input_journeys]);
									printf ("These origin taz values should be equal for purposes other than at-work.\n");
									printf ("The current HAJ file record number=%d.\n\n", haj);
									printf ("exit (-35).\n\n");
									fflush (stdout);
									exit (-35);
								}
							}
						}

						JourneyAttribs->purpose[k] = purpose;
						JourneyAttribs->orig[k] = orig;
						JourneyAttribs->walk_orig[k] = walk_orig;
						JourneyAttribs->income[k] = income;
						JourneyAttribs->workers[k] = workers;
						JourneyAttribs->children[k] = children;
						JourneyAttribs->nwas[k] = nwas;
						JourneyAttribs->autos[k] = autos;
						JourneyAttribs->autosAvail[k] = autos - numAutosRestricted;
						JourneyAttribs->person_type[k] = person_type;
						JourneyAttribs->hh[k] = seq;
						JourneyAttribs->persno[k] = persno;
						JourneyAttribs->haj[k] = haj;


						if (seq > max_seq)
							max_seq = seq;
						if (persno > max_pers)
							max_pers = persno;



						// link at-work journey with work journey for this person
						if (Ini->PURPOSE_TO_PROCESS == 7) {
							// no subarea analysis
							if (Ini->SUBAREA_ANALYSIS != 1) {
								link_atwork (hh_pers, index, work_journeys, work_mode, work_dest, JourneyAttribs->hh[k],
									JourneyAttribs->persno[k], &linked_work_mode, &linked_work_dest);

								// if no work journey was found, none exists, so increment error count
								if (linked_work_mode < 0) {
									not_linked ++;
								}
								// the work journey was linked, so change the orig and at_work_mode,
								else {
									JourneyAttribs->orig[k] = linked_work_dest;
									JourneyAttribs->at_work_mode[k] = linked_work_mode;
								}

								if (fp2 != NULL)
									fprintf (fp2, "%15d %9d %9d %9d %9d %5d\n", input_journeys, k, haj, 0, seq, i);

								k++;
							}
							// subarea analysis is turned on
							else {
								// link at-work journey with unfrozen work journey for this person if one exists
								link_atwork (hh_pers_unfrozen, index_unfrozen, work_journeys_unfrozen,
									work_mode_unfrozen, work_dest_unfrozen, JourneyAttribs->hh[k], JourneyAttribs->persno[k],
									&linked_work_mode, &linked_work_dest);

								// if no unfrozen work journey was found, search full set of work destinations
								if (linked_work_mode < 0) {
									link_atwork (hh_pers, index, work_journeys, work_mode, work_dest, JourneyAttribs->hh[k],
										JourneyAttribs->persno[k], &linked_work_mode, &linked_work_dest);

									// if no work journey was found still, none exists, so increment error count
									if (linked_work_mode < 0) {
										not_linked ++;

										if (fp2 != NULL)
											fprintf (fp2, "%15d %9d %9d %9d %9d %5d %s\n", input_journeys, k, haj, JourneyAttribs->frozen[k], seq, i, "linked_work_mode error");

									}
									// the work journey was linked, so change the orig and at_work_mode,
									// and determine its subarea index based on new orig and dest info.
									else {
										JourneyAttribs->orig[k] = linked_work_dest;
										JourneyAttribs->at_work_mode[k] = linked_work_mode;

										JourneyAttribs->frozen[k] = 1;
										if (ZonalData->OrigSubAreaOB[linked_work_dest] == 1 ||
											ZonalData->DestSubAreaOB[Ds[input_journeys]] == 1 ||
											(ZonalData->OrigSubAreaOB[linked_work_dest] == 2 && ZonalData->DestSubAreaOB[Ds[input_journeys]] == 2)) {
												JourneyAttribs->frozen[k] = 0;
										}
										if (ZonalData->OrigSubAreaIB[Ds[input_journeys]] == 1 ||
											ZonalData->DestSubAreaIB[linked_work_dest] == 1 ||
											(ZonalData->OrigSubAreaIB[Ds[input_journeys]] == 2 && ZonalData->DestSubAreaIB[linked_work_dest] == 2)) {
												JourneyAttribs->frozen[k] = 0;
										}

										if (fp2 != NULL)
											fprintf (fp2, "%15d %9d %9d %9d %9d %5d\n", input_journeys, k, haj, JourneyAttribs->frozen[k], seq, i);

										if (JourneyAttribs->frozen[k] == 0)
											k++;
									}
								}
								// if an unfrozen work journey was linked, then the at-work journey is to be unfrozen
								// regardless of its location.
								else {
									JourneyAttribs->orig[k] = linked_work_dest;
									JourneyAttribs->at_work_mode[k] = linked_work_mode;

									JourneyAttribs->frozen[k] = 0;

									if (fp2 != NULL)
										fprintf (fp2, "%15d %9d %9d %9d %9d %5d\n", input_journeys, k, haj, JourneyAttribs->frozen[k], seq, i);

									k++;
								}
							}
						}
						// Not an at-work journey
						else {
							// if subarea analysis is turned on, determine whether journey is unfrozen or not.
							if (Ini->SUBAREA_ANALYSIS == 1) {
								JourneyAttribs->frozen[k] = 1;
								if (Ini->LOW_COMPETITION == 1) {
									if (ZonalData->OrigSubAreaOB[Os[input_journeys]] == 1 ||
										ZonalData->DestSubAreaOB[Ds[input_journeys]] == 1 ||
										(ZonalData->OrigSubAreaOB[Os[input_journeys]] == 2 && ZonalData->DestSubAreaOB[Ds[input_journeys]] == 2)) {
											JourneyAttribs->frozen[k] = 0;
									}
									if (ZonalData->OrigSubAreaIB[Ds[input_journeys]] == 1 ||
										ZonalData->DestSubAreaIB[Os[input_journeys]] == 1 ||
										(ZonalData->OrigSubAreaIB[Ds[input_journeys]] == 2 && ZonalData->DestSubAreaIB[Os[input_journeys]] == 2)) {
											JourneyAttribs->frozen[k] = 0;
									}
								}
								else if (Ini->LOW_COMPETITION == 0) {
									if ((ZonalData->OrigSubAreaOB[Os[input_journeys]] == 1 || ZonalData->OrigSubAreaOB[Os[input_journeys]] == 2) &&
										(ZonalData->DestSubAreaOB[Ds[input_journeys]] == 1 || ZonalData->DestSubAreaOB[Ds[input_journeys]] == 2)) {
											JourneyAttribs->frozen[k] = 0;
									}
									if ((ZonalData->OrigSubAreaIB[Ds[input_journeys]] == 1 || ZonalData->OrigSubAreaIB[Ds[input_journeys]] == 2) &&
										(ZonalData->DestSubAreaIB[Os[input_journeys]] == 1 || ZonalData->DestSubAreaIB[Os[input_journeys]] == 2)) {
											JourneyAttribs->frozen[k] = 0;
									}
								}
								else {
									printf ("\nLOW_COMPETITION = %d in control file, but should be 0 or 1.\n", Ini->LOW_COMPETITION);
									printf ("exit (-37).\n\n");
									fflush (stdout);
									fprintf (fp_rep, "LOW_COMPETITION = %d in control file, but should be 0 or 1.\n", Ini->LOW_COMPETITION);
									fprintf (fp_rep, "exit (-37).\n\n");
									fflush (fp_rep);
									exit (-37);
								}

								if (fp2 != NULL)
									fprintf (fp2, "%15d %9d %9d %9d %9d %5d\n", input_journeys, k, haj, JourneyAttribs->frozen[k], seq, i);

								if (JourneyAttribs->frozen[k] == 0)
									k++;
							}
							else {

								if (fp2 != NULL)
									fprintf (fp2, "%15d %9d %9d %9d %9d %5d\n", input_journeys, k, haj, JourneyAttribs->frozen[k], seq, i);

								k++;
							}
						}

						input_journeys++;

						if (fp2 != NULL)
							fflush (fp2);
					}
		
				}

//				printf ("reading HAJ journey file complete\r");

			} // end of journeys in HH loop

			for (i=0; i < hh_jrnys; i++) {
				if (start_k + i < Ini->NUMBER_JOURNEYS && purpose == Ini->PURPOSE_TO_PROCESS) {
					if (purpose == 5 && child_maint >= 1 && adult_maint >= 1)
						JourneyAttribs->ak[start_k + i] = 1;
					else if (purpose == 5 && adult_maint >= 2)
						JourneyAttribs->aa[start_k + i] = 1;
					else if (purpose == 6 && child_discr >= 1 && adult_discr >= 1)
						JourneyAttribs->ak[start_k + i] = 1;
					else if (purpose == 6 && adult_discr >= 2)
						JourneyAttribs->aa[start_k + i] = 1;
				}
			}
		}
	}

//	printf ("reading HAJ journey file: complete\n");

	Ini->NUMBER_JOURNEYS = k;

	
	
//	printf ("\nmax hh id = %d\n", max_seq);
//	printf ("max pers id = %d\n", max_pers);
//	printf ("total HAJ households = %d\n", num_households);
//	printf ("total HAJ journeys = %d\n\n", num_journeys);
//	if (Ini->PURPOSE_TO_PROCESS == 7)
//		printf ("%d atwork journeys could not be linked to their corresponding work journeys\n\n", not_linked);
	fprintf (fp_rep, "\nmax hh id = %d\n", max_seq);
	fprintf (fp_rep, "max pers id = %d\n", max_pers);
	fprintf (fp_rep, "total HAJ households = %d\n", num_households);
	fprintf (fp_rep, "total HAJ journeys = %d\n\n", num_journeys);
	if (Ini->PURPOSE_TO_PROCESS == 7)
		fprintf (fp_rep, "%d atwork journeys could not be linked to their corresponding work journeys\n\n", not_linked);
//	fflush (stdout);
	fflush (fp_rep);



	if (Ini->SUBAREA_ANALYSIS == 1) {
		if (mdsc_journeys != input_journeys) {
			printf ("\nError at the end read_kpmg_journey_records() for sub-area analysis.\n");
			printf ("%d HAJ journey records were read for purpose=%d,\n", input_journeys, Ini->PURPOSE_TO_PROCESS+1);
			printf ("but the number of corresponding MDSC output records was=%d.\n", mdsc_journeys);
			printf ("The number of journeys from the two files should be equal for all purposes.\n");
			printf ("exit (-36).\n\n");
			fflush (stdout);
			exit (-36);
		}
	}

	Ini->ORIGINAL_JOURNEYS = input_journeys;



	if (Ini->SUBAREA_ANALYSIS == 1) {
		// initialize array for counting frequency of journeys originating and destined from/to each subarea (4)
		for (h=0; h < 2; h++)
			for (i=0; i < 2; i++)
				for (j=0; j < 4; j++)
					freq[h][i][j] = 0;


		// tabulate the frequencies
		for (k=0; k < Ini->ORIGINAL_JOURNEYS; k++) {
			i = Os[k];
			j = Ds[k];
			freq[0][0][ZonalData->OrigSubAreaOB[i]] ++;
			freq[0][1][ZonalData->DestSubAreaOB[j]] ++;
			freq[1][0][ZonalData->OrigSubAreaIB[j]] ++;
			freq[1][1][ZonalData->DestSubAreaIB[i]] ++;
		}


		// report the frequencies
//		printf ("\n\nFrequency of Outbound MDSC Journeys by Origin and Destination Sub-Areas:\n");
//		printf ("Sub-area     Origin Journeys     Destination Journeys\n");
//		printf ("-----------------------------------------------------\n");
//		printf ("In Sub-area      %11d %24d\n", freq[0][0][1], freq[0][1][1]);
//		printf ("Hi Competition   %11d %24d\n", freq[0][0][2], freq[0][1][2]);
//		printf ("Lo Competition   %11d %24d\n", freq[0][0][3], freq[0][1][3]);
//		printf ("No Consideration %11d %24d\n", freq[0][0][0], freq[0][1][0]);
//		printf ("-----------------------------------------------------\n");
//		printf ("Total            %11d %24d\n\n", freq[0][0][0]+freq[0][0][1]+freq[0][0][2]+freq[0][0][3], freq[0][1][0]+freq[0][1][1]+freq[0][1][2]+freq[0][1][3]);
	
		fprintf (fp_rep, "\n\nFrequency of input MDSC Journeys by Origin and Destination Sub-Areas:\n");
		fprintf (fp_rep, "Sub-area     Origin Journeys     Destination Journeys\n");
		fprintf (fp_rep, "-----------------------------------------------------\n");
		fprintf (fp_rep, "In Sub-area      %11d %24d\n", freq[0][0][0], freq[0][1][0]);
		fprintf (fp_rep, "Hi Competition   %11d %24d\n", freq[0][0][1], freq[0][1][1]);
		fprintf (fp_rep, "Lo Competition   %11d %24d\n", freq[0][0][2], freq[0][1][2]);
		fprintf (fp_rep, "No Consideration %11d %24d\n", freq[0][0][3], freq[0][1][3]);
		fprintf (fp_rep, "-----------------------------------------------------\n");
		fprintf (fp_rep, "Total            %11d %24d\n\n", freq[0][0][0]+freq[0][0][1]+freq[0][0][2]+freq[0][0][3], freq[0][1][0]+freq[0][1][1]+freq[0][1][2]+freq[0][1][3]);

		

//		printf ("\n\nFrequency of Inbound MDSC Journeys by Origin and Destination Sub-Areas:\n");
//		printf ("Sub-area     Origin Journeys     Destination Journeys\n");
//		printf ("-----------------------------------------------------\n");
//		printf ("In Sub-area      %11d %24d\n", freq[1][0][1], freq[1][1][1]);
//		printf ("Hi Competition   %11d %24d\n", freq[1][0][2], freq[1][1][2]);
//		printf ("Lo Competition   %11d %24d\n", freq[1][0][3], freq[1][1][3]);
//		printf ("No Consideration %11d %24d\n", freq[1][0][0], freq[1][1][0]);
//		printf ("-----------------------------------------------------\n");
//		printf ("Total            %11d %24d\n\n", freq[1][0][0]+freq[1][0][1]+freq[1][0][2]+freq[1][0][3], freq[1][1][0]+freq[1][1][1]+freq[1][1][2]+freq[1][1][3]);
	
		fprintf (fp_rep, "\n\nFrequency of input MDSC Journeys by Origin and Destination Sub-Areas:\n");
		fprintf (fp_rep, "Sub-area     Origin Journeys     Destination Journeys\n");
		fprintf (fp_rep, "-----------------------------------------------------\n");
		fprintf (fp_rep, "In Sub-area      %11d %24d\n", freq[1][0][0], freq[1][1][0]);
		fprintf (fp_rep, "Hi Competition   %11d %24d\n", freq[1][0][1], freq[1][1][1]);
		fprintf (fp_rep, "Lo Competition   %11d %24d\n", freq[1][0][2], freq[1][1][2]);
		fprintf (fp_rep, "No Consideration %11d %24d\n", freq[1][0][3], freq[1][1][3]);
		fprintf (fp_rep, "-----------------------------------------------------\n");
		fprintf (fp_rep, "Total            %11d %24d\n\n", freq[1][0][0]+freq[1][0][1]+freq[1][0][2]+freq[1][0][3], freq[1][1][0]+freq[1][1][1]+freq[1][1][2]+freq[1][1][3]);
	
		//fflush(stdout);
		fflush(fp_rep);
	}


	if (fp2 != NULL)
		fclose (fp2);


	if (Ini->SUBAREA_ANALYSIS == 1) {
		HeapFree (heapHandle, 0, Os);
		HeapFree (heapHandle, 0, Ds);
	}

	
	if (Ini->PURPOSE_TO_PROCESS == 7) {
		HeapFree (heapHandle, 0, hh_pers);
		HeapFree (heapHandle, 0, index);
		HeapFree (heapHandle, 0, work_mode);
		HeapFree (heapHandle, 0, work_dest);
		if (Ini->SUBAREA_ANALYSIS == 1) {
			HeapFree (heapHandle, 0, hh_pers_unfrozen);
			HeapFree (heapHandle, 0, index_unfrozen);
			HeapFree (heapHandle, 0, work_mode_unfrozen);
			HeapFree (heapHandle, 0, work_dest_unfrozen);
		}
	}
}



int getNumAutosRestricted (int hhAutoOwnership) {

	// hhAutoOwnership can by 0, 1, 2 or 3 - no cars, 1 car, 2 cars, or 3 or more cars.


	int i;
	int numAutosRestricted = 0;
	double nrand;


	// if PCT_HHS_WITH_FULL_AUTO_RESTRICTION > 0.0, apply full household auto restrictions
	if ( Ini->PCT_HHS_WITH_FULL_AUTO_RESTRICTION > 0.0 ) {

		// do monte carlo selection to see if HH's auto is restricted and if so, increment restricted count.
		nrand = (double)rand()/(double)MAX_RANDOM;
		if ( nrand < Ini->PCT_HHS_WITH_FULL_AUTO_RESTRICTION )
			numAutosRestricted = hhAutoOwnership;

	}
	// else evaluate each household auto's restriction status relative to the PCT_LP_RESTRICTION percentage.
	else {
	
		// loop over number of autos in household for households with at least one auto.
		for (i=1; i <= hhAutoOwnership; i++) {

			// do monte carlo selection to see if HH's auto is restricted and if so, increment restricted count.
			nrand = (double)rand()/(double)MAX_RANDOM;
			if ( nrand < Ini->PCT_LP_RESTRICTION )
				numAutosRestricted++;

		}

	}

	// numAutosRestricted will be used to determine an alternative auto sufficiency variable for the household
	// for use in motorized journey mode choice model for journeys with destination in restricted area.
	return numAutosRestricted;

}