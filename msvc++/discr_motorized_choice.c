#include "md.h"


// Discretionary journey Pre Mode Choice (Motorized vs NonMotorized Choice)
#define NM_NOA									-2.8970
#define NM_AFW									-3.1500
#define NM_ASW									-4.0600
#define NM_AMW									-4.2100
#define NM_LDENS2_CHILD_MANHATTAN				 0.3603
#define NM_LDENS2_CHILD_NON_MANHATTAN			 0.2707
#define NM_LDENS2_NOT_CHILD_MANHATTAN			 0.2585
#define NM_LDENS2_NOT_CHILD_NON_MANHATTAN_LO	 0.2655
#define NM_LDENS2_NOT_CHILD_NON_MANHATTAN_MD_HI	 0.1966



int discr_motorized_choice (int income, int autos, int workers, int person_type, int ut_orig, int walk_orig, int atwork_mode, int school_dist, double log_density, float ASC)
{
	double utility, nm_prop, nrand;
	
// calculate the non-motorized utility for pre mode-split	
	utility = 0.0;
	if (autos == 0)
		utility += NM_NOA;
	else if (autos < workers)
		utility += NM_AFW;
	else if (autos == workers)
		utility += NM_ASW;
	else if (autos > workers)
		utility += NM_AMW;

	if (ut_orig == 1) {			// Manhattan
		if (person_type == 1)	// Child
			utility += NM_LDENS2_CHILD_MANHATTAN*log_density;
		else					// Not Child
			utility += NM_LDENS2_NOT_CHILD_MANHATTAN*log_density;
	}
	else {						// Non-Manhattan
		if (person_type == 1) {	// Child
			utility += NM_LDENS2_CHILD_MANHATTAN*log_density;
		}
		else {					// Not Child
			if (income == 1)	// Low Income
				utility += NM_LDENS2_NOT_CHILD_NON_MANHATTAN_LO*log_density;
			else				// Medium or High Income
				utility += NM_LDENS2_NOT_CHILD_NON_MANHATTAN_MD_HI*log_density;
		}
	}


	utility += ASC;



// calculate non-motorized proportion:
//		non-motorized utility = utility
//		motorized utility = 0.0
//		therefore, non-motorized proportion = 1.0/(1.0 + exp(0.0 - utility))
	if (utility < -MAX_EXP)
		nm_prop = 0.0;
	else if (utility > MAX_EXP)
		nm_prop = 1.0;
	else
		nm_prop = (1.0/(1.0 + exp(-utility)));
		

// draw a random number between 0,1.  If it is less than nm_prop,
// this journey is non-motorized (return 0), otherwise it is motorized (return 1).
	nrand = (double)rand()/(double)MAX_RANDOM;
	if (nrand < nm_prop)
		return (0);
	else
		return (1);

}
