#include "md.h"

int nm_dest_choice (int orig, struct walk_zone_data *WalkZoneData, double *dest_props)
{

	int i, j;
	double nrand, *cum;

// compute the cumulative probability distribution of available walk destinations
	cum = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (WalkZoneData[orig].num+1)*sizeof(double));
//	if (Ini->RAM_DEBUG_LEVEL == 2)
//		addRAM ("nm_dest_choice 1",  (WalkZoneData[orig].num+1)*sizeof(double));

	cum[0] = dest_props[0];
	for (j=1; j < WalkZoneData[orig].num; j++)
		cum[j] = cum[j-1] + dest_props[j];


// select a random number, and locate the destination zone in the cumulative probability distribution
	if (cum[WalkZoneData[orig].num - 1] < 0.0000001) {
		HeapFree (heapHandle, 0, cum);
//		if (Ini->RAM_DEBUG_LEVEL == 2)
//			relRAM ("nm_dest_choice 1",  (WalkZoneData[orig].num+1)*sizeof(double));
		return (-1);
	}
	else {
		nrand = (double)rand()/(double)MAX_RANDOM;
		for (i=0; i < WalkZoneData[orig].num; i++) {
			if (nrand < cum[i]) {
				HeapFree (heapHandle, 0, cum);
//				if (Ini->RAM_DEBUG_LEVEL == 2)
//					relRAM ("nm_dest_choice 2",  (WalkZoneData[orig].num+1)*sizeof(double));
				return (WalkZoneData[orig].walk_zones[i].dest);
			}
		}
	}

	HeapFree (heapHandle, 0, cum);
//	if (Ini->RAM_DEBUG_LEVEL == 2)
//		relRAM ("nm_dest_choice 3",  (WalkZoneData[orig].num+1)*sizeof(double));

	return (-1);
}
