#include "md.h"


// Work journey Pre Mode Choice (Motorized vs NonMotorized Choice, Run 40)
#define NM_NOA_AFW			-5.6290
#define NM_ASW				-6.2540
#define NM_AMW				-6.9220
#define NM_LDENS_MANHATTAN	 0.3517
#define NM_LDENS_URBAN		 0.3388
#define NM_LDENS_SUBURBAN	 0.2833
#define NM_LDENS_RURAL		 0.3477
#define NM_SUB_NOTRAN		-1.1000


int work_motorized_choice (int income, int autos, int workers, int person_type, int ut_orig, int walk_orig, int atwork_mode, int school_dist, double log_density, float ASC)
{
	double utility, nm_prop, nrand;
	
// calculate the non-motorized utility for pre mode-split	
	utility = 0.0;
	if (autos == 0 || autos < workers)
		utility += NM_NOA_AFW;
	else if (autos == workers)
		utility += NM_ASW;
	else if (autos  > workers)
		utility += NM_AMW;

	switch (ut_orig) {
		case (1):
			  utility += NM_LDENS_MANHATTAN*log_density;
			  break;
		case (2):
			  utility += NM_LDENS_URBAN*log_density;
			  break;
		case (3):
			  utility += NM_LDENS_SUBURBAN*log_density + (1 - walk_orig)*NM_SUB_NOTRAN;
			  break;
		case (4):
			  utility += NM_LDENS_RURAL*log_density;
			  break;
	}

	utility += ASC;


// calculate non-motorized proportion:
// non-motorized utility = utility
// motorized utility = 0.0
// therefore, non-motorized proportion = 1.0/(1.0 + exp(0.0 - utility))
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
