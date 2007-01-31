#include "md.h"


// University journey Pre Mode Choice (Motorized vs NonMotorized Choice)
#define NM_ADD_WORKER							-0.7984
#define NM_NOA									-6.3290
#define NM_AFW_ASW								-6.7520
#define NM_AMW									-7.4710
#define NM_LDENS2_WORKER						 0.5724
#define NM_LDENS2_NON_WORKER					 0.6137



int univ_motorized_choice (int income, int autos, int workers, int person_type, int ut_orig, int walk_orig, int atwork_mode, int school_dist, double log_density, float ASC)
{
	double utility, nm_prop, nrand;
	
// calculate the non-motorized utility for pre mode-split	
	utility = 0.0;
	if (autos == 0)
		utility += NM_NOA;
	else if (autos < workers || autos == workers)
		utility += NM_AFW_ASW;
	else if (autos > workers)
		utility += NM_AMW;

	if ((person_type == 3) && (workers > 1))	// Worker making university journey with at least one additional worker in the household.
		utility += NM_ADD_WORKER;
	if ((person_type != 3) && (workers > 0))	// Non-worker making university journey with at least one additional worker in the household.
		utility += NM_ADD_WORKER;

	if (person_type == 3)	// Worker
		utility += NM_LDENS2_WORKER*log_density;
	else					// Not a Worker
		utility += NM_LDENS2_NON_WORKER*log_density;

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
