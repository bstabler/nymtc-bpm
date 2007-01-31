/*
 *	At work journey mode choice socioeconomic related components of systematic utilites for each modal alternative
 *
 *  Calculate the SE portions of systematic utility for each modal alternative to all destination tazs
 *  from the specified origin taz.  These are the lowest level alternatives in the NL structure.
 */

#include "md.h"
#include "atwork_coeff.h"

	
	
float atwork_sr2_se_util (struct socec_data *SocecData)
{
	return ((float)(INC12_2P*(SocecData->inc1 || SocecData->inc2) + INC3_2P*SocecData->inc3));
}


float atwork_sr3_se_util (struct socec_data *SocecData)
{
	return ((float)(INC12_3P*(SocecData->inc1 || SocecData->inc2) + INC3_3P*SocecData->inc3));
}


float atwork_wt_se_util (struct socec_data *SocecData)
{
	return ((float)(INC12_WT*(SocecData->inc1 || SocecData->inc2) + INC3_WT*SocecData->inc3));
}


float atwork_tx_se_util (struct socec_data *SocecData)
{
	return ((float)(INC12_TX*(SocecData->inc1 || SocecData->inc2) + INC3_TX*SocecData->inc3));
}