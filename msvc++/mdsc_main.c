#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md.h"
#include "Globals.h"


clock_t start, finish;
int MAX_RANDOM = RAND_MAX+1;
int Calibrating = 0;
int max_hh_key, max_pers_key;
FILE *fp_rep;
FILE *fp_ram;

/*
MDC version history:

1.0.0 - 03april2001 - follows fixes to school non-motorized logsum calculations (version 0.9.4)
1.0.1 - 06april2001 - rewind fp_work[0], fp_work[1], and fp_work[2] in run_mdc().
1.0.2 - 11april2001 - fix for 1.0.1 had typo; corrected rewind fp_work[0], fp_work[1], and fp_work[2] in run_mdc().
					- computing NMTots[] needed to be done after run_mdc() (both places) where atwork linkages are made.

1.1.0 - 31may2001	- replacing "to Manhattan" K-factors (31*4 + intra) with full set of
					  district based K-factors (31*31 + intra).  Input file for county - county coefficients
					  changed from fixed format for 31*4 and is now comma delimited for 31*31.
1.1.1 - 04jun2001	- new method for incorporating MSCs and auto calibration using lookup table of MSC indices
					  read as comma delimited file.
1.1.2 - 11jun2001	- fixed bug in reporting non-motorized MSCs and obs/est shares and relative errors.
1.1.3 - 13jun2001	- fixed bug in calibration_report() that caused access violation.
1.1.4 - 02jul2001	- fixed bug in dimensioning MSC/Targets/Indices/Labels arrays for non-motorized and motorized.
1.1.5 - 12jul2001	- fixed bug (<= instead of <) in maximum loop index for computing o/d utilities in OD_Utilities.c.
1.1.6 - 20jul2001	- allow for reading TransCAD core 12, AUTOTIME, to be combined with core 9, ACCOVT.
1.1.7 - 14aug2001	- changed nrows and ncols to MAX_TAZS in modal utility calcualtions.
1.1.8 - 14aug2001	- changed nrows and ncols to MAX_TAZS in intrazonal skims calculations
1.1.9 - 14aug2001	- changed nrows and ncols to MAX_TAZS in copying to hwy_dist[][].
					  fixed bug where DC skims were copied instead of WC skims for informal park and ride.


MDSC version history:

1.0.0  - 03oct2001	- starting with code from MDC version 1.1.9 and fragments from StopFreqEstFile and StopLocEstFile
					  project workspaces.
1.0.1  - 19oct2001	- add packeting to stop frequency density calculations to reduce envelope building time.
1.0.2  - 25oct2001	- build in final estimated discretionary stop frequency model.
1.0.3  - 29oct2001	- set stop location negative for DT & DC if dest closer than orig.
					  corrected purpose too low by 1 on stops output file.
					  included all journeys on stops output file.
1.0.3.730 - 29oct2001	- divided density coecfficients by 730 as test for Eric.
1.0.4.730 - 29oct2001	- added reporting on stop frequency and stop location.
1.0.5  - 30oct2001	- correct reporting problem; revert to originally estimated density coefficients.
1.0.6  - 31oct2001	- correct reporting format problems; change incorrect purpose index in stop freq size variable calculations.
1.0.7  - 01nov2001	- corrected bug in reading stop freq choice specifc constants from ini file; fixed district size variable reporting problem.
1.0.8  - 01nov2001	- corrected indexing bugs in discr_journey_freqs() and discr_freq_util().  Enabled work stop freq/loc models.
1.0.9  - 02nov2001	- changed stop location exists criteria from > 0 to != 0 for report tabulations.
1.0.10 - 02nov2001	- modified stop reports; added control variable to allow naming output journeys with stops file;
					  corrected work stop freq utility where auto sufficiency is used for auto modes only.

1.1.0  - 05nov2001	- enabled remaining purposes.  Corrections to utility definitions and hh and person journey frequencies.
1.1.1  - 07nov2001	- added memory use logging.  Fixed some memory allocation problems.
1.1.2  - 21nov2001	- revised memory use logging.
					- additional dist scaled utility added to hwy_dist for stops model to address river crossing for stops.
1.1.3  - 26nov2001	- rearranged memory management to facilitate model for STOPS_ONLY mode.


3.0.0  - 10dec2001  - based on version 1.1.3.  Use restricted set of stop locations.
					- Use private heaps.
3.0.1  - 11dec2001  - changed ReadODSkims() to read TransCAD matrices
3.0.2  - 12dec2001  - added STOPS_ONLY condition to relaeasing OD_Utility matrices.
					- took out SUMMARYDATA reporting
3.0.3  - 12dec2001  - added check that od_heap handle was created ok.
3.0.4  - 12dec2001  - added more reporting to screen to try to diagnose problem of running this in NY.
3.0.5  - 12dec2001  - more reporting to screen.
3.0.6  - 12dec2001  - changed nrows, ncols to Ini->MAX_TAZS in do loops in ReadODSkims().
					  current set of skims (used in NY) have 3586 internal TAZs and 3697 total (with labels to 3811).



MDSC with sub-area analysis version history:

4.0.0  - 06feb2002	- based on MDSC version 3.0.7.  Add sub-area analysis capability.
					- fixed bug in indexing MDC output records read in for STOPS processing.
4.0.1  - 13feb2002  - allowed additional separate file names in .ini file for Subarea analysis.
					  put in an option controlled by .ini parameter to allow user to select which
					  sequence number is written by MDC (correct or not).
					  added frequency table of journey origins and destinations by subarea.
4.0.2  - 20feb2002  - revised definition of subareas to account for outbound and inbound portions of journeys
					  added optional output file reporting subarea codes defined for each TAZ.
4.0.3  - 26feb2002  - added more error reporting to report file regarding synchronizing journey records
					  for subarea analysis.  Added reporting no. hhs and no. journeys read from HAJ file.
4.0.4  - 28feb2002  - revised linking of work destinations with at-work journeys.  The linking is now done at
					  the time at-work journeys are read in.
4.0.5  - 01mar2002	- reads a header record as the first record in HAJ journey file and echos it to the report file..
4.0.6  - 07mar2002	- adds LOW_COMPETITION control to add capability to tighten low competition area.
4.0.7  - 11mar2002	- corrects use of LOW_COMPETITION control while reading HAJ journeys.
4.0.8  - 19mar2002	- corrects bug while reading HAJ journeys where k was not incremented for non-at-work, no SUBAREA_ANALYSIS.
4.0.9  - 14may2002	- corrects bug while counting HAJ journeys where header record was not skipped.
					  wrote out new files with journey sequence numbers, HAJ record numbers, and frozen/unfrozen status.
4.0.10 - 15may2002	- corrects header record in SUBAREA_JOURNEYSEQUENCE file.
4.0.11 - 15may2002	- added valid data checking in read_zone_data().
4.0.12 - 19sep2002	- corrects bug in balance_attractions() where writing to fp was supposed to only be for purpose 3.
					  The last fprintf and fclose were not in an if (Ini->PURPOSE_TO_PROCESS == 3) { } block.
4.0.13 - 20sep2002	- corrects monte carlo procedures to use nrand < prob[i] to select item i in place of <= in old code.
					  This change allows a random number of 0 to be a valid number and avoids the problem of selecting
					  item 1 if nrand is 0 and prob[1] = 0.0.  The <= code would select item 1, the new < code will
					  return the first i with prob[i] > 0.0 or a -1 indicating no selection was possible.
4.0.14 - 24sep2002  - Removed memory allocation for 3 hov skims matrices in OD_Utilities() to reduce memory requirements.

4.0.15 - 14Jul2004  - Read TC skims row by row in OD_Utilities to make it capable of handling highway hov, highway, and transit skims
                      simutaneously.  get_skims_by_row() is added to read a row of TC matrix to a 1-dimensional array in memory.
					  Utility calculation methods such as school_od_utilites.c are changed.
					  md.h and and tran_data are modified to reflect the above change.

4.0.16 - 14Jun2006  - Read separate skims for HOV2 and HOV3p and use in utility equations

4.0.17 - 13Sep2006  - Read separate skims for Taxi and use in utility equations. 
					  In the tour purpose specific od utilities, taxi tolls = tolls (taxifare.txt) + skims. Doubling of skim tolls are removed.

4.0.18 - 04Oct2006  - Added capability for MDC or MDSC to be run as a base scenario to generate frozen model results for FTA New Starts Analysis.
					- Models can also be run as build scenario to generate new results using frozen destination and transit subzone choices.
					- TODO: add option to use frozen subzone OR to make new subzone choice if input zonal accessibility proportions have changed
					- TODO: add option to use frozen work mode for at-work journeys OR to use the chosen work mode for at-work journey

4.0.19 - 18Oct2006  - Finished initial debugging.  Basic functionality to run base and build scenarios for FTA new start analysis appears to be working.
					- Changed output format for frozen records file from space separated to comma separated.

4.0.20 - 23Oct2006  - Created separate controls for input and output frozen records file so that frozen records could be written out in build scenario.

4.0.21 - 24Oct2006  - Fixed bug in format string when reading frozen records file.

4.0.22 - 25Oct2006  - Fixed bug where only first record of frozen records file was read.

4.0.23 - 25Oct2006  - Fixed bug where SE Utilities were computed for the first motorized journey in a packet only if FTA_RESTART != 1.

4.0.24 - 25Oct2006  - Fixed bug where walk accessibility at origin was set for the first motorized journey in a packet only if FTA_RESTART != 1.

4.0.25 - 08Jan2007  - Added some logging statements due to problem running 2030 maintenance tours in FTA_RESTART build mode.

4.0.26 - 08Jan2007  - Added some logging statements while reading frozen records file, where it seems to be stalling.

4.0.27 - 09Jan2007  - set a maximum of 10000000 records for frozen records files.  multiple files written if more than 10000000 journeys.

4.0.28 - 29Sep2007  - enhanced debugging while trying to understand university journey size variable constraint issues - turned out to be issue with ASCs.

4.0.29.1 - 22Oct2007  - changed parking costs in dollars to costs in cents.
					  Added function to write zonal free parking proportions to file named by FREEPARKPCTS, if that control is specified.

4.0.29.2 - 24Oct2007  - added some new error reporting in atwork logsum calculation.

4.1.0.0 - 30Oct2007	- Implementing functionality to test CBD zone license plate rationing policies.
					  Journeys with origins or destinations in the restricted license plate rationing area specified in the RINGDISTDATA file
					  for each taz will use autos available variable instead of autos owned after tour destination choice.
					  Autos available are the number available after each auto owned in a household is determined to be restricted or not.
					  *** NOTE *** this is now a perminant fixture.  To run MDSC without the rationing policy, set all TAZs to 0 (unrestricted)
					  in the RINGDISTDATA file.

4.1.0.1 - 02Nov2007	-	Added PCT_LP_RESTRICTION control to set the percentage of vehicles to be restriced from restricted area defined in RINGDISTDATA.
						Set values in range [0.0, 1.0].

4.1.0.2 - 06Nov2007	-	Changed parking cost back to units of dollars in ParkingCost.c.

4.1.0.3 - 07Nov2007 -   Corrected bugs in reporting autos/autosAvail and using SEutil for journeys subject to rationing

4.1.0.4 - 08Nov2007 -   Corrected bugs in at-work purpose model where work tour mode was not preserved correctly in FTA_RESTART runs with
						USE_FROZEN_WORKTOUR_MODE set to 1 - resulted in incorrect ordering and processing of at-work journeys.

4.1.0.5 - 13Nov2007 -   Changed the hardcoded MAX_FROZEN_RECORDS from 10000000 to 8000000 so that expanded frozen records format is
						accommodated and frozen records files stay under 4GB threshold. 

4.1.0.6 - 15Nov2007 -   Added PARKING_COST_IN_CENTS in params section of control file.  If this control is specified and if set to 1, parking
						costs in cents are used; otherwise parking costs in dollars are used.

4.1.0.7 - 16Nov2007 -   Added PARKINGUSER in files section of control file.  If this control is specified the file is read with destination zone
						over-ride values for pctFreeParking (w and nw), parking cost multiplier for origins not in Manhattan (w and nw), and
						parking cost multiplier for origins in Manhattan (w and nw).  These over-rides are appplied and the parking_cost functions
						modified to apply the logic.  Also changed discr and school od_utilities functions to call nw_parking_cost() and work
						od_utility function to not divide parking cost by 2 as it was so divided in parking_cost functions.
						Finally, changed frozen record file formats to not include SE_Util fields again to shorten them.

4.1.1.0 - 28dec2007 -   Added PCT_HHS_WITH_FULL_AUTO_RESTRICTION to define a percentage of HHs which are to have full auto restriction applied.
						For this percentage of households, the number of restricted autos is set equal to the number of autos in the hh.
						The mode choice model which uses the restricted auto sufficiency value when a destination is in a restricted zone is 
						impacted by this change.

4.1.1.1 - 02jan2008 -   Changed read_kpmg_journey_records() to write a line to REPORTFILE to list number of hhs with "potential" auto restrictions.
						AutosAvailable will be used for this hh if a journey orig or dest is in a restricted zone.  Part of HAJ summary report.

4.1.1.2 - 02jan2008 -   Added another report line in read_kpmg_journey_records() to write a line to REPORTFILE to list number of hhs with one or
						more autos.  This number divided by the number of "potential" hhs reported should be approximately equal to the input number
						specified by PCT_HHS_WITH_FULL_AUTO_RESTRICTION.  Also added PCT_HHS_WITH_FULL_AUTO_RESTRICTION > 0.0 as a condition
						for using reduced auto availability for stop frequency utility tour mode choice socioeconomic utility.

4.1.1.3 - 29jan2008 -   Removed PARKING_COST_IN_CENTS parameter and made default parking prices those previously hardcoded, in cents.
						If PARKINGCOSTS file is specifed, it should contain 1 header record and 10 lines with 2 fields to define indices and parking cost in cents.
						If file is not specified, default values are used.

4.1.1.4 - 31Jan2008 -   enhanced debugging while trying to understand school journey mode choice logsum exponent overflow.

4.1.1.5 - 31Jan2008 -   moved default parking costs array initialization to a global array and declared as external

4.1.1.5.1 - 25Mar2008 -   Added console and report output to print highway time & dist skim values for 3 selected ODs to check
						  the io with compressed vs uncompressed TransCAD matrix formats.

4.1.1.5.2 - 25Mar2008 -   Added console and report output to print walk transit in-vehicle time skim values for 3 selected ODs to check
						  the io with compressed vs uncompressed TransCAD matrix formats.

4.1.1.6 - 07Apr2008 -   added a new control in the PARAMS section called CPI_RATIO.  If no control is entered in the control file,
						the default value of 1.0 is used.  If a value is entered for this control, that value is multiplied by the
						model costs in the utility calculations.

4.1.1.7 - 22Jul2008 -   Changed reporting of highway time/dist and walk-transit ivt for diagnosing TransCAD matrix IO problems.
						Print heading only for first selected o/d and flush stdout and file handle for last selected o/d for both hwy & transit.

4.1.1.8 - 07Aug2008 -   Made this change while debugging MDC auto calibration issue.
						Fixed problem Caliper introduced in motor_dc_props() where util and props arrays were not properly declared upon re-entry
						during auto-calibration.

*/

#define VERSION "4.1.1.8"
#define LAST_MODIFIED "07aug2008"

int __cdecl main (int argc, char *argv[])
{
	FILE *fp, *fp3, *fp_rep2, *fp_rep3, *fp_work[6], *fp_cal;
	int i, j, k, iter, total_journeys, nm_MSC_index;
	int **ranprkcst, **TotProds, *NMTots;
	int *origList, *destList, *modeList, orig, dest, mode;
	int idist;
	float cal_scale, *Mode_Obs;
	float ***OD_Utility;
	float **hwy_dist, **cal_obs_scaled, **cal_est_scaled;
	float **m_cal_obs, **m_cal_est, *nm_cal_obs, **nm_cal_est;
	double share_ratio;
	struct taxi_data TaxiData;
	struct zone_data ZonalData;
	struct river_crossing_data RiverData;
	struct journey_attribs JourneyAttribs;
	struct walk_zone_data *WalkZoneData;
	struct bpmdist1_coeff_data BPMDist1;
	struct co_dist_factors *DistFactors;
	struct msc_data *msc;
	char PROGRAM[100];
	char tempString[200];
	char *temp;

	HANDLE od_util_heap;
    
	start = clock();


	if (argc == 2) {
		if ((fp = fopen(argv[1], "rt")) == NULL)
			ExitWithCode(4);
	}
	else {
	  ExitWithCode(14);
	}


	strcpy (PROGRAM, "mdsc_main");
	strcat (PROGRAM, VERSION);
	strcat (PROGRAM, ".exe");
	printf ("%s  --  version %s, last modified %s\n\n", PROGRAM, VERSION, LAST_MODIFIED);


	Memory = 0;
	AvailMemory = HEAPSIZE;

	heapHandle = HeapCreate (0, 2048, 0);

//	AvailMemory = avail_ram();


	// open and read control file	
	printf ("reading control file.\n");
	Ini = (INIDATA) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY,  1*sizeof(struct IniData));
	Ini->RAM_DEBUG_LEVEL = 0;
	Ini->STOPS_ONLY = 0;
	Ini->MDC_ONLY = 0;
	Ini->LOW_COMPETITION = 1;
	read_ini_control_file (fp, Ini);
 

// load matrix dlls
	if (Ini->ZERO_UTIL != 1) {
		Ini->ZERO_UTIL = 0;

		tc_status = 0;
		InitMatDLL(&tc_status);
		if (tc_status != TC_OKAY) {
			printf ("TransCAD matrix I/O DLLs not initialized.\n");
			printf ("Make sure TransCAD key is firmly in the parallel port.\n");
			printf ("exit (-5).\n");
			fflush (stdout);
			exit (-5);
		}

	}



	// set CPI_RATIO to 1.0 if no value was specified in control file.
	if (Ini->CPI_RATIO == 0xffffffff)
		Ini->CPI_RATIO = 1.0;


	//## for debugging
	//Ini->ZERO_UTIL = 1;
	//Ini->MDC_ONLY = 1;
	//## for debugging


	Ini->TerminalTime[0] = 0;
	Ini->TerminalTime[1] = 9;
	Ini->TerminalTime[2] = 7;
	Ini->TerminalTime[3] = 5;
	Ini->TerminalTime[4] = 3;
	Ini->TerminalTime[5] = 3;
	Ini->TerminalTime[6] = 2;
	Ini->TerminalTime[7] = 2;
	Ini->TerminalTime[8] = 2;
	Ini->TerminalTime[9] = 1;
	Ini->TerminalTime[10] = 1;
	Ini->TerminalTime[11] = 1;



	if (Ini->SUBAREA_ANALYSIS != 1) {
		if ((temp = GetStringFromINI (fp, "FILES", "RAM_REPORTFILE")) != NULL)
			strcpy (Ini->RAM_REPORTFILE, temp);
		else
			strcpy (Ini->RAM_REPORTFILE, "");

		// open memory log file if one was requested
		if (strcmp(Ini->RAM_REPORTFILE, "")) {
			if ((fp_ram = fopen(Ini->RAM_REPORTFILE, "wt")) == NULL)
				ExitWithCode(118);
		}
	}
	else {
		if ((temp = GetStringFromINI (fp, "FILES", "SUBAREA_RAM_REPORT")) != NULL)
			strcpy (Ini->SUBAREA_RAM_REPORT, temp);
		else
			strcpy (Ini->SUBAREA_RAM_REPORT, "");

		// open memory log file if one was requested
		if (strcmp(Ini->SUBAREA_RAM_REPORT, "")) {
			if ((fp_ram = fopen(Ini->SUBAREA_RAM_REPORT, "wt")) == NULL)
				ExitWithCode(118);
		}
	}




//	addRAM ("mdsc_main 0:", 0);
	


// define which function names get associated with which function pointer array element.
//	printf ("setting function pointers.\n");
	set_function_pointers ();



//	addRAM ("mdsc_main 0:", sizeof(struct IniData));


	if (Ini->SUBAREA_ANALYSIS != 1) {
		// open log file
		if (strcmp(Ini->REPORTFILE, "")) {
			if ((fp_rep = fopen(Ini->REPORTFILE, "wt")) == NULL)
				ExitWithCode(8);
		}
		else
			ExitWithCode (25);
	}
	else {
		// open log file
		if (strcmp(Ini->SUBAREA_REPORT, "")) {
			if ((fp_rep = fopen(Ini->SUBAREA_REPORT, "wt")) == NULL)
				ExitWithCode(8);
		}
		else
			ExitWithCode (25);
	}



	fprintf (fp_rep, "mdsc -- version %s\n\n", VERSION);

	rewind (fp);
	echo_ini (fp, fp_rep, argv[1]);
	fclose (fp);



// Memory allocations
	m_cal_obs = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (int)pow(Ini->NUMBER_BPMDIST1+1,2)*sizeof(float *));
	for (i=0; i < pow(Ini->NUMBER_BPMDIST1+1,2); i++)
		m_cal_obs[i] = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_ALTS+1)*sizeof(float));

	m_cal_est = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (int)pow(Ini->NUMBER_BPMDIST1+1,2)*sizeof(float *));
	for (i=0; i < pow(Ini->NUMBER_BPMDIST1+1,2); i++)
		m_cal_est[i] = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_ALTS+1)*sizeof(float));

	nm_cal_obs = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_BPMDIST1+1)*sizeof(float));

	nm_cal_est = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_BPMDIST1+1)*sizeof(float *));
	for (i=0; i < Ini->NUMBER_BPMDIST1+1; i++)
		nm_cal_est[i] = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 2*sizeof(float));

	cal_obs_scaled = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (int)pow(Ini->NUMBER_BPMDIST1+1,2)*sizeof(float *));
	for (i=0; i < pow(Ini->NUMBER_BPMDIST1+1,2); i++)
		cal_obs_scaled[i] = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_ALTS+1)*sizeof(float));

	cal_est_scaled = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (int)pow(Ini->NUMBER_BPMDIST1+1,2)*sizeof(float *));
	for (i=0; i < pow(Ini->NUMBER_BPMDIST1+1,2); i++)
		cal_est_scaled[i] = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_ALTS+1)*sizeof(float));

	Mode_Obs = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_ALTS + 1)*sizeof(float));
	NMTots = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_BPMDIST1+2)*sizeof(int));

	ranprkcst = (int **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, DENSITY_RANGES*sizeof (int *));
	for (i=0; i < DENSITY_RANGES; i++)
		ranprkcst[i] = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, RANDOM_NUMBERS*sizeof (int));

	TotProds = (int **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(int *));
	for (i=0; i < Ini->MAX_TAZS+1; i++)
		TotProds[i] = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, PURPOSES*sizeof(int));

	DistFactors = (struct co_dist_factors *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_COUNTIES+1)*sizeof(struct co_dist_factors));


//	addRAM ("mdsc_main 1", 4*((int)pow(Ini->NUMBER_BPMDIST1+1,2))*(Ini->NUMBER_ALTS+1)*sizeof(float)
//						+ (Ini->NUMBER_BPMDIST1+1)*sizeof(float)
//						+ (Ini->NUMBER_BPMDIST1+1)*2*sizeof(float)
//						+ (Ini->NUMBER_ALTS+1)*sizeof(float)
//						+ (Ini->NUMBER_BPMDIST1+2)*sizeof(int)
//						+ (DENSITY_RANGES*RANDOM_NUMBERS)*sizeof(int)
//						+ (Ini->MAX_TAZS+1)*PURPOSES*sizeof(int)
//						+ (Ini->NUMBER_COUNTIES+1)*sizeof(struct co_dist_factors));
	
	

// read and process all the input data files other than skims data
	get_input_data (&fp3, &fp_cal, &fp_rep2, &fp_rep3, fp_work, ranprkcst,
		&TaxiData, &ZonalData, &RiverData, &WalkZoneData, &BPMDist1, DistFactors, &msc);
	printf ("done reading input data files.\n");



// calculate the areas of competition if subarea analysis is selected
	if (Ini->SUBAREA_ANALYSIS == 1) {
		printf ("calculating sub-areas of competition.\n");
		competition_areas (&ZonalData);
	}


// read journey records after computing od utilities so more temp memory is available for OD_Utilities().
	get_journey_data (fp_work, &ZonalData, &JourneyAttribs, TotProds);



// prepare matrices for computing od-based utilities
	if (Ini->STOPS_ONLY == 0) {
		if (Ini->ZERO_UTIL == 1) {
//			printf ("\n****************************************\n");
//			printf (  "************ W A R N I N G *************\n");
//			printf (  "** OD Utility is not being calculated **\n");
//			printf (  "****************************************\n\n");
			fprintf (fp_rep, "\n****************************************\n");
			fprintf (fp_rep,   "************ W A R N I N G *************\n");
			fprintf (fp_rep,   "** OD Utility is not being calculated **\n");
			fprintf (fp_rep,   "****************************************\n\n");

			// allocate memory for highway distance array
			hwy_dist = (float **) calloc(Ini->MAX_TAZS+1, sizeof(float *));
			for (i=0; i < Ini->MAX_TAZS+1; i++)
				hwy_dist[i] = (float *) calloc(Ini->MAX_TAZS+1, sizeof(float));
		}
		else {
			od_util_heap = HeapCreate (0, 2048, 0);

			// allocate memory for big utility matrix
			OD_Utility = (float ***) HeapAlloc (od_util_heap, HEAP_ZERO_MEMORY, (Ini->NUMBER_ALTS-2)*sizeof(float **));
			for (k=0; k < Ini->NUMBER_ALTS-2; k++) {
				OD_Utility[k] = (float **) HeapAlloc (od_util_heap, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float *));
				for (i=0; i < Ini->MAX_TAZS+1; i++)
					OD_Utility[k][i] = (float *) HeapAlloc (od_util_heap, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(float));
			}

			// read highway and transit skims and compute OD utilities for each modal alternative
			printf ("Compute OD based utilities\n");
			OD_Utilities (Ini->PURPOSE_TO_PROCESS, &ZonalData, &TaxiData, ranprkcst, &hwy_dist, OD_Utility);

		}

//		addRAM ("mdsc_main 3", (Ini->NUMBER_ALTS-2)*(Ini->MAX_TAZS+1)*(Ini->MAX_TAZS+1)*sizeof(float));

	}




	if (Ini->STOPS_ONLY == 1)
		goto STOPS;


// compute observed motorized modal proportions from input motorized target shares
	for (i=0; i < msc->motorized_max_index; i++) {
		m_cal_obs[i][Ini->NUMBER_ALTS] = 0.0;
		for (j=0; j < Ini->NUMBER_ALTS; j++) {
			m_cal_obs[i][j] = (float)(msc->motorized_targets[i][j]/100.0);
			m_cal_obs[i][Ini->NUMBER_ALTS] += msc->motorized_targets[i][j];
		}
		m_cal_obs[i][Ini->NUMBER_ALTS] /= (float)(msc->motorized_targets[i][j]/100.0);
	}



// compute observed non-motorized modal proportions from input non-motorized target shares
	for (i=0; i <= msc->nm_max_index; i++)
		nm_cal_obs[i] = (float)(msc->nm_targets[i]/100.0);



// compute observed regional modal shares from regioanl targets
	for (j=0; j < Ini->NUMBER_ALTS; j++)
		Mode_Obs[j] = msc->motorized_targets[msc->motorized_max_index][j]/(float)100.0;



// run calibration
	Calibrating = 1;
	if (Ini->CALIB_SCALE > 0.0)
		cal_scale = Ini->CALIB_SCALE;
	else
		cal_scale = 1.0;

	total_journeys = Ini->NUMBER_JOURNEYS;
	for (iter=0; iter < Ini->MAX_CALIB_ITERS; iter++) {

		// change the random number seed for each calibration iteration
		srand (Ini->RAND_SEED + iter + 1);

		Ini->NUMBER_JOURNEYS = (int)(Ini->CALIB_INCREMENT*(iter+1)*total_journeys);
		if (Ini->NUMBER_JOURNEYS > total_journeys)
			Ini->NUMBER_JOURNEYS = total_journeys;
	
		// initialize estimated motorized shares matrix
		for (i=0; i <= msc->motorized_max_index; i++) {
			for (j=0; j <= Ini->NUMBER_ALTS; j++)
				m_cal_est[i][j] = 0.0;
		}

		// initialize estimated non-motorized shares and non-motorized productions vectors
		for (i=0; i <= msc->nm_max_index; i++) {
			NMTots[i] = 0;
			for (j=0; j < 2; j++)
				nm_cal_est[i][j] = 0.0;
		}

		// run the simultaneous mode and destination choice model
		run_mdc (NULL, NULL, NULL, fp_work, msc, m_cal_est, nm_cal_est,
				hwy_dist, OD_Utility, TotProds, &JourneyAttribs, &ZonalData,
				RiverData, &TaxiData, WalkZoneData, BPMDist1, DistFactors);


//#
		sprintf (tempString, "finished with run_mdc() in calibration iteration %d\n", iter);
		printf  ("%s", tempString);
		fprintf (fp_rep, "%s", tempString);
		fflush (stdout);
		fflush (fp_rep);
//#


		// total up the number of productions by non-motorized geographic stratum for use in computing pre-mode shares
		// do this after run_mdc() so that at-work linkages have already been made.
		for (k=0; k < Ini->NUMBER_JOURNEYS; k++) {
//			nm_MSC_index = get_nm_MSC_index (JourneyAttribs.orig[k], &ZonalData, msc);
        	idist = ZonalData.bpmdist1_index[JourneyAttribs.orig[k]];
        	nm_MSC_index = msc->nm_indices[idist];

			NMTots[nm_MSC_index]++;
		}

		// convert estimated journey frequencies into mode shares by MSC strata, compare to observed shares,
		// and report for both motorized modes and non-motorized pre-mode choice.
		calibration_report (iter, fp_cal, msc, Mode_Obs, NMTots, m_cal_obs, m_cal_est, nm_cal_obs, nm_cal_est, cal_scale);


		// compute new non-motorized MSCs
		//		note: non-motorized MSCs are origin district based as opposed to O/D district based for motorized.
		//            Therefore, m_cal_est[i][9] has the frequency of nm journeys by motorized stratum i,
		//            and nm_cal_est[i] has the frequency of nm journeys by non-motorized stratum i.
		for (i=0; i < msc->nm_max_index; i++) {
			if (msc->nm_targets[i] < 0.00001) {
				msc->nmMSC[i] = MISSING;
			}
			else {
				if (nm_cal_est[i][0] > 0.0 && nm_cal_obs[i] < 1.0)
					share_ratio = (nm_cal_obs[i]*(1.0 - nm_cal_est[i][0]))/(nm_cal_est[i][0]*(1.0 - nm_cal_obs[i])) ;
				else
					share_ratio = MAX_CALIB_RATIO;

				if (share_ratio < MIN_CALIB_RATIO)
					share_ratio = MIN_CALIB_RATIO;
				else if (share_ratio > MAX_CALIB_RATIO)
					share_ratio = MAX_CALIB_RATIO;
				
				msc->nmMSC[i] += (float)log(share_ratio)*cal_scale;
			}
		}


		// compute new motorized shares without consideration of non-motorized journeys; that is,
		// using proportions of motorized journeys.
		for (i=0; i < msc->motorized_max_index; i++) {
			for (j=0; j < Ini->NUMBER_ALTS; j++) {
				if (j != 9) {
					if (m_cal_obs[i][9] < 1.0)
						cal_obs_scaled[i][j] = m_cal_obs[i][j]/((float)1.0 - m_cal_obs[i][9]);
					else
						cal_obs_scaled[i][j] = 0.0;

					if (m_cal_est[i][9] < 1.0)
						cal_est_scaled[i][j] = m_cal_est[i][j]/((float)1.0 - m_cal_est[i][9]);
					else
						cal_est_scaled[i][j] = 0.0;
				}
			}
		}

				
		// use these new motorized only shares to compute new motorized MSCs
		for (i=0; i < msc->motorized_max_index; i++) {
			for (j=0; j < Ini->NUMBER_ALTS; j++) {
				if (j != 9) {
					if (cal_obs_scaled[i][j] < 0.00001) {
						msc->MSC[i][j] = MISSING;
					}
					else {
						if (cal_est_scaled[i][j] > 0.0)
							share_ratio = cal_obs_scaled[i][j]/cal_est_scaled[i][j];
						else
							share_ratio = MAX_CALIB_RATIO;

						if (share_ratio < MIN_CALIB_RATIO)
							share_ratio = MIN_CALIB_RATIO;
						else if (share_ratio > MAX_CALIB_RATIO)
							share_ratio = MAX_CALIB_RATIO;

						msc->MSC[i][j] += (float)log(share_ratio)*cal_scale;
					}
				}
			}
		}
	}


	// run the simultaneous mode and destination choice model one final time and generate final report
	Calibrating = 0;
	Ini->NUMBER_JOURNEYS = total_journeys;

	// initialize estimated motorized shares matrix
	for (i=0; i <= msc->motorized_max_index; i++) {
		for (j=0; j <= Ini->NUMBER_ALTS; j++)
			m_cal_est[i][j] = 0.0;
	}

	// initialize estimated non-motorized shares and non-motorized productions vectors
	for (i=0; i <= msc->nm_max_index; i++) {
		NMTots[i] = 0;
		for (j=0; j < 2; j++)
			nm_cal_est[i][j] = 0.0;
	}

	srand (Ini->RAND_SEED);

	run_mdc (fp3, fp_rep2, fp_rep3, fp_work, msc, m_cal_est, nm_cal_est,
		hwy_dist, OD_Utility, TotProds, &JourneyAttribs, &ZonalData,
		RiverData, &TaxiData, WalkZoneData, BPMDist1, DistFactors);

//#
		sprintf (tempString, "finished with run_mdc()\n", i);
		printf  ("%s", tempString);
		fprintf (fp_rep, "%s", tempString);
		fflush (stdout);
		fflush (fp_rep);
//#


	// total up the number of productions by non-motorized geographic stratum for use in computing pre-mode shares
	for (k=0; k < Ini->NUMBER_JOURNEYS; k++) {
//		nm_MSC_index = get_nm_MSC_index (JourneyAttribs.orig[k], &ZonalData, msc);
      	idist = ZonalData.bpmdist1_index[JourneyAttribs.orig[k]];
       	nm_MSC_index = msc->nm_indices[idist];
		NMTots[nm_MSC_index]++;
	}

	// use cal_est_scaled[][] to temporarily save m_cal_est[][] and cat_obs_scaled[][0] to save nm_cal_est[]
	// so that calibration_report can be called twice to write the same report to two different files.
	for (i=0; i < msc->motorized_max_index; i++)
		for (j=0; j <= Ini->NUMBER_ALTS; j++)
			cal_est_scaled[i][j] = m_cal_est[i][j];

	for (i=0; i < msc->nm_max_index; i++)
		for (j=0; j < 2; j++)
			cal_obs_scaled[i][j] = nm_cal_est[i][j];



	calibration_report (iter, fp_rep, msc, Mode_Obs, NMTots, m_cal_obs, m_cal_est, nm_cal_obs, nm_cal_est, cal_scale);

	// restore vales and generate report a second time
	if (fp_cal != NULL && Ini->MAX_CALIB_ITERS > 0) {

		for (i=0; i < msc->motorized_max_index; i++)
			for (j=0; j <= Ini->NUMBER_ALTS; j++)
				m_cal_est[i][j] = cal_est_scaled[i][j];

		for (i=0; i < msc->nm_max_index; i++)
			for (j=0; j < 2; j++)
				nm_cal_est[i][j] = cal_obs_scaled[i][j];
	
		calibration_report (iter, fp_cal, msc, Mode_Obs, NMTots, m_cal_obs, m_cal_est, nm_cal_obs, nm_cal_est, cal_scale);
	}



	// free memory for od-based utilities
	if (Ini->ZERO_UTIL == 0 && Ini->STOPS_ONLY == 0) {

		for (k=0; k < Ini->NUMBER_ALTS-2; k++)
			for (i=0; i < Ini->MAX_TAZS+1; i++)
				HeapFree (od_util_heap, 0, OD_Utility[k][i]);

		for (k=0; k < Ini->NUMBER_ALTS-2; k++)
			HeapFree (od_util_heap, 0, OD_Utility[k]);

		HeapFree (od_util_heap, 0, OD_Utility);

		HeapDestroy (od_util_heap);
	}


//#
		sprintf (tempString, "done with MDC, exit or go to STOPS.\n", i);
		printf  ("%s", tempString);
		fprintf (fp_rep, "%s", tempString);
		fflush (stdout);
		fflush (fp_rep);
//#

	if (Ini->MDC_ONLY == 1)
		exit (0);

// End of MDC code






// Stop frequency and location choice code
	STOPS:

//#
		sprintf (tempString, "start of STOPS section.\n", i);
		printf  ("%s", tempString);
		fprintf (fp_rep, "%s", tempString);
		fflush (stdout);
		fflush (fp_rep);
//#

	start = clock();

	origList = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(int));
	destList = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(int));
	modeList = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(int));
//	addRAM ("mdsc_main 4", 3*Ini->NUMBER_JOURNEYS*sizeof(int));


// open MDC output journeys file, the file sent to David Rhoden, for reading MDC output journey orig, dest, mode
	if (Ini->SUBAREA_ANALYSIS != 1) {
		if (strcmp(Ini->OUTPUTJOURNEYS, "")) {
			if ((fp3 = fopen(Ini->OUTPUTJOURNEYS, "rt")) == NULL)
				ExitWithCode(115);
		}
		else
			ExitWithCode (34);
	}
	else {
		if (strcmp(Ini->SUBAREA_MDC_OUTPUT, "")) {
			if ((fp3 = fopen(Ini->SUBAREA_MDC_OUTPUT, "rt")) == NULL)
				ExitWithCode(129);
		}
		else
			ExitWithCode (128);
	}


// read journey orig, dest, and mode
	sprintf (tempString, "Reading MDC output file for STOPS Processing.\n");
	printf  ("%s", tempString);
	fprintf (fp_rep, "%s", tempString);
	fflush (stdout);
	fflush (fp_rep);

	i = 0;
	while ((fscanf(fp3, "%d %d %d %*d %d", &k, &orig, &dest, &mode)) != EOF) {
		if (i == Ini->NUMBER_JOURNEYS) {
			printf ("While reading MDC output file for STOPS Processing,\n");
			printf ("the number of MDC output file records exceeded the number of\n");
			printf ("journey records=%d of purpose=%d read in from HAJ output file.\n\n", Ini->NUMBER_JOURNEYS, Ini->PURPOSE_TO_PROCESS);
			printf ("exit (-45)\n\n");
			fprintf (fp_rep, "While reading MDC output file for STOPS Processing,\n");
			fprintf (fp_rep, "the number of MDC output file records exceeded the number of\n");
			fprintf (fp_rep, "journey records=%d of purpose=%d read in from HAJ output file.\n\n", Ini->NUMBER_JOURNEYS, Ini->PURPOSE_TO_PROCESS);
			fprintf (fp_rep, "exit (-45)\n\n");
			fflush (stdout);
			fflush (fp_rep);
			exit (-45);
		}
		origList[k] = orig;
		destList[k] = dest;
		modeList[k] = mode;
		i++;
	}
	fclose (fp3);

	if (i != Ini->NUMBER_JOURNEYS) {
		printf ("While reading MDC output file for STOPS Processing,\n");
		printf ("the number of MDC output file records=%d is different from the number of\n", i);
		printf ("journey records=%d of purpose=%d read in from HAJ output file.\n\n", Ini->NUMBER_JOURNEYS, Ini->PURPOSE_TO_PROCESS+1);
		printf ("exit (-46)\n\n");
		fprintf (fp_rep, "While reading MDC output file for STOPS Processing,\n");
		fprintf (fp_rep, "the number of MDC output file records=%d is different from the number of\n", i);
		fprintf (fp_rep, "journey records=%d of purpose=%d read in from HAJ output file.\n\n", Ini->NUMBER_JOURNEYS, Ini->PURPOSE_TO_PROCESS+1);
		fprintf (fp_rep, "exit (-46)\n\n");
		fflush (stdout);
		fflush (fp_rep);
		exit (-46);
	}


	sprintf (tempString, "Start of processing for stop frequncy choice and stop location choice models.\n");
	printf  ("%s", tempString);
	fprintf (fp_rep, "%s", tempString);
	fflush (stdout);
	fflush (fp_rep);

	stops (fp3, origList, destList, modeList, &JourneyAttribs, &ZonalData, RiverData);


	HeapFree (heapHandle, 0, origList);
	HeapFree (heapHandle, 0, destList);
	HeapFree (heapHandle, 0, modeList);


	finish = clock();
	sprintf (tempString, "total execution time for processing stops for %d %s journeys:  %.2lf minutes.\n", Ini->NUMBER_JOURNEYS, PurposeLabels[Ini->PURPOSE_TO_PROCESS], (60.0*((double)finish-start)/CLOCKS_PER_SEC)/3600.0);
	printf  ("%s", tempString);
	fprintf (fp_rep, "%s", tempString);
	
	return (0);
}
