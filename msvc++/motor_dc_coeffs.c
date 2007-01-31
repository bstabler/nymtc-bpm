#include "md.h"
#include "dc_coeffs.h"



void motor_dc_coeffs (int orig_county, int purpose, struct dc_coeff_data *DCcoeff, struct co_dist_factors *DistFactors)
{


	DCcoeff->alpha = DistFactors[orig_county].coeff;
	DCcoeff->theta = DistFactors[orig_county].exp;
	switch (purpose) {
		case (0):
			DCcoeff->beta	 = WORK_BETA_LO;
			DCcoeff->gamma[0] = WORK_EAST_LO;
			DCcoeff->gamma[1] = WORK_HUDSON_LO;
			DCcoeff->gamma[2] = WORK_MINOR_LO;
			DCcoeff->delta	 = WORK_INTRACO_LO;
			break;
		case (1):
			DCcoeff->beta	 = WORK_BETA_MD;
			DCcoeff->gamma[0] = WORK_EAST_MD;
			DCcoeff->gamma[1] = WORK_HUDSON_MD;
			DCcoeff->gamma[2] = WORK_MINOR_MD;
			DCcoeff->delta	 = WORK_INTRACO_MD;
			break;
		case (2):
			DCcoeff->beta	 = WORK_BETA_HI;
			DCcoeff->gamma[0] = WORK_EAST_HI;
			DCcoeff->gamma[1] = WORK_HUDSON_HI;
			DCcoeff->gamma[2] = WORK_MINOR_HI;
			DCcoeff->delta	 = WORK_INTRACO_HI;
			break;
		case (3):
			DCcoeff->beta	 = SCHOOL_BETA;
			DCcoeff->gamma[0] = SCHOOL_EAST;
			DCcoeff->gamma[1] = SCHOOL_HUDSON;
			DCcoeff->gamma[2] = SCHOOL_MINOR;
			DCcoeff->delta	 = SCHOOL_INTRACO;
			break;
		case (4):
			DCcoeff->beta	 = UNIV_BETA;
			DCcoeff->gamma[0] = UNIV_EAST;
			DCcoeff->gamma[1] = UNIV_HUDSON;
			DCcoeff->gamma[2] = UNIV_MINOR;
			DCcoeff->delta	 = UNIV_INTRACO;
			break;
		case (5):
			DCcoeff->beta	 = MAINT_BETA;
			DCcoeff->gamma[0] = MAINT_EAST;
			DCcoeff->gamma[1] = MAINT_HUDSON;
			DCcoeff->gamma[2] = MAINT_MINOR;
			DCcoeff->delta	 = MAINT_INTRACO;
			break;
		case (6):
			DCcoeff->beta	 = DISCR_BETA;
			DCcoeff->gamma[0] = DISCR_EAST;
			DCcoeff->gamma[1] = DISCR_HUDSON;
			DCcoeff->gamma[2] = DISCR_MINOR;
			DCcoeff->delta	 = DISCR_INTRACO;
			break;
		case (7):
			DCcoeff->beta	 = ATWORK_BETA;
			DCcoeff->gamma[0] = ATWORK_EAST;
			DCcoeff->gamma[1] = ATWORK_HUDSON;
			DCcoeff->gamma[2] = ATWORK_MINOR;
			DCcoeff->delta	 = ATWORK_INTRACO;
			break;
	}
}
