#include "md.h"

int get_motorized_MSC_index (int orig, int dest, struct zone_data *ZonalData, struct msc_data *msc)
{

	int idist, jdist;

	idist = ZonalData->bpmdist1_index[orig];
	jdist = ZonalData->bpmdist1_index[dest];

	return (msc->motorized_indices[idist][jdist]);
}
