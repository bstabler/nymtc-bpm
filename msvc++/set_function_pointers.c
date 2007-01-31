#include "md.h"

void set_function_pointers ()
{
// Purpose definitions:
//	0 = work low income
//	1 = work medium income
//	2 = work high income
//	3 = school
//	4 = university
//	5 = maintenance
//	6 = discretonary
//	7 = at work


// set elements of mode choice logsum function pointer array to appropriate logsum calculation functions.
	mc_logsum[0] = work_mc_logsum;
	mc_logsum[1] = work_mc_logsum;
	mc_logsum[2] = work_mc_logsum;
	mc_logsum[3] = school_mc_logsum;
	mc_logsum[4] = univ_mc_logsum;
	mc_logsum[5] = maint_mc_logsum;
	mc_logsum[6] = discr_mc_logsum;
	mc_logsum[7] = atwork_mc_logsum;


// set elements of mode choice proportions function pointer array to appropriate proportions calculation functions.
	mc_props[0] = work_mc_props;
	mc_props[1] = work_mc_props;
	mc_props[2] = work_mc_props;
	mc_props[3] = school_mc_props;
	mc_props[4] = univ_mc_props;
	mc_props[5] = maint_mc_props;
	mc_props[6] = discr_mc_props;
	mc_props[7] = atwork_mc_props;


// set elements of function pointer array for motorized choice.
	motorized_choice[0] = work_motorized_choice;
	motorized_choice[1] = work_motorized_choice;
	motorized_choice[2] = work_motorized_choice;
	motorized_choice[3] = school_motorized_choice;
	motorized_choice[4] = univ_motorized_choice;
	motorized_choice[5] = maint_motorized_choice;
	motorized_choice[6] = discr_motorized_choice;
	motorized_choice[7] = atwork_motorized_choice;


// set elements of function pointer array for attractions models.
	get_attractions[0] = work_attractions;
	get_attractions[1] = work_attractions;
	get_attractions[2] = work_attractions;
	get_attractions[3] = school_attractions;
	get_attractions[4] = univ_attractions;
	get_attractions[5] = maint_attractions;
	get_attractions[6] = discr_attractions;
	get_attractions[7] = atwork_attractions;


// set elements of mode choice od utility function pointer array to appropriate od utility calculation functions.
	da_od_utility[0] = work_da_od_util;
	da_od_utility[1] = work_da_od_util;
	da_od_utility[2] = work_da_od_util;
	da_od_utility[3] = school_da_od_util;
	da_od_utility[4] = univ_da_od_util;
	da_od_utility[5] = maint_da_od_util;
	da_od_utility[6] = discr_da_od_util;
	da_od_utility[7] = atwork_da_od_util;
	sr2_od_utility[0] = work_sr2_od_util;
	sr2_od_utility[1] = work_sr2_od_util;
	sr2_od_utility[2] = work_sr2_od_util;
	sr2_od_utility[3] = school_sr2_od_util;
	sr2_od_utility[4] = univ_sr2_od_util;
	sr2_od_utility[5] = maint_sr2_od_util;
	sr2_od_utility[6] = discr_sr2_od_util;
	sr2_od_utility[7] = atwork_sr2_od_util;
	sr3_od_utility[0] = work_sr3_od_util;
	sr3_od_utility[1] = work_sr3_od_util;
	sr3_od_utility[2] = work_sr3_od_util;
	sr3_od_utility[3] = school_sr3_od_util;
	sr3_od_utility[4] = univ_sr3_od_util;
	sr3_od_utility[5] = maint_sr3_od_util;
	sr3_od_utility[6] = discr_sr3_od_util;
	sr3_od_utility[7] = atwork_sr3_od_util;
	sr4_od_utility[0] = work_sr4_od_util;
	sr4_od_utility[1] = work_sr4_od_util;
	sr4_od_utility[2] = work_sr4_od_util;
	sr4_od_utility[3] = school_sr4_od_util;
	sr4_od_utility[4] = univ_sr4_od_util;
	sr4_od_utility[5] = maint_sr4_od_util;
	sr4_od_utility[6] = discr_sr4_od_util;
	sr4_od_utility[7] = NULL;
	wt_od_utility[0] = work_wt_od_util;
	wt_od_utility[1] = work_wt_od_util;
	wt_od_utility[2] = work_wt_od_util;
	wt_od_utility[3] = school_wt_od_util;
	wt_od_utility[4] = univ_wt_od_util;
	wt_od_utility[5] = maint_wt_od_util;
	wt_od_utility[6] = discr_wt_od_util;
	wt_od_utility[7] = atwork_wt_od_util;
	dt_od_utility[0] = work_dt_od_util;
	dt_od_utility[1] = work_dt_od_util;
	dt_od_utility[2] = work_dt_od_util;
	dt_od_utility[3] = school_dt_od_util;
	dt_od_utility[4] = univ_dt_od_util;
	dt_od_utility[5] = maint_dt_od_util;
	dt_od_utility[6] = discr_dt_od_util;
	dt_od_utility[7] = NULL;
	wc_od_utility[0] = work_wc_od_util;
	wc_od_utility[1] = work_wc_od_util;
	wc_od_utility[2] = work_wc_od_util;
	wc_od_utility[3] = school_wc_od_util;
	wc_od_utility[4] = univ_wc_od_util;
	wc_od_utility[5] = maint_wc_od_util;
	wc_od_utility[6] = discr_wc_od_util;
	wc_od_utility[7] = NULL;
	dc_od_utility[0] = work_dc_od_util;
	dc_od_utility[1] = work_dc_od_util;
	dc_od_utility[2] = work_dc_od_util;
	dc_od_utility[3] = school_dc_od_util;
	dc_od_utility[4] = univ_dc_od_util;
	dc_od_utility[5] = maint_dc_od_util;
	dc_od_utility[6] = discr_dc_od_util;
	dc_od_utility[7] = NULL;
	tx_od_utility[0] = work_tx_od_util;
	tx_od_utility[1] = work_tx_od_util;
	tx_od_utility[2] = work_tx_od_util;
	tx_od_utility[3] = school_tx_od_util;
	tx_od_utility[4] = univ_tx_od_util;
	tx_od_utility[5] = maint_tx_od_util;
	tx_od_utility[6] = discr_tx_od_util;
	tx_od_utility[7] = atwork_tx_od_util;


// set elements of function pointer array to appropriate se utility calculation functions.
	sr2_se_utility[0] = work_sr2_se_util;
	sr2_se_utility[1] = work_sr2_se_util;
	sr2_se_utility[2] = work_sr2_se_util;
	sr2_se_utility[3] = school_sr2_se_util;
	sr2_se_utility[4] = univ_sr2_se_util;
	sr2_se_utility[5] = maint_sr2_se_util;
	sr2_se_utility[6] = discr_sr2_se_util;
	sr2_se_utility[7] = atwork_sr2_se_util;
	sr3_se_utility[0] = work_sr3_se_util;
	sr3_se_utility[1] = work_sr3_se_util;
	sr3_se_utility[2] = work_sr3_se_util;
	sr3_se_utility[3] = school_sr3_se_util;
	sr3_se_utility[4] = univ_sr3_se_util;
	sr3_se_utility[5] = maint_sr3_se_util;
	sr3_se_utility[6] = discr_sr3_se_util;
	sr3_se_utility[7] = atwork_sr3_se_util;
	sr4_se_utility[0] = work_sr4_se_util;
	sr4_se_utility[1] = work_sr4_se_util;
	sr4_se_utility[2] = work_sr4_se_util;
	sr4_se_utility[3] = school_sr4_se_util;
	sr4_se_utility[4] = univ_sr4_se_util;
	sr4_se_utility[5] = maint_sr4_se_util;
	sr4_se_utility[6] = discr_sr4_se_util;
	sr4_se_utility[7] = NULL;
	wt_se_utility[0] = work_wt_se_util;
	wt_se_utility[1] = work_wt_se_util;
	wt_se_utility[2] = work_wt_se_util;
	wt_se_utility[3] = school_wt_se_util;
	wt_se_utility[4] = univ_wt_se_util;
	wt_se_utility[5] = maint_wt_se_util;
	wt_se_utility[6] = discr_wt_se_util;
	wt_se_utility[7] = atwork_wt_se_util;
	dt_se_utility[0] = work_dt_se_util;
	dt_se_utility[1] = work_dt_se_util;
	dt_se_utility[2] = work_dt_se_util;
	dt_se_utility[3] = school_dt_se_util;
	dt_se_utility[4] = univ_dt_se_util;
	dt_se_utility[5] = maint_dt_se_util;
	dt_se_utility[6] = discr_dt_se_util;
	dt_se_utility[7] = NULL;
	wc_se_utility[0] = work_wc_se_util;
	wc_se_utility[1] = work_wc_se_util;
	wc_se_utility[2] = work_wc_se_util;
	wc_se_utility[3] = school_wc_se_util;
	wc_se_utility[4] = univ_wc_se_util;
	wc_se_utility[5] = maint_wc_se_util;
	wc_se_utility[6] = discr_wc_se_util;
	wc_se_utility[7] = NULL;
	dc_se_utility[0] = work_dc_se_util;
	dc_se_utility[1] = work_dc_se_util;
	dc_se_utility[2] = work_dc_se_util;
	dc_se_utility[3] = school_dc_se_util;
	dc_se_utility[4] = univ_dc_se_util;
	dc_se_utility[5] = maint_dc_se_util;
	dc_se_utility[6] = discr_dc_se_util;
	dc_se_utility[7] = NULL;
	tx_se_utility[0] = work_tx_se_util;
	tx_se_utility[1] = work_tx_se_util;
	tx_se_utility[2] = work_tx_se_util;
	tx_se_utility[3] = school_tx_se_util;
	tx_se_utility[4] = univ_tx_se_util;
	tx_se_utility[5] = maint_tx_se_util;
	tx_se_utility[6] = discr_tx_se_util;
	tx_se_utility[7] = atwork_tx_se_util;

}