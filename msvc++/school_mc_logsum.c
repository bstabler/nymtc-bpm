/*
 *	School journey mode choice logsums.
 *
 *  Calculate the mode choice logsums to all destination tazs from the specified origin taz.
 *  Combine systematic utilities with alternative specific constants and calculate the root
 *  level logsum for the estimated nested logit structure.
 *
 *  Which logsum calculation is done depends on the type of household, characterized by
 *  number of autos available, and if there is walk to transit available at the origin
 *  and at the destination of the journey.
 *
 *
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

void school_mc_logsum (int journey, int orig, int dest, int *Avail, int person_type, int autos, int *walk, int PrevAuto, float *OD_Utility, float *SE_Utility, float *ASC, float *return_values)
{
// autos = number of autos in the HH.
// walk is an int array of dimension 2.  walk[0] = 0 if HH cannot walk to transit at origin, 1 otherwise.
// walk[1] = 0 if walk to transit is not available at the destination, 1 otherwise.
// ASC is a 2D float array of alternative specific constants -- urban type by modes.
// OD_Utility is a float array of the systematic portions of the modal utilities.


	int i;
	int debug = 0;
	double da, sr2, sr3, sr4, wt, dt, wc, dc, tx, sb;
	double wt_wc, sr, dt_dc;
	double SumExp, no_walk_SumExp, dval;
	double util_value;
    BOOL walk_ok, no_walk_ok;



// if journey is < 0, it was set to flag this function to print the debug statements
	if ( journey < 0 )
		debug = 1;



	if ( debug == 1 ) {
		fprintf (fp_rep, "\nDebug Info for: school journey = %d, orig=%d, dest=%d, person_type=%d, autos=%d, walk[0]=%d, walk[1]=%d\n\n", journey, orig, dest, person_type, autos, walk[0], walk[1]);
		for (i=0; i < Ini->NUMBER_ALTS; i++)
			fprintf (fp_rep, "i=%d, ASC[i] = %e, OD_Utility[i] = %e, SE_Utility[i] = %e\n", i, ASC[i], OD_Utility[i], SE_Utility[i]);
	}


// determine availability of each alternative based on non-missing od skims, autos avalable,
// walk access available at origin, and walk egress available at destination.


	for (i=0; i < Ini->NUMBER_ALTS; i++) {		// no non-motorized available for school
		if (ASC[i] == MISSING)
			Avail[i] = 0;
        else
    		Avail[i] = 1;
    }
	Avail[9] = 0;

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
	else
		Avail[10] = 0;

//	for (i=0; i < Ini->NUMBER_ALTS; i++)		// observed target proportion = 0.0 if ASC == MISSING, so make unavailable
//		if (ASC[i] == MISSING)
//			Avail[i] = 0;


	if ( debug == 1 ) {
		fprintf (fp_rep, "Avail[] = %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d\n", Avail[0], Avail[1], Avail[2], Avail[3], Avail[4], Avail[5], Avail[6], Avail[7], Avail[8], Avail[9], Avail[10]);
	}


//Calculate the total utilities at the lowest nest level.
// and calculate the next highest level (1 level higher than lowest) utilties
	SumExp	= 0.0;
	if (Avail[0]) {
		da  = OD_Utility[0] + SE_Utility[0] + ASC[0];
		if (da < -MAX_EXP || da > MAX_EXP) {
			fprintf (fp_rep, "\nprocessing journey #%d\n", journey);
			fflush(fp_rep);
			printf ("\nprocessing journey #%d\n", journey);
			fflush(stdout);
			ExitWithCode(6);
		}
		SumExp += exp(da);
	    if (SumExp > 0.0)		
	 	    da = THETA151*log(SumExp);
	    else
		    da = MISSING;	 	
	}

	if ( debug == 1 ) {
		fprintf (fp_rep, "da = %e, SumExp = %e\n", da, SumExp);
	}

	SumExp	= 0.0;
	if (Avail[1]) {
		sr2 = OD_Utility[1] + SE_Utility[1] + ASC[1];
		if (sr2 < -MAX_EXP || sr2 > MAX_EXP) {
			printf ( "\nerror in school_mc_logsum() calculating sr2 for journey #%d\n", journey);
			printf ( "Avail[1]=%d, sr2=%.4e, OD_Utility[1]=%.4e, SE_Utility[1]=%.4e, ASC[1]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[1], sr2, OD_Utility[1], SE_Utility[1], ASC[1], SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in school_mc_logsum() calculating sr2 for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[1]=%d, sr2=%.4e, OD_Utility[1]=%.4e, SE_Utility[1]=%.4e, ASC[1]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[1], sr2, OD_Utility[1], SE_Utility[1], ASC[1], SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(sr2);
	}
	if (Avail[2]) {
		sr3 = OD_Utility[2] + SE_Utility[2] + ASC[2];
		if (sr3 < -MAX_EXP || sr3 > MAX_EXP) {
			printf ( "\nerror in school_mc_logsum() calculating sr3 for journey #%d\n", journey);
			printf ( "Avail[2]=%d, sr3=%.4e, OD_Utility[2]=%.4e, SE_Utility[2]=%.4e, ASC[2]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[2], sr3, OD_Utility[2], SE_Utility[2], ASC[2], SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in school_mc_logsum() calculating sr3 for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[2]=%d, sr3=%.4e, OD_Utility[2]=%.4e, SE_Utility[2]=%.4e, ASC[2]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[2], sr3, OD_Utility[2], SE_Utility[2], ASC[2], SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(sr3);
	}
	if (Avail[3]) {
		sr4 = OD_Utility[3] + SE_Utility[3] + ASC[3];
		if (sr4 < -MAX_EXP || sr4 > MAX_EXP) {
			fprintf (fp_rep, "\nprocessing journey #%d\n", journey);
			fflush(fp_rep);
			printf ("\nprocessing journey #%d\n", journey);
			fflush(stdout);
			ExitWithCode(6);
		}
		SumExp += exp(sr4);
	}
	if (SumExp > 0.0)		
	 	sr = THETA151*log(SumExp);
	else
		sr = MISSING;
	 	
	if ( debug == 1 ) {
		fprintf (fp_rep, "sr2 = %e, sr3 = %e, sr4 = %e, sr = %e, SumExp = %e\n", sr2, sr3, sr4, sr, SumExp);
	}


	SumExp	= 0.0;
	if (Avail[4]) {
		wt  = OD_Utility[4] + SE_Utility[4] + ASC[4];
		if (wt < -MAX_EXP || wt > MAX_EXP) {
			printf ("\nerror in school_mc_logsum() calculating wt for journey #%d\n", journey);
			printf ("Avail[4]=%d, wt=%.4e, OD_Utility[4]=%.4e, SE_Utility[4]=%.4e, ASC[4]=.6e, SumExp=%.4e, no_walk_SumExp=%.4e, MAX_EXP=%f\n", Avail[4], wt, OD_Utility[4], SE_Utility[4], ASC[4], SumExp, no_walk_SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in school_mc_logsum() calculating wt for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[4]=%d, wt=%.4e, OD_Utility[4]=%.4e, SE_Utility[4]=%.4e, ASC[4]=.6e, SumExp=%.4e, no_walk_SumExp=%.4e, MAX_EXP=%f\n", Avail[4], wt, OD_Utility[4], SE_Utility[4], ASC[4], SumExp, no_walk_SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(wt);
	}
	if (Avail[6]) {
		wc  = OD_Utility[6] + SE_Utility[6] + ASC[6];
		if (wc < -MAX_EXP || wc > MAX_EXP) {
			printf ("\nerror in school_mc_logsum() calculating wc for journey #%d\n", journey);
			printf ("Avail[6]=%d, wc=%.4e, OD_Utility[6]=%.4e, SE_Utility[6]=%.4e, ASC[6]=.6e, SumExp=%.4e, no_walk_SumExp=%.4e, MAX_EXP=%f\n", Avail[6], wc, OD_Utility[6], SE_Utility[6], ASC[6], SumExp, no_walk_SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in school_mc_logsum() calculating wc for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[6]=%d, wc=%.4e, OD_Utility[6]=%.4e, SE_Utility[6]=%.4e, ASC[6]=.6e, SumExp=%.4e, no_walk_SumExp=%.4e, MAX_EXP=%f\n", Avail[6], wc, OD_Utility[6], SE_Utility[6], ASC[6], SumExp, no_walk_SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(wc);
	}
	if (SumExp > 0.0)		
	 	wt_wc = THETA151*log(SumExp);
	else
		wt_wc = MISSING;	
 	
	if ( debug == 1 ) {
		fprintf (fp_rep, "wt = %e, wc = %e, wt_wc = %e, SumExp = %e\n", wt, wc, wt_wc, SumExp);
	}


	SumExp	= 0.0;
	if (Avail[5]) {
		dt  = OD_Utility[5] + SE_Utility[5] + ASC[5];
		if (dt < -MAX_EXP || dt > MAX_EXP) {
			printf ("\nerror in school_mc_logsum() calculating dt for journey #%d\n", journey);
			printf ("Avail[5]=%d, dt=%.4e, OD_Utility[5]=%.4e, SE_Utility[5]=%.4e, ASC[5]=.6e, SumExp=%.4e, no_walk_SumExp=%.4e, MAX_EXP=%f\n", Avail[5], dt, OD_Utility[5], SE_Utility[5], ASC[5], SumExp, no_walk_SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in school_mc_logsum() calculating dt for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[5]=%d, dt=%.4e, OD_Utility[5]=%.4e, SE_Utility[5]=%.4e, ASC[5]=.6e, SumExp=%.4e, no_walk_SumExp=%.4e, MAX_EXP=%f\n", Avail[5], dt, OD_Utility[5], SE_Utility[5], ASC[5], SumExp, no_walk_SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(dt);
	}
	if (Avail[7]) {
		dc  = OD_Utility[7] + SE_Utility[7] + ASC[7];
		if (dc < -MAX_EXP || dc > MAX_EXP) {
			printf ("\nerror in school_mc_logsum() calculating dc for journey #%d\n", journey);
			printf ("Avail[7]=%d, dc=%.4e, OD_Utility[7]=%.4e, SE_Utility[7]=%.4e, ASC[7]=.6e, SumExp=%.4e, no_walk_SumExp=%.4e, MAX_EXP=%f\n", Avail[7], dc, OD_Utility[7], SE_Utility[7], ASC[7], SumExp, no_walk_SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in school_mc_logsum() calculating dc for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[7]=%d, dc=%.4e, OD_Utility[7]=%.4e, SE_Utility[7]=%.4e, ASC[7]=.6e, SumExp=%.4e, no_walk_SumExp=%.4e, MAX_EXP=%f\n", Avail[7], dc, OD_Utility[7], SE_Utility[7], ASC[7], SumExp, no_walk_SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(dc);
	}	
	if (SumExp > 0.0)		
	 	dt_dc = THETA151*log(SumExp);
	else
		dt_dc = MISSING;	 	

	if ( debug == 1 ) {
		fprintf (fp_rep, "dt = %e, dc = %e, dt_dc = %e, SumExp = %e\n", dt, dc, dt_dc, SumExp);
	}


	SumExp	= 0.0;
	if (Avail[8]) {
		tx  = OD_Utility[8] + SE_Utility[8] + ASC[8];
		if (tx < -MAX_EXP || tx > MAX_EXP) {
			printf ( "\nerror in school_mc_logsum() calculating tx for journey #%d\n", journey);
			printf ( "Avail[8]=%d, tx=%.4e, OD_Utility[8]=%.4e, SE_Utility[8]=%.4e, ASC[8]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[8], tx, OD_Utility[8], SE_Utility[8], ASC[8], SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in school_mc_logsum() calculating tx for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[8]=%d, tx=%.4e, OD_Utility[8]=%.4e, SE_Utility[8]=%.4e, ASC[8]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[8], tx, OD_Utility[8], SE_Utility[8], ASC[8], SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(tx);
	    if (SumExp > 0.0)		
	 	    tx = THETA151*log(SumExp);
	    else
		    tx = MISSING;	 	
	}
	else
		tx = MISSING;

	if ( debug == 1 ) {
		fprintf (fp_rep, "tx = %e, SumExp = %e\n", tx, SumExp);
	}

	SumExp	= 0.0;
	if (Avail[10]) {
		sb  = SE_Utility[10] + ASC[10];
		if (sb < -MAX_EXP || sb > MAX_EXP) {
			printf ( "\nerror in school_mc_logsum() calculating sb for journey #%d\n", journey);
			printf ( "Avail[10]=%d, sb=%.4e, OD_Utility[10]=%.4e, SE_Utility[8]=%.4e, ASC[10]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[10], sb, OD_Utility[10], SE_Utility[10], ASC[10], SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in school_mc_logsum() calculating sb for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[10]=%d, sb=%.4e, OD_Utility[10]=%.4e, SE_Utility[10]=%.4e, ASC[10]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[10], sb, OD_Utility[10], SE_Utility[10], ASC[10], SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(sb);
	    if (SumExp > 0.0)		
	 	    sb = THETA151*log(SumExp);
	    else
		    sb = MISSING;	 	
	}
	else
		sb = MISSING;

	if ( debug == 1 ) {
		fprintf (fp_rep, "sb = %e, SumExp = %e\n", sb, SumExp);
	}




// Now the highest level (2 levels higher than lowest) utilties
	SumExp = 0.0;
	if (Avail[0]) {
		if (da < -MAX_EXP || da > MAX_EXP) {
			printf ("\nerror in school_mc_logsum() calculating da for journey #%d\n", journey);
			printf ("Avail[0]=%d, da=%.4e, OD_Utility[0]=%.4e, SE_Utility[0]=%.4e, ASC[0]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[0], da, OD_Utility[0], SE_Utility[0], ASC[0], SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in school_mc_logsum() calculating da for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[0]=%d, da=%.4e, OD_Utility[0]=%.4e, SE_Utility[0]=%.4e, ASC[0]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[0], da, OD_Utility[0], SE_Utility[0], ASC[0], SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(da);
	}
	if (Avail[1] || Avail[2] || Avail[3]) {
		if (sr < -MAX_EXP || sr > MAX_EXP) {
			printf ("\nerror in school_mc_logsum() calculating sr for journey #%d\n", journey);
			printf ( "Avail[1]=%d, sr=%.4e, OD_Utility[1]=%.4e, SE_Utility[1]=%.4e, ASC[1]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[1], sr, OD_Utility[1], SE_Utility[1], ASC[1], SumExp, MAX_EXP );
			printf ( "Avail[2]=%d, sr=%.4e, OD_Utility[2]=%.4e, SE_Utility[2]=%.4e, ASC[2]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[2], sr, OD_Utility[2], SE_Utility[2], ASC[2], SumExp, MAX_EXP );
			printf ( "Avail[3]=%d, sr=%.4e, OD_Utility[3]=%.4e, SE_Utility[3]=%.4e, ASC[3]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[3], sr, OD_Utility[3], SE_Utility[3], ASC[3], SumExp, MAX_EXP );
			printf ( "\nerror in school_mc_logsum() calculating sr for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[1]=%d, sr=%.4e, OD_Utility[1]=%.4e, SE_Utility[1]=%.4e, ASC[1]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[1], sr, OD_Utility[1], SE_Utility[1], ASC[1], SumExp, MAX_EXP );
			fprintf (fp_rep, "Avail[2]=%d, sr=%.4e, OD_Utility[2]=%.4e, SE_Utility[2]=%.4e, ASC[2]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[2], sr, OD_Utility[2], SE_Utility[2], ASC[2], SumExp, MAX_EXP );
			fprintf (fp_rep, "Avail[3]=%d, sr=%.4e, OD_Utility[3]=%.4e, SE_Utility[3]=%.4e, ASC[3]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[3], sr, OD_Utility[3], SE_Utility[3], ASC[3], SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(sr);
	}
    no_walk_SumExp = SumExp;
	if (Avail[4] || Avail[6]) {
		if (wt_wc < -MAX_EXP || wt_wc > MAX_EXP) {
			printf ("\nerror in school_mc_logsum() calculating wt_wc for journey #%d\n", journey);
			printf ( "Avail[4]=%d, wt_wc=%.4e, OD_Utility[4]=%.4e, SE_Utility[4]=%.4e, ASC[4]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[4], wt_wc, OD_Utility[4], SE_Utility[4], ASC[4], SumExp, MAX_EXP );
			printf ( "Avail[6]=%d, wt_wc=%.4e, OD_Utility[6]=%.4e, SE_Utility[6]=%.4e, ASC[6]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[6], wt_wc, OD_Utility[6], SE_Utility[6], ASC[6], SumExp, MAX_EXP );
			printf ( "\nerror in school_mc_logsum() calculating wt_wc for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[4]=%d, wt_wc=%.4e, OD_Utility[4]=%.4e, SE_Utility[4]=%.4e, ASC[4]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[4], wt_wc, OD_Utility[4], SE_Utility[4], ASC[4], SumExp, MAX_EXP );
			fprintf (fp_rep, "Avail[6]=%d, wt_wc=%.4e, OD_Utility[6]=%.4e, SE_Utility[6]=%.4e, ASC[6]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[6], wt_wc, OD_Utility[6], SE_Utility[6], ASC[6], SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(wt_wc);
	}
	if (Avail[5]|| Avail[7]) {
		if (dt_dc < -MAX_EXP || dt_dc > MAX_EXP) {
			printf ("\nerror in school_mc_logsum() calculating dt_dc for journey #%d\n", journey);
			printf ( "Avail[5]=%d, dt_dc=%.4e, OD_Utility[5]=%.4e, SE_Utility[5]=%.4e, ASC[5]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[5], dt_dc, OD_Utility[5], SE_Utility[5], ASC[5], SumExp, MAX_EXP );
			printf ( "Avail[7]=%d, dt_dc=%.4e, OD_Utility[7]=%.4e, SE_Utility[7]=%.4e, ASC[7]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[7], dt_dc, OD_Utility[7], SE_Utility[7], ASC[7], SumExp, MAX_EXP );
			printf ( "\nerror in school_mc_logsum() calculating dt_dc for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[5]=%d, dt_dc=%.4e, OD_Utility[5]=%.4e, SE_Utility[5]=%.4e, ASC[5]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[5], dt_dc, OD_Utility[5], SE_Utility[5], ASC[5], SumExp, MAX_EXP );
			fprintf (fp_rep, "Avail[7]=%d, dt_dc=%.4e, OD_Utility[7]=%.4e, SE_Utility[7]=%.4e, ASC[7]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[7], dt_dc, OD_Utility[7], SE_Utility[7], ASC[7], SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
		SumExp += exp(dt_dc);
	}
	if (Avail[8]) {
		if (tx < -MAX_EXP || tx > MAX_EXP) {
			printf ("\nerror in school_mc_logsum() calculating tx for journey #%d\n", journey);
			printf ("Avail[8]=%d, tx=%.4e, OD_Utility[8]=%.4e, SE_Utility[8]=%.4e, ASC[8]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[8], tx, OD_Utility[8], SE_Utility[8], ASC[8], SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in school_mc_logsum() calculating tx for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[8]=%d, tx=%.4e, OD_Utility[8]=%.4e, SE_Utility[8]=%.4e, ASC[8]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[8], tx, OD_Utility[8], SE_Utility[8], ASC[8], SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
        dval = exp(tx);
		SumExp += dval;
        no_walk_SumExp += dval;
	}
	if (Avail[10]) {
		if (sb < -MAX_EXP || sb > MAX_EXP) {
			printf ("\nerror in school_mc_logsum() calculating sb for journey #%d\n", journey);
			printf ("Avail[10]=%d, sb=%.4e, OD_Utility[10]=%.4e, SE_Utility[10]=%.4e, ASC[10]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[10], sb, OD_Utility[10], SE_Utility[10], ASC[10], SumExp, MAX_EXP );
			fprintf (fp_rep, "\nerror in school_mc_logsum() calculating sb for journey #%d\n", journey);
			fprintf (fp_rep, "Avail[10]=%d, sb=%.4e, OD_Utility[10]=%.4e, SE_Utility[10]=%.4e, ASC[10]=.6e, SumExp=%.4e, MAX_EXP=%f\n", Avail[10], sb, OD_Utility[10], SE_Utility[10], ASC[10], SumExp, MAX_EXP );
			fflush(fp_rep);
			ExitWithCode(6);
		}
        dval = exp(sb);
		SumExp += dval;
        no_walk_SumExp += dval;
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



	if ( debug == 1 ) {
		fprintf (fp_rep, "no_walk_SumExp = %e, SumExp = %e, return_values[0] = %e, return_values[1] = %e\n", no_walk_SumExp, SumExp, return_values[0], return_values[1]);
		fprintf (fp_rep, "no_walk_ok = %d, walk_ok = %d\n", no_walk_ok, walk_ok);
	}




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
