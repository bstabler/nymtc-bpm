#include <float.h>
// Global definitions to be #included in EstData.c



// Error code definitions:		
char *ErrorCodes[] = 	{ 	"Normal Exit", 																	// 0	
				"Zonal text input data file not found",														// 1
				"Taxi text input data file not found",														// 2
				"Probabilities do not add up to one in props",												// 3
				"Control file (.ini) not found",															// 4
				"Journey records data file not found",														// 5
				"Argument of exponent function out of range in logsum",										// 6
				"None of the modes were available for this OD",												// 7
				"Could not open report file for writing",													// 8
				"Walk to transit skims data file not found",												// 9
				"Drive to transit skims data file not found",												// 10
				"Walk to commuter rail skims data file not found",											// 11
				"Drive to commuter rail skims data file not found",											// 12
				"Problem with TransCAD skims data file",														// 13
				"Expecting:  program-name control-file-name  (e.g. work_logsum logsum.ini)",				// 14
				"Probabilities do not add up to one in work_dest_choice",									// 15
				"Centroid coordinates file not found",														// 16
				"River Crossing text input data file not found",											// 17
				"Could not open output journeys file",														// 18
				"Could not open school district correspondance file",										// 19
				"Could not open zonal attraction percentages data file",									// 20
				"Log density value from nm_log_density is MISSING",											// 21
				"Could not open zonal ring/dists correspondance file",										// 22
				"Could not open coefficients file for BPMDIST1 areas",										// 23
				"Could not open file with zones within walk distance of origin zones",						// 24
				"REPORTFILE entry missing in control file (.ini)",											// 25
				"WALKZONEDATA entry missing in control file (.ini)",										// 26
				"ZONEDATA entry missing in control file (.ini)",											// 27
				"SCHOOLDISTDATA entry missing in control file (.ini)",										// 28
				"RINGDISTDATA entry missing in control file (.ini)",										// 29
				"RIVERDATA entry missing in control file (.ini)",											// 30
				"BPMDIST1COEFFDATA entry missing in control file (.ini)",									// 31
				"TAXIDATA entry missing in control file (.ini)",											// 32
				"INJOURNEYDATA entry missing in control file (.ini)",										// 33
				"OUTPUTJOURNEYS entry missing in control file (.ini)",										// 34
				"SUMMARYDATA entry missing in control file (.ini)",											// 35
				"WALK_TRANSIT_SKIM entry missing in control file (.ini)",									// 36
				"DRIVE_TRANSIT_SKIM entry missing in control file (.ini)",									// 37
				"WALK_COMMRAIL_SKIM entry missing in control file (.ini)",									// 38
				"DRIVE_COMMRAIL_SKIM entry missing in control file (.ini)",									// 39
				"HIGHWAY_SOV_SKIM entry missing in control file (.ini)",										// 40
				"ATTR_PCT_DATA entry missing in control file (.ini)",										// 41
				"Estimation journey records data file not found",											// 42
				"ESTJOURNEYDATA entry missing in control file (.ini)",										// 43
				"Work journey records output file could not be opened",										// 44
				"WORKDESTFILE entry missing in control file (.ini) for this work purpose",					// 45
				"Low income work journey destinations file could not be opened for reading",				// 46
				"ATWORK_LO_FILE entry missing in control file (.ini)",										// 47
				"Medium income work journey destinations file could not be opened for reading",				// 48
				"ATWORK_MD_FILE entry missing in control file (.ini)",										// 49
				"High income work journey destinations file could not be opened for reading",				// 50
				"ATWORK_HI_FILE entry missing in control file (.ini)",										// 51
				"Motorized Mode Specific Constant Indices file could not be opened for reading",			// 52
				"M_MSC_INDICES entry missing in control file (.ini)",										// 53
				"County to County by Mode journey frequency report file not opened for writing",			// 54
				"Running mode shares log report file not opened for writing",								// 55
				"Auto-calibration of MSCs report file not opened for writing",								// 56
				"Mode share target file could not be opened for reading",									// 57
				"TARGETDATA entry missing in control file, but MAX_CALIB_ITES > 0",							// 58
				"work_wt_od_util exited because xfr_wait[i][j] was MISSING",								// 59
				"work_wt_od_util exited because xfr_ovtt[i][j] was MISSING",								// 60
				"work_dt_od_util exited because xfr_wait[i][j] was MISSING",								// 61
				"work_dt_od_util exited because xfr_ovtt[i][j] was MISSING",								// 62
				"work_wc_od_util exited because xfr_wait[i][j] was MISSING",								// 63
				"work_wc_od_util exited because xfr_ovtt[i][j] was MISSING",								// 64
				"work_dc_od_util exited because xfr_wait[i][j] was MISSING",								// 65
				"work_dc_od_util exited because xfr_ovtt[i][j] was MISSING",								// 66
				"univ_wt_od_util exited because xfr_wait[i][j] was MISSING",								// 67
				"univ_wt_od_util exited because xfr_ovtt[i][j] was MISSING",								// 68
				"univ_dt_od_util exited because xfr_wait[i][j] was MISSING",								// 69
				"univ_dt_od_util exited because xfr_ovtt[i][j] was MISSING",								// 70
				"univ_wc_od_util exited because xfr_wait[i][j] was MISSING",								// 71
				"univ_wc_od_util exited because xfr_ovtt[i][j] was MISSING",								// 72
				"univ_dc_od_util exited because xfr_wait[i][j] was MISSING",								// 73
				"univ_dc_od_util exited because xfr_ovtt[i][j] was MISSING",								// 74
				"school_wt_od_util exited because xfr_wait[i][j] was MISSING",								// 75
				"school_wt_od_util exited because xfr_ovtt[i][j] was MISSING",								// 76
				"school_dt_od_util exited because xfr_wait[i][j] was MISSING",								// 77
				"school_dt_od_util exited because xfr_ovtt[i][j] was MISSING",								// 78
				"school_wc_od_util exited because xfr_wait[i][j] was MISSING",								// 79
				"school_wc_od_util exited because xfr_ovtt[i][j] was MISSING",								// 80
				"school_dc_od_util exited because xfr_wait[i][j] was MISSING",								// 81
				"school_dc_od_util exited because xfr_ovtt[i][j] was MISSING",								// 82
				"discr_wt_od_util exited because xfr_wait[i][j] was MISSING",								// 83
				"discr_wt_od_util exited because xfr_ovtt[i][j] was MISSING",								// 84
				"discr_dt_od_util exited because xfr_wait[i][j] was MISSING",								// 85
				"discr_dt_od_util exited because xfr_ovtt[i][j] was MISSING",								// 86
				"discr_wc_od_util exited because xfr_wait[i][j] was MISSING",								// 87
				"discr_wc_od_util exited because xfr_ovtt[i][j] was MISSING",								// 88
				"discr_dc_od_util exited because xfr_wait[i][j] was MISSING",								// 89
				"discr_dc_od_util exited because xfr_ovtt[i][j] was MISSING",								// 90
				"maint_wt_od_util exited because xfr_wait[i][j] was MISSING",								// 91
				"maint_wt_od_util exited because xfr_ovtt[i][j] was MISSING",								// 92
				"maint_dt_od_util exited because xfr_wait[i][j] was MISSING",								// 93
				"maint_dt_od_util exited because xfr_ovtt[i][j] was MISSING",								// 94
				"maint_wc_od_util exited because xfr_wait[i][j] was MISSING",								// 95
				"maint_wc_od_util exited because xfr_ovtt[i][j] was MISSING",								// 96
				"maint_dc_od_util exited because xfr_wait[i][j] was MISSING",								// 97
				"maint_dc_od_util exited because xfr_ovtt[i][j] was MISSING",								// 98
				"atwork_wt_od_util exited because xfr_wait[i][j] was MISSING",								// 99
				"atwork_wt_od_util exited because xfr_ovtt[i][j] was MISSING",								// 100
				"Could not open zonal attraction corrections data file",									// 101
				"",																							// 102
				"Distance based scale factors file could not be opened for reading",						// 103
				"DISTFACTORS entry missing in control file (.ini)",											// 104
				"Non-motorized Mode Specific Constant Indices file could not be opened for reading",		// 105
				"NM_MSC_INDICES entry missing in control file (.ini)",										// 106
				"Motorized Mode Specific Constants file could not be opened for reading",					// 107
				"M_MSCS entry missing in control file (.ini)",												// 108
				"Non-motorized Mode Specific Constants file could not be opened for reading",				// 109
				"NM_MSCS entry missing in control file (.ini)",												// 110
				"Motorized targets file could not be opened for reading",									// 111
				"M_TARGETS entry missing in control file (.ini)",											// 112
				"Non-motorized targets file could not be opened for reading",								// 113
				"NM_TARGETS entry missing in control file (.ini)",											// 114
				"Could not open output journeys file for reading for stop models",							// 115
				"Could not open output journeys file for writing after stop models",						// 116
				"JOURNEYS_WITH_STOPS entry missing in control file (.ini)",									// 117
				"Could not open memory log file for writing",												// 118
				"Sub-area defintions file could not be opened for reading",									// 119
				"SUBAREADATA entry missing in control file (.ini)",											// 120
				"MDSC output file from previous MDSC run could not be opened for reading for subareas",		// 121
				"MDSCOUTPUTJOURNEYS entry missing in control file (.ini)",									// 122
				"Could not open file for writing full set of MDSC output during subarea analysis",			// 123
				"Could not open file for writing frozen set of MDSC output during subarea analysis",		// 124
				"Could not open file for writing unfrozen set of MDSC output during subarea analysis",		// 125
				"SUBAREA_MDSC_OUTPUT_UNFROZEN entry missing in control file (.ini)",						// 126
				"Could not open file for writing MDC output during subarea analysis",						// 127
				"SUBAREA_MDC_OUTPUT entry missing in control file (.ini)",									// 128
				"Could not open MDC output journeys for reading for stop models in subarea analysis",		// 129
				"Co to Co by Mode journey freq report file not opened for writing for subarea analysis",	// 130
				"Work journey records output file could not be opened for subarea analysis",				// 131
				"SUBAREA_WORKDESTFILE entry missing in control file (.ini) for this work purpose",			// 132
				"Low inc work journey dests file could not be opened for reading for subarea analysis",		// 133
				"SUBAREA_ATWORK_LO_FILE entry missing in control file (.ini)",								// 134
				"Medium inc work journey dests file could not be opened for reading for subarea analysis",	// 135
				"SUBAREA_ATWORK_MD_FILE entry missing in control file (.ini)",								// 136
				"High inc work journey dests file could not be opened for reading for subarea analysis",	// 137
				"SUBAREA_ATWORK_HI_FILE entry missing in control file (.ini)",								// 138
				"File of TAZS with subarea cdes could not be opened for writing for subarea analysis",		// 139
				"Subarea analysis file of input journey sequence numbers could not be opened for writing",	// 140
				"File of input journey sequence numbers could not be opened for writing",					// 141
				"HIGHWAY_HOV2_SKIM entry missing in control file (.ini)",									// 142
				"HIGHWAY_HOV3p_SKIM entry missing in control file (.ini)",									// 143
				"Inconsistent matrix dimensions between highway skims",										// 144
				"Frozen journey file from previous MDSC run could not be opened for reading for FTA_RESTART",										// 145
				"FROZEN_MDC_INPUT entry expected, but missing in control file (.ini).  Trying to locate frozen journey filename for reading.",		// 146
				"Frozen journey file could not be opened for writing.",																				// 147
				"FROZEN_MDC_OUTPUT entry expected, but missing in control file (.ini).  Trying to locate frozen journey filename for writing.",		// 148
				"HIGHWAY_TAXI_SKIM entry missing in control file (.ini)",																			// 149
				"PARKINGUSER zonal free parking percent over-rides file could not be opened for reading",											// 150
				"PARKINGCOSTS indexed parking costs values file could not be opened for reading",													// 151
				""
			};


char *ModeLabel[] = { "drive alone", "shared ride 2", "shared ride 3", "shared ride 4+",
					"walk to transit", "drive to transit",
					"walk to commuter rail", "drive to commuter rail",
					"taxi", "non-motorized", "school bus" };


char *PurposeLabels[] = { "low inc work", "med inc work", "high inc work", "school", "university",
						"maintenance", "discretionary", "at work" };


char *CountyLabels[] = { "NEW YORK, NY", "QUEENS, NY", "BRONX, NY", "KINGS, NY", "RICHMOND, NY",
						"NASSAU, NY", "SUFFOLK, NY", "WESTCHESTER, NY", "ROCKLAND, NY", "PUTNAM, NY",
						"ORANGE, NY", "DUTCHESS, NY", "FAIRFIELD, CT", "BERGEN, NJ", "PASSAIC, NJ",
						"HUDSON, NJ", "ESSEX, NJ", "UNION, NJ", "MORRIS, NJ", "SOMERSET, NJ",
						"MIDDLESEX, NJ", "MONMOUTH, NJ", "OCEAN, NJ", "HUNTERDON, NJ", "WARREN, NJ",
						"SUSSEX, NJ", "NEW HAVEN, CT", "MERCER, NJ" };


char *BPMDist1Labels[] = { "LOWER MANHATTAN", "VALLEY MANHATTAN", "MIDTOWN MANHATTAN", "OTHER MANHATTAN",
						"QUEENS, NY", "BRONX, NY", "KINGS, NY", "RICHMOND, NY",
						"NASSAU, NY", "SUFFOLK, NY", "WESTCHESTER, NY", "ROCKLAND, NY", "PUTNAM, NY",
						"ORANGE, NY", "DUTCHESS, NY", "FAIRFIELD, CT", "BERGEN, NJ", "PASSAIC, NJ",
						"HUDSON, NJ", "ESSEX, NJ", "UNION, NJ", "MORRIS, NJ", "SOMERSET, NJ",
						"MIDDLESEX, NJ", "MONMOUTH, NJ", "OCEAN, NJ", "HUNTERDON, NJ", "WARREN, NJ",
						"SUSSEX, NJ", "NEW HAVEN, CT", "MERCER, NJ" };


char *PreModeLabels[] = { "From Lower Manhattan", "From Valley Manhattan", "From Midtown Manhattan",
						  "From Other Manhattan", "From Urban Non-Manhttan", "From Suburbs", "From Rural" };


char *UrbanTypeLabels[] = { "Manhattan", "Other Urban", "Suburbs", "Rural" };



// Global variables
INIDATA Ini;									// data structure for control file values
float MISSING = -FLT_MAX;						// negative maximum floating point value represents missing value in TransCAD.

HANDLE heapHandle;

int Memory;
int AvailMemory;
double mc_ivt_coeff;

// end of Globals.h



