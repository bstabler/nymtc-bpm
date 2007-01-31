/*
 *	Maintenance journey mode choice socioeconomic related components of systematic utilites for each modal alternative
 *
 *  Calculate the SE portions of systematic utility for each modal alternative to all destination tazs
 *  from the specified origin taz.  These are the lowest level alternatives in the NL structure.
 */

#include "md.h"
#include "maint_coeff.h"

float maint_sr2_se_util (struct socec_data *SocecData)
{
	return ((float)(NOA_2P*SocecData->noa + AFW_2P*SocecData->afw + ASW_2P*SocecData->asw + AMW_2P*SocecData->amw
		  + AA_2P*SocecData->aa + AK_2P*SocecData->ak
		  + WORK_2P*SocecData->work + NWRK_2P*SocecData->nwrk
		  + INC1_2P*SocecData->inc1 + INC2_2P*SocecData->inc2 + INC3_2P*SocecData->inc3));
}


float maint_sr3_se_util (struct socec_data *SocecData)
{
	return ((float)(NOA_3P*SocecData->noa + AFW_3P*SocecData->afw + ASW_3P*SocecData->asw + AMW_3P*SocecData->amw
		  + AA_3P*SocecData->aa + AK_3P*SocecData->ak
		  + WORK_3P*SocecData->work + NWRK_3P*SocecData->nwrk
		  + INC1_3P*SocecData->inc1 + INC2_3P*SocecData->inc2 + INC3_3P*SocecData->inc3));
}


float maint_sr4_se_util (struct socec_data *SocecData)
{
	return ((float)(NOA_4P*SocecData->noa + AFW_4P*SocecData->afw + ASW_4P*SocecData->asw + AMW_4P*SocecData->amw
		  + AA_4P*SocecData->aa + AK_4P*SocecData->ak
		  + WORK_4P*SocecData->work + NWRK_4P*SocecData->nwrk
		  + INC1_4P*SocecData->inc1 + INC2_4P*SocecData->inc2 + INC3_4P*SocecData->inc3));
}


float maint_wt_se_util (struct socec_data *SocecData)
{
	return ((float)(NOA_WT*SocecData->noa + AFW_WT*SocecData->afw + ASW_WT*SocecData->asw + AMW_WT*SocecData->amw
		  + AA_WT*SocecData->aa + AK_WT*SocecData->ak
		  + WORK_WT*SocecData->work + NWRK_WT*SocecData->nwrk
		  + INC1_WT*SocecData->inc1 + INC2_WT*SocecData->inc2 + INC3_WT*SocecData->inc3));
}


float maint_dt_se_util (struct socec_data *SocecData)
{
	return ((float)(NOA_DT*SocecData->noa + AFW_DT*SocecData->afw + ASW_DT*SocecData->asw + AMW_DT*SocecData->amw
		  + AA_DT*SocecData->aa + AK_DT*SocecData->ak
		  + WORK_DT*SocecData->work + NWRK_DT*SocecData->nwrk
		  + INC1_DT*SocecData->inc1 + INC2_DT*SocecData->inc2 + INC3_DT*SocecData->inc3));
}


float maint_wc_se_util (struct socec_data *SocecData)
{
	return ((float)(NOA_WC*SocecData->noa + AFW_WC*SocecData->afw + ASW_WC*SocecData->asw + AMW_WC*SocecData->amw
		  + AA_WC*SocecData->aa + AK_WC*SocecData->ak
		  + WORK_WC*SocecData->work + NWRK_WC*SocecData->nwrk
		  + INC1_WC*SocecData->inc1 + INC2_WC*SocecData->inc2 + INC3_WC*SocecData->inc3));
}


float maint_dc_se_util (struct socec_data *SocecData)
{
	return ((float)(NOA_DC*SocecData->noa + AFW_DC*SocecData->afw + ASW_DC*SocecData->asw + AMW_DC*SocecData->amw
		  + AA_DC*SocecData->aa + AK_DC*SocecData->ak
		  + WORK_DC*SocecData->work + NWRK_DC*SocecData->nwrk
		  + INC1_DC*SocecData->inc1 + INC2_DC*SocecData->inc2 + INC3_DC*SocecData->inc3));
}


float maint_tx_se_util (struct socec_data *SocecData)
{
	return ((float)(NOA_TX*SocecData->noa + AFW_TX*SocecData->afw + ASW_TX*SocecData->asw + AMW_TX*SocecData->amw
		  + AA_TX*SocecData->aa + AK_TX*SocecData->ak
		  + WORK_TX*SocecData->work + NWRK_TX*SocecData->nwrk
		  + INC1_TX*SocecData->inc1 + INC2_TX*SocecData->inc2 + INC3_TX*SocecData->inc3));
}