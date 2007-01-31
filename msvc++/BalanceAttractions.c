#include "md.h"

void balance_attractions (double *attr_corrections, float ***z_attrs, float **t_attrs, int **TotProds, struct zone_data *ZonalData)
{
	FILE *fp;

	int i, j, zone, Tot, SchoolDistricts;
	double rTot, remainder[2], Total;
	double balance[PURPOSES], *school_balance, *school_prods, *school_attrs, *school_tot_prods, *school_tot_attrs;
	double TotalInitialAttrs[PURPOSES], TotalFinalAttrs[PURPOSES], TotAttrs[PURPOSES];
	

// Memory allocations
	school_balance = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(double));
	school_prods   = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(double));
	school_attrs   = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(double));
	school_tot_prods   = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(double));
	school_tot_attrs   = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(double));
//	addRAM ("balance_attractions", 4*(Ini->MAX_TAZS+1)*sizeof(double));


	for (i=0; i < PURPOSES; i++)
		TotAttrs[i] = 0.0;

	for (i=0; i < PURPOSES; i++) {
		z_attrs[2][i][STATUE_OF_LIBERTY_TAZ] = 0.0;
		z_attrs[0][i][STATUE_OF_LIBERTY_TAZ] = 0.0;
		z_attrs[1][i][STATUE_OF_LIBERTY_TAZ] = 0.0;
	}
	z_attrs[2][0][STATUE_OF_LIBERTY_TAZ] = 0.0;
	z_attrs[0][0][STATUE_OF_LIBERTY_TAZ] = 0.0;
	z_attrs[1][0][STATUE_OF_LIBERTY_TAZ] = 0.0;
	for (j=1; j <= Ini->MAX_TAZS; j++) {
		for (i=0; i <PURPOSES; i++) {
			z_attrs[2][i][j] *= (float)attr_corrections[j];			// apply attraction correction factors
			TotAttrs[i] += z_attrs[2][i][j];
		}
	}



	// balancing factors for all purposes (school to be replaced)
	for (i=0; i < PURPOSES; i++) {
		TotalInitialAttrs[i] = TotAttrs[i];
		balance[i] = 0.0;
		if (TotAttrs[i] > 0.0)
			balance[i] = TotProds[0][i]/TotAttrs[i];
	}

	if (Ini->PURPOSE_TO_PROCESS == 3) {
		// special balancing factors for school purpose
		SchoolDistricts = 0;
		for (j=1; j <= Ini->MAX_TAZS; j++) {
			school_balance[j] = 0.0;
			school_prods[ZonalData->SchoolDist[j]] += TotProds[j][3];
			school_attrs[ZonalData->SchoolDist[j]] += z_attrs[2][3][j];
			if (ZonalData->SchoolDist[j] > SchoolDistricts)
				SchoolDistricts = ZonalData->SchoolDist[j];
		}

		for (j=1; j <= SchoolDistricts; j++) {
			if (school_attrs[j] > 0.0)
				school_balance[j] = school_prods[j]/school_attrs[j];
		}
	}


// apply balancing factors to zonal attractions and generate the Attractions data strucure
//		z_attrs[0][][] are attractions where no transit or CR walk access is available
//		z_attrs[1][][] are attractions where either transit or CR walk access is available
//		z_attrs[2][][] are total attractions
//		t_attrs[i][zone] are total initial attractions for purpose i and zone



	if (Ini->PURPOSE_TO_PROCESS == 3) {
		fp = fopen ("balance.csv", "w");
		fprintf (fp, "zone,schoolDist,balance,TotProds,z_attrs(init),z_attrs(final),z_attrs(round)\n");
	}


	Total = 0.0;
	for (i=0; i < PURPOSES; i++) {
		remainder[0] = 0.0;
		remainder[1] = 0.0;
		for (zone=1; zone <= Ini->MAX_TAZS; zone++) {
			if (Ini->PURPOSE_TO_PROCESS == 3 && i == 3)					// school purpose, use school balancing
				balance[i] = school_balance[ZonalData->SchoolDist[zone]];

			if (Ini->PURPOSE_TO_PROCESS == 3 && i == 3)
				fprintf (fp, "%d,%d,%.5f,%d,%.5f,", zone, ZonalData->SchoolDist[zone], balance[i], TotProds[zone][i], z_attrs[2][i][zone]);

			z_attrs[2][i][zone] *= (float)balance[i];
			z_attrs[1][i][zone] = z_attrs[2][i][zone]*ZonalData->PctAcc[zone];
			z_attrs[0][i][zone] = z_attrs[2][i][zone] - z_attrs[1][i][zone];

			if (Ini->PURPOSE_TO_PROCESS == 3 && i == 3)
				fprintf (fp, "%.5f,", z_attrs[2][i][zone]);

			// separate bucket rounding of initial size variables by sub-area
			z_attrs[0][i][zone] = (float)((int)z_attrs[0][i][zone]);
			remainder[0] += z_attrs[2][i][zone] - z_attrs[1][i][zone] - z_attrs[0][i][zone];
			if (remainder[0] > 1.0) {
				z_attrs[0][i][zone] += 1.0;
				remainder[0] -= 1.0;
			}

			z_attrs[1][i][zone] = (float)((int)z_attrs[1][i][zone]);
			remainder[1] += z_attrs[2][i][zone] - z_attrs[0][i][zone] - z_attrs[1][i][zone];
			if (remainder[1] > 1.0) {
				z_attrs[1][i][zone] += 1.0;
				remainder[1] -= 1.0;
			}

			z_attrs[2][i][zone] = z_attrs[0][i][zone] + z_attrs[1][i][zone];


			if (Ini->PURPOSE_TO_PROCESS == 3 && i == 3 && zone != Ini->MAX_TAZS) {
				school_tot_prods[ZonalData->SchoolDist[zone]] += TotProds[zone][i];
				school_tot_attrs[ZonalData->SchoolDist[zone]] += z_attrs[2][i][zone];
				fprintf (fp, "%.5f\n", z_attrs[2][i][zone]);
			}


			if (i == Ini->PURPOSE_TO_PROCESS)
				Total += z_attrs[2][i][zone];
		}
		if (remainder[0] > 0.5 && Total < Ini->NUMBER_JOURNEYS) {
			z_attrs[0][i][Ini->MAX_TAZS] += 1.0;
			Total++;
		}

		if (remainder[1] > 0.5 && Total < Ini->NUMBER_JOURNEYS) {
			z_attrs[1][i][Ini->MAX_TAZS] += 1.0;
			Total++;
		}

		z_attrs[2][i][Ini->MAX_TAZS] = z_attrs[0][i][Ini->MAX_TAZS] + z_attrs[1][i][Ini->MAX_TAZS];

	
		if (Ini->PURPOSE_TO_PROCESS == 3 && i == 3) {
			school_tot_prods[ZonalData->SchoolDist[Ini->MAX_TAZS]] += TotProds[Ini->MAX_TAZS][i];
			school_tot_attrs[ZonalData->SchoolDist[Ini->MAX_TAZS]] += z_attrs[2][i][Ini->MAX_TAZS];
			fprintf (fp, "%.5f\n", z_attrs[2][i][Ini->MAX_TAZS]);
		}
	}


	if (Ini->PURPOSE_TO_PROCESS == 3) {
		fprintf (fp, "\n\n\n\n");

		for (j=1; j <= SchoolDistricts; j++) {
			if (school_tot_prods[j] > 0 || school_tot_attrs[j] > 0)
				fprintf (fp, "%d,%.5f,%.5f\n", j, school_tot_prods[j], school_tot_attrs[j]);
		}
		fclose (fp);
	}




// total up attractions
	for (i=0; i < PURPOSES; i++)
		for (j=1; j <= Ini->MAX_TAZS; j++)
			t_attrs[i][j] = z_attrs[0][i][j] + z_attrs[1][i][j];

	for (i=0; i < PURPOSES; i++) {
		TotalFinalAttrs[i] = 0.0;
		for (j=1; j <= Ini->MAX_TAZS; j++)
			TotalFinalAttrs[i] += t_attrs[i][j];
	}


// write to report file only in apply mode
	if (Calibrating == 0) {
		fprintf (fp_rep, "Input Journey Productions by Purpose:\n");
		Tot = 0;
		for (i=0; i < PURPOSES; i++) {
			fprintf (fp_rep, "%2d  %15s:  %9d\n", i+1, PurposeLabels[i], TotProds[0][i]);
			Tot += TotProds[0][i];
		}
		fprintf (fp_rep, "-------------------------------\n");
		fprintf (fp_rep, "%2s  %15s:  %9d\n\n\n", "", "Total", Tot);
	

		fprintf (fp_rep, "Initial Unbalanced Journey Attractions by Purpose:\n");
		rTot = 0.0;
		for (i=0; i < PURPOSES; i++) {
			fprintf (fp_rep, "%2d  %15s:  %9.0f\n", i+1, PurposeLabels[i], TotalInitialAttrs[i]);
			rTot += TotalInitialAttrs[i];
		}
		fprintf (fp_rep, "-------------------------------\n");
		fprintf (fp_rep, "%2s  %15s:  %9.0f\n\n\n", "", "Total", rTot);
	

		fprintf (fp_rep, "Final Balanced Journey Attractions by Purpose:\n");
		rTot = 0.0;
		for (i=0; i < PURPOSES; i++) {
			fprintf (fp_rep, "%2d  %15s:  %9.0f\n", i+1, PurposeLabels[i], TotalFinalAttrs[i]);
			rTot += TotalFinalAttrs[i];
		}
		fprintf (fp_rep, "-------------------------------\n");
		fprintf (fp_rep, "%2s  %15s:  %9.0f\n\n\n", "", "Total", rTot);
	

		fflush (fp_rep);
	}


// apply constraining multiplier to final attractions and add total attractions by purpose and taz.
	for (i=0; i < PURPOSES; i++) {
		for (j=1; j <= Ini->MAX_TAZS; j++) {
			z_attrs[0][i][j] *= Ini->ATTR_CONSTRAINT_FACTOR;
			z_attrs[1][i][j] *= Ini->ATTR_CONSTRAINT_FACTOR;
			z_attrs[2][i][j] = z_attrs[0][i][j] + z_attrs[1][i][j];
		}
	}


// free memory
	HeapFree (heapHandle, 0, school_balance);
	HeapFree (heapHandle, 0, school_prods);
	HeapFree (heapHandle, 0, school_attrs);
	HeapFree (heapHandle, 0, school_tot_prods);
	HeapFree (heapHandle, 0, school_tot_attrs);
//	relRAM ("balance_attractions", 3*Ini->MAX_TAZS+1*sizeof(double));

}
