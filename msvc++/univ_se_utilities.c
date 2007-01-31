/*
 *	University journey mode choice socioeconomic related components of systematic utilites for each modal alternative
 *
 *  Calculate the SE portions of systematic utility for each modal alternative to all destination tazs
 *  from the specified origin taz.  These are the lowest level alternatives in the NL structure.
 */

#include "md.h"
#include "univ_coeff.h"

float univ_sr2_se_util (struct socec_data *SocecData)
{
	return ((float)(NOAUTO_2P*SocecData->noa + AUTFW_2P*SocecData->afw + AUTSW_2P*SocecData->asw + AUTMW_2P*SocecData->amw
		  + WORK_2P*SocecData->work + NWRK_2P*SocecData->nwrk
		  + INC1_2P*SocecData->inc1 + INC2_2P*SocecData->inc2 + INC3_2P*SocecData->inc3));
}


float univ_sr3_se_util (struct socec_data *SocecData)
{
	return ((float)(NOAUTO_3P*SocecData->noa + AUTFW_3P*SocecData->afw + AUTSW_3P*SocecData->asw + AUTMW_3P*SocecData->amw
		  + WORK_3P*SocecData->work + NWRK_3P*SocecData->nwrk
		  + INC1_3P*SocecData->inc1 + INC2_3P*SocecData->inc2 + INC3_3P*SocecData->inc3));
}


float univ_sr4_se_util (struct socec_data *SocecData)
{
	return ((float)(NOAUTO_4P*SocecData->noa + AUTFW_4P*SocecData->afw + AUTSW_4P*SocecData->asw + AUTMW_4P*SocecData->amw
		  + WORK_4P*SocecData->work + NWRK_4P*SocecData->nwrk
		  + INC1_4P*SocecData->inc1 + INC2_4P*SocecData->inc2 + INC3_4P*SocecData->inc3));
}


float univ_wt_se_util (struct socec_data *SocecData)
{
	return ((float)(NOAUTO_WT*SocecData->noa + AUTFW_WT*SocecData->afw + AUTSW_WT*SocecData->asw + AUTMW_WT*SocecData->amw
		  + WORK_WT*SocecData->work + NWRK_WT*SocecData->nwrk
		  + INC1_WT*SocecData->inc1 + INC2_WT*SocecData->inc2 + INC3_WT*SocecData->inc3));
}


float univ_dt_se_util (struct socec_data *SocecData)
{
	return ((float)(NOAUTO_DT*SocecData->noa + AUTFW_DT*SocecData->afw + AUTSW_DT*SocecData->asw + AUTMW_DT*SocecData->amw
		  + WORK_DT*SocecData->work + NWRK_DT*SocecData->nwrk
		  + INC1_DT*SocecData->inc1 + INC2_DT*SocecData->inc2 + INC3_DT*SocecData->inc3));
}


float univ_wc_se_util (struct socec_data *SocecData)
{
	return ((float)(NOAUTO_WC*SocecData->noa + AUTFW_WC*SocecData->afw + AUTSW_WC*SocecData->asw + AUTMW_WC*SocecData->amw
		  + WORK_WC*SocecData->work + NWRK_WC*SocecData->nwrk
		  + INC1_WC*SocecData->inc1 + INC2_WC*SocecData->inc2 + INC3_WC*SocecData->inc3));
}


float univ_dc_se_util (struct socec_data *SocecData)
{
	return ((float)(NOAUTO_DC*SocecData->noa + AUTFW_DC*SocecData->afw + AUTSW_DC*SocecData->asw + AUTMW_DC*SocecData->amw
		  + WORK_DC*SocecData->work + NWRK_DC*SocecData->nwrk
		  + INC1_DC*SocecData->inc1 + INC2_DC*SocecData->inc2 + INC3_DC*SocecData->inc3));
}


float univ_tx_se_util (struct socec_data *SocecData)
{
	return ((float)(NOAUTO_TX*SocecData->noa + AUTFW_TX*SocecData->afw + AUTSW_TX*SocecData->asw + AUTMW_TX*SocecData->amw
		  + WORK_TX*SocecData->work + NWRK_TX*SocecData->nwrk
		  + INC1_TX*SocecData->inc1 + INC2_TX*SocecData->inc2 + INC3_TX*SocecData->inc3));
}