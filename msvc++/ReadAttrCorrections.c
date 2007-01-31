#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md.h"


// attraction correction factor input file formats
#define RECORD_LENGTH		15
#define TAZ_START			 1
#define TAZ_LENGTH			 5
#define FACTOR_START		 6
#define FACTOR_LENGTH		10


void read_attr_corrections (FILE *fp, double *attr_corrections)
{
	int i, rec_len;
	char InputRecord[RECORD_LENGTH+2];
	char temp[LRECL];


// Read attraction correction factor records.
	fgets(InputRecord, RECORD_LENGTH+2, fp);										// ignore header record
	while ((fgets(InputRecord, RECORD_LENGTH+2, fp)) != NULL) {
		InputRecord[RECORD_LENGTH] = '\0';
		rec_len = (int)strlen(InputRecord);
		if (rec_len == RECORD_LENGTH) {
			strncpy (temp, &InputRecord[TAZ_START-1], TAZ_LENGTH);
			temp[TAZ_LENGTH] = '\0';
			i = atoi(temp);

			strncpy (temp, &InputRecord[FACTOR_START-1], FACTOR_LENGTH);
			temp[FACTOR_LENGTH] = '\0';
			attr_corrections[i] = atof(temp);
		}
	}
}


