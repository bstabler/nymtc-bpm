#include <stdlib.h>
#include <math.h>
#include "md.h"

// parking cost model parameters
#define POPDEN_COEFF -0.00001081
#define EMPDEN_COEFF -0.000002988

float parking_cost (int taz, struct zone_data *ZonalData, int **ranprkcst)
{

  // parking costs for each range
	float prkcst[] = { 250, 750, 1250, 1750, 2250, 2750, 3250, 3750, 4250, 4750 };
	double rand_no;
	float empden;
	int i, j, k;
	
	// get employment density index
	empden = ZonalData->empden[taz];
	if (empden > 390000)
		i = 9;
	else if (empden > 365000)
		i = 8;
	else if (empden > 335000)
		i = 7;
	else if (empden > 300000)
		i = 6;
	else if (empden > 230000)
		i = 5;
	else if (empden > 140000)
		i = 4;
	else if (empden > 100000)
		i = 3;
	else if (empden > 27000)
		i = 2;
	else if (empden > 12700)
		i = 1;
	else
		i = 0;


// determine if parking is free, and if not, return parking cost.

	rand_no = (double)rand()/(double)MAX_RANDOM;
	if (rand_no < ZonalData->PctFreePark[taz])
		return (0.0);
	else {
		// i is the employment density range index.
		// k is a random number [0,999].
		// j is the parking cost range index
		k = (int)(RANDOM_NUMBERS*((double)rand()/(double)MAX_RANDOM));
		j = ranprkcst[i][k];
		return (prkcst[j]);
	}
}


float nw_parking_cost (int taz, struct zone_data *ZonalData, int **ranprkcst)
{

  // parking costs for each range
	float prkcst[] = { 250, 750, 1250, 1750, 2250, 2750, 3250, 3750, 4250, 4750 };
	double rand_no;
	float empden;
	int i, j, k;
	
	// get employment density index
	empden = ZonalData->empden[taz];
	if (empden > 390000)
		i = 9;
	else if (empden > 365000)
		i = 8;
	else if (empden > 335000)
		i = 7;
	else if (empden > 300000)
		i = 6;
	else if (empden > 230000)
		i = 5;
	else if (empden > 140000)
		i = 4;
	else if (empden > 100000)
		i = 3;
	else if (empden > 27000)
		i = 2;
	else if (empden > 12700)
		i = 1;
	else
		i = 0;


// determine if parking is free, and if not, return parking cost.

	rand_no = (double)rand()/(double)MAX_RANDOM;
	if (rand_no < ZonalData->NW_PctFreePark[taz])
		return (0.0);
	else {
		// i is the employment density range index.
		// k is a random number [0,999].
		// j is the parking cost range index
		k = (int)(RANDOM_NUMBERS*((double)rand()/(double)MAX_RANDOM));
		j = ranprkcst[i][k];
		return ((float)0.5*prkcst[j]);
	}
}


void percent_free_parking (struct zone_data *ZonalData)
{
	int i;
	float popden, empden;
	
	// county coefficients for paid utility
	double cntcoeff[] = { 0.0000, -1.3040, -0.9696, -0.9696, -0.9696, -1.8180, -1.8180, -1.8180,
											 -1.8180, -1.8180, -1.8180, -1.8180, -1.8180, -1.8180, -2.2450,
											 -1.7470, -1.7470,  -2.2450, -2.2450, -2.2450, -2.2450, -2.2450,
											  -2.2450, -2.2450, -2.2450, -2.2450, -2.2450, -1.8180, -2.2450 };

	
	// calculate probability of free parking in each zone.
	for (i=1; i <= Ini->MAX_TAZS; i++) {
		popden = 0.0;
		empden = 0.0;
		if (ZonalData->LandArea[i] > 0.0) {
			popden = ZonalData->HHPop[i]/ZonalData->LandArea[i];
			empden = ZonalData->TotEmp[i]/ZonalData->LandArea[i];
		}
		
		// if employment density is < 3000, znpctfree = 100%
		if (empden < 3000) {
			ZonalData->PctFreePark[i] = 1.0;
		}
		else {
			ZonalData->PctFreePark[i] = (float)(exp(POPDEN_COEFF*popden + EMPDEN_COEFF*empden)
									/ (exp(POPDEN_COEFF*popden + EMPDEN_COEFF*empden) + exp(cntcoeff[ZonalData->County[i]])));
			ZonalData->NW_PctFreePark[i] = (float)(ZonalData->PctFreePark[i]*0.4895*(0.0858*log(empden)));
		}
		if (ZonalData->NW_PctFreePark[i] > 1.0)
			ZonalData->NW_PctFreePark[i] = 1.0;
		if (empden <= 3000)
			ZonalData->NW_PctFreePark[i] = 1.0;
									
		ZonalData->empden[i] = empden;
	}
}


// this function can be called after percent_free_parking() to write a file with the zonal free parking percentages that were calculated.
void write_free_parking (FILE* fp, struct zone_data *ZonalData)
{
	
	int i;

	fprintf (fp, "%s,%s,%s\n", "zone", "workPctfreePark", "nonworkPctfreePark");

	for (i=1; i <= Ini->MAX_TAZS; i++)
		fprintf (fp, "%d,%.8f,%.8f\n", i, ZonalData->PctFreePark[i], ZonalData->NW_PctFreePark[i]);

}


void parking_cost_index_lookup_table (int **ranprkcst)
{
	int i, j, k, ibeg, iend;
	
	// frequency distributions of parking cost ranges
	// first dimension - employment density range; second dimension - parking cost range
	int cstpct[10][10] = 	{ { 521, 249, 120, 57, 28, 13, 6, 3, 2, 1 },
							  { 451, 248, 135, 75, 41, 23, 12, 7, 3, 5 },
							  { 150, 348, 263, 139, 62, 24, 9, 3, 1, 1 },
							  { 100, 278, 263, 174, 97, 48, 23, 10, 4, 3 },
							  { 57, 263, 300, 201, 104, 36, 19, 6, 3, 1 },
							  { 36, 194, 269, 220, 139, 76, 37, 17, 7, 5 },
							  { 30, 171, 252, 222, 151, 87, 46, 23, 10, 8 },
							  { 15, 140, 260, 248, 168, 92, 45, 19, 8, 5 },
							  { 11, 113, 230, 243, 182, 111, 59, 29, 13, 9 },
							  { 11, 111, 229, 242, 183, 112, 60, 29, 14, 9 } };

		
	// initialize matrix from which random parking cost range indices are drawn.
	for (i=0; i < DENSITY_RANGES; i++) {
		ibeg = 0;
		for (j=0; j < PRKCOST_RANGES; j++) {
			iend = ibeg + cstpct[i][j];
			for (k=ibeg; k < iend; k++)
				ranprkcst[i][k] = j;
			ibeg = iend;
		}
	}
}