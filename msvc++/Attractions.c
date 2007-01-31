#include "md.h"

// define text file formats for work journey attraction percenatges of low, medium and high income groups
#define PCT_RECORD_LENGTH 56
#define PCT_ZONE_START 1
#define PCT_ZONE_LENGTH 14
#define PCT_LOW_START 15
#define PCT_LOW_LENGTH 14
#define PCT_MED_START 29
#define PCT_MED_LENGTH 14
#define PCT_HIGH_START 43
#define PCT_HIGH_LENGTH 14


// define coefficients for attractions model
#define WORK_MANHATTAN_EMP_COEFF			(float)0.9464
#define WORK_NON_MANHATTAN_EMP_COEFF		(float)0.7184
#define SCHOOL_POP_COEFF					(float)0.1169
#define SCHOOL_ENROLL_COEFF					(float)0.2390
#define UNIV_HHS_COEFF						(float)0.0320
#define UNIV_ENROLL_COEFF					(float)0.2181
#define MAINT_POP_COEFF						(float)0.2562
#define MAINT_MANHATTAN_RETAIL_COEFF		(float)1.7816
#define MAINT_URBAN_RETAIL_COEFF			(float)1.7300
#define MAINT_NON_URBAN_RETAIL_COEFF		(float)3.1600
#define DISCR_POP_COEFF						(float)0.0932
#define DISCR_MANHATTAN_RETAIL_COEFF		(float)0.4591
#define DISCR_URBAN_RETAIL_COEFF			(float)0.3190
#define DISCR_NON_URBAN_RETAIL_COEFF		(float)0.6012
#define ATWORK_MANHATTAN_RETAIL_COEFF		(float)1.1846
#define ATWORK_NON_MANHATTAN_RETAIL_COEFF	(float)0.4029
#define ATWORK_MANHATTAN_OFFICE_COEFF		(float)0.1281
#define ATWORK_NON_MANHATTAN_OFFICE_COEFF	(float)0.2100




void work_attractions (FILE *fp, struct zone_data *ZonalData, float **Attractions)
{

	int i;
	double Pct1, Pct2, Pct3, *InputAttrs;
	char InputRecord[PCT_RECORD_LENGTH+2];
	char temp[LRECL];

// Memory allocations
	InputAttrs = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(double));
//	addRAM ("work_attractions", (Ini->MAX_TAZS+1)*sizeof(double));


// Calculate zonal attractions for total work journeys
	for (i=1; i <= Ini->MAX_TAZS; i++) {
		if (ZonalData->UrbanType[i] == 1)
			InputAttrs[i] = ZonalData->TotEmp[i]*WORK_MANHATTAN_EMP_COEFF;
		else
			InputAttrs[i] = ZonalData->TotEmp[i]*WORK_NON_MANHATTAN_EMP_COEFF;
	}



// Now split total work journeys per zone into work journeys by income group per zone
// Read percent zonal work attractions by income group records.
	fgets(InputRecord, PCT_RECORD_LENGTH+2, fp);							// ignore header record
	while ((fgets(InputRecord, PCT_RECORD_LENGTH+2, fp)) != NULL) {

		strncpy (temp, &InputRecord[PCT_ZONE_START-1], PCT_ZONE_LENGTH);
		temp[PCT_ZONE_LENGTH] = '\0';
		i = atoi(temp);

		strncpy (temp, &InputRecord[PCT_LOW_START-1], PCT_LOW_LENGTH);
		temp[PCT_LOW_LENGTH] = '\0';
		Pct1 = atof(temp);

		strncpy (temp, &InputRecord[PCT_MED_START-1], PCT_MED_LENGTH);
		temp[PCT_MED_LENGTH] = '\0';
		Pct2 = atof(temp);

		strncpy (temp, &InputRecord[PCT_HIGH_START-1], PCT_HIGH_LENGTH);
		temp[PCT_HIGH_LENGTH] = '\0';
		Pct3 = atof(temp);

		Attractions[0][i] = (float)(Pct1*InputAttrs[i]);
		Attractions[1][i] = (float)(Pct2*InputAttrs[i]);
		Attractions[2][i] = (float)(Pct3*InputAttrs[i]);
	}


// free memory
	HeapFree (heapHandle, 0, InputAttrs);
//	relRAM ("work_attractions", PCT_RECORD_LENGTH+2*sizeof(char) + Ini->MAX_TAZS+1*sizeof(double));
}



void school_attractions (FILE *fp, struct zone_data *ZonalData, float **Attractions)
{
	int i;

// Calculate zonal attractions for total school journeys
	for (i=1; i <= Ini->MAX_TAZS; i++) {
		Attractions[3][i] = ZonalData->HHPop[i]*SCHOOL_POP_COEFF + ZonalData->K12ETot[i]*SCHOOL_ENROLL_COEFF;
	}
}



void univ_attractions (FILE *fp, struct zone_data *ZonalData, float **Attractions)
{
	int i;

// Calculate zonal attractions for total university journeys
	for (i=1; i <= Ini->MAX_TAZS; i++) {
		Attractions[4][i] = ZonalData->HHNum[i]*UNIV_HHS_COEFF + ZonalData->UnvEnrol[i]*UNIV_ENROLL_COEFF;
	}
}



void maint_attractions (FILE *fp, struct zone_data *ZonalData, float **Attractions)
{
	int i;

// Calculate zonal attractions for total maintenance journeys
	for (i=1; i <= Ini->MAX_TAZS; i++) {
		Attractions[5][i] = ZonalData->HHPop[i]*MAINT_POP_COEFF;
		if (ZonalData->UrbanType[i] == 1)
			Attractions[5][i] += ZonalData->RetEmp[i]*MAINT_MANHATTAN_RETAIL_COEFF;
		else if (ZonalData->UrbanType[i] == 2)
			Attractions[5][i] += ZonalData->RetEmp[i]*MAINT_URBAN_RETAIL_COEFF;
		else
			Attractions[5][i] += ZonalData->RetEmp[i]*MAINT_NON_URBAN_RETAIL_COEFF;
	}
}



void discr_attractions (FILE *fp, struct zone_data *ZonalData, float **Attractions)
{
	int i;

// Calculate zonal attractions for total discretionary journeys
	for (i=1; i <= Ini->MAX_TAZS; i++) {
		Attractions[6][i] = ZonalData->HHPop[i]*DISCR_POP_COEFF;
		if (ZonalData->UrbanType[i] == 1)
			Attractions[6][i] += ZonalData->RetEmp[i]*DISCR_MANHATTAN_RETAIL_COEFF;
		else if (ZonalData->UrbanType[i] == 2)
			Attractions[6][i] += ZonalData->RetEmp[i]*DISCR_URBAN_RETAIL_COEFF;
		else
			Attractions[6][i] += ZonalData->RetEmp[i]*DISCR_NON_URBAN_RETAIL_COEFF;
	}
}



void atwork_attractions (FILE *fp, struct zone_data *ZonalData, float **Attractions)
{
	int i;

// Calculate zonal attractions for total at work journeys
	for (i=1; i <= Ini->MAX_TAZS; i++) {
		if (ZonalData->UrbanType[i] == 1)
			Attractions[7][i] = ZonalData->RetEmp[i]*ATWORK_MANHATTAN_RETAIL_COEFF + ZonalData->OffEmp[i]*ATWORK_MANHATTAN_OFFICE_COEFF;
		else
			Attractions[7][i] = ZonalData->RetEmp[i]*ATWORK_NON_MANHATTAN_RETAIL_COEFF + ZonalData->OffEmp[i]*ATWORK_NON_MANHATTAN_OFFICE_COEFF;
	}
}
