#include "md.h"

void read_od_skims (float ***hwy_dist_util, float ***hwy_dist, float ***hwy_time, float ***wt_ivt, float ***dt_ivt, float ***wc_ivt, float ***dc_ivt, struct zone_data *ZonalData)
{


	int i, j;
	int nrows, ncols;
	float **Bus_ivt, **ExpBus_ivt, **Rail_ivt, **CR_ivt, **Ferry_ivt;
	MATRIX hMatrix;
	MATRIX tMatrix;

	HANDLE od_heap;


// open highway skims file
	hMatrix = MATRIX_LoadFromFile (Ini->HIGHWAY_SOV_SKIM, CONTROL_AUTOMATIC);
	if ((tc_status != TC_OKAY) || (hMatrix == NULL)) {
		printf ("11: error opening highway skims matrix file: %s for reading\n", Ini->HIGHWAY_SOV_SKIM);
		fflush (stdout);
		fflush (stderr);
		exit (11);
	}

// get the dimensions of the TransCAD matrix tables.
	nrows = MATRIX_GetNRows(hMatrix);
	ncols = MATRIX_GetNCols(hMatrix);

// Allocate memory for the highway skims and highwy distance utility array to be used in stops model
	*hwy_dist_util = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float *));
	*hwy_dist = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float *));
	*hwy_time = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float *));
	for (i=0; i <= nrows; i++) {
		(*hwy_dist_util)[i] = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
		(*hwy_dist)[i] = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
		(*hwy_time)[i] = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	}
//	addRAM ("read_od_skims", 3*(nrows+1)*(ncols+1)*sizeof(float));

// get highway distance skims
	GetSkims ("highway", 0*nrows, 2*nrows, Ini->HWY_DIST_CORE, hMatrix, Ini->HWY_INDEX_OFFSET, (*hwy_dist));
	GetSkims ("highway", 1*nrows, 2*nrows, Ini->HWY_TIME_CORE, hMatrix, Ini->HWY_INDEX_OFFSET, (*hwy_time));
	printf ("reading %s skims complete\n", "highway");
	fflush (stdout);

// set values for intrazonal skims
// distance is min[min dest skim, 0.5*sqrt(land area)].
	printf ("setting intrazonal highway skim values.\n");
	fflush (stdout);
	for (i=1; i <= Ini->MAX_TAZS; i++) {
		(*hwy_dist)[i][i] = (float)(0.5*sqrt(ZonalData->LandArea[i]));
		for (j=1; j <= Ini->MAX_TAZS; j++) {
			if ((*hwy_dist)[i][j] > 0.0 && (*hwy_dist)[i][j] < (*hwy_dist)[i][i])
				(*hwy_dist)[i][i] = (*hwy_dist)[i][j];
			(*hwy_dist_util)[i][j] = (*hwy_dist)[i][j];
		}
		// time is distance divided by speed, which is indexed by area type.
		(*hwy_time)[i][i] = (float)(60.0*(*hwy_dist)[i][i]/Ini->AUTO_SPEED[ZonalData->AreaType[i]]);
	}



// create a temp heap for use in reading skims; to be destroyed at end of function.
//	printf ("Creating temp heap.\n");
//	fflush (stdout);
	od_heap = HeapCreate (0, 2048, 0);
	if (od_heap == NULL) {
		printf ("HeapCreate returned NULL for od_heap in ReadODSkims().\n");
		printf ("Cannot allocate heap for reading transit skims.\n");
		exit (-1);
	}


// open walk transit skims file
	printf ("Opening walk transit skims file.\n");
	fflush (stdout);
	tMatrix = MATRIX_LoadFromFile (Ini->WALK_TRANSIT_SKIM, CONTROL_AUTOMATIC);
	if ((tc_status != TC_OKAY) || (tMatrix == NULL)) {
		printf ("12: error opening walk transit skims matrix file: %s for reading\n", Ini->WALK_TRANSIT_SKIM);
		fflush (stdout);
		fflush (stderr);
		exit (12);
	}

// get the dimensions of the TransCAD matrix tables.
	nrows = MATRIX_GetNRows(tMatrix);
	ncols = MATRIX_GetNCols(tMatrix);


// Allocate memory on main heap for the total ivt by mode transit skims to pass back
	printf ("allocating memory on main heap transit ivt skims to pass back.\n");
	fflush (stdout);
	*wt_ivt    = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float *));
	*dt_ivt    = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float *));
	*wc_ivt    = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float *));
	*dc_ivt    = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float *));
	for (i=0; i <= nrows; i++) {
		(*wt_ivt)[i]  = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
		(*dt_ivt)[i]  = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
		(*wc_ivt)[i]  = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
		(*dc_ivt)[i]  = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	}
//	addRAM ("read_od_skims", 4*(nrows+1)*(ncols+1)*sizeof(float));


// Allocate memory on temp heap for reading sub-modal transit skims
	printf ("allocating memory on temp heap for reading in skims.\n");
	fflush (stdout);
	Bus_ivt    = (float **) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float *));
	ExpBus_ivt = (float **) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float *));
	Rail_ivt   = (float **) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float *));
	Ferry_ivt  = (float **) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float *));
	CR_ivt     = (float **) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float *));
	for (i=0; i <= nrows; i++) {
		Bus_ivt[i]    = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
		ExpBus_ivt[i] = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
		Rail_ivt[i]   = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
		Ferry_ivt[i]  = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
		CR_ivt[i]     = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	}


// get walk transit travel skims
	GetSkims ("walk transit", 0*nrows, 5*nrows, Ini->BUS_IVTT_CORE,    tMatrix, Ini->TRAN_INDEX_OFFSET, Bus_ivt);
	GetSkims ("walk transit", 1*nrows, 5*nrows, Ini->EXPBUS_IVTT_CORE, tMatrix, Ini->TRAN_INDEX_OFFSET, ExpBus_ivt);
	GetSkims ("walk transit", 2*nrows, 5*nrows, Ini->RAIL_IVTT_CORE,   tMatrix, Ini->TRAN_INDEX_OFFSET, Rail_ivt);
	GetSkims ("walk transit", 3*nrows, 5*nrows, Ini->CR_IVTT_CORE,     tMatrix, Ini->TRAN_INDEX_OFFSET, CR_ivt);
	GetSkims ("walk transit", 4*nrows, 5*nrows, Ini->FERRY_IVTT_CORE,  tMatrix, Ini->TRAN_INDEX_OFFSET, Ferry_ivt);
	printf ("reading %s skims complete\n", "walk transit");
	fflush (stdout);


// set values for intrazonal skims, then sum sub-modes to total walk transit ivt.
	for (i=1; i <= Ini->MAX_TAZS; i++) {
		// in vehicle time is min[highway distance divided by bus speed, min bus ivtt skim].
		Bus_ivt[i][i] = (float)(60.0*(*hwy_dist)[i][i]/Ini->AUTO_SPEED[ZonalData->AreaType[i]]);
		ExpBus_ivt[i][i]	= 0.0;
		Rail_ivt[i][i]		= 0.0;
		Ferry_ivt[i][i]		= 0.0;
		CR_ivt[i][i]		= 0.0;
		for (j=1; j <= Ini->MAX_TAZS; j++) {
			if (Bus_ivt[i][j] < Bus_ivt[i][i])
				Bus_ivt[i][i] = Bus_ivt[i][j];
			(*wt_ivt)[i][j] = (Bus_ivt[i][j] + ExpBus_ivt[i][j] +
				Rail_ivt[i][j] + CR_ivt[i][j] + Ferry_ivt[i][j]);
		}
	}


// get drive transit travel skims
	tMatrix = MATRIX_LoadFromFile (Ini->DRIVE_TRANSIT_SKIM, CONTROL_AUTOMATIC);
	if ((tc_status != TC_OKAY) || (tMatrix == NULL)) {
		printf ("13: error opening drive transit skims matrix file: %s for reading\n", Ini->DRIVE_TRANSIT_SKIM);
		fflush (stdout);
		fflush (stderr);
		exit (13);
	}

	GetSkims ("drive transit", 0*nrows, 5*nrows, Ini->BUS_IVTT_CORE,    tMatrix, Ini->TRAN_INDEX_OFFSET, Bus_ivt);
	GetSkims ("drive transit", 1*nrows, 5*nrows, Ini->EXPBUS_IVTT_CORE, tMatrix, Ini->TRAN_INDEX_OFFSET, ExpBus_ivt);
	GetSkims ("drive transit", 2*nrows, 5*nrows, Ini->RAIL_IVTT_CORE,   tMatrix, Ini->TRAN_INDEX_OFFSET, Rail_ivt);
	GetSkims ("drive transit", 3*nrows, 5*nrows, Ini->CR_IVTT_CORE,     tMatrix, Ini->TRAN_INDEX_OFFSET, CR_ivt);
	GetSkims ("drive transit", 4*nrows, 5*nrows, Ini->FERRY_IVTT_CORE,  tMatrix, Ini->TRAN_INDEX_OFFSET, Ferry_ivt);
	printf ("reading %s skims complete\n", "drive transit");

	// set values for intrazonal skims, then sum sub-modes to total drive transit ivt.
	for (i=1; i <= Ini->MAX_TAZS; i++) {
		// in vehicle time is min[highway distance divided by bus speed, min bus ivtt skim].
		Bus_ivt[i][i] = (float)(60.0*(*hwy_dist)[i][i]/Ini->AUTO_SPEED[ZonalData->AreaType[i]]);
		ExpBus_ivt[i][i]	= 0.0;
		Rail_ivt[i][i]		= 0.0;
		Ferry_ivt[i][i]		= 0.0;
		CR_ivt[i][i]		= 0.0;
		for (j=1; j <= Ini->MAX_TAZS; j++) {
			if (Bus_ivt[i][j] < Bus_ivt[i][i])
				Bus_ivt[i][i] = Bus_ivt[i][j];
			(*dt_ivt)[i][j] = (Bus_ivt[i][j] + ExpBus_ivt[i][j] +
				Rail_ivt[i][j] + CR_ivt[i][j] + Ferry_ivt[i][j]);

			// informal parking lot
			if ((*dt_ivt)[i][j] == 0.0 && (*wt_ivt)[i][j] > 0.0)
				(*dt_ivt)[i][j] = (*wt_ivt)[i][j];
		}
	}


// get walk commuter rail travel skims
	tMatrix = MATRIX_LoadFromFile (Ini->WALK_COMMRAIL_SKIM, CONTROL_AUTOMATIC);
	if ((tc_status != TC_OKAY) || (tMatrix == NULL)) {
		printf ("14: error opening walk commuter rail skims matrix file: %s for reading\n", Ini->WALK_COMMRAIL_SKIM);
		fflush (stdout);
		fflush (stderr);
		exit (14);
	}

	GetSkims ("walk commuter rail", 0*nrows, 5*nrows, Ini->BUS_IVTT_CORE,    tMatrix, Ini->TRAN_INDEX_OFFSET, Bus_ivt);
	GetSkims ("walk commuter rail", 1*nrows, 5*nrows, Ini->EXPBUS_IVTT_CORE, tMatrix, Ini->TRAN_INDEX_OFFSET, ExpBus_ivt);
	GetSkims ("walk commuter rail", 2*nrows, 5*nrows, Ini->RAIL_IVTT_CORE,   tMatrix, Ini->TRAN_INDEX_OFFSET, Rail_ivt);
	GetSkims ("walk commuter rail", 3*nrows, 5*nrows, Ini->CR_IVTT_CORE,     tMatrix, Ini->TRAN_INDEX_OFFSET, CR_ivt);
	GetSkims ("walk commuter rail", 4*nrows, 5*nrows, Ini->FERRY_IVTT_CORE,  tMatrix, Ini->TRAN_INDEX_OFFSET, Ferry_ivt);
	printf ("reading %s skims complete\n", "walk commuter rail");

	// set values for intrazonal skims, then sum sub-modes to total walk commuter rail ivt.
	for (i=1; i <= Ini->MAX_TAZS; i++) {
		// in vehicle time is min[highway distance divided by bus speed, min bus ivtt skim].
		Bus_ivt[i][i] = (float)(60.0*(*hwy_dist)[i][i]/Ini->AUTO_SPEED[ZonalData->AreaType[i]]);
		ExpBus_ivt[i][i]	= 0.0;
		Rail_ivt[i][i]		= 0.0;
		Ferry_ivt[i][i]		= 0.0;
		CR_ivt[i][i]		= 0.0;
		for (j=1; j <= Ini->MAX_TAZS; j++) {
			if (Bus_ivt[i][j] < Bus_ivt[i][i])
				Bus_ivt[i][i] = Bus_ivt[i][j];
			if (CR_ivt[i][j] > 0.0)
				(*wc_ivt)[i][j] = (Bus_ivt[i][j] + ExpBus_ivt[i][j] +
					Rail_ivt[i][j] + CR_ivt[i][j] + Ferry_ivt[i][j]);
			else
				(*wc_ivt)[i][j] = -1.0;
		}
	}


// get drive commuter rail travel skims
	tMatrix = MATRIX_LoadFromFile (Ini->DRIVE_COMMRAIL_SKIM, CONTROL_AUTOMATIC);
	if ((tc_status != TC_OKAY) || (tMatrix == NULL)) {
		printf ("15: error opening drive commuter rail skims matrix file: %s for reading\n", Ini->DRIVE_COMMRAIL_SKIM);
		fflush (stdout);
		fflush (stderr);
		exit (15);
	}

	GetSkims ("drive commuter rail", 0*nrows, 5*nrows, Ini->BUS_IVTT_CORE,    tMatrix, Ini->TRAN_INDEX_OFFSET, Bus_ivt);
	GetSkims ("drive commuter rail", 1*nrows, 5*nrows, Ini->EXPBUS_IVTT_CORE, tMatrix, Ini->TRAN_INDEX_OFFSET, ExpBus_ivt);
	GetSkims ("drive commuter rail", 2*nrows, 5*nrows, Ini->RAIL_IVTT_CORE,   tMatrix, Ini->TRAN_INDEX_OFFSET, Rail_ivt);
	GetSkims ("drive commuter rail", 3*nrows, 5*nrows, Ini->CR_IVTT_CORE,     tMatrix, Ini->TRAN_INDEX_OFFSET, CR_ivt);
	GetSkims ("drive commuter rail", 4*nrows, 5*nrows, Ini->FERRY_IVTT_CORE,  tMatrix, Ini->TRAN_INDEX_OFFSET, Ferry_ivt);
	printf ("reading %s skims complete\n", "drive commuter rail");

	// set values for intrazonal skims, then sum sub-modes to total drive commuter rail ivt.
	for (i=1; i <= Ini->MAX_TAZS; i++) {
		// in vehicle time is min[highway distance divided by bus speed, min bus ivtt skim].
		Bus_ivt[i][i] = (float)(60.0*(*hwy_dist)[i][i]/Ini->AUTO_SPEED[ZonalData->AreaType[i]]);
		ExpBus_ivt[i][i]	= 0.0;
		Rail_ivt[i][i]		= 0.0;
		Ferry_ivt[i][i]		= 0.0;
		CR_ivt[i][i]		= 0.0;
		for (j=1; j <= Ini->MAX_TAZS; j++) {
			if (Bus_ivt[i][j] < Bus_ivt[i][i])
				Bus_ivt[i][i] = Bus_ivt[i][j];
			if (CR_ivt[i][j] > 0.0)
				(*dc_ivt)[i][j] = (Bus_ivt[i][j] + ExpBus_ivt[i][j] +
					Rail_ivt[i][j] + CR_ivt[i][j] + Ferry_ivt[i][j]);
			else
				(*dc_ivt)[i][j] = -1.0;

			// informal parking lot
			if ((*dc_ivt)[i][j] == 0.0 && (*wc_ivt)[i][j] > 0.0)
				(*dc_ivt)[i][j] = (*wc_ivt)[i][j];
		}
	}


	for (i=0; i <= nrows; i++) {
		HeapFree (od_heap, 0, Bus_ivt[i]);
		HeapFree (od_heap, 0, ExpBus_ivt[i]);
		HeapFree (od_heap, 0, Rail_ivt[i]);
		HeapFree (od_heap, 0, Ferry_ivt[i]);
		HeapFree (od_heap, 0, CR_ivt[i]);
	}
	HeapFree (od_heap, 0, Bus_ivt);
	HeapFree (od_heap, 0, ExpBus_ivt);
	HeapFree (od_heap, 0, Rail_ivt);
	HeapFree (od_heap, 0, Ferry_ivt);
	HeapFree (od_heap, 0, CR_ivt);
//	relRAM ("read_od_skims", 5*(nrows+1)*(ncols+1)*sizeof(float));

	HeapDestroy (od_heap);
}
