#include "md.h"

#define RECORD_LENGTH 36
#define TAZ_START 1
#define TAZ_LENGTH 12
#define SCH_DIST_START 13
#define SCH_DIST_LENGTH 12

void read_school_district_data (FILE *fp, struct zone_data *ZonalData)
{
	int taz, rec_len;
	char InputRecord[RECORD_LENGTH+2];
	char temp[RECORD_LENGTH];

	
// Read zonal data records.
	while ((fgets(InputRecord, RECORD_LENGTH+2, fp)) != NULL) {
		InputRecord[RECORD_LENGTH] = '\0';
		rec_len = (int)strlen(InputRecord);
		if (rec_len == RECORD_LENGTH) {
			strncpy (temp, &InputRecord[TAZ_START-1], TAZ_LENGTH);
			temp[TAZ_LENGTH] = '\0';
			taz = atoi(temp);

			strncpy (temp, &InputRecord[SCH_DIST_START-1], SCH_DIST_LENGTH);
			temp[SCH_DIST_LENGTH] = '\0';
			ZonalData->SchoolDist[taz] = atoi(temp);
		}
	}
}

