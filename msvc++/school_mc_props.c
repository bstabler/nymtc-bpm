/*
 *	School journey mode choice proportions.
 *
 *  Assumed NL structire:
 *
 *	              					School Journey Mode Choice
 *												|
 *												|
 *			|-----------------------------------|---------------|-----------|-------|
 *			|				|					|				|			|		|
 *			|				|					|        		|			|		|
 *			DA		|------ShRd-----|		|-Walk--|    	|-Drive-|		TX		SB
 *			|		|		|		|		|		|    	|		|		|		|
 *			|		|		|		|		|		|    	|		|		|		|
 *			DA		SR2		SR3		SR4+	WT		WC   	DT		DC		TX		SB
 *
 */


#include <math.h> 
#include "md.h"
#include "school_coeff.h"

void school_mc_props (int journey, int *Avail, int person_type, int autos, int *walk, int PrevAuto, float *OD_Utility, float *SE_Utility, float *Prob, float *ASC, float *TotalUtility)
{
// autos = number of autos in the HH.
// walk is an int array of dimension 2.  walk[0] = 0 if HH cannot walk to transit at origin, 1 otherwise.
// walk[1] = 0 if walk to transit is not available at the destination, 1 otherwise.
// OD_Utility is a float array of the systematic portions of the modal utilities.


	int i;
	double da, sr2, sr3, sr4, wt, dt, wc, dc, tx, sb;
	double wt_wc, sr, dt_dc;
	double w_prob, d_prob, sr_prob;
	double da_prob, sr2_prob, sr3_prob, sr4_prob;
	double wt_prob, dt_prob, wc_prob, dc_prob, tx_prob, sb_prob;
	double SumExp;
	double sum;



	// save IVT coeff in a global variable for use in scaling additinal distance utility for the stops models
	mc_ivt_coeff = IN_VEH_TIM;


// determine availability of each alternative based on non-missing od skims, autos avalable,
// walk access available at origin, and walk egress available at destination.


	for (i=0; i < Ini->NUMBER_ALTS; i++)
		Avail[i] = 1;
	Avail[9] = 0;						// no non-motorized

	if (OD_Utility[0] == MISSING) 		// no da mode available
		Avail[0] = 0;
	if (OD_Utility[1] == MISSING) 		// no sr2 mode available
		Avail[1] = 0;
	if (OD_Utility[2] == MISSING) 		// no sr3 mode available
		Avail[2] = 0;
	if (OD_Utility[3] == MISSING) 		// no sr4 mode available
		Avail[3] = 0;
	if (OD_Utility[8] == MISSING) 		// no tx mode available
		Avail[8] = 0;

	if (OD_Utility[4] == MISSING)		// no walk to transit mode available for O/D
		Avail[4] = 0;
	if (OD_Utility[5] == MISSING)		// no drive to transit mode available for O/D
		Avail[5] = 0;
	if (OD_Utility[6] == MISSING)		// no walk to commuter rail mode available for O/D
		Avail[6] = 0;
	if (OD_Utility[7] == MISSING)		// no drive to commuter rail mode available for O/D
		Avail[7] = 0;
		
	if (autos == 0) {				// no drive alone mode or drive to transit modes available for HHs with no autos available.
		Avail[0] = 0;
	}
		
	if (walk[0] == 0) {			// no walk to transit mode available for O/D
		Avail[4] = 0;
		Avail[6] = 0;
	}

	if (walk[1] == 0) {			// no transit mode available for O/D
		Avail[4] = 0;
		Avail[5] = 0;
		Avail[6] = 0;
		Avail[7] = 0;
	}

	if (person_type == 1)
		Avail[0] = 0;
	else
		Avail[10] = 0;

	for (i=0; i < Ini->NUMBER_ALTS; i++)		// observed target proportion = 0.0 if ASC == MISSING, so make unavailable
		if (ASC[i] == MISSING)
			Avail[i] = 0;


//Calculate the total utilities at the lowest nest level.
	if (Avail[0])
		da  = OD_Utility[0] + SE_Utility[0] + ASC[0];
	else
		da = MISSING;
	if (Avail[1])
		sr2 = OD_Utility[1] + SE_Utility[1] + ASC[1];
	else
		sr2 = MISSING;
	if (Avail[2])
		sr3 = OD_Utility[2] + SE_Utility[2] + ASC[2];
	else
		sr3 = MISSING;
	if (Avail[3])
		sr4 = OD_Utility[3] + SE_Utility[3] + ASC[3];
	else
		sr4 = MISSING;
	if (Avail[4])
		wt  = OD_Utility[4] + SE_Utility[4] + ASC[4];
	else
		wt = MISSING;
	if (Avail[5])
		dt  = OD_Utility[5] + SE_Utility[5] + ASC[5];
	else
		dt = MISSING;
	if (Avail[6])
		wc  = OD_Utility[6] + SE_Utility[6] + ASC[6];
	else
		wc = MISSING;
	if (Avail[7])
		dc  = OD_Utility[7] + SE_Utility[7] + ASC[7];
	else
		dc = MISSING;
	if (Avail[8])
		tx  = OD_Utility[8] + SE_Utility[8] + ASC[8];
	else
		tx = MISSING;
	if (Avail[10])
		sb  = SE_Utility[10] + ASC[10];
	else
		sb = MISSING;


	TotalUtility[0] = (float)da;
	TotalUtility[1] = (float)sr2;
	TotalUtility[2] = (float)sr3;
	TotalUtility[3] = (float)sr4;
	TotalUtility[4] = (float)wt;
	TotalUtility[5] = (float)dt;
	TotalUtility[6] = (float)wc;
	TotalUtility[7] = (float)dc;
	TotalUtility[8] = (float)tx;
	TotalUtility[9] = (float)MISSING;
	TotalUtility[10] = (float)sb;

// Now calculate the next highest level (1 level higher than lowest) utilties
	SumExp	= 0.0;
	if (Avail[0]) {
		if (da < -MAX_EXP || da > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(da);
	}
	if (SumExp > 0.0)		
	 	da = THETA151*log(SumExp);
	else
		da = MISSING;	 	

	SumExp	= 0.0;
	if (Avail[1]) {
		if (sr2 < -MAX_EXP || sr2 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(sr2);
	}
	if (Avail[2]) {
		if (sr3 < -MAX_EXP || sr3 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(sr3);
	}
	if (Avail[3]) {
		if (sr4 < -MAX_EXP || sr4 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(sr4);
	}
	if (SumExp > 0.0)		
	 	sr = THETA151*log(SumExp);
	else
		sr = MISSING;	 	

	SumExp	= 0.0;
	if (Avail[4]) {
		if (wt < -MAX_EXP || wt > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(wt);
	}
	if (Avail[6]) {
		if (wc < -MAX_EXP || wc > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(wc);
	}
	if (SumExp > 0.0)		
	 	wt_wc = THETA151*log(SumExp);
	else
		wt_wc = MISSING;	 	

	SumExp	= 0.0;
	if (Avail[5]) {
		if (dt < -MAX_EXP || dt > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(dt);
	}
	if (Avail[7]) {
		if (dc < -MAX_EXP || dc > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(dc);
	}	
	if (SumExp > 0.0)		
	 	dt_dc = THETA151*log(SumExp);
	else
		dt_dc = MISSING;	 	

	SumExp	= 0.0;
	if (Avail[8]) {
		if (tx < -MAX_EXP || tx > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(tx);
	}
	if (SumExp > 0.0)		
	 	tx = THETA151*log(SumExp);
	else
		tx = MISSING;	 	

	SumExp	= 0.0;
	if (Avail[10]) {
		if (sb < -MAX_EXP || sb > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(sb);
	}
	if (SumExp > 0.0)		
	 	sb = THETA151*log(SumExp);
	else
		sb = MISSING;	 	


// Now the highest level (2 levels higher than lowest) utilties
	SumExp = 0.0;
	if (Avail[0]) {
		if (da < -MAX_EXP || da > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(da);
	}
	if (Avail[1] || Avail[2] || Avail[3]) {
		if (sr < -MAX_EXP || sr > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(sr);
	}
	if (Avail[4] || Avail[6]) {
		if (wt_wc < -MAX_EXP || wt_wc > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(wt_wc);
	}
	if (Avail[5]|| Avail[7]) {
		if (dt_dc < -MAX_EXP || dt_dc > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(dt_dc);
	}
	if (Avail[8]) {
		if (tx < -MAX_EXP || tx > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(tx);
	}
	if (Avail[10]) {
		if (sb < -MAX_EXP || sb > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(sb);
	}




// First calculate the shares at the top level.
	SumExp	= 0.0;
	if (da != MISSING) {
		if (da < -MAX_EXP || da > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(da);
	}
	if (sr != MISSING) {
		if (sr < -MAX_EXP || sr > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(sr);
	}
	if (wt_wc != MISSING) {
		if (wt_wc < -MAX_EXP || wt_wc > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(wt_wc);
	}
	if (dt_dc != MISSING) {
		if (dt_dc < -MAX_EXP || dt_dc > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(dt_dc);
	}
	if (tx != MISSING) {
		if (tx < -MAX_EXP || tx > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(tx);
	}
	if (sb != MISSING) {
		if (sb < -MAX_EXP || sb > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(sb);
	}


	if (da != MISSING)
		da_prob   = exp(da)/SumExp;
	else
		da_prob = 0.0;
	if (sr != MISSING)
		sr_prob = exp(sr)/SumExp;
	else
		sr_prob = 0.0;
	if (wt_wc != MISSING)
		w_prob = exp(wt_wc)/SumExp;
	else
		w_prob = 0.0;
	if (dt_dc != MISSING)
		d_prob = exp(dt_dc)/SumExp;
	else
		d_prob = 0.0;
	if (tx != MISSING)
		tx_prob = exp(tx)/SumExp;
	else
		tx_prob = 0.0;
	if (sb != MISSING)
		sb_prob = exp(sb)/SumExp;
	else
		sb_prob = 0.0;
		
		
// Next calculate the shared ride shares.
	SumExp	= 0.0;
	if (sr2 != MISSING) {
		if (sr2 < -MAX_EXP || sr2 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(sr2);
	}
	if (sr3 != MISSING) {
		if (sr3 < -MAX_EXP || sr3 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(sr3);
	}
	if (sr4 != MISSING) {
		if (sr4 < -MAX_EXP || sr4 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(sr4);
	}

	if (sr2 != MISSING)
		sr2_prob  = exp(sr2)/SumExp;
	else
		sr2_prob = 0.0;
	if (sr3 != MISSING)
		sr3_prob  = exp(sr3)/SumExp;
	else
		sr3_prob = 0.0;
	if (sr4 != MISSING)
		sr4_prob  = exp(sr4)/SumExp;
	else
		sr4_prob = 0.0;


// Now calculate the walk shares for walk to transit and commuter rail.
	SumExp	= 0.0;
	if (wt != MISSING) {
		if (wt < -MAX_EXP || wt > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(wt);
	}
	if (wc != MISSING) {
		if (wc < -MAX_EXP || wc > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(wc);
	}

	if (wt != MISSING)
		wt_prob = exp(wt)/SumExp;
	else
		wt_prob = 0.0;
	if (wc != MISSING)
		wc_prob = exp(wc)/SumExp;
	else
		wc_prob = 0.0;


// Now calculate the drive shares for drive to transit and commuter rail.
	SumExp	= 0.0;
	if (dt != MISSING) {
		if (dt < -MAX_EXP || dt > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(dt);
	}
	if (dc != MISSING) {
		if (dc < -MAX_EXP || dc > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(dc);
	}

	if (dt != MISSING)
		dt_prob = exp(dt)/SumExp;
	else
		dt_prob = 0.0;
	if (dc != MISSING)
		dc_prob = exp(dc)/SumExp;
	else
		dc_prob = 0.0;


// Finally, calculate the shares for each alternative at the lowest level (these must always sum to 1.0).
	Prob[0]   = (float)(da_prob);
	Prob[1]   = (float)(sr_prob*sr2_prob);
	Prob[2]   = (float)(sr_prob*sr3_prob);
	Prob[3]   = (float)(sr_prob*sr4_prob);
	Prob[4]   = (float)(w_prob*wt_prob);
	Prob[5]   = (float)(d_prob*dt_prob);
	Prob[6]   = (float)(w_prob*wc_prob);
	Prob[7]   = (float)(d_prob*dc_prob);
	Prob[8]   = (float)(tx_prob);
	Prob[9]   = (float)0.0;
	Prob[10]  = (float)(sb_prob);

	sum = 0.0;
	for (i=0; i < Ini->NUMBER_ALTS; i++)
		sum += Prob[i];
	if (fabs(sum - 1.0) > PROB_EPS) {
		printf ("\nprocessing journey #%d\n", journey);
		ExitWithCode(3);
	}

}
