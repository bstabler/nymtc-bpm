/*
 *	School journey mode choice socioeconomic related components of systematic utilites for each modal alternative
 *
 *  Calculate the SE portions of systematic utility for each modal alternative to all destination tazs
 *  from the specified origin taz.  These are the lowest level alternatives in the NL structure.
 */

#include "md.h"
#include "school_coeff.h"


float school_sr2_se_util (struct socec_data *SocecData)
{
	return ((float)(AUTO0_2P*SocecData->noa + AFW_2P*SocecData->afw + ASW_2P*SocecData->asw + AMW_2P*SocecData->amw
		  + INC1_2P*SocecData->inc1 + INC2_2P*SocecData->inc2 + INC3_2P*SocecData->inc3
		  + ADULT_2P*SocecData->adult));
}


float school_sr3_se_util (struct socec_data *SocecData)
{
	return ((float)(AUTO0_3P*SocecData->noa + AFW_3P*SocecData->afw + ASW_3P*SocecData->asw + AMW_3P*SocecData->amw
		  + INC1_3P*SocecData->inc1 + INC2_3P*SocecData->inc2 + INC3_3P*SocecData->inc3
		  + ADULT_3P*SocecData->adult));
}


float school_sr4_se_util (struct socec_data *SocecData)
{
	return ((float)(AUTO0_4P*SocecData->noa + AFW_4P*SocecData->afw + ASW_4P*SocecData->asw + AMW_4P*SocecData->amw
		  + INC1_4P*SocecData->inc1 + INC2_4P*SocecData->inc2 + INC3_4P*SocecData->inc3
		  + ADULT_4P*SocecData->adult));
}


float school_wt_se_util (struct socec_data *SocecData)
{
	return ((float)(AUTO0_WT*SocecData->noa + AFW_WT*SocecData->afw + ASW_WT*SocecData->asw + AMW_WT*SocecData->amw
		  + INC1_WT*SocecData->inc1 + INC2_WT*SocecData->inc2 + INC3_WT*SocecData->inc3
		  + ADULT_WT*SocecData->adult));
}


float school_dt_se_util (struct socec_data *SocecData)
{
	return ((float)(AUTO0_DT*SocecData->noa + AFW_DT*SocecData->afw + ASW_DT*SocecData->asw + AMW_DT*SocecData->amw
		  + INC1_DT*SocecData->inc1 + INC2_DT*SocecData->inc2 + INC3_DT*SocecData->inc3
		  + ADULT_DT*SocecData->adult));
}


float school_wc_se_util (struct socec_data *SocecData)
{
	return ((float)(AUTO0_WC*SocecData->noa + AFW_WC*SocecData->afw + ASW_WC*SocecData->asw + AMW_WC*SocecData->amw
		  + INC1_WC*SocecData->inc1 + INC2_WC*SocecData->inc2 + INC3_WC*SocecData->inc3
		  + ADULT_WC*SocecData->adult));
}


float school_dc_se_util (struct socec_data *SocecData)
{
	return ((float)(AUTO0_DC*SocecData->noa + AFW_DC*SocecData->afw + ASW_DC*SocecData->asw + AMW_DC*SocecData->amw
		  + INC1_DC*SocecData->inc1 + INC2_DC*SocecData->inc2 + INC3_DC*SocecData->inc3
		  + ADULT_DC*SocecData->adult));
}


float school_tx_se_util (struct socec_data *SocecData)
{
	return ((float)(AUTO0_TX*SocecData->noa + AFW_TX*SocecData->afw + ASW_TX*SocecData->asw + AMW_TX*SocecData->amw
		  + INC1_TX*SocecData->inc1 + INC2_TX*SocecData->inc2 + INC3_TX*SocecData->inc3
		  + ADULT_TX*SocecData->adult));
}


float school_sb_se_util (struct socec_data *SocecData)
{
	return ((float)(AUTO0_SB*SocecData->noa + AFW_SB*SocecData->afw + ASW_SB*SocecData->asw + AMW_SB*SocecData->amw
		  + INC1_SB*SocecData->inc1 + INC2_SB*SocecData->inc2 + INC3_SB*SocecData->inc3
		  + AT2CO1*SocecData->at2co1+ AT3CO1*SocecData->at3co1+ AT3CO2*SocecData->at3co2+ AT3CO3*SocecData->at3co3+ AT3CO4*SocecData->at3co4+ AT4CO*SocecData->at4co
		  + ADULT_SB*SocecData->adult));
}
