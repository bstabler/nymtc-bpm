/*
 *	Discretionary journey mode choice logsums.
 *
 *  Calculate the mode choice logsums to all destination tazs from the specified origin taz.
 *  Calculate the root level logsum for the estimated nested logit structure.
 *
 *  Which logsum calculation is done depends on the type of household, characterized by
 *  number of autos available, and if there is walk to transit available at the origin
 *  and at the destination of the journey.
 *
 *
 *
 *  Assumed NL structire:
 *
 *	              Discretionary Journey Mode Choice
 *								|
 *								|
 *			|---------------------------------------|
 *			|					|					|
 *			|					|					|
 *	|---DA/TX---|		|------ShRd-----|		|---Tran----|
 *	|			|		|		|		|		|	|	|	|
 *	|			|		|		|		|		|	|	|	|
 *	DA			TX		SR2		SR3		SR4+	WT	DT	WC	DC
 *
 */


#include <math.h> 
#include "md.h"
#include "discr_coeff.h"

void discr_mc_logsum (int journey, int orig, int dest, int *Avail, int person_type, int autos, int *walk, int PrevAuto, float *OD_Utility, float *SE_Utility, float *ASC, float *return_values)
{
// autos = number of autos in the HH.
// walk is an int array of dimension 2.  walk[0] = 0 if HH cannot walk to transit at origin, 1 otherwise.
// walk[1] = 0 if walk to transit is not available at the destination, 1 otherwise.
// ASC is a 2D float array of alternative specific constants -- urban type by modes.
// Theta is a 2D float array (Levels x alternatives) of NL scale coefficeients.
// OD_Utility is a float array of the systematic portions of the modal utilities.


	int i;
	double da, sr2, sr3, sr4, wt, dt, wc, dc, tx;
	double da_tx, sr, tr_cr;
	double SumExp, no_walk_SumExp, dval;
	double util_value;
    BOOL walk_ok, no_walk_ok;



// determine availability of each alternative based on non-missing od skims, autos avalable,
// walk access available at origin, and walk egress available at destination.


	for (i=0; i < Ini->NUMBER_ALTS; i++) {		// no non-motorized available for school
		if (ASC[i] == MISSING)
			Avail[i] = 0;
        else
    		Avail[i] = 1;
    }
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

//	if (walk[1] == 0) {			// no transit mode available for O/D
//		Avail[4] = 0;
//		Avail[5] = 0;
//		Avail[6] = 0;
//		Avail[7] = 0;
//	}

	if (person_type == 1)
		Avail[0] = 0;

//Calculate the total utilities at the lowest nest level.
// and calculate the next highest level (1 level higher than lowest) utilties
	SumExp	= 0.0;
	if (Avail[0]) {
		da  = OD_Utility[0] + SE_Utility[0] + ASC[0];
		if (da < -MAX_EXP || da > MAX_EXP) {
			printf ("\nerror in discr_mc_logsum() calculating da for journey #%d\n", journey);
			printf ("Avail[0]=%d, da=%.4e, OD_Utility[0]=%.4e, SE_Utility[0]=%.4e, ASC[0]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[0], da, OD_Utility[0], SE_Utility[0], ASC[0], SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in discr_mc_logsum() calculating da for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[0]=%d, da=%.4e, OD_Utility[0]=%.4e, SE_Utility[0]=%.4e, ASC[0]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[0], da, OD_Utility[0], SE_Utility[0], ASC[0], SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(da);
	}
	if (Avail[8]) {
		tx  = OD_Utility[8] + SE_Utility[8] + ASC[8];
		if (tx < -MAX_EXP || tx > MAX_EXP) {
			printf ( "\nerror in discr_mc_logsum() calculating tx for journey #%d\n", journey);
			printf ( "Avail[8]=%d, tx=%.4e, OD_Utility[8]=%.4e, SE_Utility[8]=%.4e, ASC[8]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[8], tx, OD_Utility[8], SE_Utility[8], ASC[8], SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in discr_mc_logsum() calculating tx for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[8]=%d, tx=%.4e, OD_Utility[8]=%.4e, SE_Utility[8]=%.4e, ASC[8]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[8], tx, OD_Utility[8], SE_Utility[8], ASC[8], SumExp, MAX_EXP );
			fflush(fp_rep);
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
		sr2 = OD_Utility[1] + SE_Utility[1] + ASC[1];
		if (sr2 < -MAX_EXP || sr2 > MAX_EXP) {
			printf ( "\nerror in discr_mc_logsum() calculating sr2 for journey #%d\n", journey);
			printf ( "Avail[1]=%d, sr2=%.4e, OD_Utility[1]=%.4e, SE_Utility[1]=%.4e, ASC[1]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[1], sr2, OD_Utility[1], SE_Utility[1], ASC[1], SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in discr_mc_logsum() calculating sr2 for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[1]=%d, sr2=%.4e, OD_Utility[1]=%.4e, SE_Utility[1]=%.4e, ASC[1]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[1], sr2, OD_Utility[1], SE_Utility[1], ASC[1], SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(sr2);
	}
	if (Avail[2]) {
		sr3 = OD_Utility[2] + SE_Utility[2] + ASC[2];
		if (sr3 < -MAX_EXP || sr3 > MAX_EXP) {
			printf ( "\nerror in discr_mc_logsum() calculating sr3 for journey #%d\n", journey);
			printf ( "Avail[2]=%d, sr3=%.4e, OD_Utility[2]=%.4e, SE_Utility[2]=%.4e, ASC[2]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[2], sr3, OD_Utility[2], SE_Utility[2], ASC[2], SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in discr_mc_logsum() calculating sr3 for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[2]=%d, sr3=%.4e, OD_Utility[2]=%.4e, SE_Utility[2]=%.4e, ASC[2]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[2], sr3, OD_Utility[2], SE_Utility[2], ASC[2], SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(sr3);
	}
	if (Avail[3]) {
		sr4 = OD_Utility[3] + SE_Utility[3] + ASC[3];
		if (sr4 < -MAX_EXP || sr4 > MAX_EXP) {
			printf ( "\nerror in discr_mc_logsum() calculating sr4 for journey #%d\n", journey);
			printf ( "Avail[3]=%d, sr4=%.4e, OD_Utility[3]=%.4e, SE_Utility[3]=%.4e, ASC[3]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[3], sr4, OD_Utility[3], SE_Utility[3], ASC[3], SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in discr_mc_logsum() calculating sr4 for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[3]=%d, sr4=%.4e, OD_Utility[3]=%.4e, SE_Utility[3]=%.4e, ASC[3]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[3], sr4, OD_Utility[3], SE_Utility[3], ASC[3], SumExp, MAX_EXP );
			fflush(fp_rep);
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
		wt  = OD_Utility[4] + SE_Utility[4] + ASC[4];
		if (wt < -MAX_EXP || wt > MAX_EXP) {
			printf ("\nerror in discr_mc_logsum() calculating wt for journey #%d\n", journey);
			printf ("Avail[4]=%d, wt=%.4e, OD_Utility[4]=%.4e, SE_Utility[4]=%.4e, ASC[4]=.6e, SumExp=%.4e, no_walk_SumExp=%.4e, MAX_EXP=%f\n", Avail[4], wt, OD_Utility[4], SE_Utility[4], ASC[4], SumExp, no_walk_SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in discr_mc_logsum() calculating wt for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[4]=%d, wt=%.4e, OD_Utility[4]=%.4e, SE_Utility[4]=%.4e, ASC[4]=.6e, SumExp=%.4e, no_walk_SumExp=%.4e, MAX_EXP=%f\n", Avail[4], wt, OD_Utility[4], SE_Utility[4], ASC[4], SumExp, no_walk_SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(wt);
	}
	if (Avail[5]) {
		dt  = OD_Utility[5] + SE_Utility[5] + ASC[5];
		if (dt < -MAX_EXP || dt > MAX_EXP) {
			printf ("\nerror in discr_mc_logsum() calculating dt for journey #%d\n", journey);
			printf ("Avail[5]=%d, dt=%.4e, OD_Utility[5]=%.4e, SE_Utility[5]=%.4e, ASC[5]=.6e, SumExp=%.4e, no_walk_SumExp=%.4e, MAX_EXP=%f\n", Avail[5], dt, OD_Utility[5], SE_Utility[5], ASC[5], SumExp, no_walk_SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in discr_mc_logsum() calculating dt for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[5]=%d, dt=%.4e, OD_Utility[5]=%.4e, SE_Utility[5]=%.4e, ASC[5]=.6e, SumExp=%.4e, no_walk_SumExp=%.4e, MAX_EXP=%f\n", Avail[5], dt, OD_Utility[5], SE_Utility[5], ASC[5], SumExp, no_walk_SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(dt);
	}
	if (Avail[6]) {
		wc  = OD_Utility[6] + SE_Utility[6] + ASC[6];
		if (wc < -MAX_EXP || wc > MAX_EXP) {
			printf ("\nerror in discr_mc_logsum() calculating wc for journey #%d\n", journey);
			printf ("Avail[6]=%d, wc=%.4e, OD_Utility[6]=%.4e, SE_Utility[6]=%.4e, ASC[6]=.6e, SumExp=%.4e, no_walk_SumExp=%.4e, MAX_EXP=%f\n", Avail[6], wc, OD_Utility[6], SE_Utility[6], ASC[6], SumExp, no_walk_SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in discr_mc_logsum() calculating wc for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[6]=%d, wc=%.4e, OD_Utility[6]=%.4e, SE_Utility[6]=%.4e, ASC[6]=.6e, SumExp=%.4e, no_walk_SumExp=%.4e, MAX_EXP=%f\n", Avail[6], wc, OD_Utility[6], SE_Utility[6], ASC[6], SumExp, no_walk_SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(wc);
	}
	if (Avail[7]) {
		dc  = OD_Utility[7] + SE_Utility[7] + ASC[7];
		if (dc < -MAX_EXP || dc > MAX_EXP) {
			printf ("\nerror in discr_mc_logsum() calculating dc for journey #%d\n", journey);
			printf ("Avail[7]=%d, dc=%.4e, OD_Utility[7]=%.4e, SE_Utility[7]=%.4e, ASC[7]=.6e, SumExp=%.4e, no_walk_SumExp=%.4e, MAX_EXP=%f\n", Avail[7], dc, OD_Utility[7], SE_Utility[7], ASC[7], SumExp, no_walk_SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in discr_mc_logsum() calculating dc for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[7]=%d, dc=%.4e, OD_Utility[7]=%.4e, SE_Utility[6]=%.4e, ASC[7]=.6e, SumExp=%.4e, no_walk_SumExp=%.4e, MAX_EXP=%f\n", Avail[7], dc, OD_Utility[7], SE_Utility[7], ASC[7], SumExp, no_walk_SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(dc);
	}	
	if (SumExp > 0.0)		
	 	tr_cr = THETA151*log(SumExp);
	else
		tr_cr = MISSING;	 	



// Now the highest level (2 levels higher than lowest) utilties
	SumExp = 0.0;
	if (Avail[0] || Avail[8]) {
		if (da_tx < -MAX_EXP || da_tx > MAX_EXP) {
			printf ( "\nerror in discr_mc_logsum() calculating da_tx for journey #%d\n", journey);
			printf ( "Avail[0]=%d, da_tx=%.4e, OD_Utility[0]=%.4e, SE_Utility[0]=%.4e, ASC[0]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[0], da_tx, OD_Utility[0], SE_Utility[0], ASC[0], SumExp, MAX_EXP );
			printf ( "Avail[8]=%d, da_tx=%.4e, OD_Utility[8]=%.4e, SE_Utility[8]=%.4e, ASC[8]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[8], da_tx, OD_Utility[8], SE_Utility[8], ASC[8], SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in discr_mc_logsum() calculating da_tx for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[0]=%d, da_tx=%.4e, OD_Utility[0]=%.4e, SE_Utility[0]=%.4e, ASC[0]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[0], da_tx, OD_Utility[0], SE_Utility[0], ASC[0], SumExp, MAX_EXP );
			fprintf (fp_rep, "Avail[8]=%d, da_tx=%.4e, OD_Utility[8]=%.4e, SE_Utility[8]=%.4e, ASC[8]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[8], da_tx, OD_Utility[8], SE_Utility[8], ASC[8], SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(da_tx);
	}
	if (Avail[1] || Avail[2] || Avail[3]) {
		if (sr < -MAX_EXP || sr > MAX_EXP) {
			printf ( "\nerror in discr_mc_logsum() calculating sr for journey #%d\n", journey);
			printf ( "Avail[1]=%d, sr=%.4e, OD_Utility[1]=%.4e, SE_Utility[1]=%.4e, ASC[1]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[1], sr, OD_Utility[1], SE_Utility[1], ASC[1], SumExp, MAX_EXP );
			printf ( "Avail[2]=%d, sr=%.4e, OD_Utility[2]=%.4e, SE_Utility[2]=%.4e, ASC[2]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[2], sr, OD_Utility[2], SE_Utility[2], ASC[2], SumExp, MAX_EXP );
			printf ( "Avail[3]=%d, sr=%.4e, OD_Utility[3]=%.4e, SE_Utility[3]=%.4e, ASC[3]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[3], sr, OD_Utility[3], SE_Utility[3], ASC[3], SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in discr_mc_logsum() calculating sr for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[1]=%d, sr=%.4e, OD_Utility[1]=%.4e, SE_Utility[1]=%.4e, ASC[1]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[1], sr, OD_Utility[1], SE_Utility[1], ASC[1], SumExp, MAX_EXP );
			fprintf (fp_rep, "Avail[2]=%d, sr=%.4e, OD_Utility[2]=%.4e, SE_Utility[2]=%.4e, ASC[2]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[2], sr, OD_Utility[2], SE_Utility[2], ASC[2], SumExp, MAX_EXP );
			fprintf (fp_rep, "Avail[3]=%d, sr=%.4e, OD_Utility[3]=%.4e, SE_Utility[3]=%.4e, ASC[3]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[3], sr, OD_Utility[3], SE_Utility[3], ASC[3], SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(sr);
	}
    no_walk_SumExp = SumExp;
	if (Avail[4] || Avail[5] || Avail[6] || Avail[7]) {
		if (tr_cr < -MAX_EXP || tr_cr > MAX_EXP) {
			printf ( "\nerror in discr_mc_logsum() calculating tr_cr for journey #%d\n", journey);
			printf ( "Avail[4]=%d, tr_cr=%.4e, OD_Utility[4]=%.4e, SE_Utility[4]=%.4e, ASC[4]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[4], tr_cr, OD_Utility[4], SE_Utility[4], ASC[4], SumExp, MAX_EXP );
			printf ( "Avail[5]=%d, tr_cr=%.4e, OD_Utility[5]=%.4e, SE_Utility[5]=%.4e, ASC[5]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[5], tr_cr, OD_Utility[5], SE_Utility[5], ASC[5], SumExp, MAX_EXP );
			printf ( "Avail[6]=%d, tr_cr=%.4e, OD_Utility[6]=%.4e, SE_Utility[6]=%.4e, ASC[6]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[6], tr_cr, OD_Utility[6], SE_Utility[6], ASC[6], SumExp, MAX_EXP );
			printf ( "Avail[7]=%d, tr_cr=%.4e, OD_Utility[7]=%.4e, SE_Utility[7]=%.4e, ASC[7]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[7], tr_cr, OD_Utility[7], SE_Utility[7], ASC[7], SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in discr_mc_logsum() calculating tr_cr for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[4]=%d, tr_cr=%.4e, OD_Utility[4]=%.4e, SE_Utility[4]=%.4e, ASC[4]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[4], tr_cr, OD_Utility[4], SE_Utility[4], ASC[4], SumExp, MAX_EXP );
			fprintf (fp_rep, "Avail[5]=%d, tr_cr=%.4e, OD_Utility[5]=%.4e, SE_Utility[5]=%.4e, ASC[5]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[5], tr_cr, OD_Utility[5], SE_Utility[5], ASC[5], SumExp, MAX_EXP );
			fprintf (fp_rep, "Avail[6]=%d, tr_cr=%.4e, OD_Utility[6]=%.4e, SE_Utility[6]=%.4e, ASC[6]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[6], tr_cr, OD_Utility[6], SE_Utility[6], ASC[6], SumExp, MAX_EXP );
			fprintf (fp_rep, "Avail[7]=%d, tr_cr=%.4e, OD_Utility[7]=%.4e, SE_Utility[7]=%.4e, ASC[7]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[7], tr_cr, OD_Utility[7], SE_Utility[7], ASC[7], SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(tr_cr);
	}
	if (no_walk_SumExp > 0.0) {
		return_values[0] = log(no_walk_SumExp);
        no_walk_ok = TRUE;
    }
	else {
		return_values[0] = MISSING;
        no_walk_ok = FALSE;
    }

	if (SumExp > 0.0) {
		return_values[1] = log(SumExp);
        walk_ok = TRUE;
    }
	else {
		return_values[1] = MISSING;
        walk_ok = FALSE;
    }

// logical tests on modal alternative utilities.
// program exits if test fails.
// logical tests on modal alternative utilities.
// program exits if test fails.
	if (no_walk_ok || walk_ok) {
	    for (i=0; i < Ini->NUMBER_ALTS; i++) {
		    if (Avail[i] == 1) {
    		    util_value = OD_Utility[i] + SE_Utility[i] + ASC[i];
                dval = util_value*THETA151_X_THETA152;

	    	    if (walk_ok && dval - return_values[1] > 0.00001)
		    	    logsum_check (i, journey, orig, dest, 
                        util_value, return_values[1], THETA151, THETA152, OD_Utility, SE_Utility, ASC);
                // Avail[i] would have been 0 before we included  walk mode
                if (no_walk_ok && (i < 4 || i > 7) && (dval - return_values[0] > 0.00001))
		    	    logsum_check (i, journey, orig, dest, 
                        util_value, return_values[0], THETA151, THETA152, OD_Utility, SE_Utility, ASC);
            }
	    }
    }
}
