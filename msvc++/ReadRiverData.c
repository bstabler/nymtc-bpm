#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md.h"


// river crossing data input file formats
#define RIVER_RECORD_LENGTH  45
#define ICO_START			 1
#define ICO_LENGTH			 9
#define JCO_START			 10
#define JCO_LENGTH			 9
#define EAST_START			 19
#define EAST_LENGTH			 9
#define HUDSON_START		 28
#define HUDSON_LENGTH		 9
#define MINOR_START			 37
#define MINOR_LENGTH		 9


void read_river_data (FILE *fp, struct river_crossing_data RiverData)
{
	int i_co, j_co;
	char InputRecord[RIVER_RECORD_LENGTH+2];
	char temp[LRECL];


// Read river crossing data records.
	fgets(InputRecord, RIVER_RECORD_LENGTH+5, fp);										// ignore header record
	while ((fgets(InputRecord, RIVER_RECORD_LENGTH+5, fp)) != NULL) {

		strncpy (temp, &InputRecord[ICO_START-1], ICO_LENGTH);
		temp[ICO_LENGTH] = '\0';
		i_co = atoi(temp);

		strncpy (temp, &InputRecord[JCO_START-1], JCO_LENGTH);
		temp[JCO_LENGTH] = '\0';
		j_co = atoi(temp);

		strncpy (temp, &InputRecord[EAST_START-1], EAST_LENGTH);
		temp[EAST_LENGTH] = '\0';
		RiverData.east[i_co][j_co] = atoi(temp);
		RiverData.east[j_co][i_co] = atoi(temp);

		strncpy (temp, &InputRecord[HUDSON_START-1], HUDSON_LENGTH);
		temp[HUDSON_LENGTH] = '\0';
		RiverData.hudson[i_co][j_co] = atoi(temp);
		RiverData.hudson[j_co][i_co] = atoi(temp);

		strncpy (temp, &InputRecord[MINOR_START-1], MINOR_LENGTH);
		temp[MINOR_LENGTH] = '\0';
		RiverData.minor[i_co][j_co] = atoi(temp);
		RiverData.minor[j_co][i_co] = atoi(temp);

	}
}


