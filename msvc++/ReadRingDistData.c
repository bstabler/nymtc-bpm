#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md.h"


#define RING_DIST_RECORD_LENGTH 48
#define ZONE_START 1
#define ZONE_LENGTH 12
#define RING_START 13
#define RING_LENGTH 12
#define DIST_START 25
#define DIST_LENGTH 12
#define BPMDIST1_START 33
#define BPMDIST1_LENGTH 4
#define LP_RESTRICT_START 37
#define LP_RESTRICT_LENGTH 12


void read_ring_dist_data (FILE *fp, struct zone_data *ZonalData)
{
	int zone, ring, rec_len, lpRestrict;
	float dist;
	char InputRecord[RING_DIST_RECORD_LENGTH+2];
	char temp[LRECL], bpmdist1[LRECL];


// Read correspondance file between zones and rings/dists.
	fgets(InputRecord, RING_DIST_RECORD_LENGTH+2, fp);							// ignore header record
	while ((fgets(InputRecord, RING_DIST_RECORD_LENGTH+2, fp)) != NULL) {
		InputRecord[RING_DIST_RECORD_LENGTH] = '\0';
		rec_len = (int)strlen(InputRecord);
		if (rec_len == RING_DIST_RECORD_LENGTH) {
			strncpy (temp, &InputRecord[ZONE_START-1], ZONE_LENGTH);
			temp[ZONE_LENGTH] = '\0';
			zone = atoi(temp);

			if (zone >= 1 && zone <= Ini->MAX_TAZS) {

				strncpy (temp, &InputRecord[RING_START-1], RING_LENGTH);
				temp[RING_LENGTH] = '\0';
				ring = atoi(temp);

				strncpy (temp, &InputRecord[DIST_START-1], DIST_LENGTH);
				temp[DIST_LENGTH] = '\0';
				dist = (float)atof(temp);

				strncpy (temp, &InputRecord[BPMDIST1_START-1], BPMDIST1_LENGTH);
				temp[BPMDIST1_LENGTH] = '\0';
				strcpy (bpmdist1, temp);
	
				strncpy (temp, &InputRecord[LP_RESTRICT_START-1], LP_RESTRICT_LENGTH);
				temp[LP_RESTRICT_LENGTH] = '\0';
				lpRestrict = atoi(temp);

				
				ZonalData->ring[zone] = ring;
				ZonalData->dist[zone] = dist;
				if (!strcmp(bpmdist1, "01.1"))
					ZonalData->bpmdist1_index[zone] = 0;
				else if (!strcmp(bpmdist1, "01.2"))
					ZonalData->bpmdist1_index[zone] = 1;
				else if (!strcmp(bpmdist1, "01.3"))
					ZonalData->bpmdist1_index[zone] = 2;
				else if (!strcmp(bpmdist1, "01.4"))
					ZonalData->bpmdist1_index[zone] = 3;
				else 
					ZonalData->bpmdist1_index[zone] = (int)(atof(bpmdist1)) + 2;
				ZonalData->lpRestricted[zone] = lpRestrict;

			}
			else {
				printf ("invalid zone number = %d read in from RingDistData file.\n", zone);
				exit (-1);
			}
		}
	}
}
