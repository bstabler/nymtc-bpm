/*
 *	Read OD skims and calculate od related components of systematic utilites for each modal alternative
 *  for the purpose of interest.
 *
 *  Calculate the OD portions of systematic utility for each modal alternative to all destination tazs
 *  from the specified origin taz.  These are the lowest level alternatives in the NL structure.
 */

#include "CaliperMTX.h"
#include "md.h"

void OD_Utilities (int purpose, struct zone_data *ZonalData, struct taxi_data *TaxiData, int **ranprkcst, float ***HwyDist, float ***OD_Utility)
{
	int i, j;
	int nrows, ncols, nrows_hov2, ncols_hov2, nrows_hov3p, ncols_hov3p, nrows_taxi, ncols_taxi;

	float *hwy_time, *hwy_dist, *hwy_toll;
	float *hwy_hov2_time, *hwy_hov2_dist, *hwy_hov2_toll;
	float *hwy_hov3p_time, *hwy_hov3p_dist, *hwy_hov3p_toll;
	float *hwy_taxi_time, *hwy_taxi_dist, *hwy_taxi_toll;
	float **hwy_dist_temp;
	struct tran_data wtTranSkims;
	struct tran_data dtTranSkims;
	struct tran_data wcTranSkims;
	struct tran_data dcTranSkims;

	float *drive_acc_time;

	MATRIX hMatrix, hov2Matrix, hov3pMatrix, taxiMatrix, tMatrix[4];

	HANDLE od_heap;

	od_heap = HeapCreate (0, 2048, 0);

	// open highway skims file
	if (strcmp (Ini->HIGHWAY_SOV_SKIM, "")) {
		hMatrix = MATRIX_LoadFromFile (Ini->HIGHWAY_SOV_SKIM, CONTROL_AUTOMATIC);
		if ((tc_status != TC_OKAY) || (hMatrix == NULL))
			ExitWithCode (13);
	}
	else
		ExitWithCode (40);

	if (strcmp (Ini->HIGHWAY_HOV2_SKIM, "")) {
		hov2Matrix = MATRIX_LoadFromFile (Ini->HIGHWAY_HOV2_SKIM, CONTROL_AUTOMATIC);
		if ((tc_status != TC_OKAY) || (hov2Matrix == NULL))
			ExitWithCode (13);
	}
	else
		ExitWithCode (142);

	if (strcmp (Ini->HIGHWAY_HOV3p_SKIM, "")) {
		hov3pMatrix = MATRIX_LoadFromFile (Ini->HIGHWAY_HOV3p_SKIM, CONTROL_AUTOMATIC);
		if ((tc_status != TC_OKAY) || (hov3pMatrix == NULL))
			ExitWithCode (13);
	}
	else
		ExitWithCode (143);

	if (strcmp (Ini->HIGHWAY_TAXI_SKIM, "")) {
		taxiMatrix = MATRIX_LoadFromFile (Ini->HIGHWAY_TAXI_SKIM, CONTROL_AUTOMATIC);
		if ((tc_status != TC_OKAY) || (taxiMatrix == NULL))
			ExitWithCode (13);
	}
	else
		ExitWithCode (149);

	// open transit skims files
	printf ("opening skims matrix files ...");
	if (strcmp(Ini->WALK_TRANSIT_SKIM, "")) {
		tMatrix[0] = MATRIX_LoadFromFile (Ini->WALK_TRANSIT_SKIM, CONTROL_AUTOMATIC);
		if ((tc_status != TC_OKAY) || (tMatrix[0] == NULL))
			ExitWithCode (9);
	}
	else
		ExitWithCode (36);
	if (strcmp(Ini->DRIVE_TRANSIT_SKIM, "")) {
		tMatrix[1] = MATRIX_LoadFromFile (Ini->DRIVE_TRANSIT_SKIM, CONTROL_AUTOMATIC);
		if ((tc_status != TC_OKAY) || (tMatrix[1] == NULL))
			ExitWithCode (10);
	}
	else
		ExitWithCode (37);
	if (strcmp(Ini->WALK_COMMRAIL_SKIM, "")) {
		tMatrix[2] = MATRIX_LoadFromFile (Ini->WALK_COMMRAIL_SKIM, CONTROL_AUTOMATIC);
		if ((tc_status != TC_OKAY) || (tMatrix[2] == NULL))
			ExitWithCode (11);
	}
	else
		ExitWithCode (38);
	if (strcmp(Ini->DRIVE_COMMRAIL_SKIM, "")) {
		tMatrix[3] = MATRIX_LoadFromFile (Ini->DRIVE_COMMRAIL_SKIM, CONTROL_AUTOMATIC);
		if ((tc_status != TC_OKAY) || (tMatrix[3] == NULL))
			ExitWithCode (12);
	}
	else
		ExitWithCode (39);
	printf ("\n");


	// Now compute OD related systematic utilities:

	// Allocate memory for the highway skims (distance already allocated in calling function)
	printf ("allocating memory and reading highway skims data ...\n");
	
	// get the dimensions of the TransCAD matrix tables.
	nrows = MATRIX_GetNRows(hMatrix);
	ncols = MATRIX_GetNCols(hMatrix);
	nrows_hov2=MATRIX_GetNRows(hov2Matrix);
	ncols_hov2=MATRIX_GetNCols(hov2Matrix);
	nrows_hov3p=MATRIX_GetNRows(hov3pMatrix);
	ncols_hov3p=MATRIX_GetNCols(hov3pMatrix);
	nrows_taxi=MATRIX_GetNRows(taxiMatrix);
	ncols_taxi=MATRIX_GetNCols(taxiMatrix);

	if(nrows!=nrows_hov2){
		printf("highway sov matrix and highway hov2 matrix have different number of rows.");
		ExitWithCode(144);
	}

	if(nrows!=nrows_hov3p){
		printf("highway sov matrix and highway hov3p matrix have different number of rows.");
		ExitWithCode(144);
	}

	if(nrows!=nrows_taxi){
		printf("highway sov matrix and highway taxi matrix have different number of rows.");
		ExitWithCode(144);
	}

	if(ncols!=ncols_hov2){
		printf("highway sov matrix and highway hov2 matrix have different number of columns.");
		ExitWithCode(144);
	}

	if(ncols!=ncols_hov3p){
		printf("highway sov matrix and highway hov3p matrix have different number of columns.");
		ExitWithCode(144);
	}

	if(ncols!=ncols_taxi){
		printf("highway sov matrix and highway taxi matrix have different number of columns.");
		ExitWithCode(144);
	}


	hwy_time = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float));
	hwy_dist = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float));
	hwy_toll = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float));
	hwy_hov2_time = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float));
	hwy_hov2_dist = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float));
	hwy_hov2_toll = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float));
	hwy_hov3p_time = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float));
	hwy_hov3p_dist = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float));
	hwy_hov3p_toll = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float));
	hwy_taxi_time = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float));
	hwy_taxi_dist = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float));
	hwy_taxi_toll = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float));

	hwy_dist_temp = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float *));
	for (i=0; i <= nrows; i++)
		hwy_dist_temp[i] = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (nrows+1)*sizeof(float));
	GetSkims ("highway", 0*nrows, 3*nrows, Ini->HWY_DIST_CORE, hMatrix, Ini->HWY_INDEX_OFFSET, hwy_dist_temp);

	printf ("Running related od utilities...\n");

	// set values for intrazonal skims
	// distance is min[min dest skim, 0.5*sqrt(land area)].
	for (i=1; i <= Ini->MAX_TAZS; i++) {

		GetSkimsByRow (i, Ini->HWY_TIME_CORE, hMatrix, Ini->HWY_INDEX_OFFSET, hwy_time);
		GetSkimsByRow (i, Ini->HWY_DIST_CORE, hMatrix, Ini->HWY_INDEX_OFFSET, hwy_dist);
		GetSkimsByRow (i,Ini->HWY_TOLL_CORE, hMatrix, Ini->HWY_INDEX_OFFSET, hwy_toll);
		GetSkimsByRow (i, Ini->HWY_HOV2_TIME_CORE, hov2Matrix, Ini->HWY_INDEX_OFFSET, hwy_hov2_time);
		GetSkimsByRow (i, Ini->HWY_HOV2_DIST_CORE, hov2Matrix, Ini->HWY_INDEX_OFFSET, hwy_hov2_dist);
		GetSkimsByRow (i,Ini->HWY_HOV2_TOLL_CORE, hov2Matrix, Ini->HWY_INDEX_OFFSET, hwy_hov2_toll);
		GetSkimsByRow (i, Ini->HWY_HOV3p_TIME_CORE, hov3pMatrix, Ini->HWY_INDEX_OFFSET, hwy_hov3p_time);
		GetSkimsByRow (i, Ini->HWY_HOV3p_DIST_CORE, hov3pMatrix, Ini->HWY_INDEX_OFFSET, hwy_hov3p_dist);
		GetSkimsByRow (i,Ini->HWY_HOV3p_TOLL_CORE, hov3pMatrix, Ini->HWY_INDEX_OFFSET, hwy_hov3p_toll);
		GetSkimsByRow (i, Ini->HWY_TAXI_TIME_CORE, taxiMatrix, Ini->HWY_INDEX_OFFSET, hwy_taxi_time);
		GetSkimsByRow (i, Ini->HWY_TAXI_DIST_CORE, taxiMatrix, Ini->HWY_INDEX_OFFSET, hwy_taxi_dist);
		GetSkimsByRow (i,Ini->HWY_TAXI_TOLL_CORE, taxiMatrix, Ini->HWY_INDEX_OFFSET, hwy_taxi_toll);

		//printf ("Reading %s skims complete\n", "highway");

		hwy_dist[i] = (float)(0.5*sqrt(ZonalData->LandArea[i]));
		hwy_dist_temp[i][i]=hwy_dist[i];

		for (j=1; j <= Ini->MAX_TAZS; j++) {
			if (hwy_dist[j]> 0.0 && hwy_dist[j] < hwy_dist[i])
				hwy_dist[i] = hwy_dist[j];
				hwy_dist_temp[i][i]=hwy_dist[i];
		}

		// time is distance divided by speed, which is indexed by area type.
		hwy_time[i] = (float)(60.0*hwy_dist[i]/Ini->AUTO_SPEED[ZonalData->AreaType[i]]);
		hwy_toll[i]= (float)0.0;


		hwy_hov2_dist[i] = (float)(0.5*sqrt(ZonalData->LandArea[i]));
		for (j=1; j <= Ini->MAX_TAZS; j++) {
			if (hwy_hov2_dist[j]> 0.0 && hwy_hov2_dist[j] < hwy_hov2_dist[i])
				hwy_hov2_dist[i] = hwy_hov2_dist[j];
		}

		// time is distance divided by speed, which is indexed by area type.
		hwy_hov2_time[i] = (float)(60.0*hwy_hov2_dist[i]/Ini->AUTO_SPEED[ZonalData->AreaType[i]]);
		hwy_hov2_toll[i]= (float)0.0;

		
		// HOV 3 Skims
		hwy_hov3p_dist[i] = (float)(0.5*sqrt(ZonalData->LandArea[i]));
		for (j=1; j <= Ini->MAX_TAZS; j++) {
			if (hwy_hov3p_dist[j]> 0.0 && hwy_hov3p_dist[j] < hwy_hov3p_dist[i])
				hwy_hov3p_dist[i] = hwy_hov3p_dist[j];
		}

		hwy_hov3p_time[i] = (float)(60.0*hwy_hov3p_dist[i]/Ini->AUTO_SPEED[ZonalData->AreaType[i]]);
		hwy_hov3p_toll[i]= (float)0.0;
		
		// Taxi Skims				
		hwy_taxi_dist[i] = (float)(0.5*sqrt(ZonalData->LandArea[i]));
		for (j=1; j <= Ini->MAX_TAZS; j++) {
			if (hwy_taxi_dist[j]> 0.0 && hwy_taxi_dist[j] < hwy_taxi_dist[i])
				hwy_taxi_dist[i] = hwy_taxi_dist[j];
		}

		hwy_taxi_time[i] = (float)(60.0*hwy_taxi_dist[i]/Ini->AUTO_SPEED[ZonalData->AreaType[i]]);
		hwy_taxi_toll[i]= (float)0.0;

		
		// calculate and store the highway mode related od utilities
		if (Ini->ZERO_UTIL == 0) {
			da_od_utility[purpose]  (i, Ini->MAX_TAZS, hwy_time, hwy_dist, hwy_toll, ZonalData, ranprkcst, OD_Utility[0][i]);
			sr2_od_utility[purpose] (i, Ini->MAX_TAZS, hwy_hov2_time, hwy_hov2_dist, hwy_hov2_toll, ZonalData, ranprkcst, OD_Utility[1][i]);
			sr3_od_utility[purpose] (i, Ini->MAX_TAZS, hwy_hov3p_time, hwy_hov3p_dist, hwy_hov3p_toll, ZonalData, ranprkcst, OD_Utility[2][i]);
			if (purpose != 7)
				sr4_od_utility[purpose] (i, Ini->MAX_TAZS, hwy_hov3p_time, hwy_hov3p_dist, hwy_hov3p_toll, ZonalData, ranprkcst, OD_Utility[3][i]);
			tx_od_utility[purpose]  (i, Ini->MAX_TAZS, hwy_taxi_time, hwy_taxi_dist, hwy_taxi_toll, ZonalData, TaxiData, OD_Utility[8][i]);
		}
	}
	//printf ("Highway related od utilities complete\n");

	// Release memory used for the highway skims
	printf ("releasing memory for highway skims data ...");
	HeapFree (od_heap, 0, hwy_toll);
	HeapFree (od_heap, 0, hwy_time);
	HeapFree (od_heap, 0, hwy_hov2_time);
	HeapFree (od_heap, 0, hwy_hov2_dist);
	HeapFree (od_heap, 0, hwy_hov2_toll);
	HeapFree (od_heap, 0, hwy_hov3p_time);
	HeapFree (od_heap, 0, hwy_hov3p_dist);
	HeapFree (od_heap, 0, hwy_hov3p_toll);
	HeapFree (od_heap, 0, hwy_taxi_time);
	HeapFree (od_heap, 0, hwy_taxi_dist);
	HeapFree (od_heap, 0, hwy_taxi_toll);

	printf ("\n");

	printf ("allocating memory for transit skims data ...");
	// get the dimensions of the TransCAD matrix tables.
	nrows = MATRIX_GetNRows(tMatrix[0]);
	ncols = MATRIX_GetNCols(tMatrix[0]);

	wtTranSkims.Fare        = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	wtTranSkims.First_wait  = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	wtTranSkims.Xfr_wait    = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	wtTranSkims.Xfrs        = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	wtTranSkims.Bus_ivtt    = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	wtTranSkims.ExpBus_ivtt = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	wtTranSkims.Rail_ivtt   = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	wtTranSkims.Ferry_ivtt  = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	wtTranSkims.CR_ivtt     = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	wtTranSkims.Acc_ovtt    = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	wtTranSkims.InfLot      = (int *)   HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(int));
	wtTranSkims.Xfr_ovtt    = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));

	dtTranSkims.Fare        = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	dtTranSkims.First_wait  = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	dtTranSkims.Xfr_wait    = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	dtTranSkims.Xfrs        = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	dtTranSkims.Bus_ivtt    = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	dtTranSkims.ExpBus_ivtt = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	dtTranSkims.Rail_ivtt   = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	dtTranSkims.Ferry_ivtt  = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	dtTranSkims.CR_ivtt     = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	dtTranSkims.Acc_ovtt    = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	dtTranSkims.InfLot      = (int *)   HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(int));
	dtTranSkims.Xfr_ovtt    = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));

	wcTranSkims.Fare        = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	wcTranSkims.First_wait  = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	wcTranSkims.Xfr_wait    = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	wcTranSkims.Xfrs        = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	wcTranSkims.Bus_ivtt    = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	wcTranSkims.ExpBus_ivtt = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	wcTranSkims.Rail_ivtt   = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	wcTranSkims.Ferry_ivtt  = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	wcTranSkims.CR_ivtt     = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	wcTranSkims.Acc_ovtt    = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	wcTranSkims.InfLot      = (int *)   HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(int));
	wcTranSkims.Xfr_ovtt    = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));

	dcTranSkims.Fare        = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	dcTranSkims.First_wait  = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	dcTranSkims.Xfr_wait    = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	dcTranSkims.Xfrs        = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	dcTranSkims.Bus_ivtt    = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	dcTranSkims.ExpBus_ivtt = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	dcTranSkims.Rail_ivtt   = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	dcTranSkims.Ferry_ivtt  = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	dcTranSkims.CR_ivtt     = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	dcTranSkims.Acc_ovtt    = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));
	dcTranSkims.InfLot      = (int *)   HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(int));
	dcTranSkims.Xfr_ovtt    = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));

	drive_acc_time	     = (float *) HeapAlloc (od_heap, HEAP_ZERO_MEMORY, (ncols+1)*sizeof(float));

	printf ("\n");

	for (i=1; i <= Ini->MAX_TAZS; i++) {
		//get highway skims
		GetSkimsByRow (i, Ini->HWY_DIST_CORE, hMatrix, Ini->HWY_INDEX_OFFSET, hwy_dist);

		if (Ini->ZERO_UTIL == 0) {
			// get walk to transit travel skims
			GetSkimsByRow (i, Ini->FARE_CORE,          tMatrix[0], Ini->TRAN_INDEX_OFFSET, wtTranSkims.Fare);
			GetSkimsByRow (i, Ini->FIRST_WAIT_CORE,    tMatrix[0], Ini->TRAN_INDEX_OFFSET, wtTranSkims.First_wait);
			GetSkimsByRow (i, Ini->TOTAL_WAIT_CORE,    tMatrix[0], Ini->TRAN_INDEX_OFFSET, wtTranSkims.Xfr_wait);
			GetSkimsByRow (i, Ini->NUM_TRANSFERS_CORE, tMatrix[0], Ini->TRAN_INDEX_OFFSET, wtTranSkims.Xfrs);
			GetSkimsByRow (i, Ini->BUS_IVTT_CORE,      tMatrix[0], Ini->TRAN_INDEX_OFFSET, wtTranSkims.Bus_ivtt);
			GetSkimsByRow (i, Ini->EXPBUS_IVTT_CORE,   tMatrix[0], Ini->TRAN_INDEX_OFFSET, wtTranSkims.ExpBus_ivtt);
			GetSkimsByRow (i, Ini->RAIL_IVTT_CORE,     tMatrix[0], Ini->TRAN_INDEX_OFFSET, wtTranSkims.Rail_ivtt);
			GetSkimsByRow (i, Ini->CR_IVTT_CORE,       tMatrix[0], Ini->TRAN_INDEX_OFFSET, wtTranSkims.CR_ivtt);
			GetSkimsByRow (i, Ini->FERRY_IVTT_CORE,    tMatrix[0], Ini->TRAN_INDEX_OFFSET, wtTranSkims.Ferry_ivtt);
			GetSkimsByRow (i, Ini->ACCESS_OVTT_CORE,   tMatrix[0], Ini->TRAN_INDEX_OFFSET, wtTranSkims.Acc_ovtt);
			GetSkimsByRow (i, Ini->TRANSFER_WALK_CORE, tMatrix[0], Ini->TRAN_INDEX_OFFSET, wtTranSkims.Xfr_ovtt);
			GetSkimsByRow (i, Ini->AUTO_TIME_CORE,     tMatrix[0], Ini->TRAN_INDEX_OFFSET, drive_acc_time);

			//printf ("Reading %s skims complete\n", "walk to transit");

			// fare, first wait, access and egress ovtt are the minimum skim values.
			wtTranSkims.Fare[i] = (float)(999999);
			wtTranSkims.First_wait[i] = (float)(999999);
			// in vehicle time is min[highway distance divided by bus speed, min bus ivtt skim].
			wtTranSkims.Bus_ivtt[i] = (float)(60.0*hwy_dist[i]/Ini->AUTO_SPEED[ZonalData->AreaType[i]]);
			wtTranSkims.Acc_ovtt[i] = (float)(999999);
			drive_acc_time[i] = (float)(999999);

			for (j=1; j <= Ini->MAX_TAZS; j++) {			
				if (wtTranSkims.Fare[j] < wtTranSkims.Fare[i])
					wtTranSkims.Fare[i] = wtTranSkims.Fare[j];
				if (wtTranSkims.First_wait[j] < wtTranSkims.First_wait[i])
					wtTranSkims.First_wait[i] = wtTranSkims.First_wait[j];
				if (wtTranSkims.Bus_ivtt[j] < wtTranSkims.Bus_ivtt[i])
					wtTranSkims.Bus_ivtt[i] = wtTranSkims.Bus_ivtt[j];
				if (wtTranSkims.Acc_ovtt[j] < wtTranSkims.Acc_ovtt[i])
					wtTranSkims.Acc_ovtt[i] = wtTranSkims.Acc_ovtt[j];
				if (drive_acc_time[j] < drive_acc_time[i])
					drive_acc_time[i] = drive_acc_time[j];
				wtTranSkims.InfLot[j] = 0;
				wtTranSkims.Acc_ovtt[j] += drive_acc_time[j];
			}

			wtTranSkims.Xfr_wait[i] = (float)(0);
			wtTranSkims.Xfrs[i] = (float)(0);
			wtTranSkims.ExpBus_ivtt[i] = (float)(0);
			wtTranSkims.Rail_ivtt[i] = (float)(0);
			wtTranSkims.Ferry_ivtt[i] = (float)(0);
			wtTranSkims.CR_ivtt[i] = (float)(0);
			wtTranSkims.Xfr_ovtt[i] = (float)(0);

			// calculate and store the walk to transit mode related od utilities
			wt_od_utility[purpose] (i, Ini->MAX_TAZS, hwy_time, hwy_dist, hwy_toll, &wtTranSkims, ZonalData, OD_Utility[4][i]);
			//printf ("Walk to transit od utilities complete\n");
		}

		if (purpose != 7 && Ini->ZERO_UTIL == 0) {
			// get drive to transit travel skims
			GetSkimsByRow(i, Ini->FARE_CORE,          tMatrix[1], Ini->TRAN_INDEX_OFFSET, dtTranSkims.Fare);
			GetSkimsByRow (i, Ini->FIRST_WAIT_CORE,    tMatrix[1], Ini->TRAN_INDEX_OFFSET, dtTranSkims.First_wait);
			GetSkimsByRow (i, Ini->TOTAL_WAIT_CORE,    tMatrix[1], Ini->TRAN_INDEX_OFFSET, dtTranSkims.Xfr_wait);
			GetSkimsByRow (i, Ini->NUM_TRANSFERS_CORE, tMatrix[1], Ini->TRAN_INDEX_OFFSET, dtTranSkims.Xfrs);
			GetSkimsByRow (i, Ini->BUS_IVTT_CORE,      tMatrix[1], Ini->TRAN_INDEX_OFFSET, dtTranSkims.Bus_ivtt);
			GetSkimsByRow (i, Ini->EXPBUS_IVTT_CORE,   tMatrix[1], Ini->TRAN_INDEX_OFFSET, dtTranSkims.ExpBus_ivtt);
			GetSkimsByRow (i, Ini->RAIL_IVTT_CORE,     tMatrix[1], Ini->TRAN_INDEX_OFFSET, dtTranSkims.Rail_ivtt);
			GetSkimsByRow (i, Ini->CR_IVTT_CORE,       tMatrix[1], Ini->TRAN_INDEX_OFFSET, dtTranSkims.CR_ivtt);
			GetSkimsByRow (i, Ini->FERRY_IVTT_CORE,    tMatrix[1], Ini->TRAN_INDEX_OFFSET, dtTranSkims.Ferry_ivtt);
			GetSkimsByRow (i, Ini->ACCESS_OVTT_CORE,   tMatrix[1], Ini->TRAN_INDEX_OFFSET, dtTranSkims.Acc_ovtt);
			GetSkimsByRow (i, Ini->TRANSFER_WALK_CORE, tMatrix[1], Ini->TRAN_INDEX_OFFSET, dtTranSkims.Xfr_ovtt);
			GetSkimsByRow (i, Ini->AUTO_TIME_CORE,     tMatrix[1], Ini->TRAN_INDEX_OFFSET, drive_acc_time);
			
			// fare, first wait, access and egress ovtt are the minimum skim values.
			dtTranSkims.Fare[i] = (float)(999999);
			dtTranSkims.First_wait[i] = (float)(999999);
			// in vehicle time is min[highway distance divided by bus speed, min bus ivtt skim].
			dtTranSkims.Bus_ivtt[i] = (float)(60.0*hwy_dist[i]/Ini->AUTO_SPEED[ZonalData->AreaType[i]]);
			dtTranSkims.Acc_ovtt[i] = (float)(999999);
			drive_acc_time[i] = (float)(999999);

			for (j=1; j <= Ini->MAX_TAZS; j++) {			
				if (dtTranSkims.Fare[j] < dtTranSkims.Fare[i])
					dtTranSkims.Fare[i] = dtTranSkims.Fare[j];
				if (dtTranSkims.First_wait[j] < dtTranSkims.First_wait[i])
					dtTranSkims.First_wait[i] = dtTranSkims.First_wait[j];
				if (dtTranSkims.Bus_ivtt[j] < dtTranSkims.Bus_ivtt[i])
					dtTranSkims.Bus_ivtt[i] = dtTranSkims.Bus_ivtt[j];
				if (dtTranSkims.Acc_ovtt[j] < dtTranSkims.Acc_ovtt[i])
					dtTranSkims.Acc_ovtt[i] = dtTranSkims.Acc_ovtt[j];
				if (drive_acc_time[j] < drive_acc_time[i])
					drive_acc_time[i] = drive_acc_time[j];
				// set informal PnR lot indicator if drive to transit ivt == 0, but walk to transit ivt > 0.
				dtTranSkims.InfLot[j] = 0;
				dtTranSkims.Acc_ovtt[j] += drive_acc_time[j];

				if ((dtTranSkims.Bus_ivtt[j] + dtTranSkims.ExpBus_ivtt[j] +
					dtTranSkims.Rail_ivtt[j] + dtTranSkims.CR_ivtt[j] + dtTranSkims.Ferry_ivtt[j]) == 0 &&
					(wtTranSkims.Bus_ivtt[j] + wtTranSkims.ExpBus_ivtt[j] +
					wtTranSkims.Rail_ivtt[j] + wtTranSkims.CR_ivtt[j] + wtTranSkims.Ferry_ivtt[j]) > 0.0){
						dtTranSkims.InfLot[j]=1;
						dtTranSkims.Fare[j] = wtTranSkims.Fare[j];
						dtTranSkims.First_wait[j] = wtTranSkims.First_wait[j];
						dtTranSkims.Xfr_wait[j] = wtTranSkims.Xfr_wait[j];
						dtTranSkims.Xfrs[j] = wtTranSkims.Xfrs[j];
						dtTranSkims.Bus_ivtt[j] = wtTranSkims.Bus_ivtt[j];
						dtTranSkims.ExpBus_ivtt[j] = wtTranSkims.ExpBus_ivtt[j];
						dtTranSkims.Rail_ivtt[j] = wtTranSkims.Rail_ivtt[j];
						dtTranSkims.CR_ivtt[j] = wtTranSkims.CR_ivtt[j];
						dtTranSkims.Ferry_ivtt[j] = wtTranSkims.Ferry_ivtt[j];
						dtTranSkims.Acc_ovtt[j] = 7.0;
						dtTranSkims.Xfr_ovtt[j] = wtTranSkims.Xfr_ovtt[j];
						dtTranSkims.Acc_ovtt[j] += wtTranSkims.Acc_ovtt[j];
					}
			}

			dtTranSkims.Xfr_wait[i] = (float)(0);
			dtTranSkims.Xfrs[i] = (float)(0);
			dtTranSkims.ExpBus_ivtt[i] = (float)(0);
			dtTranSkims.Rail_ivtt[i] = (float)(0);
			dtTranSkims.Ferry_ivtt[i] = (float)(0);
			dtTranSkims.CR_ivtt[i] = (float)(0);
			dtTranSkims.Xfr_ovtt[i] = (float)(0);

			// calculate and store the drive to transit mode related od utilities
			dt_od_utility[purpose] (i, Ini->MAX_TAZS, hwy_time, hwy_dist, hwy_toll, &dtTranSkims, ZonalData, OD_Utility[5][i]);
			//printf ("Drive to transit od utilities complete\n");

			// get walk to commuter rail travel skims
			GetSkimsByRow (i, Ini->FARE_CORE,          tMatrix[2], Ini->TRAN_INDEX_OFFSET, wcTranSkims.Fare);
			GetSkimsByRow (i, Ini->FIRST_WAIT_CORE,    tMatrix[2], Ini->TRAN_INDEX_OFFSET, wcTranSkims.First_wait);
			GetSkimsByRow (i, Ini->TOTAL_WAIT_CORE,    tMatrix[2], Ini->TRAN_INDEX_OFFSET, wcTranSkims.Xfr_wait);
			GetSkimsByRow (i, Ini->NUM_TRANSFERS_CORE, tMatrix[2], Ini->TRAN_INDEX_OFFSET, wcTranSkims.Xfrs);
			GetSkimsByRow (i, Ini->BUS_IVTT_CORE,      tMatrix[2], Ini->TRAN_INDEX_OFFSET, wcTranSkims.Bus_ivtt);
			GetSkimsByRow (i, Ini->EXPBUS_IVTT_CORE,   tMatrix[2], Ini->TRAN_INDEX_OFFSET, wcTranSkims.ExpBus_ivtt);
			GetSkimsByRow (i, Ini->RAIL_IVTT_CORE,     tMatrix[2], Ini->TRAN_INDEX_OFFSET, wcTranSkims.Rail_ivtt);
			GetSkimsByRow (i, Ini->CR_IVTT_CORE,       tMatrix[2], Ini->TRAN_INDEX_OFFSET, wcTranSkims.CR_ivtt);
			GetSkimsByRow (i, Ini->FERRY_IVTT_CORE,    tMatrix[2], Ini->TRAN_INDEX_OFFSET, wcTranSkims.Ferry_ivtt);
			GetSkimsByRow (i, Ini->ACCESS_OVTT_CORE,   tMatrix[2], Ini->TRAN_INDEX_OFFSET, wcTranSkims.Acc_ovtt);
			GetSkimsByRow (i, Ini->TRANSFER_WALK_CORE, tMatrix[2], Ini->TRAN_INDEX_OFFSET, wcTranSkims.Xfr_ovtt);
			GetSkimsByRow (i, Ini->AUTO_TIME_CORE,     tMatrix[2], Ini->TRAN_INDEX_OFFSET, drive_acc_time);
			//printf ("Reading %s skims complete\n", "walk to commuter rail");

			// fare, first wait, access and egress ovtt are the minimum skim values.
			wcTranSkims.Fare[i] = (float)(999999);
			wcTranSkims.First_wait[i] = (float)(999999);
			// in vehicle time is min[highway distance divided by bus speed, min bus ivtt skim].
			wcTranSkims.Bus_ivtt[i] = (float)(60.0*hwy_dist[i]/Ini->AUTO_SPEED[ZonalData->AreaType[i]]);
			wcTranSkims.Acc_ovtt[i] = (float)(999999);
			drive_acc_time[i] = (float)(999999);

			for (j=1; j <= Ini->MAX_TAZS; j++) {			
				if (wcTranSkims.Fare[j] < wcTranSkims.Fare[i])
					wcTranSkims.Fare[i] = wcTranSkims.Fare[j];
				if (wcTranSkims.First_wait[j] < wcTranSkims.First_wait[i])
					wcTranSkims.First_wait[i] = wcTranSkims.First_wait[j];
				if (wcTranSkims.Bus_ivtt[j] < wcTranSkims.Bus_ivtt[i])
					wcTranSkims.Bus_ivtt[i] = wcTranSkims.Bus_ivtt[j];
				if (wcTranSkims.Acc_ovtt[j] < wcTranSkims.Acc_ovtt[i])
					wcTranSkims.Acc_ovtt[i] = wcTranSkims.Acc_ovtt[j];
				if (drive_acc_time[j] < drive_acc_time[i])
					drive_acc_time[i] = drive_acc_time[j];
				wcTranSkims.InfLot[j] = 0;
				wcTranSkims.Acc_ovtt[j] += drive_acc_time[j];
			}
			wcTranSkims.Xfr_wait[i] = (float)(0);
			wcTranSkims.Xfrs[i] = (float)(0);
			wcTranSkims.ExpBus_ivtt[i] = (float)(0);
			wcTranSkims.Rail_ivtt[i] = (float)(0);
			wcTranSkims.Ferry_ivtt[i] = (float)(0);
			wcTranSkims.CR_ivtt[i] = (float)(0);
			wcTranSkims.Xfr_ovtt[i] = (float)(0);

			// calculate and store the walk to commuter rail mode related od utilities
			wc_od_utility[purpose] (i, Ini->MAX_TAZS, hwy_time, hwy_dist, hwy_toll, &wcTranSkims, ZonalData, OD_Utility[6][i]);
			//printf ("Walk to commuter rail od utilities complete\n");

			// get drive to commuter rail travel skims
			GetSkimsByRow (i, Ini->FARE_CORE, tMatrix[3], Ini->TRAN_INDEX_OFFSET, dcTranSkims.Fare);
			GetSkimsByRow (i, Ini->FIRST_WAIT_CORE, tMatrix[3], Ini->TRAN_INDEX_OFFSET, dcTranSkims.First_wait);
			GetSkimsByRow (i, Ini->TOTAL_WAIT_CORE, tMatrix[3], Ini->TRAN_INDEX_OFFSET, dcTranSkims.Xfr_wait);
			GetSkimsByRow (i, Ini->NUM_TRANSFERS_CORE, tMatrix[3], Ini->TRAN_INDEX_OFFSET, dcTranSkims.Xfrs);
			GetSkimsByRow (i, Ini->BUS_IVTT_CORE,      tMatrix[3], Ini->TRAN_INDEX_OFFSET, dcTranSkims.Bus_ivtt);
			GetSkimsByRow (i, Ini->EXPBUS_IVTT_CORE,   tMatrix[3], Ini->TRAN_INDEX_OFFSET, dcTranSkims.ExpBus_ivtt);
			GetSkimsByRow (i, Ini->RAIL_IVTT_CORE,     tMatrix[3], Ini->TRAN_INDEX_OFFSET, dcTranSkims.Rail_ivtt);
			GetSkimsByRow (i, Ini->CR_IVTT_CORE,       tMatrix[3], Ini->TRAN_INDEX_OFFSET, dcTranSkims.CR_ivtt);
			GetSkimsByRow (i, Ini->FERRY_IVTT_CORE,    tMatrix[3], Ini->TRAN_INDEX_OFFSET, dcTranSkims.Ferry_ivtt);
			GetSkimsByRow (i, Ini->ACCESS_OVTT_CORE,   tMatrix[3], Ini->TRAN_INDEX_OFFSET, dcTranSkims.Acc_ovtt);
			GetSkimsByRow (i, Ini->TRANSFER_WALK_CORE, tMatrix[3], Ini->TRAN_INDEX_OFFSET, dcTranSkims.Xfr_ovtt);
			GetSkimsByRow (i, Ini->AUTO_TIME_CORE,     tMatrix[3], Ini->TRAN_INDEX_OFFSET, drive_acc_time);
			//printf ("Reading %s skims complete\n", "drive to commuter rail");

			// fare, first wait, access and egress ovtt are the minimum skim values.
			dcTranSkims.Fare[i] = (float)(999999);
			dcTranSkims.First_wait[i] = (float)(999999);
			// in vehicle time is min[highway distance divided by bus speed, min bus ivtt skim].
			dcTranSkims.Bus_ivtt[i] = (float)(60.0*hwy_dist[i]/Ini->AUTO_SPEED[ZonalData->AreaType[i]]);
			dcTranSkims.Acc_ovtt[i] = (float)(999999);
			drive_acc_time[i] = (float)(999999);

			for (j=1; j <= Ini->MAX_TAZS; j++) {			
				if (dcTranSkims.Fare[j] < dcTranSkims.Fare[i])
					dcTranSkims.Fare[i] = dcTranSkims.Fare[j];
				if (dcTranSkims.First_wait[j] < dcTranSkims.First_wait[i])
					dcTranSkims.First_wait[i] = dcTranSkims.First_wait[j];
				if (dcTranSkims.Bus_ivtt[j] < dcTranSkims.Bus_ivtt[i])
					dcTranSkims.Bus_ivtt[i] = dcTranSkims.Bus_ivtt[j];
				if (dcTranSkims.Acc_ovtt[j] < dcTranSkims.Acc_ovtt[i])
					dcTranSkims.Acc_ovtt[i] = dcTranSkims.Acc_ovtt[j];
				if (drive_acc_time[j] < drive_acc_time[i])
					drive_acc_time[i] = drive_acc_time[j];
				// set informal PnR lot indicator if drive to CR ivt == 0, but walk to CR ivt > 0.
				dcTranSkims.InfLot[j] = 0;
				dcTranSkims.Acc_ovtt[j] += drive_acc_time[j];

				if ((dcTranSkims.Bus_ivtt[j] + dcTranSkims.ExpBus_ivtt[j] +
					dcTranSkims.Rail_ivtt[j] + dcTranSkims.CR_ivtt[j] + dcTranSkims.Ferry_ivtt[j]) == 0 &&
					(wcTranSkims.Bus_ivtt[j] + wcTranSkims.ExpBus_ivtt[j] +
					wcTranSkims.Rail_ivtt[j] + wcTranSkims.CR_ivtt[j] + wcTranSkims.Ferry_ivtt[j]) > 0.0){
						dcTranSkims.InfLot[j]=1;
						dcTranSkims.Fare[j] = wcTranSkims.Fare[j];
						dcTranSkims.First_wait[j] = wcTranSkims.First_wait[j];
						dcTranSkims.Xfr_wait[j] = wcTranSkims.Xfr_wait[j];
						dcTranSkims.Xfrs[j] = wcTranSkims.Xfrs[j];
						dcTranSkims.Bus_ivtt[j] = wcTranSkims.Bus_ivtt[j];
						dcTranSkims.ExpBus_ivtt[j] = wcTranSkims.ExpBus_ivtt[j];
						dcTranSkims.Rail_ivtt[j] = wcTranSkims.Rail_ivtt[j];
						dcTranSkims.CR_ivtt[j] = wcTranSkims.CR_ivtt[j];
						dcTranSkims.Ferry_ivtt[j] = wcTranSkims.Ferry_ivtt[j];
						dcTranSkims.Acc_ovtt[j] = 7.0;
						dcTranSkims.Xfr_ovtt[j] = wcTranSkims.Xfr_ovtt[j];
						dcTranSkims.Acc_ovtt[j] += wcTranSkims.Acc_ovtt[j];
					}
			}
			dcTranSkims.Xfr_wait[i] = (float)(0);
			dcTranSkims.Xfrs[i] = (float)(0);
			dcTranSkims.ExpBus_ivtt[i] = (float)(0);
			dcTranSkims.Rail_ivtt[i] = (float)(0);
			dcTranSkims.Ferry_ivtt[i] = (float)(0);
			dcTranSkims.CR_ivtt[i] = (float)(0);
			dcTranSkims.Xfr_ovtt[i] = (float)(0);

			// calculate and store the drive to commuter rail mode related od utilities
			dc_od_utility[purpose] (i, Ini->MAX_TAZS, hwy_time, hwy_dist, hwy_toll, &dcTranSkims, ZonalData, OD_Utility[7][i]);
			//printf ("Drive to commuter rail od utilities complete\n");
		}	
	}

	// De-allocate memory for the transit skims
	if (Ini->ZERO_UTIL == 0)
		printf ("releasing memory...");

	HeapFree (od_heap, 0, hwy_dist);

	if (Ini->ZERO_UTIL == 0) {
		HeapFree (od_heap, 0, wtTranSkims.Fare);
		HeapFree (od_heap, 0, wtTranSkims.First_wait);
		HeapFree (od_heap, 0, wtTranSkims.Xfr_wait);
		HeapFree (od_heap, 0, wtTranSkims.Xfrs);
		HeapFree (od_heap, 0, wtTranSkims.Bus_ivtt);
		HeapFree (od_heap, 0, wtTranSkims.ExpBus_ivtt);
		HeapFree (od_heap, 0, wtTranSkims.Rail_ivtt);
		HeapFree (od_heap, 0, wtTranSkims.Ferry_ivtt);
		HeapFree (od_heap, 0, wtTranSkims.CR_ivtt);
		HeapFree (od_heap, 0, wtTranSkims.Acc_ovtt);
		HeapFree (od_heap, 0, wtTranSkims.InfLot);
		HeapFree (od_heap, 0, wtTranSkims.Xfr_ovtt);
		HeapFree (od_heap, 0, dtTranSkims.Fare);
		HeapFree (od_heap, 0, dtTranSkims.First_wait);
		HeapFree (od_heap, 0, dtTranSkims.Xfr_wait);
		HeapFree (od_heap, 0, dtTranSkims.Xfrs);
		HeapFree (od_heap, 0, dtTranSkims.Bus_ivtt);
		HeapFree (od_heap, 0, dtTranSkims.ExpBus_ivtt);
		HeapFree (od_heap, 0, dtTranSkims.Rail_ivtt);
		HeapFree (od_heap, 0, dtTranSkims.Ferry_ivtt);
		HeapFree (od_heap, 0, dtTranSkims.CR_ivtt);
		HeapFree (od_heap, 0, dtTranSkims.Acc_ovtt);
		HeapFree (od_heap, 0, dtTranSkims.InfLot);
		HeapFree (od_heap, 0, dtTranSkims.Xfr_ovtt);
		HeapFree (od_heap, 0, wcTranSkims.Fare);
		HeapFree (od_heap, 0, wcTranSkims.First_wait);
		HeapFree (od_heap, 0, wcTranSkims.Xfr_wait);
		HeapFree (od_heap, 0, wcTranSkims.Xfrs);
		HeapFree (od_heap, 0, wcTranSkims.Bus_ivtt);
		HeapFree (od_heap, 0, wcTranSkims.ExpBus_ivtt);
		HeapFree (od_heap, 0, wcTranSkims.Rail_ivtt);
		HeapFree (od_heap, 0, wcTranSkims.Ferry_ivtt);
		HeapFree (od_heap, 0, wcTranSkims.CR_ivtt);
		HeapFree (od_heap, 0, wcTranSkims.Acc_ovtt);
		HeapFree (od_heap, 0, wcTranSkims.InfLot);
		HeapFree (od_heap, 0, wcTranSkims.Xfr_ovtt);
		HeapFree (od_heap, 0, dcTranSkims.Fare);
		HeapFree (od_heap, 0, dcTranSkims.First_wait);
		HeapFree (od_heap, 0, dcTranSkims.Xfr_wait);
		HeapFree (od_heap, 0, dcTranSkims.Xfrs);
		HeapFree (od_heap, 0, dcTranSkims.Bus_ivtt);
		HeapFree (od_heap, 0, dcTranSkims.ExpBus_ivtt);
		HeapFree (od_heap, 0, dcTranSkims.Rail_ivtt);
		HeapFree (od_heap, 0, dcTranSkims.Ferry_ivtt);
		HeapFree (od_heap, 0, dcTranSkims.CR_ivtt);
		HeapFree (od_heap, 0, dcTranSkims.Acc_ovtt);
		HeapFree (od_heap, 0, dcTranSkims.InfLot);
		HeapFree (od_heap, 0, dcTranSkims.Xfr_ovtt);
		HeapFree (od_heap, 0, drive_acc_time);
	}
	if (Ini->ZERO_UTIL == 0)
		printf ("\n");
	
	HeapDestroy (od_heap);

	*HwyDist = hwy_dist_temp;
}	
