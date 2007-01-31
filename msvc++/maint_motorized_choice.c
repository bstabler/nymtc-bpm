#include "md.h"


// Maintenance journey Pre Mode Choice (Motorized vs NonMotorized Choice)
#define NM_NOA							-6.5360
#define NM_AFW							-7.3380
#define NM_ASW							-7.5890
#define NM_AMW							-7.9060
#define NM_LDENS2_MANHATTAN				 0.5404
#define NM_LDENS2_NON_MANHATTAN_LOW_MED	 0.5147
#define NM_LDENS2_NON_MANHATTAN_HIGH	 0.4672
#define NM_NO_TRANSIT_ACCESS			-0.2320



int maint_motorized_choice (int income, int autos, int workers, int person_type, int ut_orig, int walk_orig, int atwork_mode, int school_dist, double log_density, float ASC)
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

	if (walk_orig != 1)
		  utility += NM_NO_TRANSIT_ACCESS;

	if (ut_orig == 1)
		  utility += NM_LDENS2_MANHATTAN*log_density;
	else if (income == 3)
		  utility += NM_LDENS2_NON_MANHATTAN_HIGH*log_density;
	else
		  utility += NM_LDENS2_NON_MANHATTAN_LOW_MED*log_density;


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
