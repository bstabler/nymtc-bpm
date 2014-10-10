#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md.h"

// zone level data input file formats
#define ZONE_RECORD_LENGTH	241
#define TAZ_START			1
#define TAZ_LENGTH			11
#define LA_START			23
#define LA_LENGTH			11
#define AT_START			56
#define AT_LENGTH			11
#define UT_START			45
#define UT_LENGTH			11
#define CO_START			34
#define CO_LENGTH			11
#define POP_START			67
#define POP_LENGTH			11
#define HHS_START			122
#define HHS_LENGTH			11
#define TOTEMP_START		166
#define TOTEMP_LENGTH		11
#define RETEMP_START		177
#define RETEMP_LENGTH		11
#define OFFEMP_START		188
#define OFFEMP_LENGTH		11
#define UNVENROL_START		210
#define UNVENROL_LENGTH		11
#define SCHENROL_START		221
#define SCHENROL_LENGTH		11
#define PCTACC_START		232
#define PCTACC_LENGTH		10



void read_zone_data (FILE *fp, struct zone_data *ZonalData)
{
	int taz;
	char InputRecord[ZONE_RECORD_LENGTH+5];
	char temp[LRECL];

	

// Read zonal data records.
	fgets(InputRecord, ZONE_RECORD_LENGTH+5, fp);								// ignore header record
	while ((fgets(InputRecord, ZONE_RECORD_LENGTH+5, fp)) != NULL) {

		strncpy (temp, &InputRecord[TAZ_START-1], TAZ_LENGTH);
		temp[TAZ_LENGTH] = '\0';
		taz = atoi(temp);
		if (taz <= 0 || taz > Ini->MAX_TAZS) {
			printf ("\nError reading zonal data.  TAZ = %d is out of range: 1 <= TAZ <= %d.\n\n", taz, Ini->MAX_TAZS);
			fprintf (fp_rep, "\nError reading zonal data.  TAZ = %d out of range: 1 <= TAZ <= %d.\n\n", taz, Ini->MAX_TAZS);
			fflush (stdout);
			fflush (fp_rep);
			exit (-1);
		}

		strncpy (temp, &InputRecord[LA_START-1], LA_LENGTH);
		temp[LA_LENGTH] = '\0';
		ZonalData->LandArea[taz] = (float)atof(temp);
		if (ZonalData->LandArea[taz] < 0) {
			printf ("\nError reading zonal data.  Negative value for LandArea[%d] = %.4f.  LandArea should be non-negative.\n\n", taz, ZonalData->LandArea[taz]);
			fprintf (fp_rep, "\nError reading zonal data.  Negative value for LandArea[%d] = %.4f.  LandArea should be non-negative.\n\n", taz, ZonalData->LandArea[taz]);
			fflush (stdout);
			fflush (fp_rep);
			exit (-1);
		}

		strncpy (temp, &InputRecord[AT_START-1], AT_LENGTH);
		temp[AT_LENGTH] = '\0';
		ZonalData->AreaType[taz] = atoi(temp);
		if (ZonalData->AreaType[taz] < 1 || ZonalData->AreaType[taz] > AREA_TYPES) {
			printf ("\nError reading zonal data.  AreaType[%d] = %d is out of range:  1 <= AreaType <= %d.\n\n", taz, ZonalData->AreaType[taz], AREA_TYPES);
			fprintf (fp_rep, "\nError reading zonal data.  AreaType[%d] = %d is out of range:  1 <= AreaType <= %d.\n\n", taz, ZonalData->AreaType[taz], AREA_TYPES);
			fflush (stdout);
			fflush (fp_rep);
			exit (-1);
		}

		strncpy (temp, &InputRecord[UT_START-1], UT_LENGTH);
		temp[UT_LENGTH] = '\0';
		ZonalData->UrbanType[taz] = atoi(temp);
		if (ZonalData->UrbanType[taz] < 1 || ZonalData->UrbanType[taz] > URBAN_TYPES) {
			printf ("\nError reading zonal data.  UrbanType[%d] = %d is out of range:  1 <= UrbanType <= %d.\n\n", taz, ZonalData->UrbanType[taz], URBAN_TYPES);
			fprintf (fp_rep, "\nError reading zonal data.  UrbanType[%d] = %d is out of range:  1 <= UrbanType <= %d.\n\n", taz, ZonalData->UrbanType[taz], URBAN_TYPES);
			fflush (stdout);
			fflush (fp_rep);
			exit (-1);
		}

		strncpy (temp, &InputRecord[CO_START-1], CO_LENGTH);
		temp[CO_LENGTH] = '\0';
		ZonalData->County[taz] = atoi(temp);
		if (ZonalData->County[taz] < 1 || ZonalData->County[taz] > Ini->NUMBER_COUNTIES) {
			printf ("\nError reading zonal data.  County[%d] = %d is out of range:  1 <= County <= %d.\n\n", taz, ZonalData->County[taz], Ini->NUMBER_COUNTIES);
			fprintf (fp_rep, "\nError reading zonal data.  County[%d] = %d is out of range:  1 <= County <= %d.\n\n", taz, ZonalData->County[taz], Ini->NUMBER_COUNTIES);
			fflush (stdout);
			fflush (fp_rep);
			exit (-1);
		}

		strncpy (temp, &InputRecord[POP_START-1], POP_LENGTH);
		temp[POP_LENGTH] = '\0';
		ZonalData->HHPop[taz] = (float)atof(temp);
		if (ZonalData->HHPop[taz] < 0) {
			printf ("\nError reading zonal data.  Negative value for HHPop[%d] = %.4f.  HHPop should be non-negative.\n\n", taz, ZonalData->HHPop[taz]);
			fprintf (fp_rep, "\nError reading zonal data.  Negative value for HHPop[%d] = %.4f.  HHPop should be non-negative.\n\n", taz, ZonalData->HHPop[taz]);
			fflush (stdout);
			fflush (fp_rep);
			exit (-1);
		}

		strncpy (temp, &InputRecord[HHS_START-1], HHS_LENGTH);
		temp[HHS_LENGTH] = '\0';
		ZonalData->HHNum[taz] = (float)atof(temp);
		if (ZonalData->HHNum[taz] < 0) {
			printf ("\nError reading zonal data.  Negative value for HHNum[%d] = %.4f.  HHNum should be non-negative.\n\n", taz, ZonalData->HHNum[taz]);
			fprintf (fp_rep, "\nError reading zonal data.  Negative value for HHNum[%d] = %.4f.  HHNum should be non-negative.\n\n", taz, ZonalData->HHNum[taz]);
			fflush (stdout);
			fflush (fp_rep);
			exit (-1);
		}

		strncpy (temp, &InputRecord[TOTEMP_START-1], TOTEMP_LENGTH);
		temp[TOTEMP_LENGTH] = '\0';
		ZonalData->TotEmp[taz] = (float)atof(temp);
		if (ZonalData->TotEmp[taz] < 0) {
			printf ("\nError reading zonal data.  Negative value for TotEmp[%d] = %.4f.  TotEmp should be non-negative.\n\n", taz, ZonalData->TotEmp[taz]);
			fprintf (fp_rep, "\nError reading zonal data.  Negative value for TotEmp[%d] = %.4f.  TotEmp should be non-negative.\n\n", taz, ZonalData->TotEmp[taz]);
			fflush (stdout);
			fflush (fp_rep);
			exit (-1);
		}

		strncpy (temp, &InputRecord[RETEMP_START-1], RETEMP_LENGTH);
		temp[RETEMP_LENGTH] = '\0';
		ZonalData->RetEmp[taz] = (float)atof(temp);
		if (ZonalData->RetEmp[taz] < 0) {
			printf ("\nError reading zonal data.  Negative value for RetEmp[%d] = %.4f.  RetEmp should be non-negative.\n\n", taz, ZonalData->RetEmp[taz]);
			fprintf (fp_rep, "\nError reading zonal data.  Negative value for RetEmp[%d] = %.4f.  RetEmp should be non-negative.\n\n", taz, ZonalData->RetEmp[taz]);
			fflush (stdout);
			fflush (fp_rep);
			exit (-1);
		}

		strncpy (temp, &InputRecord[OFFEMP_START-1], OFFEMP_LENGTH);
		temp[OFFEMP_LENGTH] = '\0';
		ZonalData->OffEmp[taz] = (float)atof(temp);
		if (ZonalData->OffEmp[taz] < 0) {
			printf ("\nError reading zonal data.  Negative value for OffEmp[%d] = %.4f.  OffEmp should be non-negative.\n\n", taz, ZonalData->OffEmp[taz]);
			fprintf (fp_rep, "\nError reading zonal data.  Negative value for OffEmp[%d] = %.4f.  OffEmp should be non-negative.\n\n", taz, ZonalData->OffEmp[taz]);
			fflush (stdout);
			fflush (fp_rep);
			exit (-1);
		}

		strncpy (temp, &InputRecord[UNVENROL_START-1], UNVENROL_LENGTH);
		temp[UNVENROL_LENGTH] = '\0';
		ZonalData->UnvEnrol[taz] = (float)atof(temp);
		if (ZonalData->UnvEnrol[taz] < 0) {
			printf ("\nError reading zonal data.  Negative value for UnvEnrol[%d] = %.4f.  UnvEnrol should be non-negative.\n\n", taz, ZonalData->UnvEnrol[taz]);
			fprintf (fp_rep, "\nError reading zonal data.  Negative value for UnvEnrol[%d] = %.4f.  UnvEnrol should be non-negative.\n\n", taz, ZonalData->UnvEnrol[taz]);
			fflush (stdout);
			fflush (fp_rep);
			exit (-1);
		}

		strncpy (temp, &InputRecord[SCHENROL_START-1], SCHENROL_LENGTH);
		temp[SCHENROL_LENGTH] = '\0';
		ZonalData->K12ETot[taz] = (float)atof(temp);
		if (ZonalData->K12ETot[taz] < 0) {
			printf ("\nError reading zonal data.  Negative value for K12ETot[%d] = %.4f.  K12ETot should be non-negative.\n\n", taz, ZonalData->K12ETot[taz]);
			fprintf (fp_rep, "\nError reading zonal data.  Negative value for K12ETot[%d] = %.4f.  K12ETot should be non-negative.\n\n", taz, ZonalData->K12ETot[taz]);
			fflush (stdout);
			fflush (fp_rep);
			exit (-1);
		}

		strncpy (temp, &InputRecord[PCTACC_START-1], PCTACC_LENGTH);
		temp[PCTACC_LENGTH] = '\0';
		ZonalData->PctAcc[taz] = (float)(atoi(temp)/100.0);
		if (ZonalData->PctAcc[taz] < 0) {
			printf ("\nError reading zonal data.  Negative value for PctAcc[%d] = %.4f.  PctAcc should be non-negative.\n\n", taz, ZonalData->PctAcc[taz]);
			fprintf (fp_rep, "\nError reading zonal data.  Negative value for PctAcc[%d] = %.4f.  PctAcc should be non-negative.\n\n", taz, ZonalData->PctAcc[taz]);
			fflush (stdout);
			fflush (fp_rep);
			exit (-1);
		}

	}
}


