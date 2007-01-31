#include "md.h"


// non-motorized destination choice gammas are positive values since they are used in the
// denominator of the log density calculation.
#define WORK_NM_GAMMA_NONMANHATTAN		2.254
#define WORK_NM_GAMMA_MANHATTAN_LOWMED	2.070
#define WORK_NM_GAMMA_MANHATTAN_HIGH	1.697
#define SCHOOL_NM_GAMMA					1.867
#define UNIV_NM_GAMMA					1.913
#define MAINT_NM_GAMMA					2.266
#define DISCR_NM_GAMMA					2.246
#define ATWORK_NM_GAMMA					2.031


float nm_dest_gamma (int purpose, int ut_orig, int income)
{

	switch (purpose) {
		case (0):													// low income work
		case (1):													// medium income work
		case (2):													// high income work
			if (ut_orig != 1) 
				return ((float)WORK_NM_GAMMA_NONMANHATTAN);
			else if (income != 3)
				return ((float)WORK_NM_GAMMA_MANHATTAN_LOWMED);
			else
				return ((float)WORK_NM_GAMMA_MANHATTAN_HIGH);
			break;
		case (3):													// school
			return ((float)SCHOOL_NM_GAMMA);
			break;
		case (4):													// university
			return ((float)UNIV_NM_GAMMA);
			break;
		case (5):													// maintenance
			return ((float)MAINT_NM_GAMMA);
			break;
		case (6):													// discretionary
			return ((float)DISCR_NM_GAMMA);
			break;
		case (7):													// at work
			return ((float)ATWORK_NM_GAMMA);
			break;
	}

	printf ("Should not have reached this point in nm_dest_gamma().\n");
	printf ("purpose = %d\n", purpose);
	printf ("ut_orig = %d\n", ut_orig);
	printf ("income  = %d\n", income);
	fprintf (fp_rep, "Should not have reached this point in nm_dest_gamma().\n");
	fprintf (fp_rep, "purpose = %d\n", purpose);
	fprintf (fp_rep, "ut_orig = %d\n", ut_orig);
	fprintf (fp_rep, "income  = %d\n", income);
	exit (-16);
}
