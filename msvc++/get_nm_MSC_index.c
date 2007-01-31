#include "md.h"

int get_nm_MSC_index (int orig, struct zone_data *ZonalData, struct msc_data *msc)
{
	int idist;

	idist = ZonalData->bpmdist1_index[orig];

	return (msc->nm_indices[idist]);
}
