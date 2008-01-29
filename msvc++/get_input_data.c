#include "md.h"


// zonal parking percent and cost over-rides data input file formats
#define RECORD_LENGTH					120
#define TAZ_START						  1
#define TAZ_LENGTH						  5
#define W_PCT_FREE_START				  6
#define W_PCT_FREE_LENGTH				 15
#define NW_PCT_FREE_START				 21
#define NW_PCT_FREE_LENGTH				 20
#define W_PCT_INC_NONMAN_ORIG_START		 41
#define W_PCT_INC_NONMAN_ORIG_LENGTH	 20
#define NW_PCT_INC_NONMAN_ORIG_START	 61
#define NW_PCT_INC_NONMAN_ORIG_LENGTH	 20
#define W_PCT_INC_MAN_ORIG_START		 81
#define W_PCT_INC_MAN_ORIG_LENGTH		 20
#define NW_PCT_INC_MAN_ORIG_START		101
#define NW_PCT_INC_MAN_ORIG_LENGTH		 20
#define COST_INDEX_START				  1
#define COST_INDEX_LENGTH				  5
#define W_COST_VALUE_START				  6
#define W_COST_VALUE_LENGTH				 15
#define NW_COST_VALUE_START				 21
#define NW_COST_VALUE_LENGTH			 15



void get_input_data (FILE **fp3, FILE **fp_cal, FILE **fp_rep2, FILE **fp_rep3, FILE *fp_work[],
	int **ranprkcst, struct taxi_data *TaxiData, struct zone_data *ZonalData,
	struct river_crossing_data *RiverData, struct walk_zone_data **WalkZoneData,
	struct bpmdist1_coeff_data *BPMDist1, struct co_dist_factors *DistFactors, struct msc_data **msc)
{

	FILE *fp;
	int i, rec_len;

	char InputRecord[RECORD_LENGTH];
	char temp[RECORD_LENGTH];
	char value[RECORD_LENGTH];
	float tempValue;
	float wParkingCostArray[] = { 250, 750, 1250, 1700, 2250, 2750, 3250, 3700, 4250, 4750 };
	float nwParkingCostArray[] = { 250, 750, 1250, 1700, 2250, 2750, 3250, 3700, 4250, 4750 };


// if work purpose open file work destinations file for output; if at-work, open all 3 work destination files for input.
	if (Ini->PURPOSE_TO_PROCESS < 3) {
		// open file for writing work journey destinations for linking with at-work journeys
		if (Ini->SUBAREA_ANALYSIS != 1) {
			if (strcmp(Ini->WORKDESTFILE, "")) {
				if ((fp_work[0] = fopen(Ini->WORKDESTFILE, "wt")) == NULL)
					ExitWithCode(44);
			}
			else
				ExitWithCode (45);
		}
		else {
			if (strcmp(Ini->SUBAREA_WORKDESTFILE, "")) {
				if ((fp_work[0] = fopen(Ini->SUBAREA_WORKDESTFILE, "wt")) == NULL)
					ExitWithCode(131);
			}
			else
				ExitWithCode (132);
		}
	}
	else if (Ini->PURPOSE_TO_PROCESS == 7) {

		// open files for reading work journey destinations for linking with at-work journeys
		if (strcmp(Ini->ATWORK_LO_FILE, "")) {
			if ((fp_work[0] = fopen(Ini->ATWORK_LO_FILE, "rt")) == NULL)
				ExitWithCode(46);
		}
		else
			ExitWithCode (47);
		if (strcmp(Ini->ATWORK_MD_FILE, "")) {
			if ((fp_work[1] = fopen(Ini->ATWORK_MD_FILE, "rt")) == NULL)
				ExitWithCode(48);
		}
		else
			ExitWithCode (49);
		if (strcmp(Ini->ATWORK_HI_FILE, "")) {
			if ((fp_work[2] = fopen(Ini->ATWORK_HI_FILE, "rt")) == NULL)
				ExitWithCode(50);
		}
		else
			ExitWithCode (51);

		if (Ini->SUBAREA_ANALYSIS == 1) {
			if (strcmp(Ini->SUBAREA_ATWORK_LO_FILE, "")) {
				if ((fp_work[3] = fopen(Ini->SUBAREA_ATWORK_LO_FILE, "rt")) == NULL)
					ExitWithCode(133);
			}
			else
				ExitWithCode (134);
			if (strcmp(Ini->SUBAREA_ATWORK_MD_FILE, "")) {
				if ((fp_work[4] = fopen(Ini->SUBAREA_ATWORK_MD_FILE, "rt")) == NULL)
					ExitWithCode(135);
			}
			else
				ExitWithCode (136);
			if (strcmp(Ini->SUBAREA_ATWORK_HI_FILE, "")) {
				if ((fp_work[5] = fopen(Ini->SUBAREA_ATWORK_HI_FILE, "rt")) == NULL)
					ExitWithCode(137);
			}
			else
				ExitWithCode (138);
		}

	}



// report any inconsistency in specifying DEBUG option
	if (Ini->DEBUG_ORIG > 0 && strcmp(Ini->DEBUGFILE, "")) {
//		printf ("Debugging output was requested with DEBUG_ORIG = %d, but no DEBUGFILE entry was found in .ini file\n", Ini->DEBUG_ORIG);
//		printf ("Debugging output will be written to .\\debug.out\n\n");
		fprintf (fp_rep, "Debugging output was requested with DEBUG_ORIG = %d, but no DEBUGFILE entry was found in .ini file\n", Ini->DEBUG_ORIG);
		fprintf (fp_rep, "Debugging output will be written to .\\debug.out\n\n");
		strcpy (Ini->DEBUGFILE, "./debug.out");
	}


	
// open output journeys file, the file sent to David Rhoden for time-of-day and trip matrix file processing
	if (Ini->STOPS_ONLY != 1) {
		if (Ini->SUBAREA_ANALYSIS != 1) {
			if (strcmp(Ini->OUTPUTJOURNEYS, "")) {
				if ((*fp3 = fopen(Ini->OUTPUTJOURNEYS, "wt")) == NULL)
					ExitWithCode(18);
			}
			else
				ExitWithCode (34);
		}
		else {
			if (strcmp(Ini->SUBAREA_MDC_OUTPUT, "")) {
				if ((*fp3 = fopen(Ini->SUBAREA_MDC_OUTPUT, "wt")) == NULL)
					ExitWithCode(127);
			}
			else
				ExitWithCode (128);
		}
	}



// open report file for county to county by mode journey frequency report
	if (Ini->SUBAREA_ANALYSIS != 1) {
		if (strcmp(Ini->CO_CO_REPORTFILE, "")) {
			if ((*fp_rep2 = fopen(Ini->CO_CO_REPORTFILE, "wt")) == NULL)
				ExitWithCode(54);
		}
	}
	else {
		if (strcmp(Ini->SUBAREA_CO_CO_REPORTFILE, "")) {
			if ((*fp_rep2 = fopen(Ini->SUBAREA_CO_CO_REPORTFILE, "wt")) == NULL)
				ExitWithCode(130);
		}
	}



// open report file for running mode share percentages report
	if (strcmp(Ini->RUNNING_MODE_SHARES, "")) {
		if ((*fp_rep3 = fopen(Ini->RUNNING_MODE_SHARES, "wt")) == NULL)
			ExitWithCode(55);
	}



// open report file for auto-calibration summary reports
	if (strcmp(Ini->CALIBREPORTFILE, "")) {
		if ((*fp_cal = fopen(Ini->CALIBREPORTFILE, "wt")) == NULL)
			ExitWithCode(56);
	}


// allocate memory for mode specific constants data structure
	(*msc) = (struct msc_data *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 1*sizeof(struct msc_data));

	(*msc)->motorized_indices = (int **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_BPMDIST1+2)*sizeof(int *));
	for (i=0; i < Ini->NUMBER_BPMDIST1+2; i++)
		(*msc)->motorized_indices[i] = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_BPMDIST1+2)*sizeof(int));

	(*msc)->motorized_index_used = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, ((int)pow(Ini->NUMBER_BPMDIST1+2,2))*sizeof(int));

	(*msc)->motorized_labels = (char **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, ((int)pow(Ini->NUMBER_BPMDIST1+2,2))*sizeof(char *));
	(*msc)->motorized_targets = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, ((int)pow(Ini->NUMBER_BPMDIST1+2,2))*sizeof(float *));
	(*msc)->MSC = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, ((int)pow(Ini->NUMBER_BPMDIST1+2,2))*sizeof(float *));
	for (i=0; i < (int)pow(Ini->NUMBER_BPMDIST1+2,2); i++) {
		(*msc)->motorized_labels[i] = (char *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 64*sizeof(char));
		(*msc)->motorized_targets[i] = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_ALTS*sizeof(float));
		(*msc)->MSC[i] = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_ALTS*sizeof(float));
	}

	(*msc)->nm_indices = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 2*(Ini->NUMBER_BPMDIST1+2)*sizeof(int));
	(*msc)->nm_index_used = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 2*(Ini->NUMBER_BPMDIST1+2)*sizeof(int));
	(*msc)->nm_targets = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 2*(Ini->NUMBER_BPMDIST1+2)*sizeof(float));
	(*msc)->nmMSC = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 2*(Ini->NUMBER_BPMDIST1+2)*sizeof(float));

	(*msc)->nm_labels = (char **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 2*(Ini->NUMBER_BPMDIST1+2)*sizeof(char *));
	for (i=0; i < 2*(Ini->NUMBER_BPMDIST1+2); i++)
		(*msc)->nm_labels[i] = (char *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 64*sizeof(char));


//	addRAM ("get_input_data", 1*sizeof(struct msc_data)
//							+ (Ini->NUMBER_BPMDIST1+2)*(Ini->NUMBER_BPMDIST1+2)*sizeof(int)
//							+ ((int)pow(Ini->NUMBER_BPMDIST1+2,2))*sizeof(int)
//							+ ((int)pow(Ini->NUMBER_BPMDIST1+2,2))*64*sizeof(char)
//							+ ((int)pow(Ini->NUMBER_BPMDIST1+2,2))*Ini->NUMBER_ALTS*sizeof(float)
//							+ ((int)pow(Ini->NUMBER_BPMDIST1+2,2))*Ini->NUMBER_ALTS*sizeof(float)
//							+ 2*(2*(Ini->NUMBER_BPMDIST1+2)*sizeof(int))
//							+ 2*(2*(Ini->NUMBER_BPMDIST1+2)*sizeof(float))
//							+ (2*(Ini->NUMBER_BPMDIST1+2))*64*sizeof(char));


// read motorized mode specific constants index table
	printf ("reading motorized MSC indices.\n");
	if (strcmp(Ini->M_MSC_INDICES, "")) {
		if ((fp = fopen(Ini->M_MSC_INDICES, "rt")) == NULL)
			ExitWithCode(52);
	}
	else
		ExitWithCode (53);
	read_motorized_MSC_indices (fp, *msc);
	rewind (fp);
	echo_ini (fp, fp_rep, Ini->M_MSC_INDICES);
	fclose (fp);


// read motorized mode specific constants
	printf ("reading motorized MSC values.\n");
	if (strcmp(Ini->M_MSCS, "")) {
		if ((fp = fopen(Ini->M_MSCS, "rt")) == NULL)
			ExitWithCode(107);
	}
	else
		ExitWithCode (108);
	read_motorized_MSCs (fp, *msc);
	rewind (fp);
	echo_ini (fp, fp_rep, Ini->M_MSCS);
	fclose (fp);


// read motorized targets
	printf ("reading motorized MSC targets.\n");
	if (strcmp(Ini->M_TARGETS, "")) {
		if ((fp = fopen(Ini->M_TARGETS, "rt")) == NULL)
			ExitWithCode(111);
	}
	else
		ExitWithCode (112);
	read_motorized_targets (fp, *msc);
	rewind (fp);
	echo_ini (fp, fp_rep, Ini->M_TARGETS);
	fclose (fp);


// read non-motorized mode specific constants index table
	printf ("reading non-motorized MSC indices.\n");
	if (strcmp(Ini->NM_MSC_INDICES, "")) {
		if ((fp = fopen(Ini->NM_MSC_INDICES, "rt")) == NULL)
			ExitWithCode(105);
	}
	else
		ExitWithCode (106);
	read_nm_MSC_indices (fp, *msc);
	rewind (fp);
	echo_ini (fp, fp_rep, Ini->NM_MSC_INDICES);
	fclose (fp);


// read non-motorized mode specific constants
	printf ("reading non-motorized MSC values.\n");
	if (strcmp(Ini->NM_MSCS, "")) {
		if ((fp = fopen(Ini->NM_MSCS, "rt")) == NULL)
			ExitWithCode(109);
	}
	else
		ExitWithCode (110);
	read_nm_MSCs (fp, *msc);
	rewind (fp);
	echo_ini (fp, fp_rep, Ini->NM_MSCS);
	fclose (fp);


// read non-motorized targets
	printf ("reading non-motorized MSC targets.\n");
	if (strcmp(Ini->NM_TARGETS, "")) {
		if ((fp = fopen(Ini->NM_TARGETS, "rt")) == NULL)
			ExitWithCode(113);
	}
	else
		ExitWithCode (114);
	read_nm_targets (fp, *msc);
	rewind (fp);
	echo_ini (fp, fp_rep, Ini->NM_TARGETS);
	fclose (fp);


// read input file of origin county based distance scale factors for motorized destination choice
	printf ("reading distance factors.\n");
	if (strcmp(Ini->DISTFACTORS, "")) {
		if ((fp = fopen(Ini->DISTFACTORS, "rt")) == NULL)
			ExitWithCode(103);
		read_dist_factors (fp, DistFactors);
		rewind (fp);
		echo_ini (fp, fp_rep, Ini->DISTFACTORS);
		fclose (fp);
	}
	else {
		ExitWithCode(104);
	}



// open and read zones within walk distance of origins data file
	printf ("reading walk access zone data.\n");
	if (strcmp(Ini->WALKZONEDATA, "")) {
		if ((fp = fopen(Ini->WALKZONEDATA, "rt")) == NULL)
			ExitWithCode(24);
	}
	else
		ExitWithCode (26);
	*WalkZoneData = (struct walk_zone_data *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(struct walk_zone_data));
//	addRAM ("get_input_data", (Ini->MAX_TAZS+1)*sizeof(struct walk_zone_data));
	read_walk_zone_data (fp, *WalkZoneData);
	fclose (fp);



// open and read ring and district zonal correspondance file
	printf ("reading ring/district data.\n");
	if (strcmp(Ini->RINGDISTDATA, "")) {
		if ((fp = fopen(Ini->RINGDISTDATA, "rt")) == NULL)
			ExitWithCode(22);
	}
	else
		ExitWithCode (29);
	ZonalData->ring = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(int));
	ZonalData->dist = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float));
	ZonalData->bpmdist1_index = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(int));
	ZonalData->lpRestricted = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(int));
//	addRAM ("get_input_data", 2*(Ini->MAX_TAZS+1)*sizeof(int) + 1*(Ini->MAX_TAZS+1)*sizeof(float));
	read_ring_dist_data (fp, ZonalData);
	fclose (fp);


// open and read zonal text data file
	printf ("reading zonal data.\n");
	if (strcmp(Ini->ZONEDATA, "")) {
		if ((fp = fopen(Ini->ZONEDATA, "rt")) == NULL)
			ExitWithCode(1);
	}
	else
		ExitWithCode (27);
	ZonalData->AreaType			= (int *)   HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(int));
	ZonalData->UrbanType		= (int *)   HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(int));
	ZonalData->County			= (int *)   HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(int));
	ZonalData->SchoolDist		= (int *)   HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(int));
	ZonalData->OrigSubAreaOB	= (int *)   HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(int));
	ZonalData->DestSubAreaOB	= (int *)   HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(int));
	ZonalData->OrigSubAreaIB	= (int *)   HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(int));
	ZonalData->DestSubAreaIB	= (int *)   HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(int));
	ZonalData->LandArea			= (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float));
	ZonalData->empden			= (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float));
	ZonalData->HHPop			= (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float));
	ZonalData->HHNum			= (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float));
	ZonalData->K12ETot			= (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float));
	ZonalData->UnvEnrol			= (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float));
	ZonalData->TotEmp			= (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float));
	ZonalData->RetEmp			= (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float));
	ZonalData->OffEmp			= (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float));
	ZonalData->PctAcc			= (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float));
	ZonalData->PctFreePark		= (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float));
	ZonalData->NW_PctFreePark	= (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float));
	ZonalData->wPctIncNonMan	= (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float));
	ZonalData->nwPctIncNonMan	= (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float));
	ZonalData->wPctIncMan		= (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float));
	ZonalData->nwPctIncMan		= (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float));
	read_zone_data (fp, ZonalData);
	fclose (fp);
//	addRAM ("get_input_data", 4*(Ini->MAX_TAZS+1)*sizeof(int) + 12*(Ini->MAX_TAZS+1)*sizeof(float));



	// initialize the parking cost percenatge increases for every zone to 1.0
	for (i=0; i < Ini->MAX_TAZS + 1; i++) {
		ZonalData->wPctIncNonMan[i] = 1.0f;
		ZonalData->nwPctIncNonMan[i] = 1.0f;
		ZonalData->wPctIncMan[i] = 1.0f;
		ZonalData->nwPctIncMan[i] = 1.0f;
	}


// open and read zonal school district correspondance file if purpose is school
	if (Ini->PURPOSE_TO_PROCESS == 3) {
		printf ("reading school district data.\n");
		if (strcmp (Ini->SCHOOLDISTDATA, "")) {
			if ((fp = fopen(Ini->SCHOOLDISTDATA, "rt")) == NULL)
				ExitWithCode(19);
		}
		else
			ExitWithCode (28);
		read_school_district_data (fp, ZonalData);
		fclose (fp);
	}



// open and read river crossing text data file
	printf ("reading river crossing data.\n");
	if (strcmp(Ini->RIVERDATA, "")) {
		if ((fp = fopen(Ini->RIVERDATA, "rt")) == NULL)
			ExitWithCode(17);
	}
	else
		ExitWithCode (30);
	RiverData->east = (int **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_COUNTIES+1)*sizeof(int *));
	RiverData->hudson = (int **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_COUNTIES+1)*sizeof(int *));
	RiverData->minor = (int **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_COUNTIES+1)*sizeof(int *));
	for (i=0; i < Ini->NUMBER_COUNTIES+1; i++) {
		RiverData->east[i] = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_COUNTIES+1)*sizeof(int));
		RiverData->hudson[i] = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_COUNTIES+1)*sizeof(int));
		RiverData->minor[i] = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_COUNTIES+1)*sizeof(int));
	}
	read_river_data (fp, *RiverData);
	fclose (fp);
//	addRAM ("get_input_data", 3*(Ini->NUMBER_COUNTIES+1)*(Ini->NUMBER_COUNTIES+1)*sizeof(int));



// open and read intra-county and to-Manhattan dummy variable coefficients file
	printf ("reading county correspondance data.\n");
	if (strcmp(Ini->BPMDIST1COEFFDATA, "")) {
		if ((fp = fopen(Ini->BPMDIST1COEFFDATA, "rt")) == NULL)
			ExitWithCode(23);
	}
	else
		ExitWithCode (31);
	BPMDist1->intra_county = (float *)  HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_BPMDIST1+1)*sizeof(float));
	BPMDist1->co_co_coeffs = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_BPMDIST1+1)*sizeof(float *));
	BPMDist1->labels	   = (char**)   HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_BPMDIST1+1)*sizeof(char *));
	for (i=0; i < Ini->NUMBER_BPMDIST1 + 1; i++)
		BPMDist1->co_co_coeffs[i] = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_BPMDIST1+1)*sizeof(float));
	read_bpmdist1_coeffs (fp, *BPMDist1);
	rewind (fp);
	echo_ini (fp, fp_rep, Ini->BPMDIST1COEFFDATA);
	fclose (fp);
//	addRAM ("get_input_data", (Ini->NUMBER_BPMDIST1+1)*sizeof(float)
//							+ 3*(Ini->NUMBER_BPMDIST1+1)*(Ini->NUMBER_BPMDIST1+1)*sizeof(float)
//							+ (Ini->NUMBER_BPMDIST1+1)*sizeof(char *));



// open and read taxi text data file
	printf ("reading taxi data.\n");
	if (strcmp(Ini->TAXIDATA, "")) {
		if ((fp = fopen(Ini->TAXIDATA, "rt")) == NULL)
			ExitWithCode(2);
	}
	else
		ExitWithCode (32);
	TaxiData->wait = (float **)   HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_COUNTIES+1)*sizeof(float *));
	TaxiData->pickup = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_COUNTIES+1)*sizeof(float *));
	TaxiData->cpm = (float **)    HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_COUNTIES+1)*sizeof(float *));
	TaxiData->surchg = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_COUNTIES+1)*sizeof(float *));
	TaxiData->toll = (float **)   HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_COUNTIES+1)*sizeof(float *));
	for (i=0; i < Ini->NUMBER_COUNTIES+1; i++) {
		TaxiData->wait[i] = (float *)   HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_COUNTIES+1)*sizeof(float));
		TaxiData->pickup[i] = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_COUNTIES+1)*sizeof(float));
		TaxiData->cpm[i] = (float *)    HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_COUNTIES+1)*sizeof(float));
		TaxiData->surchg[i] = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_COUNTIES+1)*sizeof(float));
		TaxiData->toll[i] = (float *)   HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_COUNTIES+1)*sizeof(float));
	}
	read_taxi_data (fp, TaxiData);
	fclose (fp);
//	addRAM ("get_input_data", 5*(Ini->NUMBER_COUNTIES+1)*(Ini->NUMBER_COUNTIES+1)*sizeof(int));



// Initialize parking cost data structures
	// get matrix from which random parking cost range indices are drawn
	printf ("calculating parking costs.\n");
	parking_cost_index_lookup_table (ranprkcst);
	percent_free_parking (ZonalData);


	// set default parking cost array values
	Ini->wParkingCostArray = wParkingCostArray;
	Ini->nwParkingCostArray = nwParkingCostArray;

	
	// open and read PARKINGCOSTS data file that specifies density range index values and associated parking costs.
	// parking costs are assumed input as cents.  If no file is found, default values in cents are used.
	if (strcmp(Ini->PARKINGCOSTS, "")) {
		
		if ((fp = fopen(Ini->PARKINGCOSTS, "rt")) == NULL)
			ExitWithCode(151);

		fgets(InputRecord, RECORD_LENGTH, fp);		// ignore header record

		while ((fgets(InputRecord, RECORD_LENGTH+2, fp)) != NULL) {
			InputRecord[RECORD_LENGTH] = '\0';
			rec_len = (int)strlen(InputRecord);

			if (rec_len == RECORD_LENGTH) {

				// read index
				strncpy (temp, &InputRecord[COST_INDEX_START-1], COST_INDEX_LENGTH);
				temp[COST_INDEX_LENGTH] = '\0';
				i = atoi(temp);

				// read work journey parking cost in cents
				strncpy (temp, &InputRecord[W_COST_VALUE_START-1], W_COST_VALUE_LENGTH);
				temp[W_COST_VALUE_LENGTH] = '\0';
				tempValue = (float)atof(temp);

				// assume indices are 1,...,10.
				Ini->wParkingCostArray[i-1] = tempValue;

				// read non-work journey parking cost in cents
				strncpy (temp, &InputRecord[NW_COST_VALUE_START-1], NW_COST_VALUE_LENGTH);
				temp[NW_COST_VALUE_LENGTH] = '\0';
				tempValue = (float)atof(temp);

				// assume indices are 1,...,10.
				Ini->nwParkingCostArray[i-1] = tempValue;
			}
		}
		
		fclose (fp);
	}



	// open and read PARKINGUSER data file that specifies over-ride values for percent free parking in specified zones,
	// then apply zonal percentage over-rides and store zonal parking cost increases
	if (strcmp(Ini->PARKINGUSER, "")) {
		
		if ((fp = fopen(Ini->PARKINGUSER, "rt")) == NULL)
			ExitWithCode(150);

		fgets(InputRecord, RECORD_LENGTH, fp);		// ignore header record
		while ((fgets(InputRecord, RECORD_LENGTH+2, fp)) != NULL) {
			InputRecord[RECORD_LENGTH] = '\0';
			rec_len = (int)strlen(InputRecord);

			if (rec_len == RECORD_LENGTH) {

				strncpy (temp, &InputRecord[TAZ_START-1], TAZ_LENGTH);
				temp[TAZ_LENGTH] = '\0';
				i = atoi(temp);

				strncpy (temp, &InputRecord[W_PCT_FREE_START-1], W_PCT_FREE_LENGTH);
				temp[W_PCT_FREE_LENGTH] = '\0';
				sscanf(temp, "%s", value);
				// if value is "nul", strcmp returns 0, and the following is false
				if ( strcmp(value, "nul") )
					ZonalData->PctFreePark[i] = (float)atof(temp);

				strncpy (temp, &InputRecord[NW_PCT_FREE_START-1], NW_PCT_FREE_LENGTH);
				temp[NW_PCT_FREE_LENGTH] = '\0';
				sscanf(temp, "%s", value);
				// if value is "nul", strcmp returns 0, and the following is false
				if ( strcmp(value, "nul") )
					ZonalData->NW_PctFreePark[i] = (float)atof(temp);

				strncpy (temp, &InputRecord[W_PCT_INC_NONMAN_ORIG_START-1], W_PCT_INC_NONMAN_ORIG_LENGTH);
				sscanf(temp, "%s", value);
				// if value is "nul", strcmp returns 0, and the following is false
				if ( strcmp(value, "nul") )
					ZonalData->wPctIncNonMan[i] = (float)atof(temp);

				strncpy (temp, &InputRecord[NW_PCT_INC_NONMAN_ORIG_START-1], NW_PCT_INC_NONMAN_ORIG_LENGTH);
				temp[NW_PCT_INC_NONMAN_ORIG_LENGTH] = '\0';
				sscanf(temp, "%s", value);
				// if value is "nul", strcmp returns 0, and the following is false
				if ( strcmp(value, "nul") )
					ZonalData->nwPctIncNonMan[i] = (float)atof(temp);

				strncpy (temp, &InputRecord[W_PCT_INC_MAN_ORIG_START-1], W_PCT_INC_MAN_ORIG_LENGTH);
				temp[W_PCT_INC_MAN_ORIG_LENGTH] = '\0';
				sscanf(temp, "%s", value);
				// if value is "nul", strcmp returns 0, and the following is false
				if ( strcmp(value, "nul") )
					ZonalData->wPctIncMan[i] = (float)atof(temp);

				strncpy (temp, &InputRecord[NW_PCT_INC_MAN_ORIG_START-1], NW_PCT_INC_MAN_ORIG_LENGTH);
				temp[NW_PCT_INC_MAN_ORIG_LENGTH] = '\0';
				sscanf(temp, "%s", value);
				// if value is "nul", strcmp returns 0, and the following is false
				if ( strcmp(value, "nul") )
					ZonalData->nwPctIncMan[i] = (float)atof(temp);

			}
		}
		
		fclose (fp);
	}



	// if an output file was named for writing zonal free parking percentages, write them.
	if (strcmp(Ini->FREEPARKPCTS, "")) {
		if ((fp = fopen(Ini->FREEPARKPCTS, "wt")) == NULL) {
			printf ("error opening FREEPARKPCTS = %s.\nNo free parking percentages written out.\n", Ini->FREEPARKPCTS);
			fprintf (fp_rep, "error opening FREEPARKPCTS = %s.\nNo free parking percentages written out.\n", Ini->FREEPARKPCTS);
			fflush (fp_rep);
		}
		else {
			write_free_parking (fp, ZonalData);
			fclose (fp);
		}
	}


	

// open and read sub-area definition data file if SUBAREA_ANALYSIS is selected
	if (Ini->SUBAREA_ANALYSIS == 1) {
	
		printf ("reading sub-area definition data.\n");
		if (strcmp(Ini->SUBAREADATA, "")) {
			if ((fp = fopen(Ini->SUBAREADATA, "rt")) == NULL)
				ExitWithCode(119);
		}
		else
			ExitWithCode (120);
		read_subarea_data (fp, ZonalData);
		fclose (fp);

	}


}
