#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DIST_REGION_RECORD_LENGTH 48
#define ZONE_START 1
#define ZONE_LENGTH 12
#define BPMDIST1_START 33
#define BPMDIST1_LENGTH 4
#define SUBREGION_START 37
#define SUBREGION_LENGTH 12


void read_dist_region_data (FILE *fp, int *taz2dist, int *taz2reg, int TAZS)
{
	int zone, subregion, rec_len;
	char InputRecord[DIST_REGION_RECORD_LENGTH+2];
	char temp[1000], bpmdist1[1000];


// Read correspondance file between zones and rings/dists.
	fgets(InputRecord, DIST_REGION_RECORD_LENGTH+2, fp);							// ignore header record
	while ((fgets(InputRecord, DIST_REGION_RECORD_LENGTH+2, fp)) != NULL) {
		InputRecord[DIST_REGION_RECORD_LENGTH] = '\0';
		rec_len = (int)strlen(InputRecord);
		if (rec_len == DIST_REGION_RECORD_LENGTH) {
			strncpy (temp, &InputRecord[ZONE_START-1], ZONE_LENGTH);
			temp[ZONE_LENGTH] = '\0';
			zone = atoi(temp);

			if (zone >= 1 && zone <= TAZS) {
				strncpy (temp, &InputRecord[BPMDIST1_START-1], BPMDIST1_LENGTH);
				temp[BPMDIST1_LENGTH] = '\0';
				strcpy (bpmdist1, temp);
	
				strncpy (temp, &InputRecord[SUBREGION_START-1], SUBREGION_LENGTH);
				temp[SUBREGION_LENGTH] = '\0';
				subregion = atoi(temp);
				
				taz2reg[zone] = subregion;
	
				if (!strcmp(bpmdist1, "01.1"))
					taz2dist[zone] = 1;
				else if (!strcmp(bpmdist1, "01.2"))
					taz2dist[zone] = 2;
				else if (!strcmp(bpmdist1, "01.3"))
					taz2dist[zone] = 3;
				else if (!strcmp(bpmdist1, "01.4"))
					taz2dist[zone] = 4;
				else 
					taz2dist[zone] = (int)(atof(bpmdist1)) + 3;
			}
			else {
				printf ("invalid zone number = %d read in from districe_subregion correspondence file.\n", zone);
				exit (-1);
			}
		}
	}
}
