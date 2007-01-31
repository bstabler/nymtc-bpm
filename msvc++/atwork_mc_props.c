/*
 *	At work journey mode choice proportions.
 *
 *  Assumed NL structire:
 *
 *	     At work Journey Mode Choice
 *					|
 *					|
 *			|-------------------|
 *			|					|
 *			|					|
 *	|---DA/TX---|		|------ShRd-----|
 *	|			|		|		|		|
 *	|			|		|		|		|
 *	DA			TX		SR2		SR3		WT
 *
 */


#include <math.h> 
#include "md.h"
#include "atwork_coeff.h"

void atwork_mc_props (int journey, int *Avail, int person_type, int autos, int *walk, int PrevAuto, float *OD_Utility, float *SE_Utility, float *Prob, float *ASC, float *TotalUtility)
{
// autos = number of autos in the HH.
// walk is an int array of dimension 2.  walk[0] = 0 if HH cannot walk to transit at origin, 1 otherwise.
// walk[1] = 0 if walk to transit is not available at the destination, 1 otherwise.
// OD_Utility is a float array of the systematic portions of the modal utilities.


	int i;
	double da, sr2, sr3, sr4, wt, dt, wc, dc, tx;
	double da_tx, sr_tr;
	double da_tx_prob, sr_tr_prob;
	double da_prob, sr2_prob, sr3_prob;
	double wt_prob, tx_prob;
	double SumExp;
	double sum;



	// save IVT coeff in a global variable for use in scaling additinal distance utility for the stops models
	mc_ivt_coeff = IN_VEH_TIM;


// determine availability of each alternative based on non-missing od skims, autos avalable,
// walk access available at origin, and walk egress available at destination.


	for (i=0; i < Ini->NUMBER_ALTS; i++)		// no non-motorized or school bus available for at work
		Avail[i] = 1;
	Avail[9] = 0;
	Avail[10] = 0;

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
		
	if (PrevAuto == 0)
		Avail[0] = 0;
		
	Avail[3] = 0;				// no shared ride 4+ in this model
	Avail[5] = 0;				// no drive access transit in this model
	Avail[6] = 0;				// no walk access commuter rail in this model
	Avail[7] = 0;				// no drive access commuter rail in this model

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
	TotalUtility[10] = (float)MISSING;

// Now calculate the next highest level (1 level higher than lowest) utilties
	SumExp	= 0.0;
	if (Avail[0]) {
		if (da < -MAX_EXP || da > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(da);
	}
	if (Avail[8]) {
		if (tx < -MAX_EXP || tx > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(tx);
	}
	if (SumExp > 0.0)		
	 	da_tx = THETA151*log(SumExp);
	else
		da_tx = MISSING;	 	
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
	if (Avail[4]) {
		if (wt < -MAX_EXP || wt > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(wt);
	}
	if (SumExp > 0.0)		
	 	sr_tr = THETA151*log(SumExp);
	else
		sr_tr = MISSING;	 	


// Now the highest level (2 levels higher than lowest) utilties
	SumExp = 0.0;
	if (Avail[0] || Avail[8]) {
		if (da_tx < -MAX_EXP || da_tx > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(da_tx);
	}
	if (Avail[1] || Avail[2] || Avail[4]) {
		if (sr_tr < -MAX_EXP || sr_tr > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(sr_tr);
	}




// First calculate the shares at the top level.
	SumExp	= 0.0;
	if (da_tx != MISSING) {
		if (da_tx < -MAX_EXP || da_tx > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(da_tx);
	}
	if (sr_tr != MISSING) {
		if (sr_tr < -MAX_EXP || sr_tr > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(sr_tr);
	}

	if (da_tx != MISSING)
		da_tx_prob   = exp(da_tx)/SumExp;
	else
		da_tx_prob = 0.0;
	if (sr_tr != MISSING)
		sr_tr_prob = 1.0 - da_tx_prob;
	else
		sr_tr_prob = 0.0;
		
		
// Next calculate the drive alone / taxi shares.
	SumExp	= 0.0;
	if (da != MISSING) {
		if (da < -MAX_EXP || da > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(da);
	}
	if (tx != MISSING) {
		if (tx < -MAX_EXP || tx > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(tx);
	}

	if (da != MISSING)
		da_prob  = exp(da)/SumExp;
	else
		da_prob = 0.0;
	if (tx != MISSING)
		tx_prob  = 1.0 - da_prob;
	else
		tx_prob = 0.0;


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
	if (wt != MISSING) {
		if (wt < -MAX_EXP || wt > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(wt);
	}

	if (sr2 != MISSING)
		sr2_prob  = exp(sr2)/SumExp;
	else
		sr2_prob = 0.0;
	if (sr3 != MISSING)
		sr3_prob  = exp(sr3)/SumExp;
	else
		sr3_prob = 0.0;
	if (wt != MISSING)
		wt_prob = 1.0 - (sr2_prob + sr3_prob);
	else
		wt_prob = 0.0;



// Finally, calculate the shares for each alternative at the lowest level (these must always sum to 1.0).
	Prob[0]   = (float)(da_tx_prob*da_prob);
	Prob[1]   = (float)(sr_tr_prob*sr2_prob);
	Prob[2]   = (float)(sr_tr_prob*sr3_prob);
	Prob[3]   = (float)(0.0);
	Prob[4]   = (float)(sr_tr_prob*wt_prob);
	Prob[5]   = (float)(0.0);
	Prob[6]   = (float)(0.0);
	Prob[7]   = (float)(0.0);
	Prob[8]   = (float)(da_tx_prob*tx_prob);
	Prob[9]   = (float)(0.0);
	Prob[10]  = (float)(0.0);

	sum = 0.0;
	for (i=0; i < Ini->NUMBER_ALTS; i++)
		sum += Prob[i];
	if (fabs(sum - 1.0) > PROB_EPS) {
		printf ("\nprocessing journey #%d\n", journey);
		ExitWithCode(3);
	}

}
