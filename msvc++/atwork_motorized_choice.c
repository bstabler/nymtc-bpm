#include "md.h"


// At work journey Pre Mode Choice (Motorized vs NonMotorized Choice)
#define NM_WORK_MODE							-1.8820
#define NM_NO_TRANSIT							-1.2280
#define NM_LDENS2_MANHATTAN						 0.1105
#define NM_LDENS2_NON_MANHATTAN					 0.0393



int atwork_motorized_choice (int income, int autos, int workers, int person_type, int ut_orig, int walk_orig, int atwork_mode, int school_dist, double log_density, float ASC)
{
	double utility, nm_prop, nrand;
	
// calculate the non-motorized utility for pre mode-split	
	utility = 0.0;
	if (atwork_mode < 4)
		utility += NM_WORK_MODE;

	if (walk_orig != 1)
		utility += NM_NO_TRANSIT;

	if (ut_orig == 1)			// Manhattan
		utility += NM_LDENS2_MANHATTAN*log_density;
	else						// Non Manhattan
		utility += NM_LDENS2_NON_MANHATTAN*log_density;


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
