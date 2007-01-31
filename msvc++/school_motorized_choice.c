#include "md.h"


// School journey Pre Mode Choice (Motorized vs NonMotorized Choice)
#define NM_NOA									-7.1420
#define NM_AM0									-8.0940
#define NM_NO_TRANSIT							-0.5116
#define NM_MANHATTAN_SCH_DIST1					 0.6963
#define NM_LDENS2_MANHATTAN						 0.5750
#define NM_LDENS2_NON_MANHATTAN					 0.6155



int school_motorized_choice (int income, int autos, int workers, int person_type, int ut_orig, int walk_orig, int atwork_mode, int school_dist, double log_density, float ASC)
{
	double utility, nm_prop, nrand;
	
// calculate the non-motorized utility for pre mode-split	
	utility = 0.0;
	if (autos == 0)				// no autos
		utility += NM_NOA;
	else
		utility += NM_AM0;

	if (walk_orig != 1)			// no transit walk access
		utility += NM_NO_TRANSIT;

	if (school_dist == 1)		// Manhattan origin
		utility += NM_MANHATTAN_SCH_DIST1;

	if (ut_orig == 1)			// Manhattan origin
		utility += NM_LDENS2_MANHATTAN*log_density;
	else
		utility += NM_LDENS2_NON_MANHATTAN*log_density;




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

	
	utility += ASC;


// draw a random number between 0,1.  If it is less than nm_prop,
// this journey is non-motorized (return 0), otherwise it is motorized (return 1).

	nrand = (double)rand()/(double)MAX_RANDOM;
	if (nrand < nm_prop)
		return (0);
	else
		return (1);

}
