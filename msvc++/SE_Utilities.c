/*
 *	Mode choice socioeconomic related components of systematic utilites for each modal alternative
 *
 *  Calculate the SE portions of systematic utility for each modal alternative to all destination tazs
 *  from the specified origin taz.  These are the lowest level alternatives in the NL structure.
 */

#include "md.h"

// This method is used to return SE_Utility used in destination choice and mode choice for non-restricted destinations
void SE_Utilities (int n, struct journey_attribs *JourneyAttribs, struct zone_data *ZonalData, float *SE_Utility)
{
	struct socec_data SocecData;
	int i, at, co, purpose;


// Calculate the socioeconomic variables (used for work journeys) and save in data structure
	SocecData.LowNo   = 0;
	SocecData.MidNo   = 0;
	SocecData.HiNo    = 0;
	SocecData.LowFew  = 0;
	SocecData.MidFew  = 0;
	SocecData.HiFew   = 0;
	SocecData.LowSame = 0;
	SocecData.MidSame = 0;
	SocecData.HiSame  = 0;
	SocecData.LowMore = 0;
	SocecData.MidMore = 0;
	SocecData.HiMore  = 0;
	if (JourneyAttribs->autos[n] == 0) {
		SocecData.LowNo   = (JourneyAttribs->income[n] == 1);
		SocecData.MidNo   = (JourneyAttribs->income[n] == 2);
		SocecData.HiNo    = (JourneyAttribs->income[n] == 3);
	}
	else if (JourneyAttribs->autos[n] < JourneyAttribs->workers[n]) {
		SocecData.LowFew  = (JourneyAttribs->income[n] == 1);
		SocecData.MidFew  = (JourneyAttribs->income[n] == 2);
		SocecData.HiFew   = (JourneyAttribs->income[n] == 3);
	}
	else if (JourneyAttribs->autos[n] == JourneyAttribs->workers[n]) {
		SocecData.LowSame = (JourneyAttribs->income[n] == 1);
		SocecData.MidSame = (JourneyAttribs->income[n] == 2);
		SocecData.HiSame  = (JourneyAttribs->income[n] == 3);
	}
	else {
		SocecData.LowMore = (JourneyAttribs->income[n] == 1);
		SocecData.MidMore = (JourneyAttribs->income[n] == 2);
		SocecData.HiMore  = (JourneyAttribs->income[n] == 3);
	}



// Initialize the variables used for non-work journeys
	SocecData.aa = 0;
	SocecData.ak = 0;
	SocecData.noa = 0;
	SocecData.afw = 0;
	SocecData.asw = 0;
	SocecData.amw = 0;
	SocecData.work = 0;
	SocecData.nwrk = 0;
	SocecData.inc1 = 0;
	SocecData.inc2 = 0;
	SocecData.inc3 = 0;
	SocecData.adult = 0;
	SocecData.at2co1 = 0;
	SocecData.at3co1 = 0;
	SocecData.at3co2 = 0;
	SocecData.at3co3 = 0;
	SocecData.at3co4 = 0;
	SocecData.at4co = 0;
	

// Calculate the socioeconomic variables specific to school journeys and save in data structure
	if (JourneyAttribs->person_type[n] != 1)
		SocecData.adult = 1;

	at = ZonalData->AreaType[JourneyAttribs->orig[n]];
	
	switch (ZonalData->County[JourneyAttribs->orig[n]]) {
		case 1: case 2: case 3: case 4: case 14: case 15: case 16: case 17: case 18:
			co = 1;
			break;
		case 5: case 6: case 7: case 8: case 19: case 20: case 21: case 22: case 23: case 24: case 25: case 26: case 27: case 28:
			co = 2;
			break;
		case 9:
			co = 3;
			break;
		case 10: case 11: case 12: case 13:
			co = 4;
			break;
	}
	
	SocecData.at2co1 = ((at == 2) && (co == 1));
	SocecData.at3co1 = ((at == 3) && (co == 1));
	SocecData.at3co2 = ((at == 3) && (co == 2));
	SocecData.at3co3 = ((at == 3) && (co == 3));
	SocecData.at3co4 = ((at == 3) && (co == 4));
	SocecData.at4co  =  (at == 4);


// Calculate the remaining socioeconomic variables used for all non-work journeys and save in data structure
	if (JourneyAttribs->autos[n] == 0)
		SocecData.noa = 1;
	else if (JourneyAttribs->autos[n] < JourneyAttribs->workers[n])
		SocecData.afw = 1;
	else if (JourneyAttribs->autos[n] == JourneyAttribs->workers[n])
		SocecData.asw = 1;
	else
		SocecData.amw = 1;
		
	if (JourneyAttribs->person_type[n] == 3)
		SocecData.work = 1;
	else if (JourneyAttribs->person_type[n] == 2)
		SocecData.nwrk = 1;
	
	if (JourneyAttribs->ak[n] >= 1 && JourneyAttribs->aa[n] >= 1)
		SocecData.ak = 1;
	if (JourneyAttribs->aa[n] >= 2)
		SocecData.aa = 1;

	if (JourneyAttribs->income[n] == 1)
		SocecData.inc1 = 1;
	else if (JourneyAttribs->income[n] == 2)
		SocecData.inc2 = 1;
	else if (JourneyAttribs->income[n] == 3)
		SocecData.inc3 = 1;
	



// Calculate the socioeconomic related mode choice utility components.  No se variables in DA utility
	purpose = JourneyAttribs->purpose[n];

	for (i=0; i < Ini->NUMBER_ALTS; i++)
		SE_Utility[i] = 0.0;


	SE_Utility[1] = sr2_se_utility[purpose] (&SocecData);
	SE_Utility[2] = sr3_se_utility[purpose] (&SocecData);
	if (purpose != 7)
		SE_Utility[3] = sr4_se_utility[purpose] (&SocecData);
	SE_Utility[4] = wt_se_utility[purpose] (&SocecData);
	if (purpose != 7) {
		SE_Utility[5] = dt_se_utility[purpose] (&SocecData);
		SE_Utility[6] = wc_se_utility[purpose] (&SocecData);
		SE_Utility[7] = dc_se_utility[purpose] (&SocecData);
	}
	SE_Utility[8] = tx_se_utility[purpose] (&SocecData);
	if (purpose == 3)
		SE_Utility[10] = school_sb_se_util (&SocecData);
}	





// This method is used to return SE_Utility used in mode choice for restricted destinations
// Austo available are used in this case instead of autos owned.
void SE_UtilitiesRestricted (int n, struct journey_attribs *JourneyAttribs, struct zone_data *ZonalData, float *SE_Utility)
{
	struct socec_data SocecData;
	int i, at, co, purpose;


// Calculate the socioeconomic variables (used for work journeys) and save in data structure
	SocecData.LowNo   = 0;
	SocecData.MidNo   = 0;
	SocecData.HiNo    = 0;
	SocecData.LowFew  = 0;
	SocecData.MidFew  = 0;
	SocecData.HiFew   = 0;
	SocecData.LowSame = 0;
	SocecData.MidSame = 0;
	SocecData.HiSame  = 0;
	SocecData.LowMore = 0;
	SocecData.MidMore = 0;
	SocecData.HiMore  = 0;
	if (JourneyAttribs->autosAvail[n] == 0) {
		SocecData.LowNo   = (JourneyAttribs->income[n] == 1);
		SocecData.MidNo   = (JourneyAttribs->income[n] == 2);
		SocecData.HiNo    = (JourneyAttribs->income[n] == 3);
	}
	else if (JourneyAttribs->autosAvail[n] < JourneyAttribs->workers[n]) {
		SocecData.LowFew  = (JourneyAttribs->income[n] == 1);
		SocecData.MidFew  = (JourneyAttribs->income[n] == 2);
		SocecData.HiFew   = (JourneyAttribs->income[n] == 3);
	}
	else if (JourneyAttribs->autosAvail[n] == JourneyAttribs->workers[n]) {
		SocecData.LowSame = (JourneyAttribs->income[n] == 1);
		SocecData.MidSame = (JourneyAttribs->income[n] == 2);
		SocecData.HiSame  = (JourneyAttribs->income[n] == 3);
	}
	else {
		SocecData.LowMore = (JourneyAttribs->income[n] == 1);
		SocecData.MidMore = (JourneyAttribs->income[n] == 2);
		SocecData.HiMore  = (JourneyAttribs->income[n] == 3);
	}



// Initialize the variables used for non-work journeys
	SocecData.aa = 0;
	SocecData.ak = 0;
	SocecData.noa = 0;
	SocecData.afw = 0;
	SocecData.asw = 0;
	SocecData.amw = 0;
	SocecData.work = 0;
	SocecData.nwrk = 0;
	SocecData.inc1 = 0;
	SocecData.inc2 = 0;
	SocecData.inc3 = 0;
	SocecData.adult = 0;
	SocecData.at2co1 = 0;
	SocecData.at3co1 = 0;
	SocecData.at3co2 = 0;
	SocecData.at3co3 = 0;
	SocecData.at3co4 = 0;
	SocecData.at4co = 0;
	

// Calculate the socioeconomic variables specific to school journeys and save in data structure
	if (JourneyAttribs->person_type[n] != 1)
		SocecData.adult = 1;

	at = ZonalData->AreaType[JourneyAttribs->orig[n]];
	
	switch (ZonalData->County[JourneyAttribs->orig[n]]) {
		case 1: case 2: case 3: case 4: case 14: case 15: case 16: case 17: case 18:
			co = 1;
			break;
		case 5: case 6: case 7: case 8: case 19: case 20: case 21: case 22: case 23: case 24: case 25: case 26: case 27: case 28:
			co = 2;
			break;
		case 9:
			co = 3;
			break;
		case 10: case 11: case 12: case 13:
			co = 4;
			break;
	}
	
	SocecData.at2co1 = ((at == 2) && (co == 1));
	SocecData.at3co1 = ((at == 3) && (co == 1));
	SocecData.at3co2 = ((at == 3) && (co == 2));
	SocecData.at3co3 = ((at == 3) && (co == 3));
	SocecData.at3co4 = ((at == 3) && (co == 4));
	SocecData.at4co  =  (at == 4);


// Calculate the remaining socioeconomic variables used for all non-work journeys and save in data structure
	if (JourneyAttribs->autosAvail[n] == 0)
		SocecData.noa = 1;
	else if (JourneyAttribs->autosAvail[n] < JourneyAttribs->workers[n])
		SocecData.afw = 1;
	else if (JourneyAttribs->autosAvail[n] == JourneyAttribs->workers[n])
		SocecData.asw = 1;
	else
		SocecData.amw = 1;
		
	if (JourneyAttribs->person_type[n] == 3)
		SocecData.work = 1;
	else if (JourneyAttribs->person_type[n] == 2)
		SocecData.nwrk = 1;
	
	if (JourneyAttribs->ak[n] >= 1 && JourneyAttribs->aa[n] >= 1)
		SocecData.ak = 1;
	if (JourneyAttribs->aa[n] >= 2)
		SocecData.aa = 1;

	if (JourneyAttribs->income[n] == 1)
		SocecData.inc1 = 1;
	else if (JourneyAttribs->income[n] == 2)
		SocecData.inc2 = 1;
	else if (JourneyAttribs->income[n] == 3)
		SocecData.inc3 = 1;
	



// Calculate the socioeconomic related mode choice utility components.  No se variables in DA utility
	purpose = JourneyAttribs->purpose[n];

	for (i=0; i < Ini->NUMBER_ALTS; i++)
		SE_Utility[i] = 0.0;


	SE_Utility[1] = sr2_se_utility[purpose] (&SocecData);
	SE_Utility[2] = sr3_se_utility[purpose] (&SocecData);
	if (purpose != 7)
		SE_Utility[3] = sr4_se_utility[purpose] (&SocecData);
	SE_Utility[4] = wt_se_utility[purpose] (&SocecData);
	if (purpose != 7) {
		SE_Utility[5] = dt_se_utility[purpose] (&SocecData);
		SE_Utility[6] = wc_se_utility[purpose] (&SocecData);
		SE_Utility[7] = dc_se_utility[purpose] (&SocecData);
	}
	SE_Utility[8] = tx_se_utility[purpose] (&SocecData);
	if (purpose == 3)
		SE_Utility[10] = school_sb_se_util (&SocecData);
}	

