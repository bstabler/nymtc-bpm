/*
 *  Match at-work records by household/person to the corresponding work journeys.  The work destinations
 *  become origins for the at-work journeys.  The at-work-mode is 1 if drive alone, 0 otherwise.
 */

#include "md.h"

void link_atwork (int *hh_pers, int *index, int work_journeys, short *mode, short *dest, int hh, int persno,
				  int *work_mode, int *work_dest)
{

	int k, atwork_hh_pers;


	// build a hh/persno index for the at-work journey
	atwork_hh_pers = hh*MAX_PERSONS_PER_HH + persno;


	// k is the corresponding work journey number for the current at-work journey
	k = indexed_binary_search (atwork_hh_pers, 0, work_journeys, hh_pers, index);

	if (k < 0) {
		// k is negative if corresponding work journey was not found.  Set at_work_mode attribute to -1 in this case.
		*work_mode = -1;
	}
	else {
		*work_dest = dest[k];
		*work_mode = mode[k];
	}
}
