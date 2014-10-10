#include "md.h"

void run_mdc (FILE *fp3, FILE *fp_rep2, FILE *fp_rep3, FILE *fp_work[], struct m_mc_asc_data mMcAscData, struct nm_mc_asc_data nmMcAscData,
			  float **m_cal_est, float **nm_cal_est,
			  float **hwy_dist, float ***OD_Utility, int **TotProds,
			  struct journey_attribs *JourneyAttribs, struct zone_data *ZonalData,
			  struct river_crossing_data RiverData, struct taxi_data *TaxiData,
			  struct walk_zone_data *WalkZoneData, struct dc_constant_data *dcConstants,
			  struct district_definitions *districtDefinitions, struct co_dist_factors *DistFactors)
{

	FILE *fp;
	FILE **fp_freeze;
	FILE **fp_frozen;
	int badDest = 0;
	int badDestCount = 0;
	int numFiles;
	int i, j, k, m, n, p, r, seq, walk[2];
	int hh, pnum, haj;
	int acc, orig, dest, purpose, income, autos, ut_orig, mode, person_type, atwork_mode, da_work;
	int origDist, destDist, origDistIndex, destDistIndex;
	int m_msc_index_value, m_msc_index_index, nm_msc_index_value, nm_msc_index_index;
	int unmet_count, last_k, count, cnt, dist;
	int debug_part, *debug_parts, debug_mode=0;
	int se_flag, od_flag;
	int motorized[MAX_PACKET_SIZE];
	int *AvailModes, **acc_freq, **mc, *dest_freq;
	int **tlfreq, ***dist2dist;
	int *p_sort, *p_start, *p_order, *rand_nos;
	int *frozenDest, *frozenAcc, *frozenMode, *frozenAtWorkMode;
	int idist, jdist, idist_index, jdist_index;
	int count1, count2, rcount;
	int fileIndex;
	float gamma;
	float **Logsum, ***z_attrs, **t_attrs;
	float *Prob, *SEutil, *ODutil, *TotalUtility;
	float Total[3];
	double *tldist, *dest_props, *mc_cum, *dc_cum, *shares, *attr_corrections;
	double nrand, pct_acc;
	double pAcc, pNoAcc;
	char fileNameString[512];
	char appendString[10];
	char tempString[200];
	struct dc_coeff_data *DCcoeff;

	double **util, **props;


// Memory allocations
	printf ("\nallocating memory for run_mdc().\n");
	fprintf (fp_rep, "\nallocating memory for run_mdc().\n");

	frozenDest = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_JOURNEYS+1)*sizeof(int));
	frozenAcc = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_JOURNEYS+1)*sizeof(int));
	frozenMode = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_JOURNEYS+1)*sizeof(int));
	frozenAtWorkMode = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_JOURNEYS+1)*sizeof(int));
	debug_parts = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, ((int)(100/Ini->DEBUG_PERCENT) + 1)*sizeof(int));
	AvailModes = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_ALTS*sizeof(int));
	dest_freq = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof (int));
	ODutil = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_ALTS*sizeof(float));
	SEutil = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_ALTS*sizeof(float));
	TotalUtility = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_ALTS*sizeof(float));
	Prob   = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_ALTS*sizeof(float));
	mc_cum = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_ALTS*sizeof(double));
	dc_cum = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->MAX_TAZS*2*sizeof(double));
	tldist = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_ALTS*sizeof(double));
	shares = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_ALTS*sizeof(double));
	attr_corrections = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(double));
	dest_props = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(double));
	DCcoeff = (struct dc_coeff_data *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 1*sizeof(struct dc_coeff_data));


	z_attrs = (float ***) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (2+1)*sizeof(float **));
	for (j=0; j < 2+1; j++) {
		z_attrs[j] = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, PURPOSES*sizeof(float *));
		for (i=0; i < PURPOSES; i++)
			z_attrs[j][i] = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float));
	}

	t_attrs = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, PURPOSES*sizeof(float *));
	for (i=0; i < PURPOSES; i++)
		t_attrs[i] = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float));

	Logsum = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float *));
	for (i=0; i < Ini->MAX_TAZS+1; i++)
		Logsum[i] = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 2*sizeof(float));

	mc = (int **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (districtDefinitions->modeReportDefinitions->numValues+1)*sizeof(int *));
	for (i=0; i <= districtDefinitions->modeReportDefinitions->numValues; i++)
		mc[i] = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_ALTS+1)*sizeof(int));

	acc_freq = (int **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (districtDefinitions->modeReportDefinitions->numValues+1)*sizeof(int *));
	for (i=0; i <= districtDefinitions->modeReportDefinitions->numValues; i++)
		acc_freq[i] = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_ALTS+1)*sizeof(int));

	tlfreq = (int **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->TRIP_LENGTH_RANGES+2)*sizeof(int *));
	for (i=0; i < Ini->TRIP_LENGTH_RANGES+2; i++)
		tlfreq[i] = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_ALTS+1)*sizeof(int));

	dist2dist = (int ***) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_ALTS+1)*sizeof(int **));
	for (i=0; i < Ini->NUMBER_ALTS+1; i++) {
		dist2dist[i] = (int **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, districtDefinitions->distToDistReportDefinitions->numValues*sizeof(int *));
		for (j=0; j < districtDefinitions->distToDistReportDefinitions->numValues; j++)
			dist2dist[i][j] = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, districtDefinitions->distToDistReportDefinitions->numValues*sizeof(int));
	}



	// these are used in motor_dc_props(), but allocated here so it only has to be done once, and they can be re-used on re-entry to motor_dc_props().
    util  = (double**) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(double *));
	props = (double**) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(double *));
	for (i=0; i < Ini->MAX_TAZS+1; i++) {
		util[i]  = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 2*sizeof(double));
		props[i] = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 2*sizeof(double));
	}



//	addRAM ("run_mdc", ((int)(100/Ini->DEBUG_PERCENT) + 1)*sizeof(int)
//						+ (Ini->NUMBER_ALTS)*sizeof(int)
//						+ (Ini->MAX_TAZS+1)*sizeof(int)
//						+ 3*(Ini->NUMBER_ALTS)*sizeof(float)
//						+ 3*(Ini->NUMBER_ALTS)*sizeof(double)
//						+ (Ini->MAX_TAZS*2)*sizeof(double)
//						+ 2*(Ini->MAX_TAZS+1)*sizeof(double)
//						+ sizeof(struct dc_coeff_data)
//						+ 3*PURPOSES*(Ini->MAX_TAZS+1)*sizeof(float)
//						+ PURPOSES*(Ini->MAX_TAZS+1)*sizeof(float)
//						+ (Ini->MAX_TAZS+1)*2*sizeof(float)
//						+ 2*(Ini->NUMBER_BPMDIST1+1)*(Ini->NUMBER_ALTS+1)*sizeof(int)
//						+ (Ini->TRIP_LENGTH_RANGES+2)*(Ini->NUMBER_ALTS+1)*sizeof(int)
//						+ (Ini->NUMBER_ALTS+1)*(Ini->NUMBER_BPMDIST1+1)*(Ini->NUMBER_BPMDIST1+1)*sizeof(int));



	numFiles = Ini->NUMBER_JOURNEYS/MAX_FROZEN_RECORDS + 1;


	fp_freeze = (FILE **) calloc(numFiles, sizeof(FILE *));
	fp_frozen = (FILE **) calloc(numFiles, sizeof(FILE *));
	for (i=0; i < numFiles; i++) {
		fp_freeze[i] = NULL;
		fp_frozen[i] = NULL;
	}



	// open the file(s) to which the frozen records will be written
	if (Ini->FREEZE_MDC_OUTPUT == 1) {
		if (strcmp(Ini->FROZEN_MDC_OUTPUT, "")) {

			if ( numFiles > 1 ) {

				// find last occurrence of '.' in filename if there is one
				for (i=strlen(Ini->FROZEN_MDC_OUTPUT)-1; i >= 0; i--) {
					if (Ini->FROZEN_MDC_OUTPUT[i] == '.')
						break;
				}
					
				for (j=0; j < numFiles; j++) {

					// make a string to insert before the '.' or at the end of there's no '.'.
					sprintf (appendString, "_%d", j);

					if (i >= 0) {
						strncpy (fileNameString, Ini->FROZEN_MDC_OUTPUT, i);
						fileNameString[i] = '\0';
						strcat (fileNameString, appendString);
						strcat (fileNameString, &Ini->FROZEN_MDC_OUTPUT[i]);
					}
					else {
						strcpy (fileNameString, Ini->FROZEN_MDC_OUTPUT);
						strcat (fileNameString, appendString);
					}

					
					if ((fp_freeze[j] = fopen(fileNameString, "wt")) == NULL)
						ExitWithCode(147);

				}

			}
			else {

				if ((fp_freeze[0] = fopen(Ini->FROZEN_MDC_OUTPUT, "wt")) == NULL)
					ExitWithCode(147);
			}

		}
		else
			ExitWithCode (148);
	}


	// if FTA_RESTART mode, open the file where frozen records were written and read frozen results.
	if (Ini->FTA_RESTART == 1) {
		if (strcmp(Ini->FROZEN_MDC_INPUT, "")) {

			if ( numFiles > 1 ) {

				// find last occurrence of '.' in filename if there is one
				for (i=strlen(Ini->FROZEN_MDC_INPUT)-1; i >= 0; i--) {
					if (Ini->FROZEN_MDC_INPUT[i] == '.')
						break;
				}
					
				for (j=0; j < numFiles; j++) {

					// make a string to insert before the '.' or at the end of there's no '.'.
					sprintf (appendString, "_%d", j);

					if (i >= 0) {
						strncpy (fileNameString, Ini->FROZEN_MDC_INPUT, i);
						fileNameString[i] = '\0';
						strcat (fileNameString, appendString);
						strcat (fileNameString, &Ini->FROZEN_MDC_INPUT[i]);
					}
					else {
						strcpy (fileNameString, Ini->FROZEN_MDC_INPUT);
						strcat (fileNameString, appendString);
					}

					
					if ((fp_frozen[j] = fopen(fileNameString, "rt")) == NULL)
						ExitWithCode(145);

				}

			}
			else {

				if ((fp_frozen[0] = fopen(Ini->FROZEN_MDC_INPUT, "rt")) == NULL) {
					ExitWithCode(145);
				}

			}

			fileIndex = 0;
			r = 0;
			rcount = 0;
			printf ("\nreading frozen journey records...");
			fprintf (fp_rep, "\nreading frozen journey records...");
			while ( (fscanf (fp_frozen[fileIndex], "%*d,%d,%d,%d,%d,%*d,%*d,%*d,%*d,%*d,%d,%*d,%d,%d,%d,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f", &k, &hh, &pnum, &haj, &dest, &acc, &m, &atwork_mode)) != EOF) {
				if (hh != JourneyAttribs->hh[k] || pnum != JourneyAttribs->persno[k] || haj != JourneyAttribs->haj[k]) {
					printf ("\ninconsistency between order of frozen journey records and model journey records");
					printf ("\nfrozen record: k=%d, hh=%d, persno=%d, haj=%d", k, hh, pnum, haj);
					printf ("\njourney structure record: k=%d, JourneyAttribs->hh[k]=%d, JourneyAttribs->persno[k]=%d, JourneyAttribs->haj[k]=%d", k, JourneyAttribs->hh[k], JourneyAttribs->persno[k], JourneyAttribs->haj[k]);
					fprintf (fp_rep, "\nerror - inconsistency between order of frozen journey records and model journey records.");
					fprintf (fp_rep, "\nfrozen record: k=%d, hh=%d, persno=%d, haj=%d", k, hh, pnum, haj);
					fprintf (fp_rep, "\njourney structure record: k=%d, JourneyAttribs->hh[k]=%d, JourneyAttribs->persno[k]=%d, JourneyAttribs->haj[k]=%d", k, JourneyAttribs->hh[k], JourneyAttribs->persno[k], JourneyAttribs->haj[k]);
					fflush (stdout);
					fflush (fp_rep);
					exit (-71);
				}
				frozenDest[k] = dest;
				frozenAcc[k] = acc;
				frozenMode[k] = m;
				frozenAtWorkMode[k] = atwork_mode;
				rcount++;
				r++;

				// if this is FTA_RESTART for at-work purpose, and if flag is set to 1, change the mode used for the work tour on the journey record
				// to the value in the frozen records file;
				// otherwise, the mode used for the work tour will be what was linked when reading the HAJ journeys, i.e. the mode used in the work tour
				// for the work tour model run in FTA_RESTART mode.
				if (Ini->FTA_RESTART && Ini->PURPOSE_TO_PROCESS == 7 && Ini->USE_FROZEN_WORKTOUR_MODE == 1) {
					JourneyAttribs->at_work_mode[k] = frozenAtWorkMode[k];
				}


				if (r == MAX_FROZEN_RECORDS) {
					r = 0;
					fileIndex++;
				}

			}

			printf ("\nread %d frozen journey records from %d file(s).\n", rcount, fileIndex+1);
			fprintf (fp_rep, "\nread %d frozen journey records from %d file(s).\n", rcount, fileIndex+1);

		}
		else {
			ExitWithCode (146);
		}

	}





	// open and read journey attraction correction factors to be applied after attraction models, use 1.0 if no file is specified.
	if (strcmp(Ini->ATTRACTION_CORRECTIONS, "")) {
		if ((fp = fopen(Ini->ATTRACTION_CORRECTIONS, "rt")) == NULL)
			ExitWithCode(101);
		read_attr_corrections (fp, attr_corrections);
		fclose (fp);
	}
	else {
		for (i=1; i <= Ini->MAX_TAZS; i++)
			attr_corrections[i] = 1.0;
	}
// open and read journey attraction income group percentages file and perform balancing,
	// 8 PURPOSES ==> [ 0 work low, 1 work medium, 2 work high, 3 school, 4 university, 5 maintenance, 6 discretionary, 7 at work ]
	// codes 0-8 are for internal use; add 1 for external reporting and data input
	// compute zonal attractions
	printf ("calculating attractions\n");
	if (Ini->PURPOSE_TO_PROCESS < 3) {
		if (strcmp(Ini->ATTR_PCT_DATA, "")) {
			if ((fp = fopen(Ini->ATTR_PCT_DATA, "rt")) == NULL)
				ExitWithCode(20);
		}
		else
			ExitWithCode (41);
	}
	get_attractions[Ini->PURPOSE_TO_PROCESS] (fp, ZonalData, z_attrs[2]);
	if (Ini->PURPOSE_TO_PROCESS < 3)
		if (strcmp(Ini->ATTR_PCT_DATA, ""))
			fclose (fp);

	// apply income group proportions to zonal total work attractions and balance to total productions by income group
	printf ("balancing attractions to journey productions\n");
	balance_attractions (attr_corrections, z_attrs, t_attrs, TotProds, ZonalData);

	for (i=0; i <3; i++)
		Total[i] = 0.0;
	for (i=1; i <= Ini->MAX_TAZS; i++) {
		Total[0] += z_attrs[0][Ini->PURPOSE_TO_PROCESS][i];
		Total[1] += z_attrs[1][Ini->PURPOSE_TO_PROCESS][i];
		Total[2] += z_attrs[2][Ini->PURPOSE_TO_PROCESS][i];
	}



// generate clustering data structures
 	printf ("Generating packets of journey records\n");
	cluster_journeys (JourneyAttribs, &p_sort, &p_start);
	rand_nos = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_PACKETS*sizeof(int));
	p_order = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_PACKETS*sizeof(int));
//	addRAM ("run_main", 2*Ini->NUMBER_PACKETS*sizeof(int));
	for (i=0; i < Ini->NUMBER_PACKETS; i++) {
		rand_nos[i] = (int)(1000000000*rand()/MAX_RANDOM);
		p_order[i] = i;
	}
	isort (rand_nos, p_order, Ini->NUMBER_PACKETS);
	printf ("Done assigning packets to journey records\n");




	printf ("\nProcessing %s journey records:\n\n", PurposeLabels[Ini->PURPOSE_TO_PROCESS]);
//	printf ("Running Modal Proportions                        Journeys Processed\n");
//	printf ("-------------------------------------------      ------------------\n");
//	printf (" DA SR2 SR3 SR4  WT  DT  WC  DC  TX  NM  SB      JRNY #  OF   JRNYS\n");
	if (strcmp(Ini->RUNNING_MODE_SHARES, "") && fp_rep3 != NULL) {
		fprintf (fp_rep3, "\nProcessing %s journey records:\n\n", PurposeLabels[Ini->PURPOSE_TO_PROCESS]);
		fprintf (fp_rep3, "Running Modal Proportions                        Journeys Processed\n");
		fprintf (fp_rep3, "-------------------------------------------      ------------------\n");
		fprintf (fp_rep3, " DA SR2 SR3 SR4  WT  DT  WC  DC  TX  NM  SB      JRNY #  OF   JRNYS\n");
	}

	
	last_k = 0;
	unmet_count = 0;
	count = 0;
	seq = 0;
	count1 = 0;
	count2 = 0;
	rcount = 0;
	r = 0;
	fileIndex = 0;


	// process packets of journey records
	for (p=0; p < Ini->NUMBER_PACKETS; p++) {

		m = p_order[p];				// p_order is the list of packets in random order

		// get common information for the packet using first journey in the packet.
		k = p_sort[p_start[m]];
		orig = JourneyAttribs->orig[k];
		ut_orig = ZonalData->UrbanType[JourneyAttribs->orig[k]];
		income = JourneyAttribs->income[k];
		purpose = JourneyAttribs->purpose[k];
		person_type = JourneyAttribs->person_type[k];
		atwork_mode = JourneyAttribs->at_work_mode[k];



		// if this is a build scenario in an FTA_RESTART model run, pre-mode choice does not need to be run.
		// motorized/non-motorized is determined from the frozen base scenario results, and does not change for build.
		// dest and acc for non-motorized journeys are determined from the frozen base scenario results, and does not change for build.
		// dest for motorized journeys is determined from the frozen base scenario results, and does not change for build.
		// acc for motorized journeys may optionally be determined from the frozen base scenario results and not change, or
		// from applying a switching model to determine possibly a new choice.
		// mode choice for motorized journeys is determined by applying the mode choice model.
		if (Ini->FTA_RESTART == 1) {

			cnt = 0;
			for (n=p_start[m]; n < p_start[m+1]; n++) {

				k = p_sort[n];			// p_sort is the list of journey records sorted by packet number

				// determine if this journey is non-motorized or not
				if (frozenMode[k]-1 == 9) {
					motorized[cnt] = 0;
				}
				else {
					motorized[cnt] = 1;
				}
				cnt++;

			}

		}
		else {

			// determine the exponent parameter used in non-motorized destination choice for pre-mode choice logsum
			gamma = nm_dest_gamma (purpose, ut_orig, income);

			// determine pre-mode choice of all journey records within a packet
			cnt = 0;
			for (n=p_start[m]; n < p_start[m+1]; n++) {		// p_start is a list of pointers to beginning of packets of journeys
				k = p_sort[n];								// p_sort is the list indices for journey records sorted by packet number

				// write debug proportions at user specified intervals
				if (Ini->DEBUG_ORIG > 0) {
					debug_part = (int)((100*(count + cnt)/Ini->NUMBER_JOURNEYS) / Ini->DEBUG_PERCENT);
					if ((((int)(100*(count + cnt)/Ini->NUMBER_JOURNEYS) % (int)Ini->DEBUG_PERCENT) == 0) || ((count + cnt) == (Ini->NUMBER_JOURNEYS - 1))) {
						if (debug_parts[debug_part] == 0 || ((count + cnt) == (Ini->NUMBER_JOURNEYS - 1))) {
							debug_props (0, DCcoeff, JourneyAttribs, ZonalData, OD_Utility, z_attrs, RiverData, hwy_dist, dcConstants, mMcAscData, nmMcAscData, util, props);
							debug_parts[debug_part] = 1;
						}
					}
				}

				// pre-mode choice
				motorized[cnt] = pre_mode_choice (k, gamma, JourneyAttribs, z_attrs[2], t_attrs, ZonalData, WalkZoneData, mMcAscData, nmMcAscData);
				cnt++;
			}



			// determine destination choice proportions of all non-motorized journey records within a packet
			for (j=0; j < Ini->MAX_TAZS+1; j++)
				dest_props[j] = 0.0;
			nm_dest_props (purpose, orig, gamma, z_attrs[2][purpose], ZonalData->SchoolDist, WalkZoneData, dest_props);

		}



		cnt = 0;
		for (n=p_start[m]; n < p_start[m+1]; n++) {

			k = p_sort[n];			// p_sort is the list of journey records sorted by packet number

			// determine if this journey is non-motorized or not
			if (motorized[cnt] == 0) {


				if (Ini->FTA_RESTART != 1) {

					// non-motorized destination choice
					dest = nm_dest_choice (orig, WalkZoneData, dest_props);
					if (dest <= 0) {
						motorized[cnt] = -1;
						cnt++;
						continue;
					}

					// non-motorized transit accessibility sub-zone choice
					// determine if the chosen destination is walk accessible at destination or not
					if (z_attrs[0][purpose][dest] <= 0.0 && z_attrs[1][purpose][dest] > 0.0)
						acc = 1;
					else if (z_attrs[1][purpose][dest] <= 0.0 && z_attrs[0][purpose][dest] > 0.0)
						acc = 0;
					else if (z_attrs[0][purpose][dest] <= 0.0 && z_attrs[1][purpose][dest] <= 0.0) {
						if (z_attrs[0][purpose][dest] > z_attrs[1][purpose][dest])
							acc = 0;
						else
							acc = 1;
					}
					else {
						nrand = (double)rand()/(double)MAX_RANDOM;
						pct_acc = z_attrs[1][purpose][dest]/z_attrs[2][purpose][dest];
						if (nrand < pct_acc)
							acc = 1;
						else
							acc = 0;
					}

				}
				else {

					dest = frozenDest[k];

					if (Ini->USE_FROZEN_SUBZONE == 1) {
						acc = frozenAcc[k];
					}
					else {
						printf ("\nsubzone choice during FTA_RESTART run not currently implemented.\nset USE_FROZEN_SUBZONE = 1 for now.\nexit (-38)\n");
						fprintf (fp_rep, "\nsubzone choice during FTA_RESTART run not currently implemented.\nset USE_FROZEN_SUBZONE = 1 for now.\nexit (-38)\n");
						fflush (fp_rep);
						fflush (stdout);
						exit (-38);
					}

				}


				dest_freq[dest]++;
				mode = 9;


				if ((dest_freq[dest] - Ini->ATTR_CONSTRAINT_FACTOR*t_attrs[purpose][dest]) >= Ini->MAX_PACKET ) {
					printf ("\nnon-motorized dest choice\np=%d\nm=%d\nn=%d\nk=%d\norig=%d\ndest=%d\nacc=%d\nnon-acc size=%f\nacc size=%f\nsize=%f\ndest_freq=%f\ninitial size=%f\nexit (-97)\n", p, m, n, k, orig, dest, acc, z_attrs[0][purpose][dest], z_attrs[1][purpose][dest], z_attrs[2][purpose][dest], dest_freq[dest], t_attrs[purpose][dest]);
					fprintf (fp_rep, "\nnon-motorized dest choice\np=%d\nm=%d\nn=%d\nk=%d\norig=%d\ndest=%d\nacc=%d\nnon-acc size=%f\nacc size=%f\nsize=%f\ndest_freq=%f\ninitial size=%f\nexit (-97)\n", p, m, n, k, orig, dest, acc, z_attrs[0][purpose][dest], z_attrs[1][purpose][dest], z_attrs[2][purpose][dest], dest_freq[dest], t_attrs[purpose][dest]);
					fflush (stdout);
					fflush (fp_rep);
					exit (-97);
				}


				// if purpose is work (0, 1, or 2) write journey info to output file.
				if (purpose < 3 && Calibrating == 0)
					fprintf (fp_work[0], "%d %d %d %d\n", JourneyAttribs->hh[k], JourneyAttribs->persno[k], dest, mode+1);

				


				// decrement zonal attractions
				z_attrs[acc][purpose][dest] --;
				z_attrs[2][purpose][dest] --;
				Total[acc] --;
				Total[2] --;

				idist = ZonalData->nm_mc_asc_index[orig];
				idist_index = nmMcAscData.nmMcAscIndices->indexIndices[idist];
				nm_msc_index_value = nmMcAscData.indices[idist_index];
				nm_msc_index_index = nmMcAscData.nmMcAscConstants->indexIndices[nm_msc_index_value];
				nm_cal_est[nm_msc_index_index][0]++;
				nm_cal_est[nm_msc_index_index][1]++;

				idist = ZonalData->m_mc_asc_index[orig];
				idist_index = mMcAscData.mMcAscIndices->indexIndices[idist];
				jdist = ZonalData->m_mc_asc_index[dest];
				jdist_index = mMcAscData.mMcAscIndices->indexIndices[jdist];
				m_msc_index_value = mMcAscData.indices[idist_index][jdist_index];
				m_msc_index_index = mMcAscData.mMcAscConstants->indexIndices[m_msc_index_value];
				m_cal_est[m_msc_index_index][mode]++;

				if ( Ini->ZERO_UTIL == 0 ) {
					if (hwy_dist[orig][dest] <= 0) {
						printf ("\nnon-positive highway distance skim found from %d to %d, dist = %f\nexit (-98)\n", orig, dest, hwy_dist[orig][dest]);
						fprintf (fp_rep, "\nnon-positive highway distance skim found from %d to %d, dist = %f\nexit (-98)\n", orig, dest, hwy_dist[orig][dest]);
						fflush (fp_rep);
						fflush (stdout);
						exit (-98);
					}
					else {
						dist = (int)(hwy_dist[orig][dest]/Ini->TRIP_LENGTH_WIDTH);
						if (dist >= Ini->TRIP_LENGTH_RANGES)
							dist = Ini->TRIP_LENGTH_RANGES - 1;
						tlfreq[dist+1][mode] ++;
						tlfreq[0][mode] ++;
						tldist[mode] += hwy_dist[orig][dest];
					}
				}

				// increment running count on modal shares
				mc[districtDefinitions->modeReportDefinitions->numValues][mode]++;
				idist = ZonalData->mode_report_index[dest];
				idist_index = districtDefinitions->modeReportDefinitions->indexIndices[idist];
				mc[idist_index][mode]++;
				acc_freq[0][mode] += acc;
				acc_freq[idist_index][mode] += acc;;
				shares[9] += 1.0;

				origDist = ZonalData->dist_to_dist_report_index[orig];
				origDistIndex = districtDefinitions->distToDistReportDefinitions->indexIndices[origDist];
				destDist = ZonalData->dist_to_dist_report_index[dest];
				destDistIndex = districtDefinitions->distToDistReportDefinitions->indexIndices[destDist];
				dist2dist[mode+1][origDistIndex][destDistIndex]++;
				dist2dist[0][origDistIndex][destDistIndex]++;

			
				// don't write out MDC output if doing Sub-area analysis or if not the last auto-calibration iteration
				if ( fp3 != NULL ) {
					if ( Ini->CORRECT_ORDER == 1 )
						fprintf (fp3, "%d %d %d %d %d\n", k, orig, dest, purpose+1, mode+1);
					else
						fprintf (fp3, "%d %d %d %d %d\n", seq, orig, dest, purpose+1, mode+1);
				}
				seq++;



				// write out the information needed for FTA_RESTART analysis
				if (Ini->FREEZE_MDC_OUTPUT == 1 && fp_freeze[fileIndex] != NULL) {
					// removing 4 fields not present in the file read by user benefits calculation program
					//fprintf (fp_freeze[fileIndex], "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf\n", seq,k,JourneyAttribs->hh[k],JourneyAttribs->persno[k],JourneyAttribs->haj[k],purpose+1,income,JourneyAttribs->autos[k],JourneyAttribs->autos[k],JourneyAttribs->workers[k],orig,dest,JourneyAttribs->walk_orig[k],acc,ZonalData->lpRestricted[orig],ZonalData->lpRestricted[dest],mode+1,atwork_mode,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0);
					fprintf (fp_freeze[fileIndex], "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf\n", seq,k,JourneyAttribs->hh[k],JourneyAttribs->persno[k],JourneyAttribs->haj[k],purpose+1,income,JourneyAttribs->autos[k],JourneyAttribs->autos[k],orig,dest,JourneyAttribs->walk_orig[k],acc,mode+1,atwork_mode,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0);
					if ( count1 == 0 ) {
						fprintf (fp_rep,"first frozen non-motorized journey record:\n");
						fprintf (fp_rep,"seq=%d\nk=%d\nhh=%d\npers=%d\nhaj=%d\npurpose=%d\nincome=%d\nautos=%d\nworkers=%d\norig=%d\ndest=%d\nwalk_orig=%d\nacc=%d\nmode=%d\natwork_mode=%d\nPr[0]=%.1lf\nPr[1]=%.1lf\nPr[2]=%.1lf\nPr[3]=%.1lf\nPr[4]=%.1lf\nPr[5]=%.1lf\nPr[6]=%.1lf\nPr[7]=%.1lf\nPr[8]=%.1lf\nPr[9]=%.1lf\nPr[10]=%.1lf\nUt[0]=%.1lf\nUt[1]=%.1lf\nUt[2]=%.1lf\nUt[3]=%.1lf\nUt[4]=%.1lf\nUt[5]=%.1lf\nUt[6]=%.1lf\nUt[7]=%.1lf\nUt[8]=%.1lf\nUt[9]=%.1lf\nUt[10]=%.1lf\nPrNoAcc=%.1lf\nPrAcc=%.1lf\n", seq,k,JourneyAttribs->hh[k],JourneyAttribs->persno[k],JourneyAttribs->haj[k],purpose+1,income,JourneyAttribs->autos[k],JourneyAttribs->workers[k],orig,dest,JourneyAttribs->walk_orig[k],acc,mode+1,atwork_mode,0,0,0,0,0,0,0,0,0,1.0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
						fflush(fp_rep);
					}
					count1++;
					rcount++;
					r++;

					if (r == MAX_FROZEN_RECORDS) {
						r = 0;
						fileIndex++;
					}
				}



				
				if (count % PROGRESS_INCREMENT1 == 0) {
					printf ("%3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d     %7d  of %7d\r",
					//printf ("\nnon-motorized dest choice\np=%d\nm=%d\nn=%d\nk=%d\norig=%d\ndest=%d\nacc=%d\nnon-acc size=%f\nacc size=%f\nsize=%f\ndest_freq=%f\ninitial size=%f\nexit (-97)\n", p, m, n, k, orig, dest, acc, z_attrs[0][purpose][dest], z_attrs[1][purpose][dest], z_attrs[2][purpose][dest], dest_freq[dest], t_attrs[purpose][dest]);
					//printf ("%3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d     %7d  of %7d\n",
						pct(mc[districtDefinitions->modeReportDefinitions->numValues][0], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][1], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][2], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][3], count),
						pct(mc[districtDefinitions->modeReportDefinitions->numValues][4], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][5], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][6], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][7], count),
						pct(mc[districtDefinitions->modeReportDefinitions->numValues][8], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][9], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][10], count),
						count, Ini->NUMBER_JOURNEYS);
				}
				if (count % PROGRESS_INCREMENT == 0) {
					if (strcmp(Ini->RUNNING_MODE_SHARES, "") && Calibrating == 0) {
						fprintf (fp_rep3, "%3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d     %7d  of %7d\n",
							pct(mc[districtDefinitions->modeReportDefinitions->numValues][0], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][1], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][2], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][3], count),
							pct(mc[districtDefinitions->modeReportDefinitions->numValues][4], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][5], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][6], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][7], count),
							pct(mc[districtDefinitions->modeReportDefinitions->numValues][8], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][9], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][10], count),
							count, Ini->NUMBER_JOURNEYS);
						fflush (fp_rep3);
					}
				}

				// count is the count all journeys
				count++;
					
			}

			// cnt is the count of journeys in the packet
			cnt++;
		}





		// determine destination choice proportions for motorized journey records within a packet
		// use the first motorized journey in the packet to determine choice proportions.
		n = p_start[m];
		cnt = 0;
		while (motorized[cnt] == 0 && n < p_start[m+1]) {
			cnt++;
			n++;
		}
		k = p_sort[n];			// p_sort is the list of journey records sorted by packet number

		for	(i=0; i < Ini->NUMBER_ALTS; i++)
			SEutil[i] = 0.0;
		SE_Utilities (k, JourneyAttribs, ZonalData, SEutil);


		walk[0] = JourneyAttribs->walk_orig[k];


		if (Ini->FTA_RESTART != 1) {

			da_work = (JourneyAttribs->at_work_mode[k] == 1);

			for (j=1; j <= Ini->MAX_TAZS; j++) {
				se_flag = 0;
				od_flag = 0;
				for	(i=0; i < Ini->NUMBER_ALTS; i++)
					ODutil[i] = 0.0;
				for	(i=0; i < Ini->NUMBER_ALTS; i++) {
					if (Ini->ZERO_UTIL == 0 && i < Ini->NUMBER_ALTS - 2) {
						ODutil[i] = OD_Utility[i][orig][j];
						if (ODutil[i] == MISSING)
							od_flag++;
					}
					if (SEutil[i] == MISSING)
						se_flag++;
				}

				// calculate motorized mode choice logsums for each destination (with walk access at dest and without)
				if (se_flag < Ini->NUMBER_ALTS && od_flag < Ini->NUMBER_ALTS - 2) {
					walk[1] = 0;

					idist = ZonalData->m_mc_asc_index[orig];
					idist_index = mMcAscData.mMcAscIndices->indexIndices[idist];
					jdist = ZonalData->m_mc_asc_index[j];
					jdist_index = mMcAscData.mMcAscIndices->indexIndices[jdist];
					m_msc_index_value = mMcAscData.indices[idist_index][jdist_index];
					m_msc_index_index = mMcAscData.mMcAscConstants->indexIndices[m_msc_index_value];

					// if a packet we want to explore, change k to negative to trigger debug info in mc_logsum[] function.
//					if ( j == 2438 && ( p == 139020 || p == 139069 || p == 139073 ) )
//						k *= -1;
	
					if ( k == 2980115 )
							k *= -1;

					mc_logsum[purpose] (k, orig, j, AvailModes, person_type, JourneyAttribs->autos[k], walk, da_work, ODutil, SEutil, (float *)mMcAscData.constants[m_msc_index_index], Logsum[j]);
					walk[1] = 1;
				}
				else {
					Logsum[j][0] = MISSING;
					Logsum[j][1] = MISSING;
				}
			}



			// motorized choice proportions, returns cumulative probability distribution
			// of joint destination/access probability distribution.
		
			motor_dc_coeffs (ZonalData->County[orig], Ini->PURPOSE_TO_PROCESS, DCcoeff, DistFactors);

			for (i=0; i < Ini->MAX_TAZS*2; i++)
				dc_cum[i] = 0.0;


//			if ( p == 139020 || p == 139069 || p == 139073 ) {
//				debug_mode = 1;
//				sprintf(tempString, "writing debug info for p=%d to DEBUGFILE=%s.\n", p, Ini->DEBUGFILE);
//				fprintf(fp_rep, "%s", tempString);
//				printf("%s", tempString);
//				fflush(fp_rep);
//			}
//			else {
//				debug_mode = 0;
//			}


			motor_dc_props (orig, purpose, DCcoeff, OD_Utility, SEutil, Logsum, z_attrs, RiverData, ZonalData, dcConstants, dc_cum, hwy_dist[JourneyAttribs->orig[k]], debug_mode, util, props, 0);

		}



		cnt = 0;
		for (n=p_start[m]; n < p_start[m+1]; n++) {

			k = p_sort[n];			// p_sort is the list of journey records sorted by packet number

			// use this to stop on last packet in debugger.
			//if ((p == Ini->NUMBER_PACKETS - 1) && (n == p_start[m+1] - 1)) {
			//	k = p_sort[n];
			//}


			// determine if this journey is motorized or not
			// motorized choice if motorized == 1 or motorized == -1 (i.e. != 0)
			if (motorized[cnt] != 0) {


				if (motorized[cnt] == -1) {
					unmet_count++;
				}

				if (Ini->FTA_RESTART != 1) {

					// select a destination/access pair at random
					nrand = (double)rand()/(double)MAX_RANDOM;
					i = binary_search (nrand, 0, 2*Ini->MAX_TAZS - 1, dc_cum);


					if ( i < 0 ) {
						sprintf (tempString, "m=%d, p=%d, n=%d, k=%d journey had no available destination choice alternatives to choose from; count=%d.\n", m, p, n, k, badDestCount);
						fprintf(fp_rep, "%s", tempString);
						badDest = 1;

						dest = orig;
						acc = 0;
						pAcc = 0.0;
						pNoAcc = 1.0;
					}
					else {

						badDest = 0;

						dest = i/2 + 1;
						dest_freq[dest]++;
						if ((i+1) % 2 == 1)
							acc = 0;
						else
							acc = 1;


						// for base scenario runs, get the relative proportions of subzone choice from the cumulative joint dest/subzone choice probabilities
						if (i == 0) {
							pNoAcc = dc_cum[0];
							pAcc = dc_cum[1];
						}
						else if (i == 1) {
							pAcc = dc_cum[1];
							pNoAcc = dc_cum[0];
						}
						else {
							if (acc == 0) {
								pNoAcc = dc_cum[i] - dc_cum[i-1];
								pAcc = dc_cum[i+1] - dc_cum[i];
							}
							else {
								pAcc = dc_cum[i] - dc_cum[i-1];
								pNoAcc = dc_cum[i-1] - dc_cum[i-2];
							}
						}



						if ((dest_freq[dest] - Ini->ATTR_CONSTRAINT_FACTOR*t_attrs[purpose][dest]) >= Ini->MAX_PACKET ) {
							//printf ("\nmotorized dest choice\np=%d\nk=%d\nm=%d\nn=%d\ni=%d\ndc_cum[i]=%f\norig=%d\ndest=%d\nacc=%d\nnon-acc size=%f\nacc size=%f\nsize=%f\ndest_freq=%d\ninitial size=%f\nexit (-99)\n", p, k, m, n, i, dc_cum[i], orig, dest, acc, z_attrs[0][purpose][dest], z_attrs[1][purpose][dest], z_attrs[2][purpose][dest], dest_freq[dest], t_attrs[purpose][dest]);
							fprintf (fp_rep, "\nmotorized dest choice for count=%d\np=%d\nk=%d\nm=%d\nn=%d\ni=%d\ndc_cum[i]=%f\norig=%d\ndest=%d\nacc=%d\nnon-acc size=%f\nacc size=%f\nsize=%f\ndest_freq=%d\ninitial size=%f\nexit (-99)\n", count, p, k, m, n, i, dc_cum[i], orig, dest, acc, z_attrs[0][purpose][dest], z_attrs[1][purpose][dest], z_attrs[2][purpose][dest], dest_freq[dest], t_attrs[purpose][dest]);
							//fflush (stdout);
							fflush (fp_rep);
							exit (-99);
						}


					}

				}
				else {

					dest = frozenDest[k];

					// for FTA_RESTART, see if we're using frozen subzone and if so use that value; otherwise apply subzone switching choice model.
					if (Ini->USE_FROZEN_SUBZONE == 1) {
						acc = frozenAcc[k];
						pAcc = 0.0;
						pNoAcc = 0.0;
					}
					else {
						printf ("\nsubzone choice during FTA_RESTART run not currently implemented.\nset USE_FROZEN_SUBZONE = 1 for now.\nexit (-38)\n");
						fprintf (fp_rep, "\nsubzone choice during FTA_RESTART run not currently implemented.\nset USE_FROZEN_SUBZONE = 1 for now.\nexit (-38)\n");
						fflush (fp_rep);
						fflush (stdout);
						exit (-38);
					}

					if (Ini->USE_FROZEN_WORKTOUR_MODE == 1) {
						da_work = (frozenAtWorkMode[k] == 1);
					}
					else {
						da_work = (JourneyAttribs->at_work_mode[k] == 1);
					}

				}




				// get the modal proportions for this destination

				// if orig or selected destination TAZ is in the restricted area, use restricted SE_Utility to reflect
				// license plate rationing policy implemented.  Restricted SE_Utility reflects altered auto sufficiency
				// if a HHs auto was subject to rationing.
				// also use autosAvail instead of autos owned for mode choice proportions.

				autos = JourneyAttribs->autos[k];
				if ( (Ini->PCT_LP_RESTRICTION > 0.0 || Ini->PCT_HHS_WITH_FULL_AUTO_RESTRICTION > 0.0)  &&  (ZonalData->lpRestricted[orig] || ZonalData->lpRestricted[dest]) ) {
					for	(i=0; i < Ini->NUMBER_ALTS; i++)
						SEutil[i] = 0.0;
					SE_UtilitiesRestricted (k, JourneyAttribs, ZonalData, SEutil);
					autos = JourneyAttribs->autosAvail[k];

				}
				else {

					for	(i=0; i < Ini->NUMBER_ALTS; i++)
						SEutil[i] = 0.0;
					SE_Utilities (k, JourneyAttribs, ZonalData, SEutil);

				}


				se_flag = 0;
				od_flag = 0;
				for	(i=0; i < Ini->NUMBER_ALTS; i++) {
					Prob[i] = 0.0;
					if (Ini->ZERO_UTIL == 0 && i < Ini->NUMBER_ALTS - 2) {
						ODutil[i] = OD_Utility[i][orig][dest];
						if (ODutil[i] == MISSING)
							od_flag++;
					}
					if (SEutil[i] == MISSING)
						se_flag++;
				}
				if (se_flag < Ini->NUMBER_ALTS && od_flag < Ini->NUMBER_ALTS - 2) {
					walk[1] = acc;
					idist = ZonalData->m_mc_asc_index[orig];
					idist_index = mMcAscData.mMcAscIndices->indexIndices[idist];
					jdist = ZonalData->m_mc_asc_index[dest];
					jdist_index = mMcAscData.mMcAscIndices->indexIndices[jdist];
					m_msc_index_value = mMcAscData.indices[idist_index][jdist_index];
					m_msc_index_index = mMcAscData.mMcAscConstants->indexIndices[m_msc_index_value];
					mc_props[purpose] (k, AvailModes, person_type, autos, walk, da_work, ODutil, SEutil, Prob, (float *)mMcAscData.constants[m_msc_index_index], TotalUtility);
				}
				else {
					//printf ("no modes available for journey=%d , packet=%d, from %d to %d\n", k, p, orig, dest);
					//printf ("se_flag=%d , od_flag=%d\n", se_flag, od_flag);
					fprintf (fp_rep, "no modes available for journey=%d , packet=%d, from %d to %d\n", k, p, orig, dest);
					fprintf (fp_rep, "se_flag=%d , od_flag=%d\n", se_flag, od_flag);
					fflush (fp_rep);
					//fflush (stdout);
					exit (-17);
				}
					

				
				// now select a mode at random
				for (i=0; i < Ini->NUMBER_ALTS; i++)
					mc_cum[i] = 0.0;
				mc_cum[0] = Prob[0];
				shares[0] += Prob[0];
				for (i=1; i < Ini->NUMBER_ALTS; i++) {
					mc_cum[i] = mc_cum[i-1] + Prob[i];
					shares[i] += Prob[i];
				}
				mc_cum[Ini->NUMBER_ALTS - 1] = 1.0;

				nrand = (double)rand()/(double)MAX_RANDOM;
				for (i=0; i < Ini->NUMBER_ALTS; i++) {
					if (nrand < mc_cum[i]) {
						mode = i;
						i = Ini->NUMBER_ALTS;
					}
				}





				// if purpose is work (0,1, or 2) write journey info to output file.
				if (purpose < 3 && Calibrating == 0)
					fprintf (fp_work[0], "%d %d %d %d\n", JourneyAttribs->hh[k], JourneyAttribs->persno[k], dest, mode+1);




				if ( badDest == 1 ) {

					badDestCount++;

				}
				else {

					// decrement zonal attractions
					z_attrs[acc][purpose][dest] --;
					z_attrs[2][purpose][dest] --;
					Total[acc] --;
					Total[2] --;

				}



				m_cal_est[m_msc_index_index][mode]++;

				idist = ZonalData->nm_mc_asc_index[orig];
				idist_index = nmMcAscData.nmMcAscIndices->indexIndices[idist];
				nm_msc_index_value = nmMcAscData.indices[idist_index];
				nm_msc_index_index = nmMcAscData.nmMcAscConstants->indexIndices[nm_msc_index_value];
				nm_cal_est[nm_msc_index_index][1]++;


				mc[districtDefinitions->modeReportDefinitions->numValues][mode]++;
				idist = ZonalData->mode_report_index[dest];
				idist_index = districtDefinitions->modeReportDefinitions->indexIndices[idist];
				mc[idist_index][mode]++;
				acc_freq[0][mode] += acc;
				acc_freq[ZonalData->mode_report_index[dest]][mode] += acc;

				dist = (int)(hwy_dist[orig][dest]/Ini->TRIP_LENGTH_WIDTH);
				if (dist >= Ini->TRIP_LENGTH_RANGES)
					dist = Ini->TRIP_LENGTH_RANGES - 1;
				tlfreq[dist+1][mode] ++;
				tlfreq[0][mode] ++;
				if (hwy_dist[orig][dest] < 0) {
					i = 0;
				}
				else {
					tldist[mode] += hwy_dist[orig][dest];
				}

				origDist = ZonalData->dist_to_dist_report_index[orig];
				origDistIndex = districtDefinitions->distToDistReportDefinitions->indexIndices[origDist];
				destDist = ZonalData->dist_to_dist_report_index[dest];
				destDistIndex = districtDefinitions->distToDistReportDefinitions->indexIndices[destDist];
				dist2dist[mode+1][origDistIndex][destDistIndex]++;
				dist2dist[0][origDistIndex][destDistIndex]++;



				// don't write out MDC output if doing Sub-area analysis or if not the last auto-calibration iteration
				if ( fp3 != NULL ) {
					if ( Ini->CORRECT_ORDER == 1 )
						fprintf (fp3, "%d %d %d %d %d\n", k, orig, dest, purpose+1, mode+1);
					else
						fprintf (fp3, "%d %d %d %d %d\n", seq, orig, dest, purpose+1, mode+1);
				}
				seq++;



				// write out the information needed for FTA_RESTART analysis
				if (Ini->FREEZE_MDC_OUTPUT == 1 && fp_freeze[fileIndex] != NULL) {
					// removing 4 fields not present in the file read by user benefits calculation program
					//fprintf (fp_freeze[fileIndex], "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8le,%.8le,%.8le,%.8le,%.8le,%.8le,%.8le,%.8le,%.8le,%.8le,%.8le,%.8le,%.8le\n", seq,k,JourneyAttribs->hh[k],JourneyAttribs->persno[k],JourneyAttribs->haj[k],purpose+1,income,JourneyAttribs->autos[k],autos,JourneyAttribs->workers[k],orig,dest,JourneyAttribs->walk_orig[k],acc,ZonalData->lpRestricted[orig],ZonalData->lpRestricted[dest],mode+1,atwork_mode,nrand,Prob[0],Prob[1],Prob[2],Prob[3],Prob[4],Prob[5],Prob[6],Prob[7],Prob[8],Prob[9],Prob[10],TotalUtility[0],TotalUtility[1],TotalUtility[2],TotalUtility[3],TotalUtility[4],TotalUtility[5],TotalUtility[6],TotalUtility[7],TotalUtility[8],TotalUtility[9],TotalUtility[10],pNoAcc,pAcc);
					fprintf (fp_freeze[fileIndex], "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8lf,%.8le,%.8le,%.8le,%.8le,%.8le,%.8le,%.8le,%.8le,%.8le,%.8le,%.8le,%.8le,%.8le\n", seq,k,JourneyAttribs->hh[k],JourneyAttribs->persno[k],JourneyAttribs->haj[k],purpose+1,income,JourneyAttribs->autos[k],JourneyAttribs->workers[k],orig,dest,JourneyAttribs->walk_orig[k],acc,mode+1,atwork_mode,Prob[0],Prob[1],Prob[2],Prob[3],Prob[4],Prob[5],Prob[6],Prob[7],Prob[8],Prob[9],Prob[10],TotalUtility[0],TotalUtility[1],TotalUtility[2],TotalUtility[3],TotalUtility[4],TotalUtility[5],TotalUtility[6],TotalUtility[7],TotalUtility[8],TotalUtility[9],TotalUtility[10],pNoAcc,pAcc);
					if ( count2 == 0 ) {
						fprintf (fp_rep,"first frozen motorized journey record:\n");
						fprintf (fp_rep,"seq=%d\nk=%d\nhh=%d\npers=%d\nhaj=%d\npurpose=%d\nincome=%d\nautos=%d\nworkers=%d\norig=%d\ndest=%d\nwalk_orig=%d\nacc=%d\nmode=%d\natwork_mode=%d\nPr[0]=%.8lf\nPr[1]=%.8lf\nPr[2]=%.8lf\nPr[3]=%.8lf\nPr[4]=%.8lf\nPr[5]=%.8lf\nPr[6]=%.8lf\nPr[7]=%.8lf\nPr[8]=%.8lf\nPr[9]=%.8lf\nPr[10]=%.8lf\nUt[0]=%.8le\nUt[1]=%.8le\nUt[2]=%.8le\nUt[3]=%.8le\nUt[4]=%.8le\nUt[5]=%.8le\nUt[6]=%.8le\nUt[7]=%.8le\nUt[8]=%.8le\nUt[9]=%.8le\nUt[10]=%.8le\nPrNoAcc=%.8lf\nPrAcc=%.8lf\n", seq,k,JourneyAttribs->hh[k],JourneyAttribs->persno[k],JourneyAttribs->haj[k],purpose+1,income,JourneyAttribs->autos[k],JourneyAttribs->workers[k],orig,dest,JourneyAttribs->walk_orig[k],acc,mode+1,atwork_mode,Prob[0],Prob[1],Prob[2],Prob[3],Prob[4],Prob[5],Prob[6],Prob[7],Prob[8],Prob[9],Prob[10],TotalUtility[0],TotalUtility[1],TotalUtility[2],TotalUtility[3],TotalUtility[4],TotalUtility[5],TotalUtility[6],TotalUtility[7],TotalUtility[8],TotalUtility[9],TotalUtility[10],pNoAcc,pAcc);
						fflush(fp_rep);
					}
					count2++;
					rcount++;
					r++;

					if (r == MAX_FROZEN_RECORDS) {
						r = 0;
						fileIndex++;
					}
				}



				if (count % PROGRESS_INCREMENT1 == 0) {
					//if ( count >= 2000 && count < 3000 )
					//	printf ("\nmotorized dest choice\ncount=%d\np=%d\nk=%d\nm=%d\nn=%d\ni=%d\ndc_cum[i]=%f\norig=%d\ndest=%d\nacc=%d\nnon-acc size=%f\nacc size=%f\nsize=%f\ndest_freq=%d\ninitial size=%f\nexit (-99)\nnrand=%.8f\n", count, p, k, m, n, i, dc_cum[i], orig, dest, acc, z_attrs[0][purpose][dest], z_attrs[1][purpose][dest], z_attrs[2][purpose][dest], dest_freq[dest], t_attrs[purpose][dest], nrand);
					//printf ("%3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d     %7d  of %7d\n",
					printf ("%3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d     %7d  of %7d\r",
						pct(mc[districtDefinitions->modeReportDefinitions->numValues][0], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][1], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][2], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][3], count),
						pct(mc[districtDefinitions->modeReportDefinitions->numValues][4], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][5], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][6], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][7], count),
						pct(mc[districtDefinitions->modeReportDefinitions->numValues][8], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][9], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][10], count),
						count, Ini->NUMBER_JOURNEYS);
				}

				if (count % PROGRESS_INCREMENT == 0) {
					if (strcmp(Ini->RUNNING_MODE_SHARES, "") && Calibrating == 0) {
						fprintf (fp_rep3, "%3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d     %7d  of %7d\n",
							pct(mc[districtDefinitions->modeReportDefinitions->numValues][0], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][1], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][2], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][3], count),
							pct(mc[districtDefinitions->modeReportDefinitions->numValues][4], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][5], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][6], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][7], count),
							pct(mc[districtDefinitions->modeReportDefinitions->numValues][8], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][9], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][10], count),
							count, Ini->NUMBER_JOURNEYS);
						fflush (fp_rep3);
					}
				}

				count++;
			}

			cnt++;
		} // end of for loop over journeys for given packet

	} // end of for loop over packets




	if (strcmp(Ini->RUNNING_MODE_SHARES, "") && Calibrating == 0) {
		fprintf (fp_rep3, "%3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d     %7d  of %7d\n",
			pct(mc[districtDefinitions->modeReportDefinitions->numValues][0], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][1], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][2], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][3], count),
			pct(mc[districtDefinitions->modeReportDefinitions->numValues][4], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][5], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][6], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][7], count),
			pct(mc[districtDefinitions->modeReportDefinitions->numValues][8], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][9], count), pct(mc[districtDefinitions->modeReportDefinitions->numValues][10], count),
			count, Ini->NUMBER_JOURNEYS);
		fflush (fp_rep3);
	}


	sprintf (tempString, "%d journeys had no available destination choice alternatives to choose from.  dest set to orig, and acc set to 0 for each.  %d total packets\n", badDestCount, Ini->NUMBER_PACKETS);
	printf ("%s", tempString);
	fprintf(fp_rep, "%s", tempString);


	
	if (fp3 != NULL )
		fclose (fp3);


	if (Ini->FREEZE_MDC_OUTPUT == 1 && fp_freeze[0] != NULL) {
	
		printf ("\nwrote %d frozen journey records to %d file(s).\n", rcount, fileIndex+1);
		fprintf (fp_rep, "\nwrote %d frozen journey records to %d file(s).\n", rcount, fileIndex+1);

		for (j=0; j < numFiles; j++)
			fclose(fp_freeze[j]);

	}


// produce final reports
	if (Calibrating == 0)
		final_reports (fp_rep2, mc, shares, acc_freq, tlfreq, tldist, dist2dist, t_attrs, ZonalData, districtDefinitions, unmet_count);



// Memory Deallocations
	for (j=0; j < 2+1; j++) {
		for (i=0; i < PURPOSES; i++)
			HeapFree (heapHandle, 0, z_attrs[j][i]);
		HeapFree (heapHandle, 0, z_attrs[j]);
	}
	HeapFree (heapHandle, 0, z_attrs);

	for (i=0; i < (Ini->MAX_TAZS+1); i++)
		HeapFree (heapHandle, 0, Logsum[i]);
	HeapFree (heapHandle, 0, Logsum);

	for (i=0; i < PURPOSES; i++)
		HeapFree (heapHandle, 0, t_attrs[i]);
	HeapFree (heapHandle, 0, t_attrs);

	for (i=0; i < Ini->NUMBER_ALTS+1; i++) {
		for (j=0; j < districtDefinitions->distToDistReportDefinitions->numValues; j++)
			HeapFree (heapHandle, 0, dist2dist[i][j]);
		HeapFree (heapHandle, 0, dist2dist[i]);
	}
	HeapFree (heapHandle, 0, dist2dist);

	for (i=0; i <= districtDefinitions->modeReportDefinitions->numValues; i++)
		HeapFree (heapHandle, 0, mc[i]);
	HeapFree (heapHandle, 0, mc);


	for (i=0; i <= districtDefinitions->modeReportDefinitions->numValues; i++)
		HeapFree (heapHandle, 0, acc_freq[i]);
	HeapFree (heapHandle, 0, acc_freq);

	for (i=0; i < Ini->TRIP_LENGTH_RANGES+2; i++)
		HeapFree (heapHandle, 0, tlfreq[i]);
	HeapFree (heapHandle, 0, tlfreq);

	HeapFree (heapHandle, 0, ODutil);
	HeapFree (heapHandle, 0, SEutil);
	HeapFree (heapHandle, 0, Prob);
	HeapFree (heapHandle, 0, mc_cum);
	HeapFree (heapHandle, 0, dc_cum);
	HeapFree (heapHandle, 0, debug_parts);
	HeapFree (heapHandle, 0, AvailModes);
	HeapFree (heapHandle, 0, tldist);
	HeapFree (heapHandle, 0, dest_freq);
	HeapFree (heapHandle, 0, dest_props);
	HeapFree (heapHandle, 0, attr_corrections);
	HeapFree (heapHandle, 0, shares);
	HeapFree (heapHandle, 0, DCcoeff);
	HeapFree (heapHandle, 0, frozenDest);
	HeapFree (heapHandle, 0, frozenAcc);
	HeapFree (heapHandle, 0, frozenMode);
	HeapFree (heapHandle, 0, frozenAtWorkMode);
//	relRAM ("run_mdc", ((int)(100/Ini->DEBUG_PERCENT) + 1)*sizeof(int)
//						+ (Ini->NUMBER_ALTS)*sizeof(int)
//						+ (Ini->MAX_TAZS+1)*sizeof(int)
//						+ 3*(Ini->NUMBER_ALTS)*sizeof(float)
//						+ 3*(Ini->NUMBER_ALTS)*sizeof(double)
//						+ (Ini->MAX_TAZS*2)*sizeof(double)
//						+ 2*(Ini->MAX_TAZS+1)*sizeof(double)
//						+ sizeof(struct dc_coeff_data)
//						+ 3*PURPOSES*(Ini->MAX_TAZS+1)*sizeof(float)
//						+ PURPOSES*(Ini->MAX_TAZS+1)*sizeof(float)
//						+ (Ini->MAX_TAZS+1)*2*sizeof(float)
//						+ 2*(Ini->NUMBER_BPMDIST1+1)*(Ini->NUMBER_ALTS+1)*sizeof(int)
//						+ (Ini->TRIP_LENGTH_RANGES+2)*(Ini->NUMBER_ALTS+1)*sizeof(int)
//						+ (Ini->NUMBER_ALTS+1)*(Ini->NUMBER_BPMDIST1+1)*(Ini->NUMBER_BPMDIST1+1)*sizeof(int));

	HeapFree (heapHandle, 0, rand_nos);
	HeapFree (heapHandle, 0, p_order);
//	relRAM ("run_main", 2*Ini->NUMBER_PACKETS*sizeof(int));

	HeapFree (heapHandle, 0, p_sort);
	HeapFree (heapHandle, 0, p_start);
//	relRAM ("run_main", Ini->NUMBER_JOURNEYS*sizeof(int) + (Ini->NUMBER_PACKETS+1)*sizeof(int));


	for (i=0; i < Ini->MAX_TAZS+1; i++) {
		HeapFree (heapHandle, 0, util[i]);
		HeapFree (heapHandle, 0, props[i]);
	}
    HeapFree (heapHandle, 0, util);
    HeapFree (heapHandle, 0, props);

}
