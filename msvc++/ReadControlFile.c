#include "md.h"


void read_ini_control_file (FILE *fp, INIDATA Ini)
{

	char *temp;


// read the file names to use for input data and output data and reports

	if ((temp = GetStringFromINI (fp, "FILES", "INJOURNEYDATA")) != NULL)
		strcpy (Ini->INJOURNEYDATA, temp);
	else
		strcpy (Ini->INJOURNEYDATA, "");

	if ((temp = GetStringFromINI (fp, "FILES", "JOURNEYSEQUENCE")) != NULL)
		strcpy (Ini->JOURNEYSEQUENCE, temp);
	else
		strcpy (Ini->JOURNEYSEQUENCE, "");

	if ((temp = GetStringFromINI (fp, "FILES", "SUBAREA_JOURNEYSEQUENCE")) != NULL)
		strcpy (Ini->SUBAREA_JOURNEYSEQUENCE, temp);
	else
		strcpy (Ini->SUBAREA_JOURNEYSEQUENCE, "");

	if ((temp = GetStringFromINI (fp, "FILES", "MDSCOUTPUTJOURNEYS")) != NULL)
		strcpy (Ini->MDSCOUTPUTJOURNEYS, temp);
	else
		strcpy (Ini->MDSCOUTPUTJOURNEYS, "");

	if ((temp = GetStringFromINI (fp, "FILES", "FROZEN_MDC_OUTPUT")) != NULL)
		strcpy (Ini->FROZEN_MDC_OUTPUT, temp);
	else
		strcpy (Ini->FROZEN_MDC_OUTPUT, "");

	if ((temp = GetStringFromINI (fp, "FILES", "FROZEN_MDC_INPUT")) != NULL)
		strcpy (Ini->FROZEN_MDC_INPUT, temp);
	else
		strcpy (Ini->FROZEN_MDC_INPUT, "");

	if ((temp = GetStringFromINI (fp, "FILES", "SUBAREA_OUTPUT")) != NULL)
		strcpy (Ini->SUBAREA_OUTPUT, temp);
	else
		strcpy (Ini->SUBAREA_OUTPUT, "");

	if ((temp = GetStringFromINI (fp, "FILES", "SUBAREA_REPORT")) != NULL)
		strcpy (Ini->SUBAREA_REPORT, temp);
	else
		strcpy (Ini->SUBAREA_REPORT, "");

	if ((temp = GetStringFromINI (fp, "FILES", "SUBAREA_MDC_OUTPUT")) != NULL)
		strcpy (Ini->SUBAREA_MDC_OUTPUT, temp);
	else
		strcpy (Ini->SUBAREA_MDC_OUTPUT, "");

	if ((temp = GetStringFromINI (fp, "FILES", "SUBAREA_MDSC_OUTPUT_FULL")) != NULL)
		strcpy (Ini->SUBAREA_MDSC_OUTPUT_FULL, temp);
	else
		strcpy (Ini->SUBAREA_MDSC_OUTPUT_FULL, "");

	if ((temp = GetStringFromINI (fp, "FILES", "SUBAREA_MDSC_OUTPUT_FROZEN")) != NULL)
		strcpy (Ini->SUBAREA_MDSC_OUTPUT_FROZEN, temp);
	else
		strcpy (Ini->SUBAREA_MDSC_OUTPUT_FROZEN, "");

	if ((temp = GetStringFromINI (fp, "FILES", "SUBAREA_MDSC_OUTPUT_UNFROZEN")) != NULL)
		strcpy (Ini->SUBAREA_MDSC_OUTPUT_UNFROZEN, temp);
	else
		strcpy (Ini->SUBAREA_MDSC_OUTPUT_UNFROZEN, "");

	if ((temp = GetStringFromINI (fp, "FILES", "SUBAREA_CO_CO_REPORTFILE")) != NULL)
		strcpy (Ini->SUBAREA_CO_CO_REPORTFILE, temp);
	else
		strcpy (Ini->SUBAREA_CO_CO_REPORTFILE, "");

	if ((temp = GetStringFromINI (fp, "FILES", "SUBAREA_WORKDESTFILE")) != NULL)
		strcpy (Ini->SUBAREA_WORKDESTFILE, temp);
	else
		strcpy (Ini->SUBAREA_WORKDESTFILE, "");

	if ((temp = GetStringFromINI (fp, "FILES", "SUBAREA_ATWORK_LO_FILE")) != NULL)
		strcpy (Ini->SUBAREA_ATWORK_LO_FILE, temp);
	else
		strcpy (Ini->SUBAREA_ATWORK_LO_FILE, "");

	if ((temp = GetStringFromINI (fp, "FILES", "SUBAREA_ATWORK_MD_FILE")) != NULL)
		strcpy (Ini->SUBAREA_ATWORK_MD_FILE, temp);
	else
		strcpy (Ini->SUBAREA_ATWORK_MD_FILE, "");

	if ((temp = GetStringFromINI (fp, "FILES", "SUBAREA_ATWORK_HI_FILE")) != NULL)
		strcpy (Ini->SUBAREA_ATWORK_HI_FILE, temp);
	else
		strcpy (Ini->SUBAREA_ATWORK_HI_FILE, "");

	if ((temp = GetStringFromINI (fp, "FILES", "FREEPARKPCTS")) != NULL)
		strcpy (Ini->FREEPARKPCTS, temp);
	else
		strcpy (Ini->FREEPARKPCTS, "");

	if ((temp = GetStringFromINI (fp, "FILES", "PARKINGUSER")) != NULL)
		strcpy (Ini->PARKINGUSER, temp);
	else
		strcpy (Ini->PARKINGUSER, "");

	if ((temp = GetStringFromINI (fp, "FILES", "PARKINGCOSTS")) != NULL)
		strcpy (Ini->PARKINGCOSTS, temp);
	else
		strcpy (Ini->PARKINGCOSTS, "");

	if ((temp = GetStringFromINI (fp, "FILES", "OUTPUTJOURNEYS")) != NULL)
		strcpy (Ini->OUTPUTJOURNEYS, temp);
	else
		strcpy (Ini->OUTPUTJOURNEYS, "");

	if ((temp = GetStringFromINI (fp, "FILES", "SUBAREADATA")) != NULL)
		strcpy (Ini->SUBAREADATA, temp);
	else
		strcpy (Ini->SUBAREADATA, "");

	if ((temp = GetStringFromINI (fp, "FILES", "JOURNEYS_WITH_STOPS")) != NULL)
		strcpy (Ini->JOURNEYS_WITH_STOPS, temp);
	else
		strcpy (Ini->JOURNEYS_WITH_STOPS, "");

	if ((temp = GetStringFromINI (fp, "FILES", "WORKDESTFILE")) != NULL)
		strcpy (Ini->WORKDESTFILE, temp);
	else
		strcpy (Ini->WORKDESTFILE, "");

	if ((temp = GetStringFromINI (fp, "FILES", "M_MSC_INDICES")) != NULL)
		strcpy (Ini->M_MSC_INDICES, temp);
	else
		strcpy (Ini->M_MSC_INDICES, "");

	if ((temp = GetStringFromINI (fp, "FILES", "NM_MSC_INDICES")) != NULL)
		strcpy (Ini->NM_MSC_INDICES, temp);
	else
		strcpy (Ini->NM_MSC_INDICES, "");

	if ((temp = GetStringFromINI (fp, "FILES", "M_MSCS")) != NULL)
		strcpy (Ini->M_MSCS, temp);
	else
		strcpy (Ini->M_MSCS, "");

	if ((temp = GetStringFromINI (fp, "FILES", "NM_MSCS")) != NULL)
		strcpy (Ini->NM_MSCS, temp);
	else
		strcpy (Ini->NM_MSCS, "");

	if ((temp = GetStringFromINI (fp, "FILES", "M_TARGETS")) != NULL)
		strcpy (Ini->M_TARGETS, temp);
	else
		strcpy (Ini->M_TARGETS, "");

	if ((temp = GetStringFromINI (fp, "FILES", "NM_TARGETS")) != NULL)
		strcpy (Ini->NM_TARGETS, temp);
	else
		strcpy (Ini->NM_TARGETS, "");

	if ((temp = GetStringFromINI (fp, "FILES", "DISTFACTORS")) != NULL)
		strcpy (Ini->DISTFACTORS, temp);
	else
		strcpy (Ini->DISTFACTORS, "");

	if ((temp = GetStringFromINI (fp, "FILES", "ATWORK_LO_FILE")) != NULL)
		strcpy (Ini->ATWORK_LO_FILE, temp);
	else
		strcpy (Ini->ATWORK_LO_FILE, "");

	if ((temp = GetStringFromINI (fp, "FILES", "ATWORK_MD_FILE")) != NULL)
		strcpy (Ini->ATWORK_MD_FILE, temp);
	else
		strcpy (Ini->ATWORK_MD_FILE, "");

	if ((temp = GetStringFromINI (fp, "FILES", "ATWORK_HI_FILE")) != NULL)
		strcpy (Ini->ATWORK_HI_FILE, temp);
	else
		strcpy (Ini->ATWORK_HI_FILE, "");

	if ((temp = GetStringFromINI (fp, "FILES", "REPORTFILE")) != NULL)
		strcpy (Ini->REPORTFILE, temp);
	else
		strcpy (Ini->REPORTFILE, "");

	if ((temp = GetStringFromINI (fp, "FILES", "CALIBREPORTFILE")) != NULL)
		strcpy (Ini->CALIBREPORTFILE, temp);
	else
		strcpy (Ini->CALIBREPORTFILE, "");

	if ((temp = GetStringFromINI (fp, "FILES", "CO_CO_REPORTFILE")) != NULL)
		strcpy (Ini->CO_CO_REPORTFILE, temp);
	else
		strcpy (Ini->CO_CO_REPORTFILE, "");

	if ((temp = GetStringFromINI (fp, "FILES", "RUNNING_MODE_SHARES")) != NULL)
		strcpy (Ini->RUNNING_MODE_SHARES, temp);
	else
		strcpy (Ini->RUNNING_MODE_SHARES, "");

	if ((temp = GetStringFromINI (fp, "FILES", "DEBUGFILE")) != NULL)
		strcpy (Ini->DEBUGFILE, temp);
	else
		strcpy (Ini->DEBUGFILE, "");

	if ((temp = GetStringFromINI (fp, "FILES", "BPMDIST1COEFFDATA")) != NULL)
		strcpy (Ini->BPMDIST1COEFFDATA, temp);
	else
		strcpy (Ini->BPMDIST1COEFFDATA, "");

	if ((temp = GetStringFromINI (fp, "FILES", "ATTRACTION_CORRECTIONS")) != NULL)
		strcpy (Ini->ATTRACTION_CORRECTIONS, temp);
	else
		strcpy (Ini->ATTRACTION_CORRECTIONS, "");

	if ((temp = GetStringFromINI (fp, "FILES", "ATTR_PCT_DATA")) != NULL)
		strcpy (Ini->ATTR_PCT_DATA, temp);
	else
		strcpy (Ini->ATTR_PCT_DATA, "");

	if ((temp = GetStringFromINI (fp, "FILES", "SCHOOLDISTDATA")) != NULL)
		strcpy (Ini->SCHOOLDISTDATA, temp);
	else
		strcpy (Ini->SCHOOLDISTDATA, "");

	if ((temp = GetStringFromINI (fp, "FILES", "WALKZONEDATA")) != NULL)
		strcpy (Ini->WALKZONEDATA, temp);
	else
		strcpy (Ini->WALKZONEDATA, "");

	if ((temp = GetStringFromINI (fp, "FILES", "ZONEDATA")) != NULL)
		strcpy (Ini->ZONEDATA, temp);
	else
		strcpy (Ini->ZONEDATA, "");

	if ((temp = GetStringFromINI (fp, "FILES", "RINGDISTDATA")) != NULL)
		strcpy (Ini->RINGDISTDATA, temp);
	else
		strcpy (Ini->RINGDISTDATA, "");

	if ((temp = GetStringFromINI (fp, "FILES", "RIVERDATA")) != NULL)
		strcpy (Ini->RIVERDATA, temp);
	else
		strcpy (Ini->RIVERDATA, "");

	if ((temp = GetStringFromINI (fp, "FILES", "TAXIDATA")) != NULL)
		strcpy (Ini->TAXIDATA, temp);
	else
		strcpy (Ini->TAXIDATA, "");

	if ((temp = GetStringFromINI (fp, "FILES", "HIGHWAY_SOV_SKIM")) != NULL)
		strcpy (Ini->HIGHWAY_SOV_SKIM, temp);
	else
		strcpy (Ini->HIGHWAY_SOV_SKIM, "");

	if ((temp = GetStringFromINI (fp, "FILES", "HIGHWAY_HOV2_SKIM")) != NULL)
		strcpy (Ini->HIGHWAY_HOV2_SKIM, temp);
	else
		strcpy (Ini->HIGHWAY_HOV2_SKIM, "");

	if ((temp = GetStringFromINI (fp, "FILES", "HIGHWAY_HOV3p_SKIM")) != NULL)
		strcpy (Ini->HIGHWAY_HOV3p_SKIM, temp);
	else
		strcpy (Ini->HIGHWAY_HOV3p_SKIM, "");

	if ((temp = GetStringFromINI (fp, "FILES", "HIGHWAY_TAXI_SKIM")) != NULL)
		strcpy (Ini->HIGHWAY_TAXI_SKIM, temp);
	else
		strcpy (Ini->HIGHWAY_TAXI_SKIM, "");

	if ((temp = GetStringFromINI (fp, "FILES", "WALK_TRANSIT_SKIM")) != NULL)
		strcpy (Ini->WALK_TRANSIT_SKIM, temp);
	else
		strcpy (Ini->WALK_TRANSIT_SKIM, "");

	if ((temp = GetStringFromINI (fp, "FILES", "DRIVE_TRANSIT_SKIM")) != NULL)
		strcpy (Ini->DRIVE_TRANSIT_SKIM, temp);
	else
		strcpy (Ini->DRIVE_TRANSIT_SKIM, "");

	if ((temp = GetStringFromINI (fp, "FILES", "WALK_COMMRAIL_SKIM")) != NULL)
		strcpy (Ini->WALK_COMMRAIL_SKIM, temp);
	else
		strcpy (Ini->WALK_COMMRAIL_SKIM, "");

	if ((temp = GetStringFromINI (fp, "FILES", "DRIVE_COMMRAIL_SKIM")) != NULL)
		strcpy (Ini->DRIVE_COMMRAIL_SKIM, temp);
	else
		strcpy (Ini->DRIVE_COMMRAIL_SKIM, "");


// Matrix I/O Parameters
	Ini->BASE_INDEX			= (short)GetIntFromINI (fp, "MATRIX_PARAMS", "BASE_INDEX");
	Ini->HWY_INDEX_OFFSET	= GetIntFromINI (fp, "MATRIX_PARAMS", "HWY_INDEX_OFFSET");
	Ini->TRAN_INDEX_OFFSET	= GetIntFromINI (fp, "MATRIX_PARAMS", "TRAN_INDEX_OFFSET");
	Ini->CACHE_SIZE			= GetIntFromINI (fp, "MATRIX_PARAMS", "CACHE_SIZE");

// Model Parameters
	Ini->FTA_RESTART			= GetIntFromINI (fp, "PARAMS", "FTA_RESTART");
	Ini->FREEZE_MDC_OUTPUT		= GetIntFromINI (fp, "PARAMS", "FREEZE_MDC_OUTPUT");
	Ini->USE_FROZEN_SUBZONE			= GetIntFromINI (fp, "PARAMS", "USE_FROZEN_SUBZONE");
	Ini->USE_FROZEN_WORKTOUR_MODE	= GetIntFromINI (fp, "PARAMS", "USE_FROZEN_WORKTOUR_MODE");
	Ini->ZERO_UTIL				= GetIntFromINI (fp, "PARAMS", "ZERO_UTIL");
	Ini->INTRACOUNTY_STOPS_VAR	= GetIntFromINI (fp, "PARAMS", "INTRACOUNTY_STOPS_VAR");
	Ini->STOPS_ONLY				= GetIntFromINI (fp, "PARAMS", "STOPS_ONLY");
	Ini->MDC_ONLY				= GetIntFromINI (fp, "PARAMS", "MDC_ONLY");
	Ini->SUBAREA_ANALYSIS		= GetIntFromINI (fp, "PARAMS", "SUBAREA_ANALYSIS");
	Ini->PCT_LP_RESTRICTION		= GetFloatFromINI (fp, "PARAMS", "PCT_LP_RESTRICTION");
	Ini->PCT_HHS_WITH_FULL_AUTO_RESTRICTION		= GetFloatFromINI (fp, "PARAMS", "PCT_HHS_WITH_FULL_AUTO_RESTRICTION");
	Ini->LOW_COMPETITION		= GetIntFromINI (fp, "PARAMS", "LOW_COMPETITION");
	Ini->CORRECT_ORDER			= GetIntFromINI (fp, "PARAMS", "CORRECT_ORDER");
	Ini->DEGREE_OF_RELEVANCE	= GetFloatFromINI (fp, "PARAMS", "DEGREE_OF_RELEVANCE");
	Ini->RAM_DEBUG_LEVEL		= GetIntFromINI (fp, "PARAMS", "RAM_DEBUG_LEVEL");
	Ini->MAX_CALIB_ITERS		= GetIntFromINI (fp, "PARAMS", "MAX_CALIB_ITERS");
	Ini->CALIB_INCREMENT		= GetFloatFromINI (fp, "PARAMS", "CALIB_INCREMENT");
	Ini->CALIB_SCALE			= GetFloatFromINI (fp, "PARAMS", "CALIB_SCALE");
	Ini->RAND_SEED				= GetIntFromINI (fp, "PARAMS", "RAND_SEED");
	Ini->MAX_TAZS				= GetIntFromINI (fp, "PARAMS", "MAX_TAZS");
	Ini->FREQ_ALTS				= GetIntFromINI (fp, "PARAMS", "FREQ_ALTS");
	Ini->NUMBER_ALTS			= GetIntFromINI (fp, "PARAMS", "NUMBER_ALTS");
	Ini->NUMBER_COUNTIES		= GetIntFromINI (fp, "PARAMS", "NUMBER_COUNTIES");
	Ini->NUMBER_BPMDIST1		= GetIntFromINI (fp, "PARAMS", "NUMBER_BPMDIST1");
	Ini->NUMBER_JOURNEYS		= GetIntFromINI (fp, "PARAMS", "NUMBER_JOURNEYS");
	Ini->PURPOSE_TO_PROCESS		= GetIntFromINI (fp, "PARAMS", "PURPOSE_TO_PROCESS") - 1;
	Ini->MAX_PACKET				= GetIntFromINI (fp, "PARAMS", "MAX_PACKET");
	Ini->MAX_WORK_JOURNEYS		= GetIntFromINI (fp, "PARAMS", "MAX_WORK_JOURNEYS");
	Ini->ATTR_CONSTRAINT_FACTOR = GetFloatFromINI (fp, "PARAMS", "ATTR_CONSTRAINT_FACTOR");
	Ini->DEBUG_ORIG				= GetIntFromINI (fp, "PARAMS", "DEBUG_ORIG");
	Ini->DEBUG_WALK_ORIG		= GetIntFromINI (fp, "PARAMS", "DEBUG_WALK_ORIG");
	Ini->DEBUG_PURPOSE			= GetIntFromINI (fp, "PARAMS", "DEBUG_PURPOSE") - 1;
	Ini->DEBUG_INCOME			= GetIntFromINI (fp, "PARAMS", "DEBUG_INCOME");
	Ini->DEBUG_AUTOS			= GetIntFromINI (fp, "PARAMS", "DEBUG_AUTOS");
	Ini->DEBUG_WORKERS			= GetIntFromINI (fp, "PARAMS", "DEBUG_WORKERS");
	Ini->DEBUG_PERSON_TYPE		= GetIntFromINI (fp, "PARAMS", "DEBUG_PERSON_TYPE");
	Ini->DEBUG_PERCENT			= GetIntFromINI (fp, "PARAMS", "DEBUG_PERCENT");
	Ini->TRIP_LENGTH_RANGES		= GetIntFromINI (fp, "PARAMS", "TRIP_LENGTH_RANGES");
	Ini->TRIP_LENGTH_WIDTH		= GetIntFromINI (fp, "PARAMS", "TRIP_LENGTH_WIDTH");

	Ini->OB_FSC					= GetDoubleFromINI (fp, "PARAMS", "OB_FSC");
	Ini->IB_FSC					= GetDoubleFromINI (fp, "PARAMS", "IB_FSC");
	Ini->OBIB_FSC				= GetDoubleFromINI (fp, "PARAMS", "OBIB_FSC");

// transit skim matrix table identifiers
	Ini->FARE_CORE          =  (short)GetIntFromINI (fp, "TABLES", "FARE_CORE");
	Ini->FIRST_WAIT_CORE    =  (short)GetIntFromINI (fp, "TABLES", "FIRST_WAIT_CORE");
	Ini->TOTAL_WAIT_CORE    =  (short)GetIntFromINI (fp, "TABLES", "TOTAL_WAIT_CORE");
	Ini->NUM_TRANSFERS_CORE =  (short)GetIntFromINI (fp, "TABLES", "NUM_TRANSFERS_CORE");
	Ini->BUS_IVTT_CORE      =  (short)GetIntFromINI (fp, "TABLES", "BUS_IVTT_CORE");
	Ini->EXPBUS_IVTT_CORE   =  (short)GetIntFromINI (fp, "TABLES", "EXPBUS_IVTT_CORE");
	Ini->RAIL_IVTT_CORE     =  (short)GetIntFromINI (fp, "TABLES", "RAIL_IVTT_CORE");
	Ini->CR_IVTT_CORE       =  (short)GetIntFromINI (fp, "TABLES", "CR_IVTT_CORE");
	Ini->FERRY_IVTT_CORE    =  (short)GetIntFromINI (fp, "TABLES", "FERRY_IVTT_CORE");
	Ini->ACCESS_OVTT_CORE   =  (short)GetIntFromINI (fp, "TABLES", "ACCESS_TIME_CORE");
	Ini->EGRESS_OVTT_CORE   =  (short)GetIntFromINI (fp, "TABLES", "EGRESS_TIME_CORE");
	Ini->TRANSFER_WALK_CORE =  (short)GetIntFromINI (fp, "TABLES", "TRANSFER_WALK_CORE");
	Ini->AUTO_TIME_CORE		=  (short)GetIntFromINI (fp, "TABLES", "AUTO_TIME_CORE");

// highway skim matrix table identifiers
	Ini->HWY_TIME_CORE	=  (short)GetIntFromINI (fp, "TABLES", "HWY_TIME_CORE");
	Ini->HWY_DIST_CORE	=  (short)GetIntFromINI (fp, "TABLES", "HWY_DIST_CORE");
	Ini->HWY_TOLL_CORE	=  (short)GetIntFromINI (fp, "TABLES", "HWY_TOLL_CORE");

// highway hov skim matrix table identifiers
	Ini->HWY_HOV2_TIME_CORE	=  (short)GetIntFromINI (fp, "TABLES", "HWY_HOV2_TIME_CORE");
	Ini->HWY_HOV2_DIST_CORE	=  (short)GetIntFromINI (fp, "TABLES", "HWY_HOV2_DIST_CORE");
	Ini->HWY_HOV2_TOLL_CORE	=  (short)GetIntFromINI (fp, "TABLES", "HWY_HOV2_TOLL_CORE");
	Ini->HWY_HOV3p_TIME_CORE	=  (short)GetIntFromINI (fp, "TABLES", "HWY_HOV3p_TIME_CORE");
	Ini->HWY_HOV3p_DIST_CORE	=  (short)GetIntFromINI (fp, "TABLES", "HWY_HOV3p_DIST_CORE");
	Ini->HWY_HOV3p_TOLL_CORE	=  (short)GetIntFromINI (fp, "TABLES", "HWY_HOV3p_TOLL_CORE");
	Ini->HWY_TAXI_TIME_CORE	=  (short)GetIntFromINI (fp, "TABLES", "HWY_TAXI_TIME_CORE");
	Ini->HWY_TAXI_DIST_CORE	=  (short)GetIntFromINI (fp, "TABLES", "HWY_TAXI_DIST_CORE");
	Ini->HWY_TAXI_TOLL_CORE	=  (short)GetIntFromINI (fp, "TABLES", "HWY_TAXI_TOLL_CORE");
		
// other utility constants
	Ini->AVG_AUTO_OCCUPANCY 	= GetFloatFromINI (fp, "OTHER_UTILITY_CONSTANTS", "AVG_AUTO_OCCUPANCY");
	Ini->AUTO_OPERATING_COST 	= GetFloatFromINI (fp, "OTHER_UTILITY_CONSTANTS", "AUTO_OPERATING_COST");
	Ini->HWY_CENTS_PER_MILE 	= GetFloatFromINI (fp, "OTHER_UTILITY_CONSTANTS", "HWY_CENTS_PER_MILE");
	Ini->WALK_POTENTIAL_DIST	= GetFloatFromINI (fp, "OTHER_UTILITY_CONSTANTS", "WALK_POTENTIAL_DIST");
	Ini->WALK_CIRCUITY			= GetFloatFromINI (fp, "OTHER_UTILITY_CONSTANTS", "WALK_CIRCUITY");
	Ini->WALK_SPEED		 		= GetFloatFromINI (fp, "OTHER_UTILITY_CONSTANTS", "WALK_SPEED");

// intrazonal speeds by area type
	Ini->AUTO_SPEED = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (AREA_TYPES+1)*sizeof(float));
//	addRAM ("read_ini_control_file", (AREA_TYPES+1)*sizeof(float));

	Ini->AUTO_SPEED[1]		= GetFloatFromINI (fp, "INTRAZONAL_SPEEDS", "AUTO1");
	Ini->AUTO_SPEED[2]		= GetFloatFromINI (fp, "INTRAZONAL_SPEEDS", "AUTO2");
	Ini->AUTO_SPEED[3]		= GetFloatFromINI (fp, "INTRAZONAL_SPEEDS", "AUTO3");
	Ini->AUTO_SPEED[4]		= GetFloatFromINI (fp, "INTRAZONAL_SPEEDS", "AUTO4");
	Ini->AUTO_SPEED[5]		= GetFloatFromINI (fp, "INTRAZONAL_SPEEDS", "AUTO5");
	Ini->AUTO_SPEED[6]		= GetFloatFromINI (fp, "INTRAZONAL_SPEEDS", "AUTO6");
	Ini->AUTO_SPEED[7]		= GetFloatFromINI (fp, "INTRAZONAL_SPEEDS", "AUTO7");
	Ini->AUTO_SPEED[8]		= GetFloatFromINI (fp, "INTRAZONAL_SPEEDS", "AUTO8");
	Ini->AUTO_SPEED[9]		= GetFloatFromINI (fp, "INTRAZONAL_SPEEDS", "AUTO9");
	Ini->AUTO_SPEED[10]		= GetFloatFromINI (fp, "INTRAZONAL_SPEEDS", "AUTO10");
	Ini->AUTO_SPEED[11]		= GetFloatFromINI (fp, "INTRAZONAL_SPEEDS", "AUTO11");
	Ini->BUS_FACTOR			= GetFloatFromINI (fp, "INTRAZONAL_SPEEDS", "BUS_FACTOR");


}
