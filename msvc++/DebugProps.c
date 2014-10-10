#include "md.h"

void debug_props (int k, struct dc_coeff_data *DCcoeff, struct journey_attribs *JourneyAttribs, struct zone_data *ZonalData,
				  float ***OD_Utility, float ***z_attrs, struct river_crossing_data RiverData, float **hwy_dist,
				  struct dc_constant_data *dcConstants, struct m_mc_asc_data mMcAscData, struct nm_mc_asc_data nmMcAscData, double **util, double **props)
{

	int i, j, walk[2], debug_mode=1;
	int orig, walk_orig, purpose, income, workers, autos, person_type, atwork_mode;
	int se_flag, od_flag, motorized_MSC_index, *AvailModes;
	int idist, jdist;
	float *ODutil, *SEutil, **Logsum, *Prob;
	double *dc_cum;


	AvailModes = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_ALTS*sizeof(int));
	ODutil =   (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_ALTS*sizeof(float));
	SEutil =   (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_ALTS*sizeof(float));
	Prob   =   (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_ALTS*sizeof(float));
	dc_cum =  (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS*2 + 2)*sizeof(double));
	Logsum =  (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float *));
	for (i=0; i < Ini->MAX_TAZS+1; i++)
		Logsum[i] = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 2*sizeof(float));
//	addRAM ("debug_props", Ini->NUMBER_ALTS*sizeof(int) + 3*Ini->NUMBER_ALTS*sizeof(float) +
//		(Ini->MAX_TAZS*2 + 2)*sizeof(double) + (Ini->MAX_TAZS+1)*2*sizeof(float));



	// save values to restore for original journey record
	orig = JourneyAttribs->orig[k];
	walk_orig = JourneyAttribs->walk_orig[k];
	purpose = JourneyAttribs->purpose[k];
	income = JourneyAttribs->income[k];
	workers = JourneyAttribs->workers[k];
	autos = JourneyAttribs->autos[k];
	person_type = JourneyAttribs->person_type[k];
	atwork_mode = JourneyAttribs->at_work_mode[k];



	// use these values for debugging this origin zone
	JourneyAttribs->orig[k] = Ini->DEBUG_ORIG;
	JourneyAttribs->purpose[k] = Ini->DEBUG_PURPOSE;
	JourneyAttribs->income[k] = Ini->DEBUG_INCOME;
	JourneyAttribs->workers[k] = Ini->DEBUG_WORKERS;
	JourneyAttribs->autos[k] = Ini->DEBUG_AUTOS;
	JourneyAttribs->person_type[k] = Ini->DEBUG_PERSON_TYPE;

	if ((Ini->DEBUG_WALK_ORIG == 0 && ZonalData->PctAcc[Ini->DEBUG_ORIG] < 1.0) ||
		(Ini->DEBUG_WALK_ORIG == 1 && ZonalData->PctAcc[Ini->DEBUG_ORIG] > 0.0)) {
			JourneyAttribs->walk_orig[k] = Ini->DEBUG_WALK_ORIG;
	}
	else {
		printf ("fatal error.  exiting.\n");
		printf ("Debug origin zone = %d has walk accessibility specified as %d but the accessible percentage for this zone is %3d%%.\n", Ini->DEBUG_ORIG, Ini->DEBUG_WALK_ORIG, (int)(100*ZonalData->PctAcc[Ini->DEBUG_ORIG]));
		printf ("Please change DEBUG_WALK_ORIG in .ini file to reflect this zonal access percentage and rerun.\n\n");
		fprintf (fp_rep, "fatal error.  exiting.\n");
		fprintf (fp_rep, "Debug origin zone = %d has walk accessibility specified as %d but the accessible percentage for this zone is %3d%%.\n", Ini->DEBUG_ORIG, Ini->DEBUG_WALK_ORIG, (int)(100*ZonalData->PctAcc[Ini->DEBUG_ORIG]));
		fprintf (fp_rep, "Please change DEBUG_WALK_ORIG in .ini file to reflect this zonal access percentage and rerun.\n\n");
		fflush (fp_rep);
		exit (-10);
	}


	// determine destination choice proportions for motorized journey records within a packet
	for	(i=0; i < Ini->NUMBER_ALTS; i++)
		SEutil[i] = 0.0;
	SE_Utilities (k, JourneyAttribs, ZonalData, SEutil);
	walk[0] = JourneyAttribs->walk_orig[k];
	for (j=1; j <= Ini->MAX_TAZS; j++) {
		se_flag = 0;
		od_flag = 0;
		for	(i=0; i < Ini->NUMBER_ALTS; i++)
			ODutil[i] = 0.0;
		for	(i=0; i < Ini->NUMBER_ALTS; i++) {
			if (Ini->ZERO_UTIL == 0 && i < Ini->NUMBER_ALTS - 2) {
				ODutil[i] = OD_Utility[i][JourneyAttribs->orig[k]][j];
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
            jdist = ZonalData->m_mc_asc_index[j];
			motorized_MSC_index = mMcAscData.indices[mMcAscData.mMcAscIndices->indexIndices[idist]][mMcAscData.mMcAscIndices->indexIndices[jdist]];
//			Logsum[j][0] = mc_logsum[JourneyAttribs->purpose[k]] (k, orig, j, AvailModes, person_type, JourneyAttribs->autos[k], walk, atwork_mode, ODutil, SEutil, (float *)mMcAscData.constants[motorized_MSC_index]);
			walk[1] = 1;
//			Logsum[j][1] = mc_logsum[JourneyAttribs->purpose[k]] (k, orig, j, AvailModes, person_type, JourneyAttribs->autos[k], walk, atwork_mode, ODutil, SEutil, (float *)mMcAscData.constants[motorized_MSC_index]);
		}
		else {
			Logsum[j][0] = MISSING;
			Logsum[j][1] = MISSING;
		}
	}

			
	// motorized choice proportions, returns cumulative probability distribution
	// of joint destination/access probability distribution.
	for (i=0; i < Ini->MAX_TAZS*2 + 2; i++)
		dc_cum[i] = 0.0;
	motor_dc_props (JourneyAttribs->orig[k], JourneyAttribs->purpose[k], DCcoeff, OD_Utility, SEutil, Logsum, z_attrs, RiverData, ZonalData, dcConstants, dc_cum, hwy_dist[JourneyAttribs->orig[k]], debug_mode, util, props, 0);


	// save values to restore for original journey record
	JourneyAttribs->orig[k] = orig;
	JourneyAttribs->walk_orig[k] = walk_orig;
	JourneyAttribs->purpose[k] = purpose;
	JourneyAttribs->income[k] = income;
	JourneyAttribs->workers[k] = workers;
	JourneyAttribs->autos[k] = autos;
	JourneyAttribs->person_type[k] = person_type;
	JourneyAttribs->at_work_mode[k] = atwork_mode;


	HeapFree (heapHandle, 0, ODutil);
	HeapFree (heapHandle, 0, SEutil);
	HeapFree (heapHandle, 0, Prob);
	HeapFree (heapHandle, 0, dc_cum);
	HeapFree (heapHandle, 0, AvailModes);
	for (i=0; i < Ini->MAX_TAZS+1; i++)
		HeapFree (heapHandle, 0, Logsum[i]);
	HeapFree (heapHandle, 0, Logsum);
//	relRAM ("debug_props", Ini->NUMBER_ALTS*sizeof(int) + 3*Ini->NUMBER_ALTS*sizeof(float)
//		+ (Ini->MAX_TAZS*2 + 2)*sizeof(double) + (Ini->MAX_TAZS+1)*2*sizeof(float));
}
