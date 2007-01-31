#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md.h"


#define WALK_ZONE_RECORD_LENGTH 43
#define ORIG_START 1
#define ORIG_LENGTH 8
#define DEST_START 9
#define DEST_LENGTH 8
#define DIST_START 17
#define DIST_LENGTH 9
#define WALKPCT_START 26
#define WALKPCT_LENGTH 10


void read_walk_zone_data (FILE *fp, struct walk_zone_data *WalkZoneData)
{
	int j, orig, old_orig, count, rec_len, tempRAM;
	char InputRecord[WALK_ZONE_RECORD_LENGTH+2];
	char temp[LRECL];
	struct avail_dests *temp_list;

// Memory allocations
	temp_list = (struct avail_dests *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(struct avail_dests));
//	addRAM ("read_walk_zone_data", (Ini->MAX_TAZS+1)*sizeof(struct avail_dests));


// Read zones and distances within walk range.
	old_orig = 1;
	j = 0;
	count = 0;
	tempRAM = 0;
	while ((fgets(InputRecord, WALK_ZONE_RECORD_LENGTH+2, fp)) != NULL) {
		InputRecord[WALK_ZONE_RECORD_LENGTH] = '\0';
		rec_len = (int)strlen(InputRecord);
		if (rec_len == WALK_ZONE_RECORD_LENGTH) {
			strncpy (temp, &InputRecord[ORIG_START-1], ORIG_LENGTH);
			temp[ORIG_LENGTH] = '\0';
			orig = atoi(temp);

			if (orig != old_orig) {
				count  = j;
				WalkZoneData[old_orig].num = count;
				WalkZoneData[old_orig].walk_zones = (struct avail_dests *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, count*sizeof(struct avail_dests));
				tempRAM += count*sizeof(struct avail_dests);
				for (j=0; j < count; j++) {
					WalkZoneData[old_orig].walk_zones[j].dest = temp_list[j].dest;
					WalkZoneData[old_orig].walk_zones[j].distance = temp_list[j].distance;
					WalkZoneData[old_orig].walk_zones[j].walk_pct = temp_list[j].walk_pct;
				}
				old_orig = orig;
				j = 0;
			}

			strncpy (temp, &InputRecord[DEST_START-1], DEST_LENGTH);
			temp[DEST_LENGTH] = '\0';
			temp_list[j].dest = atoi(temp);

			strncpy (temp, &InputRecord[DIST_START-1], DIST_LENGTH);
			temp[DIST_LENGTH] = '\0';
			temp_list[j].distance = (float)atof(temp);

			strncpy (temp, &InputRecord[WALKPCT_START-1], WALKPCT_LENGTH);
			temp[WALKPCT_LENGTH] = '\0';
			temp_list[j].walk_pct = (float)atof(temp);

			j++;
		}
	}


	count  = j;
	WalkZoneData[orig].num = count;
	WalkZoneData[orig].walk_zones = (struct avail_dests *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, count*sizeof(struct avail_dests));
	tempRAM += count*sizeof(struct avail_dests);
	for (j=0; j < count; j++) {
		WalkZoneData[orig].walk_zones[j].dest = temp_list[j].dest;
		WalkZoneData[orig].walk_zones[j].distance = temp_list[j].distance;
		WalkZoneData[orig].walk_zones[j].walk_pct = temp_list[j].walk_pct;
	}

//	addRAM ("read_walk_zone_data", tempRAM);

	HeapFree (heapHandle, 0, temp_list);
//	relRAM ("read_walk_zone_data", (Ini->MAX_TAZS+1)*sizeof(struct avail_dests));
}
