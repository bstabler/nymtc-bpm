#include "md.h"

#define INTRA_SCHOOL_COEFF 1.243

void nm_dest_props (int purpose, int orig, float gamma, float *z_attrs, int *SchoolDists, struct walk_zone_data *WalkZoneData, double *dest_props)
{

// z_attrs are the remaining available attractions within a zone for non-motorized destination choice proportions

	
	int j, dest;
	double denominator, percent, distance, size;

	
// start with denominator of choice function
	denominator = 0.0;	
	for (j=0; j < WalkZoneData[orig].num; j++) {
		dest = WalkZoneData[orig].walk_zones[j].dest;
		distance = WalkZoneData[orig].walk_zones[j].distance;
		percent = WalkZoneData[orig].walk_zones[j].walk_pct;
		size = percent*z_attrs[dest];

		if (size > 0) {
			if (purpose != 3)
				denominator += (size/pow(distance,gamma));
			else
				denominator += (size*exp(INTRA_SCHOOL_COEFF*(SchoolDists[orig] == SchoolDists[dest]))/pow(distance,gamma));
		}
	}
	

// calculate the non-motorized destination choice proportions and pass back
	for (j=0; j < WalkZoneData[orig].num; j++) {
		dest = WalkZoneData[orig].walk_zones[j].dest;
		distance = WalkZoneData[orig].walk_zones[j].distance;
		percent = WalkZoneData[orig].walk_zones[j].walk_pct;
		size = percent*z_attrs[dest];

		if (size > 0) {
			if (purpose != 3)
				dest_props[j] = (size/pow(distance,gamma))/denominator;
			else
				dest_props[j] = (size*exp(INTRA_SCHOOL_COEFF*(SchoolDists[orig] == SchoolDists[dest]))/pow(distance,gamma))/denominator;
		}
		else {
			dest_props[j] = 0.0;
		}
	}
}
