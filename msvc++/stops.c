/*
 * Determine whether stops were made on outbound and/or inbound legs of journey, and if so,
 * in which TAZs they were made.
 */

#include "md.h"

// stop frequency density gammas
#define WORK_LOWMED_OUTBOUND_AUTO_GAMMA	-0.8200
#define WORK_LOWMED_INBOUND_AUTO_GAMMA	-0.5123
#define WORK_LOWMED_OUTBOUND_TRAN_GAMMA	-1.5720
#define WORK_LOWMED_INBOUND_TRAN_GAMMA	-0.7868
#define WORK_HI_OUTBOUND_AUTO_GAMMA		-1.2800
#define WORK_HI_INBOUND_AUTO_GAMMA		-0.6161
#define WORK_HI_OUTBOUND_TRAN_GAMMA		-1.5720
#define WORK_HI_INBOUND_TRAN_GAMMA		-0.7868
#define SCHOOL_OUTBOUND_AUTO_GAMMA		-0.5422
#define SCHOOL_INBOUND_AUTO_GAMMA		-0.3978
#define SCHOOL_OUTBOUND_TRAN_GAMMA		-2.1610
#define SCHOOL_INBOUND_TRAN_GAMMA		-0.8911
#define UNIV_OUTBOUND_GAMMA				-0.6863
#define UNIV_INBOUND_GAMMA				-0.2814
#define MAINT_AUTO_GAMMA				-1.1210
#define MAINT_TRAN_GAMMA				-2.0560
#define DISCR_AUTO_GAMMA				-1.3380
#define DISCR_TRAN_GAMMA				-2.5170
#define ATWORK_AUTO_GAMMA				-1.3900
#define ATWORK_TRAN_GAMMA				-4.1160


void stops (FILE *fp, int *orig, int *dest, int *mode,	struct journey_attribs *JourneyAttribs, struct zone_data *ZonalData, struct river_crossing_data RiverData, struct district_definitions districtDefinitions)
{

	FILE *fp1, *fp2, *fp3, *fp4;

//	int i, j, k, n, p, old_p, packet_number, count, temp_len, freqChoice, obLocChoice, ibLocChoice;
	int i, j, k, n, p, old_p, packet_number, count, freqChoice, obLocChoice, ibLocChoice;
	int origin, destination, md, purp, obstop, ibstop;
	int *freqAvail, *locAvail;
	int *near_orig, *near_dest, *midway;
	int *ob_mode_freqs, *ib_mode_freqs, *obib_mode_freqs, *none_mode_freqs;
	int **ob_area_freqs, **ib_area_freqs, **obib_area_freqs, **none_area_freqs;
	int *p_sort, *p_start, *temp_start, *packet;
	int *hh2id;
	int *ob_district_freqs, *ib_district_freqs;
	int *frozen;
	short *hh_freqs, **hh_pers_freqs;
	float **hwy_dist_util, **hwy_dist, **hwy_time, **wt_ivt, **dt_ivt, **wc_ivt, **dc_ivt;
	double ivtt;
    double *ob_stop_size, *ib_stop_size;
	double obDensity, ibDensity;
	double gamma_o, gamma_i, nrand, denom;
	double *freqUtil, *freqProp, *freqCum;
	double *RelDistDev, *locUtil, *locProp, *locCum;
	double *ob_district_size, *ib_district_size;
//	char temp[128];

	printf ("calculating stop frequency and locations...\n");

	// allocate memory for the stop frequency choice arrays
	freqCum   = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->FREQ_ALTS*sizeof(double));
	freqUtil  = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->FREQ_ALTS*sizeof(double));
	freqProp  = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->FREQ_ALTS*sizeof(double));
	freqAvail = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->FREQ_ALTS*sizeof(int));

	// allocate memory for the stop location choice arrays
	locCum    = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(double));
	locUtil   = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(double));
	locProp   = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(double));
	locAvail  = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(int));

	// allocate memory for the available stop TAZs arrays
	near_orig = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(int));
	near_dest = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(int));
	midway    = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(int));
	RelDistDev = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(double));

	ob_mode_freqs = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_ALTS+1)*sizeof(int));
	ib_mode_freqs = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_ALTS+1)*sizeof(int));
	obib_mode_freqs = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_ALTS+1)*sizeof(int));
	none_mode_freqs = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_ALTS+1)*sizeof(int));

	ob_district_freqs = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (districtDefinitions.countyExtendedDefinitions->numValues+1)*sizeof(int));
	ib_district_freqs = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (districtDefinitions.countyExtendedDefinitions->numValues+1)*sizeof(int));
	ob_district_size = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (districtDefinitions.countyExtendedDefinitions->numValues+1)*sizeof(double));
	ib_district_size = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (districtDefinitions.countyExtendedDefinitions->numValues+1)*sizeof(double));

	ob_area_freqs = (int **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (4+1)*sizeof(int *));
	ib_area_freqs = (int **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (4+1)*sizeof(int *));
	obib_area_freqs = (int **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (4+1)*sizeof(int *));
	none_area_freqs = (int **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (4+1)*sizeof(int *));
	for (i=0; i < 4+1; i++) {
		ob_area_freqs[i] = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (4+1)*sizeof(int));
		ib_area_freqs[i] = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (4+1)*sizeof(int));
		obib_area_freqs[i] = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (4+1)*sizeof(int));
		none_area_freqs[i] = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (4+1)*sizeof(int));
	}

	frozen = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->ORIGINAL_JOURNEYS)*sizeof(int));
	
//	addRAM ("stops", 4*Ini->FREQ_ALTS*sizeof(double)
//					+ 4*(Ini->MAX_TAZS+1)*sizeof(double)
//					+ 4*(Ini->MAX_TAZS+1)*sizeof(int)
//					+ 4*(Ini->NUMBER_ALTS+1)*sizeof(int)
//					+ 2*(Ini->NUMBER_BPMDIST1+1)*sizeof(int)
//					+ 2*(Ini->NUMBER_BPMDIST1+1)*sizeof(double)
//					+ 4*5*5*sizeof(int)
//					+ (Ini->ORIGINAL_JOURNEYS)*sizeof(int));


	for (k=0; k < Ini->ORIGINAL_JOURNEYS; k++)
		frozen[k] = 1;



	// summarize journey frequencies needed for the relevant model
	switch (Ini->PURPOSE_TO_PROCESS) {
		case 0:
		case 1:
		case 2:
			work_jrny_freqs (JourneyAttribs, &hh_freqs, &hh_pers_freqs, &hh2id);
			break;
		case 3:
			school_jrny_freqs (JourneyAttribs, &hh_freqs, &hh_pers_freqs, &hh2id);
			break;
		case 4:
			univ_jrny_freqs (JourneyAttribs, &hh_freqs, &hh_pers_freqs, &hh2id);
			break;
		case 5:
			maint_jrny_freqs (JourneyAttribs, &hh_pers_freqs, &hh2id);
			break;
		case 6:
			discr_jrny_freqs (JourneyAttribs, &hh_pers_freqs, &hh2id);
			break;
		case 7:
			break;
	}



// read highway distance and WT, DT, WC, an DC IVT od skim mtrices
	read_od_skims (&hwy_dist_util, &hwy_dist, &hwy_time, &wt_ivt, &dt_ivt, &wc_ivt, &dc_ivt, ZonalData);


// calculate size variables for stop density calculation
	get_stop_size (&ob_stop_size, &ib_stop_size, ZonalData);


// open MDSC output journeys file, the file sent to David Rhoden, for writing journey orig, dest, purpose, mode, ob_stop, ib_stop
	if (Ini->SUBAREA_ANALYSIS != 1) {
		if (strcmp(Ini->JOURNEYS_WITH_STOPS, "")) {
			if ((fp = fopen(Ini->JOURNEYS_WITH_STOPS, "wt")) == NULL)
				ExitWithCode(116);
		}
		else
			ExitWithCode (117);

	}


// open output files for subarea analysis
	if (Ini->SUBAREA_ANALYSIS == 1) {

	// optionally open file for writing full set of output journeys during Sub-Area analysis.
	// if no file was specified in .ini control file, full set of output is not written.
		if (strcmp(Ini->SUBAREA_MDSC_OUTPUT_FULL, "")) {
			if ((fp1 = fopen(Ini->SUBAREA_MDSC_OUTPUT_FULL, "wt")) == NULL)
				ExitWithCode(123);
		}


	// optionally open file for writing frozen set of output journeys during Sub-Area analysis.
	// if no file was specified in .ini control file, frozen set of output is not written.
		if (strcmp(Ini->SUBAREA_MDSC_OUTPUT_FROZEN, "")) {
			if ((fp2 = fopen(Ini->SUBAREA_MDSC_OUTPUT_FROZEN, "wt")) == NULL)
				ExitWithCode(124);
		}


	// Open file for writing unfrozen set of output journeys during Sub-Area analysis.  Required.
		if (strcmp(Ini->SUBAREA_MDSC_OUTPUT_UNFROZEN, "")) {
			if ((fp3 = fopen(Ini->SUBAREA_MDSC_OUTPUT_UNFROZEN, "wt")) == NULL)
				ExitWithCode(125);
		}
		else {
				ExitWithCode(126);
		}
	}


	
// memory allocations for clustering
	packet = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(int));
	temp_start = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_JOURNEYS+1)*sizeof(int));
	p_sort = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(int));
//	addRAM ("stops", (3*Ini->NUMBER_JOURNEYS + 1)*sizeof(int));



// assign cluster codes to journeys
	for (i=0; i < Ini->NUMBER_JOURNEYS; i++) {
		packet[i] = set_stops_cluster (orig[i], dest[i], mode[i]);
		p_sort[i] = i;
	}


// create a set of indices for journeys sorted by packet number
	isort (packet, p_sort, Ini->NUMBER_JOURNEYS);


// go through the journeys, count cluster frequency, and assign packet numbers, keep track of start journey of packets
	p = packet[p_sort[0]];
	old_p = p;
	temp_start[0] = 0;
	packet_number = 1;
	count = 1;
	for (i=1; i < Ini->NUMBER_JOURNEYS; i++) {

		p = packet[p_sort[i]];
		if (p != old_p) {
			// set the journey number of the start of the next packet in temp_start.
			temp_start[packet_number] = temp_start[packet_number - 1] + count;
			packet_number++;
			count = 1;
			old_p = p;
		}
		else {
			count++;
		}
	}
	temp_start[packet_number] = temp_start[packet_number - 1] + count;


// allocate correct amount of memory for the packets determined and store start of packet locations
	p_start = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (packet_number+1)*sizeof(int));
	for (i=0; i <= packet_number; i++)
		p_start[i] = temp_start[i];
//	addRAM ("stops", (packet_number+1)*sizeof(int));


	HeapFree (heapHandle, 0, packet);
	HeapFree (heapHandle, 0, temp_start);
//	relRAM ("stops", (2*Ini->NUMBER_JOURNEYS + 1)*sizeof(int));






	for (p=0; p < packet_number; p++) {



		// orig, dest, and mode are the same for all journeys in this packet.  Use the first to get a journey index.
		i = p_sort[p_start[p]];


		obDensity = 0.0;
		ibDensity = 0.0;

		// skip non-motorized and school bus for all journeys, and CR for non-work journeys
		if ((Ini->PURPOSE_TO_PROCESS < 3 && mode[i] <= 9) || (Ini->PURPOSE_TO_PROCESS >= 3 && (mode[i] <= 9 && mode[i] != 7 && mode[i] != 8))) {


			// get availability of stop TAZs for this O/D pair.
			stop_location_avail (orig[i], dest[i], mode[i], ZonalData, hwy_dist_util, wt_ivt, dt_ivt, wc_ivt, dc_ivt, near_orig, near_dest, midway);



			// determine which gamma to use
			if ((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) {			// auto based journeys
				switch (Ini->PURPOSE_TO_PROCESS) {
					case 0:
						gamma_o = WORK_LOWMED_OUTBOUND_AUTO_GAMMA;
						gamma_i = WORK_LOWMED_INBOUND_AUTO_GAMMA;
						break;
					case 1:
						gamma_o = WORK_LOWMED_OUTBOUND_AUTO_GAMMA;
						gamma_i = WORK_LOWMED_INBOUND_AUTO_GAMMA;
						break;
					case 2:
						gamma_o = WORK_HI_OUTBOUND_AUTO_GAMMA;
						gamma_i = WORK_HI_INBOUND_AUTO_GAMMA;
						break;
					case 3:
						gamma_o = SCHOOL_OUTBOUND_AUTO_GAMMA;
						gamma_i = SCHOOL_INBOUND_AUTO_GAMMA;
						break;
					case 4:
						gamma_o = UNIV_OUTBOUND_GAMMA;
						gamma_i = UNIV_INBOUND_GAMMA;
						break;
					case 5:
						gamma_o = MAINT_AUTO_GAMMA;
						gamma_i = MAINT_AUTO_GAMMA;
						break;
					case 6:
						gamma_o = DISCR_AUTO_GAMMA;
						gamma_i = DISCR_AUTO_GAMMA;
						break;
					case 7:
						gamma_o = ATWORK_AUTO_GAMMA;
						gamma_i = ATWORK_AUTO_GAMMA;
						break;
				}
			}
			else if ((mode[i] >= 5 && mode[i] <= 8))				{		// transit based journeys
				switch (Ini->PURPOSE_TO_PROCESS) {
					case 0:
						gamma_o = WORK_LOWMED_OUTBOUND_TRAN_GAMMA;
						gamma_i = WORK_LOWMED_INBOUND_TRAN_GAMMA;
						break;
					case 1:
						gamma_o = WORK_LOWMED_OUTBOUND_TRAN_GAMMA;
						gamma_i = WORK_LOWMED_INBOUND_TRAN_GAMMA;
						break;
					case 2:
						gamma_o = WORK_HI_OUTBOUND_TRAN_GAMMA;
						gamma_i = WORK_HI_INBOUND_TRAN_GAMMA;
						break;
					case 3:
						gamma_o = SCHOOL_OUTBOUND_TRAN_GAMMA;
						gamma_i = SCHOOL_INBOUND_TRAN_GAMMA;
						break;
					case 4:
						gamma_o = UNIV_OUTBOUND_GAMMA;
						gamma_i = UNIV_INBOUND_GAMMA;
						break;
					case 5:
						gamma_o = MAINT_TRAN_GAMMA;
						gamma_i = MAINT_TRAN_GAMMA;
						break;
					case 6:
						gamma_o = DISCR_TRAN_GAMMA;
						gamma_i = DISCR_TRAN_GAMMA;
						break;
					case 7:
						gamma_o = ATWORK_TRAN_GAMMA;
						gamma_i = ATWORK_TRAN_GAMMA;
						break;
				}
			}
			else {
				continue;
			}




			// calculate inbound and outbound densities
			obDensity = 0.0;
			ibDensity = 0.0;
			for (k=1; k <= Ini->MAX_TAZS; k++) {
				// only consider TAZs within the circles and the elliptical area as possible stop locations
				RelDistDev[k] = -1.0;
				if (near_orig[k] == 1 || near_dest[k] == 1 || midway[k] == 1) {

					// calculate relative distance deviations for all stops for this o/d combination
					RelDistDev[k] = ((hwy_dist_util[orig[i]][k] + hwy_dist_util[k][dest[i]])/hwy_dist_util[orig[i]][dest[i]]) - 1.0;
					if (RelDistDev[k] < 0.0)
						RelDistDev[k] = 0.0;
					else if (RelDistDev[k] > 9.0)
						RelDistDev[k] = 9.0;

					obDensity += ob_stop_size[k]*exp(gamma_o*RelDistDev[k]);
					ibDensity += ib_stop_size[k]*exp(gamma_i*RelDistDev[k]);
				}
			}

		}


		// calculate stop frequency choice utilities individually for each journey within this packet 
		for (j=p_start[p]; j < p_start[p+1]; j++) {

			// i is the actual journey index
			i = p_sort[j];

			frozen[i] = 0;
			ivtt=0;
			
			if ((Ini->PURPOSE_TO_PROCESS < 3 && mode[i] <= 9) || (Ini->PURPOSE_TO_PROCESS >= 3 && (mode[i] <= 9 && mode[i] != 7 && mode[i] != 8))) {

				if (mode[i] == 5)
					ivtt = wt_ivt[orig[i]][dest[i]];
				else if (mode[i] == 6)
					ivtt = dt_ivt[orig[i]][dest[i]];
				else if (mode[i] == 7)
					ivtt = wc_ivt[orig[i]][dest[i]];
				else if (mode[i] == 8)
					ivtt = dc_ivt[orig[i]][dest[i]];

				// calculate stop frequency choice utilities
				switch (Ini->PURPOSE_TO_PROCESS) {
					case 0:
					case 1:
					case 2:
						work_freq_util (i, obDensity, ibDensity, ivtt, orig, dest, mode, JourneyAttribs, ZonalData,
							freqUtil, freqAvail, hh_freqs, hh_pers_freqs, hh2id);
						break;
					case 3:
						school_freq_util (i, obDensity, ibDensity, (double)hwy_dist[orig[i]][dest[i]], orig, dest, mode,
							JourneyAttribs, ZonalData, freqUtil, freqAvail, hh_freqs, hh_pers_freqs, hh2id);
						break;
					case 4:
						univ_freq_util (i, obDensity, ibDensity, orig, dest, mode, JourneyAttribs, ZonalData,
							freqUtil, freqAvail, hh_freqs, hh_pers_freqs, hh2id);
						break;
					case 5:
						maint_freq_util (i, obDensity, ibDensity, ivtt, (double)hwy_dist[orig[i]][dest[i]],
							orig, dest, mode, JourneyAttribs, ZonalData, freqUtil, freqAvail, hh_pers_freqs, hh2id);
						break;
					case 6:
						discr_freq_util (i, obDensity, ibDensity, orig, dest, mode, JourneyAttribs, ZonalData,
							freqUtil, freqAvail, hh_pers_freqs, hh2id);
						break;
					case 7:
						atwork_freq_util (i, obDensity, ibDensity, orig, dest, mode, JourneyAttribs,
							freqUtil, freqAvail);
						break;
				}


				// calculate stop frequency choice proportions individually for each journey within this packet
				denom = 0.0;
				for (n=0; n < Ini->FREQ_ALTS; n++)
					if (freqAvail[n] > 0)
						denom += exp(freqUtil[n]);

				for (n=0; n < Ini->FREQ_ALTS; n++) {
					if (freqAvail[n] > 0)
						freqProp[n] = exp(freqUtil[n])/denom;
					else
						freqProp[n] = 0.0;
				}



				// now select a stop frequency choice at random for each journey in the packet
				for (n=0; n < Ini->FREQ_ALTS; n++)
					freqCum[n] = 0.0;

				freqCum[0] = freqProp[0];
				for (n=1; n < Ini->FREQ_ALTS; n++)
					freqCum[n] = freqCum[n-1] + freqProp[n];
				freqCum[Ini->FREQ_ALTS - 1] = 1.0;

				nrand = (double)rand()/(double)MAX_RANDOM;
				for (n=0; n < Ini->FREQ_ALTS; n++) {
					if (nrand < freqCum[n]) {
						freqChoice = n;
						break;
					}
				}



				// stop location choice for each journey in the packet
				obLocChoice = 0;
				ibLocChoice = 0;

				if (freqChoice == 1) {

					// calculate outbound stop location choice utilities
					denom = 0.0;
					for (k=1; k <= Ini->MAX_TAZS; k++) {
						locAvail[k] = 0;
						if (near_orig[k] == 1 || near_dest[k] == 1 || midway[k] == 1) {
							locAvail[k] = 1;

							if (ob_stop_size[k] > 0.0) {
								locUtil[k] = log(ob_stop_size[k]) + gamma_o*RelDistDev[k];
								denom += exp(locUtil[k]);
							}
							else {
								locAvail[k] = 0;
							}
						}
					}


					// calculate outbound stop location choice proportions
					for (k=1; k <= Ini->MAX_TAZS; k++) {
						if (locAvail[k] > 0)
							locProp[k] = exp(locUtil[k])/denom;
						else
							locProp[k] = 0.0;
					}


					// now select an outbound stop location at random
					for (k=1; k <= Ini->MAX_TAZS; k++)
						locCum[k] = 0.0;
	
					locCum[1] = locProp[1];
					for (k=2; k <= Ini->MAX_TAZS; k++)
						locCum[k] = locCum[k-1] + locProp[k];
					locCum[Ini->MAX_TAZS] = 1.0;
	
					nrand = (double)rand()/(double)MAX_RANDOM;
					for (k=0; k < Ini->MAX_TAZS; k++) {
						if (nrand < locCum[k]) {
							obLocChoice = k;
							ibLocChoice = 0;
							break;
						}
					}
	
				}
				else if (freqChoice == 2) {

					// calculate inbound stop location choice utilities
					denom = 0.0;
					for (k=1; k <= Ini->MAX_TAZS; k++) {
						locAvail[k] = 0;
						if (near_orig[k] == 1 || near_dest[k] == 1 || midway[k] == 1) {
							locAvail[k] = 1;

							if (ib_stop_size[k] > 0.0) {
								locUtil[k] = log(ib_stop_size[k]) + gamma_i*RelDistDev[k];
								denom += exp(locUtil[k]);
							}
							else {
								locAvail[k] = 0;
							}
						}
					}


					// calculate inbound stop location choice proportions
					for (k=1; k <= Ini->MAX_TAZS; k++) {
						if (locAvail[k] > 0)
							locProp[k] = exp(locUtil[k])/denom;
						else
							locProp[k] = 0.0;
					}


					// now select an inbound stop location at random
					for (k=1; k <= Ini->MAX_TAZS; k++)
						locCum[k] = 0.0;
	
					locCum[1] = locProp[1];
					for (k=2; k <= Ini->MAX_TAZS; k++)
						locCum[k] = locCum[k-1] + locProp[k];
					locCum[Ini->MAX_TAZS] = 1.0;
	
					nrand = (double)rand()/(double)MAX_RANDOM;
					for (k=0; k < Ini->MAX_TAZS; k++) {
						if (nrand < locCum[k]) {
							obLocChoice = 0;
							ibLocChoice = k;
							break;
						}
					}
			
				}
				else if (freqChoice == 3) {
	
					// calculate outbound stop location choice utilities
					denom = 0.0;
					for (k=1; k <= Ini->MAX_TAZS; k++) {
						locAvail[k] = 0;
						if (near_orig[k] == 1 || near_dest[k] == 1 || midway[k] == 1) {
							locAvail[k] = 1;

							if (ob_stop_size[k] > 0.0) {
								locUtil[k] = log(ob_stop_size[k]) + gamma_o*RelDistDev[k];
								denom += exp(locUtil[k]);
							}
							else {
								locAvail[k] = 0;
							}
						}
					}
	

					// calculate outbound stop location choice proportions
					for (k=1; k <= Ini->MAX_TAZS; k++) {
						if (locAvail[k] > 0)
							locProp[k] = exp(locUtil[k])/denom;
						else
							locProp[k] = 0.0;
					}


					// now select an outbound stop location at random
					for (k=1; k <= Ini->MAX_TAZS; k++)
						locCum[k] = 0.0;
		
					locCum[1] = locProp[1];
					for (k=2; k <= Ini->MAX_TAZS; k++)
						locCum[k] = locCum[k-1] + locProp[k];
					locCum[Ini->MAX_TAZS] = 1.0;

					nrand = (double)rand()/(double)MAX_RANDOM;
					for (k=0; k < Ini->MAX_TAZS; k++) {
						if (nrand < locCum[k]) {
							obLocChoice = k;
							break;
						}
					}

			
					// calculate inbound stop location choice utilities
					denom = 0.0;
					for (k=1; k <= Ini->MAX_TAZS; k++) {
						locAvail[k] = 0;
						if (near_orig[k] == 1 || near_dest[k] == 1 || midway[k] == 1) {
							locAvail[k] = 1;

							if (ib_stop_size[k] > 0.0) {
								locUtil[k] = log(ib_stop_size[k]) + gamma_i*RelDistDev[k];
								denom += exp(locUtil[k]);
							}
							else {
								locAvail[k] = 0;
							}
						}
					}


					// calculate inbound stop location choice proportions
					for (k=1; k <= Ini->MAX_TAZS; k++) {
						if (locAvail[k] > 0)
							locProp[k] = exp(locUtil[k])/denom;
						else
							locProp[k] = 0.0;
					}


					// now select an inbound stop location at random
					for (k=1; k <= Ini->MAX_TAZS; k++)
						locCum[k] = 0.0;
	
					locCum[1] = locProp[1];
					for (k=2; k <= Ini->MAX_TAZS; k++)
						locCum[k] = locCum[k-1] + locProp[k];
					locCum[Ini->MAX_TAZS] = 1.0;

					nrand = (double)rand()/(double)MAX_RANDOM;
					for (k=0; k < Ini->MAX_TAZS; k++) {
						if (nrand < locCum[k]) {
							ibLocChoice = k;
							break;
						}
					}
			
				}

				// for DT and DC, set stop TAZ negative if closer to dest than orig
				if (mode[i] == 6 || mode[i] == 8) {
					if (obLocChoice > 0 && (hwy_dist_util[orig[i]][obLocChoice] > hwy_dist_util[obLocChoice][dest[i]]))
						obLocChoice *= -1;
					if (ibLocChoice > 0 && (hwy_dist_util[orig[i]][ibLocChoice] > hwy_dist_util[ibLocChoice][dest[i]]))
						ibLocChoice *= -1;
				}

			}
			else {
				// stop TAZs for non-motorized journeys are 0.
				obLocChoice = 0;
				ibLocChoice = 0;
			}



			// write MDSC output
			if (Ini->SUBAREA_ANALYSIS != 1) {

				fprintf (fp, "%d %d %d %d %d %d %d\n", i, orig[i], dest[i], Ini->PURPOSE_TO_PROCESS + 1, mode[i], obLocChoice, ibLocChoice);

			}
			else {

				if (strcmp(Ini->SUBAREA_MDSC_OUTPUT_FULL, ""))
					fprintf (fp1, "%d %d %d %d %d %d %d\n", i, orig[i], dest[i], Ini->PURPOSE_TO_PROCESS + 1, mode[i], obLocChoice, ibLocChoice);

				fprintf (fp3, "%d %d %d %d %d %d %d\n", i, orig[i], dest[i], Ini->PURPOSE_TO_PROCESS + 1, mode[i], obLocChoice, ibLocChoice);
			}




			// summarize for reporting purposes
			if (obLocChoice != 0 && ibLocChoice == 0) {
				ob_mode_freqs[mode[i]] ++;
				ob_area_freqs[ZonalData->UrbanType[orig[i]]][ZonalData->UrbanType[dest[i]]] ++;
				ob_district_freqs[ZonalData->county_extended_index[abs(obLocChoice)]] ++;
			}
			else if (obLocChoice == 0 && ibLocChoice != 0) {
				ib_mode_freqs[mode[i]] ++;
				ib_area_freqs[ZonalData->UrbanType[orig[i]]][ZonalData->UrbanType[dest[i]]] ++;
				ib_district_freqs[ZonalData->county_extended_index[abs(ibLocChoice)]] ++;
			}
			else if (obLocChoice != 0 && ibLocChoice != 0) {
				obib_mode_freqs[mode[i]] ++;
				obib_area_freqs[ZonalData->UrbanType[orig[i]]][ZonalData->UrbanType[dest[i]]] ++;
				ob_district_freqs[ZonalData->county_extended_index[abs(obLocChoice)]] ++;
				ib_district_freqs[ZonalData->county_extended_index[abs(ibLocChoice)]] ++;
			}
			else if (obLocChoice == 0 && ibLocChoice == 0) {
				none_mode_freqs[mode[i]] ++;
				none_area_freqs[ZonalData->UrbanType[orig[i]]][ZonalData->UrbanType[dest[i]]] ++;
			}

		}

	
//		sprintf (temp, "calculating stop frequency and locations %3d%% complete", 100*p/packet_number);
//		temp_len = (int)strlen(temp);
//		for (n=0; n < temp_len; n++)
//			sprintf (&temp[temp_len + n], "\b");
//		printf ("%s", temp);

	}
	printf ("calculating stop frequency and locations complete\n");




	// read in the original MDSC output file to write out frozen journeys
	if (Ini->SUBAREA_ANALYSIS == 1) {
	
		if (strcmp(Ini->SUBAREA_MDSC_OUTPUT_FULL, "") || strcmp(Ini->SUBAREA_MDSC_OUTPUT_FROZEN, "")) {

			// read MDSC journeys
			if (strcmp(Ini->MDSCOUTPUTJOURNEYS, "")) {
				if ((fp4 = fopen(Ini->MDSCOUTPUTJOURNEYS, "rt")) == NULL)
					ExitWithCode(121);
			}
			else
				ExitWithCode (122);

			i = 0;
			while ((fscanf(fp4, "%d %d %d %d %d %d %d", &k, &origin, &destination, &purp, &md, &obstop, &ibstop)) != EOF) {
				if (strcmp(Ini->SUBAREA_MDSC_OUTPUT_FULL, "")) {
					if (frozen[k] == 1)
						fprintf (fp1, "%d %d %d %d %d %d %d\n", k, origin, destination, purp, md, obstop, ibstop);
				}

				if (strcmp(Ini->SUBAREA_MDSC_OUTPUT_FROZEN, "")) {
					if (frozen[k] == 1)
						fprintf (fp2, "%d %d %d %d %d %d %d\n", k, origin, destination, purp, md, obstop, ibstop);
				}
			}
			fclose (fp4);
		}
	}



	if (Ini->SUBAREA_ANALYSIS != 1) {
		fclose (fp);
	}
	else {
		if (strcmp(Ini->SUBAREA_MDSC_OUTPUT_FULL, ""))
			fclose (fp1);

		if (strcmp(Ini->SUBAREA_MDSC_OUTPUT_FROZEN, ""))
			fclose (fp2);

		fclose (fp3);
	}




	// calculate inbound and outbound size by BPMDIST1
	for (k=1; k <= Ini->MAX_TAZS; k++) {
		ob_district_size[ZonalData->county_extended_index[k]] += ob_stop_size[k];
		ib_district_size[ZonalData->county_extended_index[k]] += ib_stop_size[k];
	}


	// print the stop freq and stop loc summary reports
	printf ("printing stop freq and stop loc summary reports.\n");
	stops_reports (ob_mode_freqs, ib_mode_freqs, obib_mode_freqs, none_mode_freqs,
		ob_area_freqs, ib_area_freqs, obib_area_freqs, none_area_freqs,
		ob_district_freqs, ib_district_freqs, ob_district_size, ib_district_size, districtDefinitions);




	HeapFree (heapHandle, 0, near_orig);
	HeapFree (heapHandle, 0, near_dest);
	HeapFree (heapHandle, 0, midway);
	HeapFree (heapHandle, 0, RelDistDev);

	HeapFree (heapHandle, 0, freqCum);
	HeapFree (heapHandle, 0, freqUtil);
	HeapFree (heapHandle, 0, freqProp);
	HeapFree (heapHandle, 0, freqAvail);

	HeapFree (heapHandle, 0, locCum);
	HeapFree (heapHandle, 0, locUtil);
	HeapFree (heapHandle, 0, locProp);
	HeapFree (heapHandle, 0, locAvail);

	HeapFree (heapHandle, 0, ob_mode_freqs);
	HeapFree (heapHandle, 0, ib_mode_freqs);
	HeapFree (heapHandle, 0, obib_mode_freqs);
	HeapFree (heapHandle, 0, none_mode_freqs);

	for (i=0; i < 4+1; i++) {
		HeapFree (heapHandle, 0, ob_area_freqs[i]);
		HeapFree (heapHandle, 0, ib_area_freqs[i]);
		HeapFree (heapHandle, 0, obib_area_freqs[i]);
		HeapFree (heapHandle, 0, none_area_freqs[i]);
	}
	HeapFree (heapHandle, 0, ob_area_freqs);
	HeapFree (heapHandle, 0, ib_area_freqs);
	HeapFree (heapHandle, 0, obib_area_freqs);
	HeapFree (heapHandle, 0, none_area_freqs);

	HeapFree (heapHandle, 0, ob_district_freqs);
	HeapFree (heapHandle, 0, ib_district_freqs);
	HeapFree (heapHandle, 0, ob_district_size);
	HeapFree (heapHandle, 0, ib_district_size);


//	relRAM ("stops", 4*Ini->FREQ_ALTS*sizeof(double)
//					+ 4*(Ini->MAX_TAZS+1)*sizeof(double)
//					+ 4*(Ini->MAX_TAZS+1)*sizeof(int)
//					+ 4*(Ini->NUMBER_ALTS+1)*sizeof(int)
//					+ 2*(Ini->NUMBER_BPMDIST1+1)*sizeof(int)
//					+ 2*(Ini->NUMBER_BPMDIST1+1)*sizeof(double)
//					+ 4*5*5*sizeof(int));

	HeapFree (heapHandle, 0, p_sort);
	HeapFree (heapHandle, 0, p_start);
//	relRAM ("stops", (packet_number+1)*sizeof(int) + Ini->NUMBER_JOURNEYS*sizeof(int));
}
