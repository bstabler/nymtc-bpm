// pre-mode choice (motorized / non-motorized split)

#include "md.h"

int pre_mode_choice (int k, float gamma, struct journey_attribs *JourneyAttribs, float **z_attrs, float **t_attrs, struct zone_data *ZonalData, struct walk_zone_data *WalkZoneData, struct msc_data *msc)
{

	int j, orig, dest, walk_orig, ut_orig, autos, workers, income, purpose;
	int person_type, at_work_mode, school_dist, return_value, nm_msc_index;
    int idist;
	double log_density, avail_attrs;

	orig = JourneyAttribs->orig[k];
	walk_orig = JourneyAttribs->walk_orig[k];
	ut_orig = ZonalData->UrbanType[orig];
	autos = JourneyAttribs->autos[k];
	workers = JourneyAttribs->workers[k];
	income = JourneyAttribs->income[k];
	purpose = JourneyAttribs->purpose[k];
	person_type = JourneyAttribs->person_type[k];
	at_work_mode = JourneyAttribs->at_work_mode[k];
	school_dist = ZonalData->SchoolDist[orig];


// calculate pre-mode choice log_density based on total attractions, and ring_props based on availble attractions
	log_density = nm_log_density (purpose, orig, gamma, t_attrs[purpose], ZonalData->SchoolDist, WalkZoneData);


// next determine if the journey is motorized (1) or not (0).  If log_density is MISSING, there are no non-motorized attractions.
//	nm_msc_index = get_nm_MSC_index (orig, ZonalData, msc);
    idist = ZonalData->bpmdist1_index[orig];
    nm_msc_index = msc->nm_indices[idist];
	if (log_density != MISSING)
		return_value = motorized_choice[purpose] (income, autos, workers, person_type, ut_orig, walk_orig, at_work_mode, school_dist, log_density, msc->nmMSC[nm_msc_index]);
	else
		return_value = 1;


// return_value == 0 and avail_attrs == 0, it means non-motorized was selected by pre-mode choice,
// but there are no non-motorized locations available, so return -1 (unmet non-motorized).
	if (return_value == 0) {
		avail_attrs = 0.0;
		for (j=0; j < WalkZoneData[orig].num; j++) {
			dest = WalkZoneData[orig].walk_zones[j].dest;
			avail_attrs += WalkZoneData[orig].walk_zones[j].walk_pct*z_attrs[purpose][dest];
		}

		if (avail_attrs <= 0)
			return_value = -1;			// will re-classify as motorized
	}



	return(return_value);
}