#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "CaliperMTX.h"
#include "GetFromINI.h"
#include <windows.h>


// Constant definitions
#define MB 1024*1024
#define HEAPSIZE 1400*MB

#define LRECL 1024
#define TC_OKAY 0
#define PURPOSES 8
#define MAX_EXP 700
#define MAX_DISTANCE_SCALE 650
#define MAX_PACKET_SIZE 100
#define MAX_PERSONS_PER_HH 20
#define PROB_EPS 0.0001
#define AREA_TYPES 11
#define URBAN_TYPES 4
#define LATITUDE_PER_FEET 2.7
#define LONGITUDE_PER_FEET 3.6
#define PROGRESS_INCREMENT 1000
#define PROGRESS_INCREMENT1 10000
#define STATUE_OF_LIBERTY_TAZ 141

#define MAX_FROZEN_RECORDS 8000000

#define HH_KEY 6
#define PERS_KEY 3

#define MIN_CALIB_RATIO 0.5
#define MAX_CALIB_RATIO 2.0

#define DENSITY_RANGES 10
#define PRKCOST_RANGES 10
#define RANDOM_NUMBERS 1000

#define KPMG_JOURNEYS 1



#define HWY_DIST		"\\jim\\nymtc\\data\\binary\\op_hwy_dist.binary"
#define HWY_TIME		"\\jim\\nymtc\\data\\binary\\op_hwy_time.binary"
#define HWY_TOLL		"\\jim\\nymtc\\data\\binary\\op_hwy_toll.binary"

#define WT_FARE			"\\jim\\nymtc\\data\\binary\\op_wt_fare.binary"
#define WT_FIRST_WAIT	"\\jim\\nymtc\\data\\binary\\op_wt_first_wait.binary"
#define WT_XFR_WAIT		"\\jim\\nymtc\\data\\binary\\op_wt_xfr_wait.binary"
#define WT_XFRS			"\\jim\\nymtc\\data\\binary\\op_wt_xfrs.binary"
#define WT_BUS_IVTT		"\\jim\\nymtc\\data\\binary\\op_wt_bus_ivtt.binary"
#define WT_EXPBUS_IVTT	"\\jim\\nymtc\\data\\binary\\op_wt_expbus_ivtt.binary"
#define WT_RAIL_IVTT	"\\jim\\nymtc\\data\\binary\\op_wt_rail_ivtt.binary"
#define WT_CR_IVTT		"\\jim\\nymtc\\data\\binary\\op_wt_cr_ivtt.binary"
#define WT_FERRY_IVTT	"\\jim\\nymtc\\data\\binary\\op_wt_ferry_ivtt.binary"
#define WT_ACC_OVTT		"\\jim\\nymtc\\data\\binary\\op_wt_acc_ovtt.binary"
#define WT_XFR_OVTT		"\\jim\\nymtc\\data\\binary\\op_wt_xfr_ovtt.binary"
#define WT_DR_ACC		"\\jim\\nymtc\\data\\binary\\op_wt_dr_acc.binary"

#define DT_FARE			"\\jim\\nymtc\\data\\binary\\op_dt_fare.binary"
#define DT_FIRST_WAIT	"\\jim\\nymtc\\data\\binary\\op_dt_first_wait.binary"
#define DT_XFR_WAIT		"\\jim\\nymtc\\data\\binary\\op_dt_xfr_wait.binary"
#define DT_XFRS			"\\jim\\nymtc\\data\\binary\\op_dt_xfrs.binary"
#define DT_BUS_IVTT		"\\jim\\nymtc\\data\\binary\\op_dt_bus_ivtt.binary"
#define DT_EXPBUS_IVTT	"\\jim\\nymtc\\data\\binary\\op_dt_expbus_ivtt.binary"
#define DT_RAIL_IVTT	"\\jim\\nymtc\\data\\binary\\op_dt_rail_ivtt.binary"
#define DT_CR_IVTT		"\\jim\\nymtc\\data\\binary\\op_dt_cr_ivtt.binary"
#define DT_FERRY_IVTT	"\\jim\\nymtc\\data\\binary\\op_dt_ferry_ivtt.binary"
#define DT_ACC_OVTT		"\\jim\\nymtc\\data\\binary\\op_dt_acc_ovtt.binary"
#define DT_XFR_OVTT		"\\jim\\nymtc\\data\\binary\\op_dt_xfr_ovtt.binary"
#define DT_DR_ACC		"\\jim\\nymtc\\data\\binary\\op_dt_dr_acc.binary"

#define WC_FARE			"\\jim\\nymtc\\data\\binary\\op_wc_fare.binary"
#define WC_FIRST_WAIT	"\\jim\\nymtc\\data\\binary\\op_wc_first_wait.binary"
#define WC_XFR_WAIT		"\\jim\\nymtc\\data\\binary\\op_wc_xfr_wait.binary"
#define WC_XFRS			"\\jim\\nymtc\\data\\binary\\op_wc_xfrs.binary"
#define WC_BUS_IVTT		"\\jim\\nymtc\\data\\binary\\op_wc_bus_ivtt.binary"
#define WC_EXPBUS_IVTT	"\\jim\\nymtc\\data\\binary\\op_wc_expbus_ivtt.binary"
#define WC_RAIL_IVTT	"\\jim\\nymtc\\data\\binary\\op_wc_rail_ivtt.binary"
#define WC_CR_IVTT		"\\jim\\nymtc\\data\\binary\\op_wc_cr_ivtt.binary"
#define WC_FERRY_IVTT	"\\jim\\nymtc\\data\\binary\\op_wc_ferry_ivtt.binary"
#define WC_ACC_OVTT		"\\jim\\nymtc\\data\\binary\\op_wc_acc_ovtt.binary"
#define WC_XFR_OVTT		"\\jim\\nymtc\\data\\binary\\op_wc_xfr_ovtt.binary"
#define WC_DR_ACC		"\\jim\\nymtc\\data\\binary\\op_wc_dr_acc.binary"

#define DC_FARE			"\\jim\\nymtc\\data\\binary\\op_dc_fare.binary"
#define DC_FIRST_WAIT	"\\jim\\nymtc\\data\\binary\\op_dc_first_wait.binary"
#define DC_XFR_WAIT		"\\jim\\nymtc\\data\\binary\\op_dc_xfr_wait.binary"
#define DC_XFRS			"\\jim\\nymtc\\data\\binary\\op_dc_xfrs.binary"
#define DC_BUS_IVTT		"\\jim\\nymtc\\data\\binary\\op_dc_bus_ivtt.binary"
#define DC_EXPBUS_IVTT	"\\jim\\nymtc\\data\\binary\\op_dc_expbus_ivtt.binary"
#define DC_RAIL_IVTT	"\\jim\\nymtc\\data\\binary\\op_dc_rail_ivtt.binary"
#define DC_CR_IVTT		"\\jim\\nymtc\\data\\binary\\op_dc_cr_ivtt.binary"
#define DC_FERRY_IVTT	"\\jim\\nymtc\\data\\binary\\op_dc_ferry_ivtt.binary"
#define DC_ACC_OVTT		"\\jim\\nymtc\\data\\binary\\op_dc_acc_ovtt.binary"
#define DC_XFR_OVTT		"\\jim\\nymtc\\data\\binary\\op_dc_xfr_ovtt.binary"
#define DC_DR_ACC		"\\jim\\nymtc\\data\\binary\\op_dc_dr_acc.binary"






extern int MAX_RANDOM;
extern FILE *fp_rep;
extern FILE *fp_ram;

extern HANDLE heapHandle;

int tc_status;
extern char *PurposeLabels[];
extern char *ModeLabel[];
extern char *ErrorCodes[];
extern char *BPMDist1Labels[];
extern char *PreModeLabels[];
extern char *UrbanTypeLabels[];
extern float MISSING;
extern int Memory;
extern int AvailMemory;
extern int Calibrating;
extern int max_hh_key, max_pers_key;
extern double mc_ivt_coeff;
extern clock_t start, finish;


// define a structure to contain all the information in the ini control file.
// Ini is declared as a pointer to this structure with global access
// so all functions can access the user specified values.
struct IniData {

// filenames	
	char INJOURNEYDATA[256];
	char JOURNEYSEQUENCE[256];
	char SUBAREA_JOURNEYSEQUENCE[256];
	char OUTPUTJOURNEYS[256];
	char MDSCOUTPUTJOURNEYS[256];
	char FROZEN_MDC_OUTPUT[256];
	char FROZEN_MDC_INPUT[256];
	char SUBAREA_REPORT[256];
	char SUBAREA_RAM_REPORT[256];
	char SUBAREA_MDC_OUTPUT[256];
	char SUBAREA_MDSC_OUTPUT_FULL[256];
	char SUBAREA_MDSC_OUTPUT_FROZEN[256];
	char SUBAREA_MDSC_OUTPUT_UNFROZEN[256];
	char SUBAREADATA[256];
	char SUBAREA_OUTPUT[256];
	char SUBAREA_CO_CO_REPORTFILE[256];
	char SUBAREA_WORKDESTFILE[256];
	char SUBAREA_ATWORK_LO_FILE[256];
	char SUBAREA_ATWORK_MD_FILE[256];
	char SUBAREA_ATWORK_HI_FILE[256];
	char FREEPARKPCTS[256];
	char PARKINGUSER[256];
	char JOURNEYS_WITH_STOPS[256];
	char WORKDESTFILE[256];
	char ATWORK_LO_FILE[256];
	char ATWORK_MD_FILE[256];
	char ATWORK_HI_FILE[256];
	char M_MSC_INDICES[256];
	char NM_MSC_INDICES[256];
	char M_MSCS[256];
	char NM_MSCS[256];
	char M_TARGETS[256];
	char NM_TARGETS[256];
	char DISTFACTORS[256];
	char REPORTFILE[256];
	char RAM_REPORTFILE[256];
	char CALIBREPORTFILE[256];
	char CO_CO_REPORTFILE[256];
	char RUNNING_MODE_SHARES[256];
	char DEBUGFILE[256];
	char ZONEDATA[256];
	char SCHOOLDISTDATA[256];
	char BPMDIST1COEFFDATA[256];
	char RINGDISTDATA[256];
	char WALKZONEDATA[256];
	char ATTRACTION_CORRECTIONS[256];
	char ATTR_PCT_DATA[256];
	char RIVERDATA[256];
	char TAXIDATA[256];
	char HIGHWAY_SOV_SKIM[256];
	char HIGHWAY_HOV2_SKIM[256];
	char HIGHWAY_HOV3p_SKIM[256];
	char HIGHWAY_TAXI_SKIM[256];
	char WALK_TRANSIT_SKIM[256];
	char DRIVE_TRANSIT_SKIM[256];
	char WALK_COMMRAIL_SKIM[256];
	char DRIVE_COMMRAIL_SKIM[256];

// matrix related parameters
	short BASE_INDEX;
	int HWY_INDEX_OFFSET;
	int TRAN_INDEX_OFFSET;
	int CACHE_SIZE;

// model parameters
	int PARKING_COST_IN_CENTS;		// if specified, and if 1, use parking cost values specified in cents, otherwise use values specified in dollars;
	int FTA_RESTART;				// if 1, run model using frozen dest choices and optionally frozen transit subzone and work tour mode choices from previous MDSC model run.  0 is normal operating mode.
	int FREEZE_MDC_OUTPUT;			// if 1, frozen MDC model output will be written so a subsequent FTA_RESTART model can be run.  0 is normal operating mode.
	int USE_FROZEN_SUBZONE;			// if 1, use frozen transit subzone choice.  0 is normal operating mode and a new choice will be made for each tour.
	int USE_FROZEN_WORKTOUR_MODE;	// if 1, use frozen work tour mode for this atwork tour.  0 is normal operating mode and the mode choice made for the work tour will be used.
	int ZERO_UTIL;					// use 0 (no) to include OD utility, 1 (yes) to omit OD utility for debugging
	int STOPS_ONLY;
	int MDC_ONLY;
	float PCT_LP_RESTRICTION;
	int SUBAREA_ANALYSIS;
	int LOW_COMPETITION;			// if 1, competition areas defined by default rules; if 0, use Peter's memo item 2.3.4 rules.
	int CORRECT_ORDER;				// if 0, use order stops models were originally calibrated to; if 1, use correct journey sequence.
	float DEGREE_OF_RELEVANCE;
	int INTRACOUNTY_STOPS_VAR;
	int RAM_DEBUG_LEVEL;
	int MAX_CALIB_ITERS;
	float CALIB_INCREMENT;
	float CALIB_SCALE;
	int RAND_SEED;
	int MAX_TAZS;
	int FREQ_ALTS;
	int NUMBER_ALTS;
	int NUMBER_COUNTIES;
	int ORIGINAL_JOURNEYS;
	int NUMBER_JOURNEYS;
	int NUMBER_BPMDIST1;
	int MAX_WORK_JOURNEYS;
	int PURPOSE_TO_PROCESS;
	int MAX_PACKET;
	int NUMBER_PACKETS;
	float ATTR_CONSTRAINT_FACTOR;
	int DEBUG_ORIG;
	int DEBUG_WALK_ORIG;
	int DEBUG_PURPOSE;
	int DEBUG_INCOME;
	int DEBUG_AUTOS;
	int DEBUG_WORKERS;
	int DEBUG_PERSON_TYPE;
	int DEBUG_PERCENT;
	int TRIP_LENGTH_RANGES;
	int TRIP_LENGTH_WIDTH;

// highway sov skims table indices
	short HWY_TIME_CORE;
	short HWY_DIST_CORE;
	short HWY_TOLL_CORE;

// highway hov skims table indices
	short HWY_HOV2_TIME_CORE;
	short HWY_HOV2_DIST_CORE;
	short HWY_HOV2_TOLL_CORE;
	short HWY_HOV3p_TIME_CORE;
	short HWY_HOV3p_DIST_CORE;
	short HWY_HOV3p_TOLL_CORE;
	short HWY_TAXI_TIME_CORE;
	short HWY_TAXI_DIST_CORE;
	short HWY_TAXI_TOLL_CORE;

// transit skims table indices
	short FARE_CORE;
	short FIRST_WAIT_CORE;
	short TOTAL_WAIT_CORE;
	short NUM_TRANSFERS_CORE;
	short BUS_IVTT_CORE;
	short EXPBUS_IVTT_CORE;
	short RAIL_IVTT_CORE;
	short CR_IVTT_CORE;
	short FERRY_IVTT_CORE;
	short ACCESS_OVTT_CORE;
	short EGRESS_OVTT_CORE;
	short TRANSFER_WALK_CORE;
	short AUTO_TIME_CORE;

// frequency choice specific constanrs
	double OB_FSC;
	double IB_FSC;
	double OBIB_FSC;

// other utility constants
	float AVG_AUTO_OCCUPANCY;
	float AUTO_OPERATING_COST;
	float HWY_CENTS_PER_MILE;
	float WALK_POTENTIAL_DIST;
	float WALK_CIRCUITY;
	float WALK_SPEED;

// intrazonal speeds by area type
	float* AUTO_SPEED;
	float BUS_FACTOR;

// terminal times by area type for hwy ovtt
	float TerminalTime[AREA_TYPES+1];
	
} ;
typedef struct IniData * INIDATA;
extern INIDATA Ini;

	
struct taxi_data {
	float **wait;
	float **pickup;
	float **cpm;
	float **surchg;
	float **toll;
};

struct river_crossing_data {
	int **east;
	int **hudson;
	int **minor;
};

struct zone_data {
// zonal attributes
	int *AreaType, *UrbanType, *County, *ring, *bpmdist1_index, *SchoolDist, *lpRestricted;
	int *OrigSubAreaIB, *DestSubAreaIB, *OrigSubAreaOB, *DestSubAreaOB;
	float *TotEmp, *LandArea, *HHPop, *HHNum, *UnvEnrol, *K12ETot, *empden, *PctFreePark, *NW_PctFreePark;
	float *RetEmp, *OffEmp, *PctAcc, *dist, *wPctIncNonMan, *nwPctIncNonMan, *wPctIncMan, *nwPctIncMan;
} ;

struct tran_data {
// transit skims row vectors
	float *Fare;
	float *First_wait;
	float *Xfr_wait;
	float *Xfrs;
	float *Bus_ivtt;
	float *ExpBus_ivtt;
	float *Rail_ivtt;
	float *CR_ivtt;
	float *Ferry_ivtt;
	float *Acc_ovtt;
	float *Xfr_ovtt;
	int *InfLot;
};

struct journey_attribs {
	short *orig, *purpose, *walk_orig, *income, *autos, *workers, *children, *person_type;
	short *ak, *aa, *at_work_mode, *persno, *nwas, *frozen, *autosAvail;					// autosAvail are the number of autos available to hh for travel, subject to rationing.
	int *hh, *packet, *haj;	
};

struct socec_data {
	int LowFew, LowSame, LowMore, LowNo;
	int MidFew, MidSame, MidMore, MidNo;
	int HiFew, HiSame, HiMore, HiNo;
	int aa, ak, noa, afw, asw, amw;
	int work, nwrk, inc1, inc2, inc3;
	int adult, at2co1, at3co1, at3co2, at3co3, at3co4, at4co;
};

struct avail_dests {
	short dest;
	float distance, walk_pct;
};

struct walk_zone_data {
	short num;
	struct avail_dests *walk_zones;
};

struct out_data {
	short orig, o_ring;
	short dest, d_ring;
	short purpose, mode;
	float o_dist, d_dist, distance;
	float od_util, se_util, logsum;
};

struct cluster_parts {
	int orig, purpose, walk_orig, income, auto_suff;
	int at_work_mode, person_type, aa, ak;
};

struct bpmdist1_coeff_data {
	float *intra_county, **co_co_coeffs;
	char **labels;
};

struct co_dist_factors {
	double coeff, exp;
};

struct dc_coeff_data {
	double alpha, beta, gamma[3], delta, theta;
};

struct msc_data {
	int nm_max_index, motorized_max_index;
	int *nm_indices, **motorized_indices;
	int *nm_index_used, *motorized_index_used;
	char **nm_labels, **motorized_labels;
	float *nm_targets, **motorized_targets;
	float *nmMSC, **MSC;
};

// Function Prototypes:
int avail_ram ();
int pct (int, int);
void isort (int *, int *, int);
void echo_ini (FILE *, FILE *, char *);
void ExitWithCode (int);
int binary_search (double, int, int, double *);
int indexed_binary_search (int, int, int, int *, int *);
void read_Targets (FILE *, float **);
void read_ini_control_file (FILE *, INIDATA);
void read_taxi_data (FILE *, struct taxi_data *);
void read_zone_data (FILE *, struct zone_data *);
void read_river_data (FILE *, struct river_crossing_data);
void read_walk_zone_data (FILE *, struct walk_zone_data *);
void read_bpmdist1_coeffs (FILE *, struct bpmdist1_coeff_data);
void read_school_district_data (FILE *, struct zone_data *);
void read_kpmg_journey_records (FILE *, FILE **, struct zone_data *, struct journey_attribs *, int **);
int getNumAutosRestricted (int hhAutoOwnership);
void read_ring_dist_data (FILE *, struct zone_data *);
void read_attr_corrections (FILE *, double *);
void read_dist_factors (FILE *, struct co_dist_factors *);
void read_motorized_MSCs (FILE *, struct msc_data *);
void read_motorized_MSC_indices (FILE *, struct msc_data *);
void read_motorized_targets (FILE *, struct msc_data *);
void read_nm_MSCs (FILE *, struct msc_data *);
void read_nm_MSC_indices (FILE *, struct msc_data *);
void read_nm_targets (FILE *, struct msc_data *);
void read_subarea_data (FILE *fp, struct zone_data *ZonalData);
void OD_Utilities (int, struct zone_data *, struct taxi_data *, int **, float ***, float ***);
void work_da_od_util  (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void work_sr2_od_util (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void work_sr3_od_util (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void work_sr4_od_util (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void work_wt_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void work_dt_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void work_wc_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void work_dc_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void work_tx_od_util  (int, int, float *, float *, float *, struct zone_data *, struct taxi_data *, float *);
void school_da_od_util  (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void school_sr2_od_util (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void school_sr3_od_util (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void school_sr4_od_util (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void school_wt_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void school_dt_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void school_wc_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void school_dc_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void school_tx_od_util  (int, int, float *, float *, float *, struct zone_data *, struct taxi_data *, float *);
void univ_da_od_util  (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void univ_sr2_od_util (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void univ_sr3_od_util (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void univ_sr4_od_util (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void univ_wt_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void univ_dt_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void univ_wc_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void univ_dc_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void univ_tx_od_util  (int, int, float *, float *, float *, struct zone_data *, struct taxi_data *, float *);
void maint_da_od_util  (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void maint_sr2_od_util (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void maint_sr3_od_util (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void maint_sr4_od_util (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void maint_wt_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void maint_dt_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void maint_wc_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void maint_dc_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void maint_tx_od_util  (int, int, float *, float *, float *, struct zone_data *, struct taxi_data *, float *);
void discr_da_od_util  (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void discr_sr2_od_util (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void discr_sr3_od_util (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void discr_sr4_od_util (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void discr_wt_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void discr_dt_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void discr_wc_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void discr_dc_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void discr_tx_od_util  (int, int, float *, float *, float *, struct zone_data *, struct taxi_data *, float *);
void atwork_da_od_util  (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void atwork_sr2_od_util (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void atwork_sr3_od_util (int, int, float *, float *, float *, struct zone_data *, int **, float *);
//void atwork_sr4_od_util (int, int, float **, float **, float **, struct zone_data *, int **, float *);
void atwork_wt_od_util  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
//void atwork_dt_od_util  (int, int, float **, float **, float **, struct tran_data *, struct zone_data *, float *);
//void atwork_wc_od_util  (int, int, float **, float **, float **, struct tran_data *, struct zone_data *, float *);
//void atwork_dc_od_util  (int, int, float **, float **, float **, struct tran_data *, struct zone_data *, float *);
void atwork_tx_od_util  (int, int, float *, float *, float *, struct zone_data *, struct taxi_data *, float *);
void SE_Utilities (int, struct journey_attribs *, struct zone_data *, float *);
void SE_UtilitiesRestricted (int, struct journey_attribs *, struct zone_data *, float *);
float work_sr2_se_util (struct socec_data *);
float work_sr3_se_util (struct socec_data *);
float work_sr4_se_util (struct socec_data *);
float work_wt_se_util (struct socec_data *);
float work_dt_se_util (struct socec_data *);
float work_wc_se_util (struct socec_data *);
float work_dc_se_util (struct socec_data *);
float work_tx_se_util (struct socec_data *);
float school_sr2_se_util (struct socec_data *);
float school_sr3_se_util (struct socec_data *);
float school_sr4_se_util (struct socec_data *);
float school_wt_se_util (struct socec_data *);
float school_dt_se_util (struct socec_data *);
float school_wc_se_util (struct socec_data *);
float school_dc_se_util (struct socec_data *);
float school_tx_se_util (struct socec_data *);
float school_sb_se_util (struct socec_data *);
float univ_sr2_se_util (struct socec_data *);
float univ_sr3_se_util (struct socec_data *);
float univ_sr4_se_util (struct socec_data *);
float univ_wt_se_util (struct socec_data *);
float univ_dt_se_util (struct socec_data *);
float univ_wc_se_util (struct socec_data *);
float univ_dc_se_util (struct socec_data *);
float univ_tx_se_util (struct socec_data *);
float maint_sr2_se_util (struct socec_data *);
float maint_sr3_se_util (struct socec_data *);
float maint_sr4_se_util (struct socec_data *);
float maint_wt_se_util (struct socec_data *);
float maint_dt_se_util (struct socec_data *);
float maint_wc_se_util (struct socec_data *);
float maint_dc_se_util (struct socec_data *);
float maint_tx_se_util (struct socec_data *);
float discr_sr2_se_util (struct socec_data *);
float discr_sr3_se_util (struct socec_data *);
float discr_sr4_se_util (struct socec_data *);
float discr_wt_se_util (struct socec_data *);
float discr_dt_se_util (struct socec_data *);
float discr_wc_se_util (struct socec_data *);
float discr_dc_se_util (struct socec_data *);
float discr_tx_se_util (struct socec_data *);
float atwork_sr2_se_util (struct socec_data *);
float atwork_sr3_se_util (struct socec_data *);
//float atwork_sr4_se_util (struct socec_data *);
float atwork_wt_se_util (struct socec_data *);
//float atwork_dt_se_util (struct socec_data *);
//float atwork_wc_se_util (struct socec_data *);
//float atwork_dc_se_util (struct socec_data *);
float atwork_tx_se_util (struct socec_data *);
void set_function_pointers ();
void GetSkims (char *, int, int, short, MATRIX, int, float **);
void GetSkimsByRow (int, short, MATRIX, int, float *);
int get_motorized_MSC_index (int, int, struct zone_data *, struct msc_data *);
int get_nm_MSC_index (int, struct zone_data *, struct msc_data *);
void get_avail_dests (FILE *, int **);
void work_attractions (FILE *, struct zone_data *, float **);
void balance_attractions (double *, float ***, float **, int **, struct zone_data *);
int set_cluster (short, short, short, short, short, short, short, short, short);
void get_cluster_parts (int, struct cluster_parts);
void cluster_journeys (struct journey_attribs *, int **, int **);
float parking_cost (int, int, struct zone_data *, int **);
float nw_parking_cost (int, int, struct zone_data *, int **);
void percent_free_parking (struct zone_data *);
void write_free_parking (FILE* fp, struct zone_data *ZonalData);
void parking_cost_index_lookup_table (int **);
void final_reports (FILE *, int **, double *, int **, int **, double *, int ***, float **, struct zone_data *, int);
void calibration_report (int, FILE *, struct msc_data *, float*, int *, float **, float **, float *, float **, float);
void count_kpmg_journey_records (FILE *fp);
void get_input_data (FILE **, FILE **, FILE **, FILE **, FILE *[], int **,
			struct taxi_data *, struct zone_data *, struct river_crossing_data *,
			struct walk_zone_data **, struct bpmdist1_coeff_data *, struct co_dist_factors *, struct msc_data **);
void get_journey_data (FILE **fp_work, struct zone_data *ZonalData, struct journey_attribs *JourneyAttribs, int **TotProds);
void run_mdc (FILE *, FILE *, FILE *, FILE **, struct msc_data *, float **, float **, float **, float ***, int **,
			struct journey_attribs *, struct zone_data *, struct river_crossing_data, struct taxi_data *,
			struct walk_zone_data *, struct bpmdist1_coeff_data, struct co_dist_factors *);
void logsum_check (int, int, int, int, double, double, double, double, float *, float *, float *);
int pre_mode_choice (int, float, struct journey_attribs *, float **, float **, struct zone_data *, struct walk_zone_data *, struct msc_data *);
void debug_props (int, struct dc_coeff_data *, struct journey_attribs *, struct zone_data *, float ***, float ***, struct river_crossing_data, float **, struct bpmdist1_coeff_data, struct msc_data *);
void motor_dc_coeffs (int, int, struct dc_coeff_data *, struct co_dist_factors *);
void motor_dc_props   (int, int, struct dc_coeff_data *, float ***, float *, float **, float***, struct river_crossing_data, struct zone_data *, struct bpmdist1_coeff_data, double *, float *, int, short);
float nm_dest_gamma (int, int, int);
double nm_log_density (int, int, float, float *, int *, struct walk_zone_data *);
int nm_dest_choice (int, struct walk_zone_data *, double *);
void nm_dest_props (int, int, float, float *, int *, struct walk_zone_data *, double *);
void work_mc_logsum   (int, int, int, int *, int, int, int *, int, float *, float *, float *, float *);
void school_mc_logsum (int, int, int, int *, int, int, int *, int, float *, float *, float *, float *);
void univ_mc_logsum   (int, int, int, int *, int, int, int *, int, float *, float *, float *, float *);
void maint_mc_logsum  (int, int, int, int *, int, int, int *, int, float *, float *, float *, float *);
void discr_mc_logsum  (int, int, int, int *, int, int, int *, int, float *, float *, float *, float *);
void atwork_mc_logsum (int, int, int, int *, int, int, int *, int, float *, float *, float *, float *);
void work_mc_props    (int, int *, int, int, int *, int, float *, float *, float *, float *, float *);
void school_mc_props  (int, int *, int, int, int *, int, float *, float *, float *, float *, float *);
void univ_mc_props    (int, int *, int, int, int *, int, float *, float *, float *, float *, float *);
void maint_mc_props   (int, int *, int, int, int *, int, float *, float *, float *, float *, float *);
void discr_mc_props   (int, int *, int, int, int *, int, float *, float *, float *, float *, float *);
void atwork_mc_props (int, int *, int, int, int *, int, float *, float *, float *, float *, float *);
int work_motorized_choice   (int, int, int, int, int, int, int, int, double, float);
int school_motorized_choice (int, int, int, int, int, int, int, int, double, float);
int univ_motorized_choice   (int, int, int, int, int, int, int, int, double, float);
int maint_motorized_choice  (int, int, int, int, int, int, int, int, double, float);
int discr_motorized_choice  (int, int, int, int, int, int, int, int, double, float);
int atwork_motorized_choice (int, int, int, int, int, int, int, int, double, float);
void work_attractions   (FILE *, struct zone_data *, float **);
void school_attractions (FILE *, struct zone_data *, float **);
void univ_attractions   (FILE *, struct zone_data *, float **);
void maint_attractions  (FILE *, struct zone_data *, float **);
void discr_attractions  (FILE *, struct zone_data *, float **);
void atwork_attractions (FILE *, struct zone_data *, float **);

void work_freq_util (int i, double obDensity, double ibDensity, double ivtt, int *orig, int *dest, int *mode,
	struct journey_attribs *JourneyAttribs, struct zone_data *ZonalData, double *freqUtil, int *freqAvail,
	short *hh_freqs, short **hh_pers_freqs, int *hh2id);
void school_freq_util (int i, double obDensity, double ibDensity, double hwy_dist, int *orig, int *dest, int *mode,
	struct journey_attribs *JourneyAttribs, struct zone_data *ZonalData, double *freqUtil, int *freqAvail,
	short *hh_freqs, short **hh_pers_freqs, int *hh2id);
void univ_freq_util (int i, double obDensity, double ibDensity, int *orig, int *dest, int *mode,
	struct journey_attribs *JourneyAttribs, struct zone_data *ZonalData, double *freqUtil, int *freqAvail,
	short *hh_freqs, short **hh_pers_freqs, int *hh2id);
void maint_freq_util (int i, double obDensity, double ibDensity, double ivtt, double hwy_dist,
	int *orig, int *dest, int *mode, struct journey_attribs *JourneyAttribs, struct zone_data *ZonalData,
	double *freqUtil, int *freqAvail, short **hh_pers_freqs, int *hh2id);
void discr_freq_util (int i, double obDensity, double ibDensity, int *orig, int *dest, int *mode,
	struct journey_attribs *JourneyAttribs, struct zone_data *ZonalData, double *freqUtil, int *freqAvail,
	short **hh_pers_freqs, int *hh2id);
void atwork_freq_util (int i, double obDensity, double ibDensity, int *orig, int *dest, int *mode,
	struct journey_attribs *JourneyAttribs, double *freqUtil, int *freqAvail);

void stop_location_avail (int orig, int dest, int mode, struct zone_data *ZonalData, float **hwy_dist, float **wt_ivt, float **dt_ivt,
						  float **wc_ivt, float **dc_ivt, int *near_orig, int *near_dest, int *midway);
void stops (FILE *fp3, int *orig, int *dest, int *mode, struct journey_attribs *, struct zone_data *ZonalData, struct river_crossing_data RiverData);
void stops_reports (int *ob_mode_freqs, int *ib_mode_freqs, int *obib_mode_freqs, int *none_mode_freqs, int **ob_area_freqs, int **ib_area_freqs, int **obib_area_freqs, int **none_area_freqs, int *ob_district_freqs, int *ib_district_freqs, double *ob_district_size, double *ib_district_size);
void get_stop_size (double **ob_stop_size, double **ib_stop_size, struct zone_data *ZonalData);
void read_od_skims (float ***hwy_dist_util, float ***hwy_dist, float ***hwy_time, float ***wt_ivt, float ***dt_ivt, float ***wc_ivt, float ***dc_ivt, struct zone_data *ZonalData);
int set_stops_cluster (int orig, int dest, int mode);

void work_jrny_freqs   (struct journey_attribs *JourneyAttribs, short **hh_freqs, short ***hh_pers_freqs, int **hh2id);
void school_jrny_freqs (struct journey_attribs *JourneyAttribs, short **hh_freqs, short ***hh_pers_freqs, int **hh2id);
void univ_jrny_freqs   (struct journey_attribs *JourneyAttribs, short **hh_freqs, short ***hh_pers_freqs, int **hh2id);
void maint_jrny_freqs  (struct journey_attribs *JourneyAttribs, short ***hh_pers_freqs, int **hh2id);
void discr_jrny_freqs  (struct journey_attribs *JourneyAttribs, short ***hh_pers_freqs, int **hh2id);

void modify_dist_skims (float **hwy_dist_util, float **hwy_dist, float **hwy_time, struct zone_data *ZonalData, struct river_crossing_data RiverData);
void modify_dist_coeffs (struct dc_coeff_data *DCcoeff);
int ValidStop (int idist, int jdist, int kdist);

void addRAM (char *function, int size);
void relRAM (char *function, int size);

void competition_areas (struct zone_data *ZonalData);
void link_atwork (int *hh_pers, int *index, int work_journeys, short *mode, short *dest, int hh, int persno,
				  int *work_mode, int *work_dest);


// Function Pointers:
void (*mc_logsum[PURPOSES]) (int, int, int, int *, int, int, int *, int, float *, float *, float *, float *);
//float (*mc_logsum[PURPOSES]) (int, int, int, int *, int, int, int *, int, float *, float *, float *);
void  (*mc_props[PURPOSES])  (int, int *, int, int, int *, int, float *, float *, float*, float *, float *);
int   (*motorized_choice[PURPOSES])  (int, int, int, int, int, int, int, int, double, float);
void  (*get_attractions[PURPOSES]) (FILE *, struct zone_data *, float **);

void  (*da_od_utility[PURPOSES])  (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void  (*sr2_od_utility[PURPOSES]) (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void  (*sr3_od_utility[PURPOSES]) (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void  (*sr4_od_utility[PURPOSES]) (int, int, float *, float *, float *, struct zone_data *, int **, float *);
void  (*wt_od_utility[PURPOSES])  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void  (*dt_od_utility[PURPOSES])  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void  (*wc_od_utility[PURPOSES])  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void  (*dc_od_utility[PURPOSES])  (int, int, float *, float *, float *, struct tran_data *, struct zone_data *, float *);
void  (*tx_od_utility[PURPOSES])  (int, int, float *, float *, float *, struct zone_data *, struct taxi_data *, float *);

float  (*sr2_se_utility[PURPOSES]) (struct socec_data *);
float  (*sr3_se_utility[PURPOSES]) (struct socec_data *);
float  (*sr4_se_utility[PURPOSES]) (struct socec_data *);
float  (*wt_se_utility[PURPOSES]) (struct socec_data *);
float  (*dt_se_utility[PURPOSES]) (struct socec_data *);
float  (*wc_se_utility[PURPOSES]) (struct socec_data *);
float  (*dc_se_utility[PURPOSES]) (struct socec_data *);
float  (*tx_se_utility[PURPOSES]) (struct socec_data *);
