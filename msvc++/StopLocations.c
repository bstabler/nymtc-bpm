/*
 *	Determine which intermediate zones between the journey origin and destination
 *  are available for stop locations and whether they are close to the origin,
 *  close to the destination, on the way, or any combination of the three.
 *
 */

#include "md.h"


#define WALK_DIST 2.0


// stop radius
#define WORK_LO_STOP_RADIUS	6.0
#define WORK_MD_STOP_RADIUS	6.0
#define WORK_HI_STOP_RADIUS	6.0
#define SCHOOL_STOP_RADIUS	6.0
#define UNIV_STOP_RADIUS	6.0
#define MAINT_STOP_RADIUS	5.0
#define DISCR_STOP_RADIUS	5.0
#define ATWORK_STOP_RADIUS	5.0


// stop deviation pct. value is added to 1.0 to get the actual deviation
#define WORK_LO_STOP_DEV	1.0
#define WORK_MD_STOP_DEV	1.0
#define WORK_HI_STOP_DEV	1.0
#define SCHOOL_STOP_DEV		1.0
#define UNIV_STOP_DEV		1.0
#define MAINT_STOP_DEV		0.3
#define DISCR_STOP_DEV		0.3
#define ATWORK_STOP_DEV		0.3




void stop_location_avail (int orig, int dest, int mode, struct zone_data *ZonalData, float **hwy_dist, 
    float **wt_ivt, float **dt_ivt, float **wc_ivt, float **dc_ivt, int *near_orig, int *near_dest, 
    int *midway)
{

	int k;
	float o_ivt_limit, d_ivt_limit, walk_time_ik, walk_time_kj;
	double stop_radius, stop_dev;
    int *bpmdist1_index;
    int bpmdist1_index_orig, bpmdist1_index_dest;
    float *hwy_dist_orig;
    float *wt_ivt_orig;
    float *dt_ivt_orig;
    float *wc_ivt_orig;
    float *dc_ivt_orig;

    bpmdist1_index = ZonalData->bpmdist1_index;
    bpmdist1_index_orig = bpmdist1_index[orig];
    bpmdist1_index_dest = bpmdist1_index[dest];

    hwy_dist_orig = hwy_dist[orig];
    wt_ivt_orig = wt_ivt[orig];
    dt_ivt_orig = dt_ivt[orig];
    wc_ivt_orig = wc_ivt[orig];
    dc_ivt_orig = dc_ivt[orig];
        
	switch (Ini->PURPOSE_TO_PROCESS) {
		case 0:
			stop_radius = WORK_LO_STOP_RADIUS;
			stop_dev = WORK_LO_STOP_DEV;
			break;
		case 1:
			stop_radius = WORK_MD_STOP_RADIUS;
			stop_dev = WORK_MD_STOP_DEV;
			break;
		case 2:
			stop_radius = WORK_HI_STOP_RADIUS;
			stop_dev = WORK_HI_STOP_DEV;
			break;
		case 3:
			stop_radius = SCHOOL_STOP_RADIUS;
			stop_dev = SCHOOL_STOP_DEV;
			break;
		case 4:
			stop_radius = UNIV_STOP_RADIUS;
			stop_dev = UNIV_STOP_DEV;
			break;
		case 5:
			stop_radius = MAINT_STOP_RADIUS;
			stop_dev = MAINT_STOP_DEV;
			break;
		case 6:
			stop_radius = DISCR_STOP_RADIUS;
			stop_dev = DISCR_STOP_DEV;
			break;
		case 7
			:
			stop_radius = ATWORK_STOP_RADIUS;
			stop_dev = ATWORK_STOP_DEV;
			break;
	}


	o_ivt_limit = (float)(60.0*stop_radius/(Ini->BUS_FACTOR*Ini->AUTO_SPEED[ZonalData->AreaType[orig]]));
	d_ivt_limit = (float)(60.0*stop_radius/(Ini->BUS_FACTOR*Ini->AUTO_SPEED[ZonalData->AreaType[dest]]));

	for (k=1; k <= Ini->MAX_TAZS; k++) {
		near_orig[k] = 0;
		near_dest[k] = 0;
		midway[k] = 0;

		if (ValidStop(bpmdist1_index_orig, bpmdist1_index_dest, bpmdist1_index[k])) {

			walk_time_ik = (float)(60.0*hwy_dist_orig[k]/Ini->WALK_SPEED);
			walk_time_kj = (float)(60.0*hwy_dist[k][dest]/Ini->WALK_SPEED);

			// highway modes
			if ((mode < 5 || mode == 9) && (hwy_dist_orig[k] > 0.0 && hwy_dist[k][dest] > 0.0)) {
				if (hwy_dist_orig[k] < stop_radius)
					near_orig[k] = 1;
				if (hwy_dist[k][dest] < stop_radius)
					near_dest[k] = 1;
				if ((hwy_dist_orig[k] + hwy_dist[k][dest]) < ((1.0 + stop_dev)*hwy_dist_orig[dest]))
					midway[k] = 1;
			}

			// walk transit mode (nm, wt to stop, nm, wt from stop, at least one leg must have wt_ivt > 0.0)
			else if (mode == 5 && ((hwy_dist_orig[k] < WALK_DIST && wt_ivt[k][dest] > 0.0) ||
				(wt_ivt_orig[k] > 0.0 && hwy_dist[k][dest] < WALK_DIST) ||
				(wt_ivt_orig[k] > 0.0 && wt_ivt[k][dest] > 0.0))) {

				if ((wt_ivt_orig[k] < o_ivt_limit) || (hwy_dist_orig[k] < WALK_DIST && wt_ivt[k][dest] > 0.0))
					near_orig[k] = 1;

				if ((wt_ivt[k][dest] < d_ivt_limit) || (wt_ivt_orig[k] > 0.0 && hwy_dist[k][dest] < WALK_DIST))
					near_dest[k] = 1;

				if ((wt_ivt_orig[k] + wt_ivt[k][dest]) < ((1.0 + stop_dev)*wt_ivt_orig[dest]) ||
					(walk_time_ik + wt_ivt[k][dest]) < ((1.0 + stop_dev)*wt_ivt_orig[dest]) ||
					(wt_ivt_orig[k] + walk_time_kj) < ((1.0 + stop_dev)*wt_ivt_orig[dest]))
						midway[k] = 1;
			}

			// drive transit mode (dt to stop, nm, wt from stop)
			else if (mode == 6 && ((dt_ivt_orig[k] > 0.0 && wt_ivt[k][dest] > 0.0) ||
				(dt_ivt_orig[k] > 0.0 && hwy_dist[k][dest] < WALK_DIST))) {

				if (dt_ivt_orig[k] < o_ivt_limit)
					near_orig[k] = 1;

				if (wt_ivt[k][dest] < d_ivt_limit || (dt_ivt_orig[k] > 0.0 && hwy_dist[k][dest] < WALK_DIST))
					near_dest[k] = 1;

				if ((dt_ivt_orig[k] + wt_ivt[k][dest]) < ((1.0 + stop_dev)*dt_ivt_orig[dest]) ||
					(dt_ivt_orig[k] + walk_time_kj) < ((1.0 + stop_dev)*dt_ivt_orig[dest]))
						midway[k] = 1;
			}

			// walk commuter rail mode (nm, wc to stop, nm, wc from stop, at least one leg must have wc_ivt > 0.0)
			else if (mode == 7 && ((hwy_dist_orig[k] < WALK_DIST && wc_ivt[k][dest] > 0.0) ||
				(wc_ivt_orig[k] > 0.0 && hwy_dist[k][dest] < WALK_DIST) ||
				(wc_ivt_orig[k] > 0.0 && wc_ivt[k][dest] > 0.0))) {

				if ((wc_ivt_orig[k] < o_ivt_limit) || (hwy_dist_orig[k] < WALK_DIST && wc_ivt[k][dest] > 0.0))
					near_orig[k] = 1;

				if ((wc_ivt[k][dest] < d_ivt_limit) || (wc_ivt_orig[k] > 0.0 && hwy_dist[k][dest] < WALK_DIST))
					near_dest[k] = 1;

				if ((wc_ivt_orig[k] + wc_ivt[k][dest]) < ((1.0 + stop_dev)*wc_ivt_orig[dest]) ||
					(walk_time_ik + wc_ivt[k][dest]) < ((1.0 + stop_dev)*wc_ivt_orig[dest]) ||
					(wc_ivt_orig[k] + walk_time_kj) < ((1.0 + stop_dev)*wc_ivt_orig[dest]))
						midway[k] = 1;
			}

			// drive commuter rail mode (dc to stop, nm, wt, wc from stop)
			else if (mode == 8 && ((dc_ivt_orig[k] > 0.0 && wc_ivt[k][dest] > 0.0) ||
				(dc_ivt_orig[k] > 0.0 && wt_ivt[k][dest] > 0.0) ||
				(dc_ivt_orig[k] > 0.0 && hwy_dist[k][dest] < WALK_DIST))) {

				if (dc_ivt_orig[k] < o_ivt_limit)
					near_orig[k] = 1;

				if ((wc_ivt[k][dest] < d_ivt_limit) || (dc_ivt_orig[k] > 0.0 && hwy_dist[k][dest] < WALK_DIST) ||
					(dc_ivt_orig[k] > 0.0 && wt_ivt[dest][k] < d_ivt_limit))
						near_dest[k] = 1;

				if ((dc_ivt_orig[k] + wc_ivt[k][dest]) < ((1.0 + stop_dev)*dc_ivt_orig[dest]) ||
					(dc_ivt_orig[k] + wt_ivt[k][dest]) < ((1.0 + stop_dev)*dc_ivt_orig[dest]) ||
					(dc_ivt_orig[k] + walk_time_kj) < ((1.0 + stop_dev)*dc_ivt_orig[dest]))
						midway[k] = 1;

			}	
		}

	}
}
