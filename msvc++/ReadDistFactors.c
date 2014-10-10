#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md.h"


// origin county based distance scale factor input file formats
#define RECORD_LENGTH		25
#define CO_START			 1
#define CO_LENGTH			 5
#define COEFF_START			 6
#define COEFF_LENGTH		10
#define EXP_START			16
#define EXP_LENGTH			10


void read_dist_factors (FILE *fp, struct co_dist_factors *DistFactors)
{
	int co;
	char InputRecord[RECORD_LENGTH+2];
	char temp[LRECL];


// Read attraction correction factor records.
	fgets(InputRecord, RECORD_LENGTH+5, fp);										// ignore header record
	while ((fgets(InputRecord, RECORD_LENGTH+5, fp)) != NULL) {

		strncpy (temp, &InputRecord[CO_START-1], CO_LENGTH);
		temp[CO_LENGTH] = '\0';
		co = atoi(temp);

		strncpy (temp, &InputRecord[COEFF_START-1], COEFF_LENGTH);
		temp[COEFF_LENGTH] = '\0';
		DistFactors[co].coeff = atof(temp);

		strncpy (temp, &InputRecord[EXP_START-1], EXP_LENGTH);
		temp[EXP_LENGTH] = '\0';
		DistFactors[co].exp = atof(temp);

	}
}


