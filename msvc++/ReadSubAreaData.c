#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md.h"


#define RECORD_LENGTH 8
#define ZONE_START 1
#define ZONE_LENGTH 8


void read_subarea_data (FILE *fp, struct zone_data *ZonalData)
{
	int zone, rec_len;
	char InputRecord[RECORD_LENGTH+2];
	char temp[LRECL];


// Read sub-area definitions file.
	fgets(InputRecord, 200, fp);							// ignore header record for origin subset
	while ((fgets(InputRecord, RECORD_LENGTH+5, fp)) != NULL) {
		//InputRecord[RECORD_LENGTH] = '\0';
		//rec_len = (int)strlen(InputRecord);
		//if (rec_len == RECORD_LENGTH) {

			strncpy (temp, &InputRecord[ZONE_START-1], ZONE_LENGTH);
			temp[ZONE_LENGTH] = '\0';
			zone = atoi(temp);

			if (zone >= 1 && zone <= Ini->MAX_TAZS) {
				ZonalData->OrigSubAreaOB[zone] = 1;
				ZonalData->OrigSubAreaIB[zone] = 1;
			}
			else {
				printf ("invalid origin zone number = %d read in from sub-area defintions file.\n", zone);
				exit (-1);
			}
		//}
		//else {
	    //	break;
		//}
	}


	fgets(InputRecord, 200, fp);							// ignore header record for destination subset
	while ((fgets(InputRecord, RECORD_LENGTH+5, fp)) != NULL) {
		InputRecord[RECORD_LENGTH] = '\0';
		rec_len = (int)strlen(InputRecord);
		if (rec_len == RECORD_LENGTH) {

			strncpy (temp, &InputRecord[ZONE_START-1], ZONE_LENGTH);
			temp[ZONE_LENGTH] = '\0';
			zone = atoi(temp);

			if (zone >= 1 && zone <= Ini->MAX_TAZS) {
				ZonalData->DestSubAreaOB[zone] = 1;
				ZonalData->DestSubAreaIB[zone] = 1;
			}
			else {
				printf ("invalid destination zone number = %d read in from sub-area defintions file.\n", zone);
				exit (-1);
			}
		}
		else {
			break;
		}
	}
}
