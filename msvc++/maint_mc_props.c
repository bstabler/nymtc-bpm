/*
 *	Maintenance journey mode choice proportions.
 *
 *
 *  Assumed NL structire:
 *
 *		          Maintenance Journey Mode Choice
 *								|
 *								|
 *	|---------------------------------------------------------------|
 *	|							|									|
 *	|							|									|
 *	DA		|--------------------ShRd/Tran--------------|			TX
 *	|		|		|		|			|				|			|
 *	|		|		|		|			|				|			|
 *	DA		SR2		SR3		SR4+	|---Tr--|		|--CR---|		TX
 *	|		|		|		|		|		|		|		|		|
 *	|		|		|		|		|		|		|		|		|
 *	DA		SR2		SR3		SR4+	WT		DT		WC		DC		TX
 *
 */


#include <math.h> 
#include "md.h"
#include "maint_coeff.h"

void maint_mc_props (int journey, int *Avail, int person_type, int autos, int *walk, int PrevAuto, float *OD_Utility, float *SE_Utility, float *Prob, float *ASC, float *TotalUtility)
{
// autos = number of autos in the HH.
// walk is an int array of dimension 2.  walk[0] = 0 if HH cannot walk to transit at origin, 1 otherwise.
// walk[1] = 0 if walk to transit is not available at the destination, 1 otherwise.
// ASC is a 2D float array of alternative specific constants -- urban type by modes.
// Theta is a 2D float array (Levels x alternatives) of NL scale coefficeients.
// OD_Utility is a float array of the systematic portions of the modal utilities.


	int i;
	double da, sr2, sr3, sr4, wt, dt, wc, dc, tx;
	double da_2, sr2_2, sr3_2, sr4_2, tr_2, cr_2, tx_2;
	double da_3, srtr_3, tx_3;
	double da_prob, srtr_prob, tx_prob;
	double sr2_prob, sr3_prob, sr4_prob, tr_prob, cr_prob;
	double wt_prob, dt_prob, wc_prob, dc_prob;
	double SumExp;
	double sum;



	// save IVT coeff in a global variable for use in scaling additinal distance utility for the stops models
	mc_ivt_coeff = IN_VEH_TIME;


// determine availability of each alternative based on non-missing od skims, autos avalable,
// walk access available at origin, and walk egress available at destination.


	for (i=0; i < Ini->NUMBER_ALTS; i++)		// no non-motorized or school bus available for maintenance
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
//		Avail[5] = 0;
//		Avail[7] = 0;
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
	if (Avail[0])
		da_2 = THETA151*da;
	else
		da_2 = MISSING;
	if (Avail[1])
		sr2_2	= THETA151*sr2;
	else
		sr2_2 = MISSING;
	if (Avail[2])
		sr3_2	= THETA151*sr3;
	else
		sr3_2 = MISSING;
	if (Avail[3])
		sr4_2	= THETA151*sr4;
	else
		sr4_2 = MISSING;
	SumExp	= 0.0;
	if (Avail[4]) {
		if (wt < -MAX_EXP || wt > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(wt);
	}
	if (Avail[5]) {
		if (dt < -MAX_EXP || dt > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(dt);
	}
	if (SumExp > 0.0)		
	 	tr_2 = THETA151*log(SumExp);
	else
		tr_2 = MISSING;	 	
	SumExp	= 0.0;
	if (Avail[6]) {
		if (wc < -MAX_EXP || wc > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(wc);
	}
	if (Avail[7]) {
		if (dc < -MAX_EXP || dc > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(dc);
	}	
	if (SumExp > 0.0)		
	 	cr_2 = THETA151*log(SumExp);
	else
		cr_2 = MISSING;	 	
	if (Avail[8])
		tx_2 = THETA151*tx;
	else
		tx_2 = MISSING;


// Now the next highest level (2 levels higher than lowest) utilties
	if (Avail[0])
		da_3 = THETA152*da_2;
	else
		da_3 = MISSING;
	SumExp	= 0.0;
	if (Avail[1]) {
		if (sr2_2 < -MAX_EXP || sr2_2 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(sr2_2);
	}	
	if (Avail[2]) {
		if (sr3_2 < -MAX_EXP || sr3_2 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(sr3_2);
	}	
	if (Avail[3]) {
		if (sr4_2 < -MAX_EXP || sr4_2 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(sr4_2);
	}	
	if (Avail[4] || Avail[5]) {
		if (tr_2 < -MAX_EXP || tr_2 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(tr_2);
	}	
	if (Avail[6] || Avail[7]) {
		if (cr_2 < -MAX_EXP || cr_2 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(cr_2);
	}
	if (SumExp > 0.0)
		srtr_3 = THETA152*log(SumExp);
	else
		srtr_3 = MISSING;
	if (Avail[8])
		tx_3 = THETA152*tx_2;
	else
		tx_3 = MISSING;


// Now the highest level (3 levels higher than lowest) utilties
	SumExp = 0.0;
	if (Avail[0]) {
		if (da_3 < -MAX_EXP || da_3 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(da_3);
	}
	if (Avail[1] || Avail[2] || Avail[3] || Avail[4] || Avail[5] || Avail[6] || Avail[7]) {
		if (srtr_3 < -MAX_EXP || srtr_3 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(srtr_3);
	}
	if (Avail[8]) {
		if (tx_3 < -MAX_EXP || tx_3 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(tx_3);
	}



// First calculate the shares at the top level.
	SumExp	= 0.0;
	if (da_3 != MISSING) {
		if (da_3 < -MAX_EXP || da_3 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(da_3);
	}
	if (srtr_3 != MISSING) {
		if (srtr_3 < -MAX_EXP || srtr_3 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(srtr_3);
	}
	if (tx_3 != MISSING) {
		if (tx_3 < -MAX_EXP || tx_3 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(tx_3);
	}

	if (da_3 != MISSING)
		da_prob   = exp(da_3)/SumExp;
	else
		da_prob = 0.0;
	if (srtr_3 != MISSING)
		srtr_prob = exp(srtr_3)/SumExp;
	else
		srtr_prob = 0.0;
	if (tx_3 != MISSING)
		tx_prob = 1.0 - (da_prob + srtr_prob);
	else
		tx_prob = 0.0;
		
		
// Next calculate the shared ride/transit shares (da_prob and tx_prob don't change).
	SumExp	= 0.0;
	if (sr2_2 != MISSING) {
		if (sr2_2 < -MAX_EXP || sr2_2 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(sr2_2);
	}
	if (sr3_2 != MISSING) {
		if (sr3_2 < -MAX_EXP || sr3_2 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(sr3_2);
	}
	if (sr4_2 != MISSING) {
		if (sr4_2 < -MAX_EXP || sr4_2 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(sr4_2);
	}
	if (tr_2 != MISSING) {
		if (tr_2 < -MAX_EXP || tr_2 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(tr_2);
	}
	if (cr_2 != MISSING) {
		if (cr_2 < -MAX_EXP || cr_2 > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(cr_2);
	}

	if (sr2_2 != MISSING)
		sr2_prob  = exp(sr2_2)/SumExp;
	else
		sr2_prob = 0.0;
	if (sr3_2 != MISSING)
		sr3_prob  = exp(sr3_2)/SumExp;
	else
		sr3_prob = 0.0;
	if (sr4_2 != MISSING)
		sr4_prob  = exp(sr4_2)/SumExp;
	else
		sr4_prob = 0.0;
	if (tr_2 != MISSING)
		tr_prob   = exp(tr_2)/SumExp;
	else
		tr_prob = 0.0;
	if (cr_2 != MISSING)
		cr_prob   = exp(cr_2)/SumExp;
	else
		cr_prob = 0.0;


// Next calculate the walk/drive shares for transit.
	SumExp	= 0.0;
	if (wt != MISSING) {
		if (wt < -MAX_EXP || wt > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(wt);
	}
	if (dt != MISSING) {
		if (dt < -MAX_EXP || dt > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(dt);
	}

	if (wt != MISSING)
		wt_prob = exp(wt)/SumExp;
	else
		wt_prob = 0.0;
	if (dt != MISSING)
		dt_prob = exp(dt)/SumExp;
	else
		dt_prob = 0.0;


// Now calculate the walk/drive shares for commuter rail.
	SumExp	= 0.0;
	if (wc != MISSING) {
		if (wc < -MAX_EXP || wc > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(wc);
	}
	if (dc != MISSING) {
		if (dc < -MAX_EXP || dc > MAX_EXP) {
			printf ("\nprocessing journey #%d\n", journey);
			ExitWithCode(6);
		}
		SumExp += exp(dc);
	}

	if (wc != MISSING)
		wc_prob   = exp(wc)/SumExp;
	else
		wc_prob = 0.0;
	if (dc != MISSING)
		dc_prob   = exp(dc)/SumExp;
	else
		dc_prob = 0.0;


// Finally, calculate the shares for each alternative at the lowest level (these must always sum to 1.0).
	Prob[0]   = (float)(da_prob);
	Prob[1]   = (float)(srtr_prob*sr2_prob);
	Prob[2]   = (float)(srtr_prob*sr3_prob);
	Prob[3]   = (float)(srtr_prob*sr4_prob);
	Prob[4]   = (float)(srtr_prob*tr_prob*wt_prob);
	Prob[5]   = (float)(srtr_prob*tr_prob*dt_prob);
	Prob[6]   = (float)(srtr_prob*cr_prob*wc_prob);
	Prob[7]   = (float)(srtr_prob*cr_prob*dc_prob);
	Prob[8]   = (float)(tx_prob);
	Prob[9]   = (float)0.0;
	Prob[10]  = (float)0.0;

	sum = 0.0;
	for (i=0; i < Ini->NUMBER_ALTS; i++)
		sum += Prob[i];
	if (fabs(sum - 1.0) > PROB_EPS) {
		printf ("\nprocessing journey #%d\n", journey);
		ExitWithCode(3);
	}
}
