/*
 *	Work journey mode choice socioeconomic related components of systematic utilites for each modal alternative
 *
 */

#include "md.h"
#include "work_coeff.h"

float work_sr2_se_util (struct socec_data *SocecData)
{
	return (float)(LOAFW_2P*SocecData->LowFew + LOASW_2P*SocecData->LowSame + LOAMW_2P*SocecData->LowMore + LONOA_2P*SocecData->LowNo
		  + MDAFW_2P*SocecData->MidFew + MDASW_2P*SocecData->MidSame + MDAMW_2P*SocecData->MidMore + MDNOA_2P*SocecData->MidNo
		  + HIAFW_2P*SocecData->HiFew  + HIASW_2P*SocecData->HiSame  + HIAMW_2P*SocecData->HiMore  + HINOA_2P*SocecData->HiNo);
}


float work_sr3_se_util (struct socec_data *SocecData)
{
	return (float)(LOAFW_3P*SocecData->LowFew + LOASW_3P*SocecData->LowSame + LOAMW_3P*SocecData->LowMore + LONOA_3P*SocecData->LowNo
		  + MDAFW_3P*SocecData->MidFew + MDASW_3P*SocecData->MidSame + MDAMW_3P*SocecData->MidMore + MDNOA_3P*SocecData->MidNo
		  + HIAFW_3P*SocecData->HiFew  + HIASW_3P*SocecData->HiSame  + HIAMW_3P*SocecData->HiMore  + HINOA_3P*SocecData->HiNo);
}


float work_sr4_se_util (struct socec_data *SocecData)
{
	return (float)(LOAFW_4P*SocecData->LowFew + LOASW_4P*SocecData->LowSame + LOAMW_4P*SocecData->LowMore + LONOA_4P*SocecData->LowNo
		  + MDAFW_4P*SocecData->MidFew + MDASW_4P*SocecData->MidSame + MDAMW_4P*SocecData->MidMore + MDNOA_4P*SocecData->MidNo
		  + HIAFW_4P*SocecData->HiFew  + HIASW_4P*SocecData->HiSame  + HIAMW_4P*SocecData->HiMore  + HINOA_4P*SocecData->HiNo);
}


float work_wt_se_util (struct socec_data *SocecData)
{
	return (float)(LOAFW_WT*SocecData->LowFew + LOASW_WT*SocecData->LowSame + LOAMW_WT*SocecData->LowMore + LONOA_WT*SocecData->LowNo
		  + MDAFW_WT*SocecData->MidFew + MDASW_WT*SocecData->MidSame + MDAMW_WT*SocecData->MidMore + MDNOA_WT*SocecData->MidNo
		  + HIAFW_WT*SocecData->HiFew  + HIASW_WT*SocecData->HiSame  + HIAMW_WT*SocecData->HiMore  + HINOA_WT*SocecData->HiNo);
}


float work_dt_se_util (struct socec_data *SocecData)
{
	return (float)(LOAFW_DT*SocecData->LowFew + LOASW_DT*SocecData->LowSame + LOAMW_DT*SocecData->LowMore + LONOA_DT*SocecData->LowNo
		  + MDAFW_DT*SocecData->MidFew + MDASW_DT*SocecData->MidSame + MDAMW_DT*SocecData->MidMore + MDNOA_DT*SocecData->MidNo
		  + HIAFW_DT*SocecData->HiFew  + HIASW_DT*SocecData->HiSame  + HIAMW_DT*SocecData->HiMore  + HINOA_DT*SocecData->HiNo);
}


float work_wc_se_util (struct socec_data *SocecData)
{
	return (float)(LOAFW_WC*SocecData->LowFew + LOASW_WC*SocecData->LowSame + LOAMW_WC*SocecData->LowMore + LONOA_WC*SocecData->LowNo
		  + MDAFW_WC*SocecData->MidFew + MDASW_WC*SocecData->MidSame + MDAMW_WC*SocecData->MidMore + MDNOA_WC*SocecData->MidNo
		  + HIAFW_WC*SocecData->HiFew  + HIASW_WC*SocecData->HiSame  + HIAMW_WC*SocecData->HiMore  + HINOA_WC*SocecData->HiNo);
}


float work_dc_se_util (struct socec_data *SocecData)
{
	return (float)(LOAFW_DC*SocecData->LowFew + LOASW_DC*SocecData->LowSame + LOAMW_DC*SocecData->LowMore + LONOA_DC*SocecData->LowNo
		  + MDAFW_DC*SocecData->MidFew + MDASW_DC*SocecData->MidSame + MDAMW_DC*SocecData->MidMore + MDNOA_DC*SocecData->MidNo
		  + HIAFW_DC*SocecData->HiFew  + HIASW_DC*SocecData->HiSame  + HIAMW_DC*SocecData->HiMore  + HINOA_DC*SocecData->HiNo);
}


float work_tx_se_util (struct socec_data *SocecData)
{
	return (float)(LOAFW_TX*SocecData->LowFew + LOASW_TX*SocecData->LowSame + LOAMW_TX*SocecData->LowMore + LONOA_TX*SocecData->LowNo
		  + MDAFW_TX*SocecData->MidFew + MDASW_TX*SocecData->MidSame + MDAMW_TX*SocecData->MidMore + MDNOA_TX*SocecData->MidNo
		  + HIAFW_TX*SocecData->HiFew  + HIASW_TX*SocecData->HiSame  + HIAMW_TX*SocecData->HiMore  + HINOA_TX*SocecData->HiNo);
}