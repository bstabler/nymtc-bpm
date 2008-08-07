#include "md.h"


void motor_dc_props (int orig, int purpose, struct dc_coeff_data *DCcoeff, float ***ODutil, float *SEutil,
					 float **Logsum, float ***z_attrs, struct river_crossing_data RiverData,
					 struct zone_data *ZonalData, struct bpmdist1_coeff_data BPMDist1,
					 double *cum, float *hwydist, int debug_mode, double **util, double **props, short only_free)
{

	FILE *fp;
	int i, j, k, m, ico, jco, ibpm, jbpm, IntraCounty;
//	double dist_scale, denominator, **util, **props;
	double dist_scale, denominator, delta_IntraCounty;
    double gamma0, gamma1, gamma2;
    double gamma0River, gamma1River, gamma2River, util_val;
	static int AlreadyOpen=0, allocated = 0;
    int *RiverData_east, *RiverData_hudson, *RiverData_minor;
    float *BPMDist1_co_co_coeffs, BPMDist1_intra_county;


//  07aug2008:
//	Caliper had changed this part of the code, introducing some structure to hold util and props values.
//	The array of structures, defined as static, weren't properly declared upon re-entry, on multiple auto-calibration iterations.
//	I changed the util and props values back to simple 2-D arrays and declared and freed them in run_mdc, so they were setup
//	only once per calibration iteration.  There's no need to initialize them each time, they are assigned, not used to accumulate values.



//    if (!allocated)
//        {
//    	util  = (double**) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(double *));
//	    props = (double**) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(double *));
//		for (i=0; i < Ini->MAX_TAZS+1; i++) {
//			util[i]  = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 2*sizeof(double));
//			props[i] = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 2*sizeof(double));
//		}
//        allocated = 1;
//    	if (Ini->RAM_DEBUG_LEVEL == 2)
//	    	addRAM ("motor_dc_props", 2*(Ini->MAX_TAZS+1)*2*sizeof(struct d_vals));
//        }
//    else if (only_free)
//        {
//		for (i=0; i < Ini->MAX_TAZS+1; i++) {
//			HeapFree (heapHandle, 0, util[i]);
//			HeapFree (heapHandle, 0, props[i]);
//		}
//        HeapFree (heapHandle, 0, util);
//        HeapFree (heapHandle, 0, props);
//        return;
//        }
//    else
//        {

//		for (i=0; i < Ini->MAX_TAZS+1; i++) {
//			memset (util[i], 0, 2*sizeof(double));
//			memset (props[i], 0, 2*sizeof(double));
//		}

        // init the first couple of elemets - a lot of what they do is 1-based
//        util[0].d[0] = 0.;
//        util[0].d[1] = 0.;
//        util[1].d[0] = 0.;
//        util[1].d[1] = 0.;
//        props[0].d[0] = 0.;
//        props[0].d[1] = 0.;
//        props[1].d[0] = 0.;
//        props[1].d[1] = 0.;
        // looks like the memset is not necessary - it always sets it to 0 with if then else
//        memset(util, 0, (Ini->MAX_TAZS+1) * sizeof(struct d_vals));
//        memset(props, 0, (Ini->MAX_TAZS+1) * sizeof(struct d_vals));
//        }
//	util  = (double **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(double *));
//	props = (double **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(double *));
//	for (i=0; i < Ini->MAX_TAZS+1; i++) {
//		util[i]  = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 2*sizeof(double));
//		props[i] = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 2*sizeof(double));
//	}
//	if (Ini->RAM_DEBUG_LEVEL == 2)
//		addRAM ("motor_dc_props", 2*(Ini->MAX_TAZS+1)*2*sizeof(double));


// first calculate destination/access choice utilities
	denominator = 0.0;
	ibpm = ZonalData->bpmdist1_index[orig];
	ico = ZonalData->County[orig];
    gamma0 = DCcoeff->gamma[0];
    gamma1 = DCcoeff->gamma[1];
    gamma2 = DCcoeff->gamma[2];
	RiverData_east   = RiverData.east[ico];
	RiverData_hudson = RiverData.hudson[ico];
	RiverData_minor  = RiverData.minor[ico];
    BPMDist1_intra_county = BPMDist1.intra_county[ibpm];
    BPMDist1_co_co_coeffs = BPMDist1.co_co_coeffs[ibpm];

	for (j=1; j <= Ini->MAX_TAZS; j++) {
		if (z_attrs[2][purpose][j] > 0.0) {
			jbpm = ZonalData->bpmdist1_index[j];
			jco = ZonalData->County[j];
			if (purpose == 3)					// use intra-school district for school journeys, otherwise intra-county
				IntraCounty = (ZonalData->SchoolDist[orig] == ZonalData->SchoolDist[j]);
			else
				IntraCounty = (ZonalData->County[orig] == ZonalData->County[j]);
			if (z_attrs[0][purpose][j] > 0.0 || z_attrs[1][purpose][j] > 0.0) {
                delta_IntraCounty = DCcoeff->delta*(IntraCounty);
				gamma0River = gamma0*RiverData_east[jco];
				gamma1River = gamma1*RiverData_hudson[jco];
				gamma2River = gamma2*RiverData_minor[jco];
				if (ibpm == jbpm)
					util_val = BPMDist1_intra_county;	
				else
					util_val = BPMDist1_co_co_coeffs[jbpm];
				dist_scale = 0.0;
				if (hwydist[j] > 0.0) {
					dist_scale = DCcoeff->alpha*pow(hwydist[j],DCcoeff->theta);
					if (dist_scale > MAX_DISTANCE_SCALE)
						dist_scale = MAX_DISTANCE_SCALE;
					else if (dist_scale < -MAX_DISTANCE_SCALE)
						dist_scale = -MAX_DISTANCE_SCALE;
				}
			    for (m=0; m < 2; m++) {
				    if (z_attrs[m][purpose][j] > 0.0) {
					    util[j][m] = log(z_attrs[m][purpose][j])
						    + DCcoeff->beta*Logsum[j][m]
                            + gamma0River
                            + gamma1River
                            + gamma2River
						    + delta_IntraCounty
					        + util_val
    					    + dist_scale;
					    if (util[j][m] > MAX_EXP) {
						    util[j][m] = MAX_EXP;
						    denominator += exp(util[j][m]);
					    }
					    else if (util[j][m] > -MAX_EXP && util[j][m] < MAX_EXP) {
						    denominator += exp(util[j][m]);
					    }
				    }
				    else {
					    util[j][m] = MISSING;
				    }
			    }
			}
			else {
				util[j][0] = MISSING;
				util[j][1] = MISSING;
			}
		}
		else {
			util[j][0] = MISSING;
			util[j][1] = MISSING;
		}
	}



// calculate destination/access choice proportions

	if (debug_mode == 1) {

		fprintf (fp_rep, "writing debug output for orig=%d and purpose=%d to %s.\n", orig, purpose, Ini->DEBUGFILE );


		if (!AlreadyOpen)
			fp = fopen (Ini->DEBUGFILE, "w");
		else
			fp = fopen (Ini->DEBUGFILE, "a");


		fprintf (fp, "data dictionary\n\nPercent of journeys processed\norig\nj (dest)\npurpose\nm (walk acc)\nico (orig county)\njco (dest county)\nibpm (orig county with Manhattan sub-areas)\njbpm (dest county with Manhattan sub-areas)\nod distance\nprops[j][m] (proportions)\ncum[i] (cumulative proportions)\nutil[j][m]\nz_attrs[m][purpose][j]\nLogsum[j][m]\nRiverData.east[ico][jco]\nRiverData.hudson[ico][jco]\nRiverData.minor[ico][jco]\n(ico == jco)\n\n");
		for (k=0; k < Ini->NUMBER_ALTS - 2; k++)
			fprintf (fp, "%s od based utility\n", ModeLabel[k]);
		fprintf (fp, "\n");
		for (k=0; k < Ini->NUMBER_ALTS; k++)
			fprintf (fp, "%s se based utility\n", ModeLabel[k]);
		fprintf (fp, "\n");
		fprintf (fp, "logsum coeff                = %10.5f\n", DCcoeff->beta);
		fprintf (fp, "east river crossing coeff   = %10.5f\n", DCcoeff->gamma[0]);
		fprintf (fp, "hudson river crossing coeff = %10.5f\n", DCcoeff->gamma[1]);
		fprintf (fp, "minor river crossing coeff  = %10.5f\n", DCcoeff->gamma[2]);
		fprintf (fp, "intra county coeff          = %10.5f\n", DCcoeff->delta);
		fprintf (fp, "distance scaling coeff      = %10.5f\n", DCcoeff->alpha);
		fprintf (fp, "distance scaling exponent   = %10.5f\n", DCcoeff->theta);

		AlreadyOpen++;

	}

	
	i = 0;
	if (util[1][0] > -MAX_EXP && util[1][0] < MAX_EXP)
		props[1][0] = exp(util[1][0])/denominator;
	else
		props[1][0] = 0.0;
	cum[0] = props[1][0];

	j = 1;
	m = 0;
	ico = ZonalData->County[orig];
	jco = ZonalData->County[j];
	jbpm = ZonalData->bpmdist1_index[j];
	if (debug_mode == 1) {
		fprintf (fp, "%5d%% %5d %5d %3d %3d %3d %3d %3d %3d %16.8e %16.8e %16.8e %16.8e %16.8e %16.8e %16.8e %16.8e %3d %3d %3d %3d %3d %3d %3d",
			(AlreadyOpen - 1)*Ini->DEBUG_PERCENT, orig, j, purpose + 1, m, ico, jco, ibpm, jbpm, hwydist[j], props[j][m], cum[i], util[j][m], z_attrs[0][purpose][j], z_attrs[1][purpose][j], z_attrs[2][purpose][j], Logsum[j][m],
			RiverData.east[ico][jco], RiverData.hudson[ico][jco], RiverData.minor[ico][jco], (ico == jco),
			(ibpm == jbpm), (jbpm == 0), (jbpm == 1), (jbpm == 2), (jbpm == 3));
		for (k=0; k < Ini->NUMBER_ALTS - 2; k++) {
			if (Ini->ZERO_UTIL == 0)
				fprintf (fp, " %16.8e", ODutil[k][orig][j]);
			else
				fprintf (fp, " %16.8e", 0.0);
		}
		for (k=0; k < Ini->NUMBER_ALTS; k++)
			fprintf (fp, " %16.8e", SEutil[k]);
		fprintf (fp, "\n");
	}

	if (util[1][1] > -MAX_EXP && util[1][1] < MAX_EXP)
		props[1][1] = exp(util[1][1])/denominator;
	else
		props[1][1] = 0.0;
	cum[1] = cum[0] + props[1][1];

	i = 1;
	j = 1;
	m = 1;
	ico = ZonalData->County[orig];
	jco = ZonalData->County[j];
	jbpm = ZonalData->bpmdist1_index[j];
	if (debug_mode == 1) {
		fprintf (fp, "%5d%% %5d %5d %3d %3d %3d %3d %3d %3d %16.8e %16.8e %16.8e %16.8e %16.8e %16.8e %16.8e %16.8e %3d %3d %3d %3d %3d %3d %3d",
			(AlreadyOpen - 1)*Ini->DEBUG_PERCENT, orig, j, purpose + 1, m, ico, jco, ibpm, jbpm, hwydist[j], props[j][m], cum[i], util[j][m], z_attrs[0][purpose][j], z_attrs[1][purpose][j], z_attrs[2][purpose][j], Logsum[j][m],
			RiverData.east[ico][jco], RiverData.hudson[ico][jco], RiverData.minor[ico][jco], (ico == jco),
			(ibpm == jbpm), (jbpm == 0), (jbpm == 1), (jbpm == 2), (jbpm == 3));
		for (k=0; k < Ini->NUMBER_ALTS - 2; k++) {
			if (Ini->ZERO_UTIL == 0)
				fprintf (fp, " %16.8e", ODutil[k][orig][j]);
			else
				fprintf (fp, " %16.8e", 0.0);
		}
		for (k=0; k < Ini->NUMBER_ALTS; k++)
			fprintf (fp, " %16.8e", SEutil[k]);
		fprintf (fp, "\n");
	}

	i = 2;
	ico = ZonalData->County[orig];
	for (j=2; j <= Ini->MAX_TAZS; j++) {
		for (m=0; m < 2; m++) {
			if (util[j][m] > -MAX_EXP && util[j][m] < MAX_EXP)
				props[j][m] = exp(util[j][m])/denominator;
			else
				props[j][m] = 0.0;
			cum[i] = cum[i-1] + props[j][m];

			if (debug_mode == 1) {
        		jco = ZonalData->County[j];
        		jbpm = ZonalData->bpmdist1_index[j];
				fprintf (fp, "%5d%% %5d %5d %3d %3d %3d %3d %3d %3d %16.8e %16.8e %16.8e %16.8e %16.8e %16.8e %16.8e %16.8e %3d %3d %3d %3d %3d %3d %3d",
					(AlreadyOpen - 1)*Ini->DEBUG_PERCENT, orig, j, purpose + 1, m, ico, jco, ibpm, jbpm, hwydist[j], props[j][m], cum[i], util[j][m], z_attrs[0][purpose][j], z_attrs[1][purpose][j], z_attrs[2][purpose][j], Logsum[j][m],
					RiverData.east[ico][jco], RiverData.hudson[ico][jco], RiverData.minor[ico][jco], (ico == jco),
					(ibpm == jbpm), (jbpm == 0), (jbpm == 1), (jbpm == 2), (jbpm == 3));
				for (k=0; k < Ini->NUMBER_ALTS - 2; k++) {
					if (Ini->ZERO_UTIL == 0)
						fprintf (fp, " %16.8e", ODutil[k][orig][j]);
					else
						fprintf (fp, " %16.8e", 0.0);
				}
				for (k=0; k < Ini->NUMBER_ALTS; k++)
					fprintf (fp, " %16.8e", SEutil[k]);
				fprintf (fp, "\n");
			}

			i++;
		}
	}

//	for (i=0; i <= Ini->MAX_TAZS; i++) {
//		HeapFree (heapHandle, 0, util[i]);
//		HeapFree (heapHandle, 0, props[i]);
//	}
//	HeapFree (heapHandle, 0, $util);
//	HeapFree (heapHandle, 0, props);
//	if (Ini->RAM_DEBUG_LEVEL == 2)
//		relRAM ("motor_dc_props", 2*(Ini->MAX_TAZS+1)*2*sizeof(struct dvals));


	if (debug_mode == 1)
		fflush (fp);

}
