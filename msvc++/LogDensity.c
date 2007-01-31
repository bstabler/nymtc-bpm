#include "md.h"

#define INTRA_SCHOOL_COEFF 1.243

double nm_log_density (int purpose, int orig, float gamma, float *t_attrs, int *SchoolDists, struct walk_zone_data *WalkZoneData)
{

// z_attrs are the remaining available attractions within a zone for non-motorized destination choice proportions
// t_attrs are the total initial attractions within a zone for pre-mode choice densities

	
	int j, dest;
	double density, percent, distance, size;
	
	density = 0.0;	
	for (j=0; j < WalkZoneData[orig].num; j++) {
		dest = WalkZoneData[orig].walk_zones[j].dest;
		distance = WalkZoneData[orig].walk_zones[j].distance;
		percent = WalkZoneData[orig].walk_zones[j].walk_pct;
		size = percent*t_attrs[dest];
		if (size > 0) {
			if (purpose != 3)
				density += size/pow(distance,gamma);
			else
				density += size*exp(INTRA_SCHOOL_COEFF*(SchoolDists[orig] == SchoolDists[dest]))/pow(distance,gamma);
		}
	}
	
	if (density > 0.0)
		return (log(density));
	else
		return (MISSING);
}
