/*
 * Calculate size variable for stop frequency density.
 */

#include "md.h"


#define OB_WORK_POP      0.0000
#define OB_WORK_POP1     0.0000
#define OB_WORK_POP2     0.0000
#define OB_WORK_POP3     0.0000
#define OB_WORK_RETAIL   0.0000
#define OB_WORK_RETAIL1  0.4617
#define OB_WORK_RETAIL2  0.3770
#define OB_WORK_RETAIL3  0.2301
#define OB_WORK_OFFICE   0.0000
#define OB_WORK_OFFICE1  0.0000
#define OB_WORK_OFFICE2  0.0000
#define OB_WORK_OFFICE3  0.2164
#define OB_WORK_OTHER    0.0000
#define OB_WORK_OTHER1   0.0000
#define OB_WORK_OTHER2   0.0000
#define OB_WORK_OTHER3   0.0000
#define OB_WORK_TOTAL    0.0000
#define OB_WORK_TOTAL1   0.0000
#define OB_WORK_TOTAL2   0.0000
#define OB_WORK_TOTAL3   0.0000
#define OB_WORK_K12ETOT1 0.2896
#define OB_WORK_K12ETOT2 0.0709
#define OB_WORK_K12ETOT3 0.1545
#define OB_WORK_UNIV1    0.0000
#define OB_WORK_UNIV2    0.0000
#define OB_WORK_UNIV3    0.0000

#define IB_WORK_POP      0.0000
#define IB_WORK_POP1     0.0628
#define IB_WORK_POP2     0.0386
#define IB_WORK_POP3     0.0436
#define IB_WORK_RETAIL   0.0000
#define IB_WORK_RETAIL1  0.8404
#define IB_WORK_RETAIL2  0.2579
#define IB_WORK_RETAIL3  0.4390
#define IB_WORK_OFFICE   0.0000
#define IB_WORK_OFFICE1  0.0000
#define IB_WORK_OFFICE2  0.1111
#define IB_WORK_OFFICE3  0.1234
#define IB_WORK_OTHER    0.0000
#define IB_WORK_OTHER1   0.0000
#define IB_WORK_OTHER2   0.0000
#define IB_WORK_OTHER3   0.0000
#define IB_WORK_TOTAL    0.0000
#define IB_WORK_TOTAL1   0.0000
#define IB_WORK_TOTAL2   0.0000
#define IB_WORK_TOTAL3   0.0000
#define IB_WORK_K12ETOT1 0.0000
#define IB_WORK_K12ETOT2 0.0000
#define IB_WORK_K12ETOT3 0.0000
#define IB_WORK_UNIV1    0.0730
#define IB_WORK_UNIV2    0.0586
#define IB_WORK_UNIV3    0.0000



#define OB_SCHOOL_POP      0.0000
#define OB_SCHOOL_POP1     0.0043
#define OB_SCHOOL_POP2     0.0049
#define OB_SCHOOL_POP3     0.0060
#define OB_SCHOOL_RETAIL   0.0000
#define OB_SCHOOL_RETAIL1  0.0000
#define OB_SCHOOL_RETAIL2  0.0604
#define OB_SCHOOL_RETAIL3  0.0487
#define OB_SCHOOL_OFFICE   0.0000
#define OB_SCHOOL_OFFICE1  0.0000
#define OB_SCHOOL_OFFICE2  0.0000
#define OB_SCHOOL_OFFICE3  0.0000
#define OB_SCHOOL_OTHER    0.0000
#define OB_SCHOOL_OTHER1   0.0000
#define OB_SCHOOL_OTHER2   0.0000
#define OB_SCHOOL_OTHER3   0.0000
#define OB_SCHOOL_TOTAL    0.0000
#define OB_SCHOOL_TOTAL1   0.0000
#define OB_SCHOOL_TOTAL2   0.0000
#define OB_SCHOOL_TOTAL3   0.0000
#define OB_SCHOOL_K12ETOT1 0.0000
#define OB_SCHOOL_K12ETOT2 0.0000
#define OB_SCHOOL_K12ETOT3 0.0000
#define OB_SCHOOL_UNIV1    0.0000
#define OB_SCHOOL_UNIV2    0.0000
#define OB_SCHOOL_UNIV3    0.0000

#define IB_SCHOOL_POP      0.0000
#define IB_SCHOOL_POP1     0.0192
#define IB_SCHOOL_POP2     0.0165
#define IB_SCHOOL_POP3     0.0091
#define IB_SCHOOL_RETAIL   0.0000
#define IB_SCHOOL_RETAIL1  0.0000
#define IB_SCHOOL_RETAIL2  0.0000
#define IB_SCHOOL_RETAIL3  0.0498
#define IB_SCHOOL_OFFICE   0.0000
#define IB_SCHOOL_OFFICE1  0.0000
#define IB_SCHOOL_OFFICE2  0.0000
#define IB_SCHOOL_OFFICE3  0.0000
#define IB_SCHOOL_OTHER    0.0000
#define IB_SCHOOL_OTHER1   0.0000
#define IB_SCHOOL_OTHER2   0.0000
#define IB_SCHOOL_OTHER3   0.0000
#define IB_SCHOOL_TOTAL    0.0000
#define IB_SCHOOL_TOTAL1   0.0000
#define IB_SCHOOL_TOTAL2   0.0000
#define IB_SCHOOL_TOTAL3   0.0000
#define IB_SCHOOL_K12ETOT1 0.0000
#define IB_SCHOOL_K12ETOT2 0.0000
#define IB_SCHOOL_K12ETOT3 0.0627
#define IB_SCHOOL_UNIV1    0.0000
#define IB_SCHOOL_UNIV2    0.0000
#define IB_SCHOOL_UNIV3    0.0000



#define OB_UNIV_POP      0.0000
#define OB_UNIV_POP1     0.0000
#define OB_UNIV_POP2     0.0028
#define OB_UNIV_POP3     0.0000
#define OB_UNIV_RETAIL   0.0000
#define OB_UNIV_RETAIL1  0.0000
#define OB_UNIV_RETAIL2  0.0000
#define OB_UNIV_RETAIL3  0.0271
#define OB_UNIV_OFFICE   0.0000
#define OB_UNIV_OFFICE1  0.0055
#define OB_UNIV_OFFICE2  0.0113
#define OB_UNIV_OFFICE3  0.0000
#define OB_UNIV_OTHER    0.0000
#define OB_UNIV_OTHER1   0.0000
#define OB_UNIV_OTHER2   0.0000
#define OB_UNIV_OTHER3   0.0000
#define OB_UNIV_TOTAL    0.0000
#define OB_UNIV_TOTAL1   0.0000
#define OB_UNIV_TOTAL2   0.0000
#define OB_UNIV_TOTAL3   0.0000
#define OB_UNIV_K12ETOT1 0.0000
#define OB_UNIV_K12ETOT2 0.0000
#define OB_UNIV_K12ETOT3 0.0000
#define OB_UNIV_UNIV1    0.0083
#define OB_UNIV_UNIV2    0.0169
#define OB_UNIV_UNIV3    0.0000

#define IB_UNIV_POP      0.0000
#define IB_UNIV_POP1     0.0000
#define IB_UNIV_POP2     0.0000
#define IB_UNIV_POP3     0.0000
#define IB_UNIV_RETAIL   0.0276
#define IB_UNIV_RETAIL1  0.0000
#define IB_UNIV_RETAIL2  0.0000
#define IB_UNIV_RETAIL3  0.0000
#define IB_UNIV_OFFICE   0.0030
#define IB_UNIV_OFFICE1  0.0000
#define IB_UNIV_OFFICE2  0.0000
#define IB_UNIV_OFFICE3  0.0000
#define IB_UNIV_OTHER    0.0063
#define IB_UNIV_OTHER1   0.0000
#define IB_UNIV_OTHER2   0.0000
#define IB_UNIV_OTHER3   0.0000
#define IB_UNIV_TOTAL    0.0000
#define IB_UNIV_TOTAL1   0.0000
#define IB_UNIV_TOTAL2   0.0000
#define IB_UNIV_TOTAL3   0.0000
#define IB_UNIV_K12ETOT1 0.0000
#define IB_UNIV_K12ETOT2 0.0000
#define IB_UNIV_K12ETOT3 0.0000
#define IB_UNIV_UNIV1    0.0000
#define IB_UNIV_UNIV2    0.0000
#define IB_UNIV_UNIV3    0.0000



#define MAINT_POP      0.0000
#define MAINT_POP1     0.0803
#define MAINT_POP2     0.0915
#define MAINT_POP3     0.1121
#define MAINT_RETAIL   0.0000
#define MAINT_RETAIL1  0.5324
#define MAINT_RETAIL2  0.5680
#define MAINT_RETAIL3  0.8460
#define MAINT_OFFICE   0.0000
#define MAINT_OFFICE1  0.0000
#define MAINT_OFFICE2  0.0000
#define MAINT_OFFICE3  0.0000
#define MAINT_OTHER    0.0000
#define MAINT_OTHER1   0.0000
#define MAINT_OTHER2   0.0000
#define MAINT_OTHER3   0.0000
#define MAINT_TOTAL    0.0000
#define MAINT_TOTAL1   0.0000
#define MAINT_TOTAL2   0.0000
#define MAINT_TOTAL3   0.0000
#define MAINT_K12ETOT1 0.0000
#define MAINT_K12ETOT2 0.0000
#define MAINT_K12ETOT3 0.0000
#define MAINT_UNIV1    0.0000
#define MAINT_UNIV2    0.0000
#define MAINT_UNIV3    0.0000



#define DISCR_POP      0.0217
#define DISCR_POP1     0.0000
#define DISCR_POP2     0.0000
#define DISCR_POP3     0.0000
#define DISCR_RETAIL   0.0000
#define DISCR_RETAIL1  0.1200
#define DISCR_RETAIL2  0.0766
#define DISCR_RETAIL3  0.1852
#define DISCR_OFFICE   0.0000
#define DISCR_OFFICE1  0.0000
#define DISCR_OFFICE2  0.0000
#define DISCR_OFFICE3  0.0000
#define DISCR_OTHER    0.0000
#define DISCR_OTHER1   0.0000
#define DISCR_OTHER2   0.0000
#define DISCR_OTHER3   0.0000
#define DISCR_TOTAL    0.0000
#define DISCR_TOTAL1   0.0000
#define DISCR_TOTAL2   0.0000
#define DISCR_TOTAL3   0.0000
#define DISCR_K12ETOT1 0.0000
#define DISCR_K12ETOT2 0.0000
#define DISCR_K12ETOT3 0.0000
#define DISCR_UNIV1    0.0000
#define DISCR_UNIV2    0.0000
#define DISCR_UNIV3    0.0000


#define ATWORK_POP      0.0000
#define ATWORK_POP1     0.0000
#define ATWORK_POP2     0.0000
#define ATWORK_POP3     0.0000
#define ATWORK_RETAIL   0.0000
#define ATWORK_RETAIL1  0.0000
#define ATWORK_RETAIL2  0.0000
#define ATWORK_RETAIL3  0.0551
#define ATWORK_OFFICE   0.0000
#define ATWORK_OFFICE1  0.0000
#define ATWORK_OFFICE2  0.0000
#define ATWORK_OFFICE3  0.0000
#define ATWORK_OTHER    0.0000
#define ATWORK_OTHER1   0.0000
#define ATWORK_OTHER2   0.0000
#define ATWORK_OTHER3   0.0000
#define ATWORK_TOTAL    0.0071
#define ATWORK_TOTAL1   0.0000
#define ATWORK_TOTAL2   0.0000
#define ATWORK_TOTAL3   0.0000
#define ATWORK_K12ETOT1 0.0000
#define ATWORK_K12ETOT2 0.0000
#define ATWORK_K12ETOT3 0.0000
#define ATWORK_UNIV1    0.0000
#define ATWORK_UNIV2    0.0000
#define ATWORK_UNIV3    0.0000




void get_stop_size (double **ob_stop_size, double **ib_stop_size, struct zone_data *ZonalData)
{


	int i;
	double total_ob_size=0.0, total_ib_size=0.0;

	*ob_stop_size = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(double));
	*ib_stop_size = (double *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->MAX_TAZS+1)*sizeof(double));
//	addRAM ("get_stop_size", 2*(Ini->MAX_TAZS+1)*sizeof(double));


	printf ("calculating size variables for stop densities\n");


	// work purposes
	if (Ini->PURPOSE_TO_PROCESS < 3) {
		for (i=1; i <= Ini->MAX_TAZS; i++) {
			(*ob_stop_size)[i] =
				OB_WORK_POP*ZonalData->HHPop[i] +
				OB_WORK_POP1*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 1) +
				OB_WORK_POP2*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 2) +
				OB_WORK_POP3*ZonalData->HHPop[i]*(ZonalData->UrbanType[i]  > 2) +
				OB_WORK_RETAIL*ZonalData->RetEmp[i] +
				OB_WORK_RETAIL1*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 1) +
				OB_WORK_RETAIL2*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 2) +
				OB_WORK_RETAIL3*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				OB_WORK_OFFICE*ZonalData->OffEmp[i] +
				OB_WORK_OFFICE1*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 1) +
				OB_WORK_OFFICE2*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 2) +
				OB_WORK_OFFICE3*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				OB_WORK_OTHER*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i]) +
				OB_WORK_OTHER1*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 1) +
				OB_WORK_OTHER2*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 2) +
				OB_WORK_OTHER3*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i]  > 2) +
				OB_WORK_TOTAL*ZonalData->TotEmp[i] +
				OB_WORK_TOTAL1*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 1) +
				OB_WORK_TOTAL2*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 2) +
				OB_WORK_TOTAL3*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				OB_WORK_K12ETOT1*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 1) +
				OB_WORK_K12ETOT2*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 2) +
				OB_WORK_K12ETOT3*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i]  > 2) +
				OB_WORK_UNIV1*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 1) +
				OB_WORK_UNIV2*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 2) +
				OB_WORK_UNIV3*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i]  > 2);

			(*ib_stop_size)[i] =
				IB_WORK_POP*ZonalData->HHPop[i] +
				IB_WORK_POP1*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 1) +
				IB_WORK_POP2*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 2) +
				IB_WORK_POP3*ZonalData->HHPop[i]*(ZonalData->UrbanType[i]  > 2) +
				IB_WORK_RETAIL*ZonalData->RetEmp[i] +
				IB_WORK_RETAIL1*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 1) +
				IB_WORK_RETAIL2*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 2) +
				IB_WORK_RETAIL3*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				IB_WORK_OFFICE*ZonalData->OffEmp[i] +
				IB_WORK_OFFICE1*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 1) +
				IB_WORK_OFFICE2*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 2) +
				IB_WORK_OFFICE3*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				IB_WORK_OTHER*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i]) +
				IB_WORK_OTHER1*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 1) +
				IB_WORK_OTHER2*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 2) +
				IB_WORK_OTHER3*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i]  > 2) +
				IB_WORK_TOTAL*ZonalData->TotEmp[i] +
				IB_WORK_TOTAL1*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 1) +
				IB_WORK_TOTAL2*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 2) +
				IB_WORK_TOTAL3*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				IB_WORK_K12ETOT1*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 1) +
				IB_WORK_K12ETOT2*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 2) +
				IB_WORK_K12ETOT3*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i]  > 2) +
				IB_WORK_UNIV1*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 1) +
				IB_WORK_UNIV2*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 2) +
				IB_WORK_UNIV3*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i]  > 2);

			total_ob_size += (*ob_stop_size)[i];
			total_ib_size += (*ib_stop_size)[i];
		}
	}


	// school purpose
	else if (Ini->PURPOSE_TO_PROCESS == 3) {
		for (i=1; i <= Ini->MAX_TAZS; i++) {
			(*ob_stop_size)[i] =
				OB_SCHOOL_POP*ZonalData->HHPop[i] +
				OB_SCHOOL_POP1*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 1) +
				OB_SCHOOL_POP2*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 2) +
				OB_SCHOOL_POP3*ZonalData->HHPop[i]*(ZonalData->UrbanType[i]  > 2) +
				OB_SCHOOL_RETAIL*ZonalData->RetEmp[i] +
				OB_SCHOOL_RETAIL1*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 1) +
				OB_SCHOOL_RETAIL2*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 2) +
				OB_SCHOOL_RETAIL3*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				OB_SCHOOL_OFFICE*ZonalData->OffEmp[i] +
				OB_SCHOOL_OFFICE1*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 1) +
				OB_SCHOOL_OFFICE2*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 2) +
				OB_SCHOOL_OFFICE3*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				OB_SCHOOL_OTHER*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i]) +
				OB_SCHOOL_OTHER1*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 1) +
				OB_SCHOOL_OTHER2*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 2) +
				OB_SCHOOL_OTHER3*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i]  > 2) +
				OB_SCHOOL_TOTAL*ZonalData->TotEmp[i] +
				OB_SCHOOL_TOTAL1*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 1) +
				OB_SCHOOL_TOTAL2*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 2) +
				OB_SCHOOL_TOTAL3*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				OB_SCHOOL_K12ETOT1*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 1) +
				OB_SCHOOL_K12ETOT2*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 2) +
				OB_SCHOOL_K12ETOT3*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i]  > 2) +
				OB_SCHOOL_UNIV1*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 1) +
				OB_SCHOOL_UNIV2*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 2) +
				OB_SCHOOL_UNIV3*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i]  > 2);

			(*ib_stop_size)[i] =
				IB_SCHOOL_POP*ZonalData->HHPop[i] +
				IB_SCHOOL_POP1*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 1) +
				IB_SCHOOL_POP2*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 2) +
				IB_SCHOOL_POP3*ZonalData->HHPop[i]*(ZonalData->UrbanType[i]  > 2) +
				IB_SCHOOL_RETAIL*ZonalData->RetEmp[i] +
				IB_SCHOOL_RETAIL1*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 1) +
				IB_SCHOOL_RETAIL2*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 2) +
				IB_SCHOOL_RETAIL3*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				IB_SCHOOL_OFFICE*ZonalData->OffEmp[i] +
				IB_SCHOOL_OFFICE1*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 1) +
				IB_SCHOOL_OFFICE2*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 2) +
				IB_SCHOOL_OFFICE3*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				IB_SCHOOL_OTHER*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i]) +
				IB_SCHOOL_OTHER1*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 1) +
				IB_SCHOOL_OTHER2*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 2) +
				IB_SCHOOL_OTHER3*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i]  > 2) +
				IB_SCHOOL_TOTAL*ZonalData->TotEmp[i] +
				IB_SCHOOL_TOTAL1*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 1) +
				IB_SCHOOL_TOTAL2*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 2) +
				IB_SCHOOL_TOTAL3*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				IB_SCHOOL_K12ETOT1*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 1) +
				IB_SCHOOL_K12ETOT2*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 2) +
				IB_SCHOOL_K12ETOT3*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i]  > 2) +
				IB_SCHOOL_UNIV1*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 1) +
				IB_SCHOOL_UNIV2*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 2) +
				IB_SCHOOL_UNIV3*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i]  > 2);

			total_ob_size += (*ob_stop_size)[i];
			total_ib_size += (*ib_stop_size)[i];
		}
	}


	// university purpose
	else if (Ini->PURPOSE_TO_PROCESS == 4) {
		for (i=1; i <= Ini->MAX_TAZS; i++) {
			(*ob_stop_size)[i] =
				OB_UNIV_POP*ZonalData->HHPop[i] +
				OB_UNIV_POP1*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 1) +
				OB_UNIV_POP2*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 2) +
				OB_UNIV_POP3*ZonalData->HHPop[i]*(ZonalData->UrbanType[i]  > 2) +
				OB_UNIV_RETAIL*ZonalData->RetEmp[i] +
				OB_UNIV_RETAIL1*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 1) +
				OB_UNIV_RETAIL2*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 2) +
				OB_UNIV_RETAIL3*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				OB_UNIV_OFFICE*ZonalData->OffEmp[i] +
				OB_UNIV_OFFICE1*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 1) +
				OB_UNIV_OFFICE2*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 2) +
				OB_UNIV_OFFICE3*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				OB_UNIV_OTHER*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i]) +
				OB_UNIV_OTHER1*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 1) +
				OB_UNIV_OTHER2*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 2) +
				OB_UNIV_OTHER3*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i]  > 2) +
				OB_UNIV_TOTAL*ZonalData->TotEmp[i] +
				OB_UNIV_TOTAL1*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 1) +
				OB_UNIV_TOTAL2*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 2) +
				OB_UNIV_TOTAL3*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				OB_UNIV_K12ETOT1*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 1) +
				OB_UNIV_K12ETOT2*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 2) +
				OB_UNIV_K12ETOT3*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i]  > 2) +
				OB_UNIV_UNIV1*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 1) +
				OB_UNIV_UNIV2*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 2) +
				OB_UNIV_UNIV3*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i]  > 2);

			(*ib_stop_size)[i] =
				IB_UNIV_POP*ZonalData->HHPop[i] +
				IB_UNIV_POP1*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 1) +
				IB_UNIV_POP2*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 2) +
				IB_UNIV_POP3*ZonalData->HHPop[i]*(ZonalData->UrbanType[i]  > 2) +
				IB_UNIV_RETAIL*ZonalData->RetEmp[i] +
				IB_UNIV_RETAIL1*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 1) +
				IB_UNIV_RETAIL2*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 2) +
				IB_UNIV_RETAIL3*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				IB_UNIV_OFFICE*ZonalData->OffEmp[i] +
				IB_UNIV_OFFICE1*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 1) +
				IB_UNIV_OFFICE2*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 2) +
				IB_UNIV_OFFICE3*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				IB_UNIV_OTHER*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i]) +
				IB_UNIV_OTHER1*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 1) +
				IB_UNIV_OTHER2*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 2) +
				IB_UNIV_OTHER3*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i]  > 2) +
				IB_UNIV_TOTAL*ZonalData->TotEmp[i] +
				IB_UNIV_TOTAL1*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 1) +
				IB_UNIV_TOTAL2*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 2) +
				IB_UNIV_TOTAL3*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				IB_UNIV_K12ETOT1*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 1) +
				IB_UNIV_K12ETOT2*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 2) +
				IB_UNIV_K12ETOT3*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i]  > 2) +
				IB_UNIV_UNIV1*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 1) +
				IB_UNIV_UNIV2*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 2) +
				IB_UNIV_UNIV3*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i]  > 2);

			total_ob_size += (*ob_stop_size)[i];
			total_ib_size += (*ib_stop_size)[i];
		}
	}


	// maintenance purpose
	else if (Ini->PURPOSE_TO_PROCESS == 5) {
		for (i=1; i <= Ini->MAX_TAZS; i++) {
			(*ob_stop_size)[i] =
				MAINT_POP*ZonalData->HHPop[i] +
				MAINT_POP1*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 1) +
				MAINT_POP2*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 2) +
				MAINT_POP3*ZonalData->HHPop[i]*(ZonalData->UrbanType[i]  > 2) +
				MAINT_RETAIL*ZonalData->RetEmp[i] +
				MAINT_RETAIL1*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 1) +
				MAINT_RETAIL2*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 2) +
				MAINT_RETAIL3*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				MAINT_OFFICE*ZonalData->OffEmp[i] +
				MAINT_OFFICE1*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 1) +
				MAINT_OFFICE2*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 2) +
				MAINT_OFFICE3*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				MAINT_OTHER*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i]) +
				MAINT_OTHER1*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 1) +
				MAINT_OTHER2*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 2) +
				MAINT_OTHER3*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i]  > 2) +
				MAINT_TOTAL*ZonalData->TotEmp[i] +
				MAINT_TOTAL1*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 1) +
				MAINT_TOTAL2*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 2) +
				MAINT_TOTAL3*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				MAINT_K12ETOT1*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 1) +
				MAINT_K12ETOT2*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 2) +
				MAINT_K12ETOT3*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i]  > 2) +
				MAINT_UNIV1*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 1) +
				MAINT_UNIV2*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 2) +
				MAINT_UNIV3*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i]  > 2);

			(*ib_stop_size)[i] =
				MAINT_POP*ZonalData->HHPop[i] +
				MAINT_POP1*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 1) +
				MAINT_POP2*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 2) +
				MAINT_POP3*ZonalData->HHPop[i]*(ZonalData->UrbanType[i]  > 2) +
				MAINT_RETAIL*ZonalData->RetEmp[i] +
				MAINT_RETAIL1*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 1) +
				MAINT_RETAIL2*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 2) +
				MAINT_RETAIL3*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				MAINT_OFFICE*ZonalData->OffEmp[i] +
				MAINT_OFFICE1*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 1) +
				MAINT_OFFICE2*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 2) +
				MAINT_OFFICE3*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				MAINT_OTHER*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i]) +
				MAINT_OTHER1*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 1) +
				MAINT_OTHER2*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 2) +
				MAINT_OTHER3*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i]  > 2) +
				MAINT_TOTAL*ZonalData->TotEmp[i] +
				MAINT_TOTAL1*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 1) +
				MAINT_TOTAL2*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 2) +
				MAINT_TOTAL3*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				MAINT_K12ETOT1*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 1) +
				MAINT_K12ETOT2*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 2) +
				MAINT_K12ETOT3*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i]  > 2) +
				MAINT_UNIV1*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 1) +
				MAINT_UNIV2*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 2) +
				MAINT_UNIV3*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i]  > 2);

			total_ob_size += (*ob_stop_size)[i];
			total_ib_size += (*ib_stop_size)[i];
		}
	}


	// dscretionary purpose
	else if (Ini->PURPOSE_TO_PROCESS == 6) {
		for (i=1; i <= Ini->MAX_TAZS; i++) {
			(*ob_stop_size)[i] =
				DISCR_POP*ZonalData->HHPop[i] +
				DISCR_POP1*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 1) +
				DISCR_POP2*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 2) +
				DISCR_POP3*ZonalData->HHPop[i]*(ZonalData->UrbanType[i]  > 2) +
				DISCR_RETAIL*ZonalData->RetEmp[i] +
				DISCR_RETAIL1*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 1) +
				DISCR_RETAIL2*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 2) +
				DISCR_RETAIL3*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				DISCR_OFFICE*ZonalData->OffEmp[i] +
				DISCR_OFFICE1*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 1) +
				DISCR_OFFICE2*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 2) +
				DISCR_OFFICE3*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				DISCR_OTHER*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i]) +
				DISCR_OTHER1*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 1) +
				DISCR_OTHER2*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 2) +
				DISCR_OTHER3*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i]  > 2) +
				DISCR_TOTAL*ZonalData->TotEmp[i] +
				DISCR_TOTAL1*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 1) +
				DISCR_TOTAL2*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 2) +
				DISCR_TOTAL3*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				DISCR_K12ETOT1*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 1) +
				DISCR_K12ETOT2*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 2) +
				DISCR_K12ETOT3*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i]  > 2) +
				DISCR_UNIV1*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 1) +
				DISCR_UNIV2*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 2) +
				DISCR_UNIV3*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i]  > 2);

			(*ib_stop_size)[i] =
				DISCR_POP*ZonalData->HHPop[i] +
				DISCR_POP1*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 1) +
				DISCR_POP2*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 2) +
				DISCR_POP3*ZonalData->HHPop[i]*(ZonalData->UrbanType[i]  > 2) +
				DISCR_RETAIL*ZonalData->RetEmp[i] +
				DISCR_RETAIL1*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 1) +
				DISCR_RETAIL2*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 2) +
				DISCR_RETAIL3*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				DISCR_OFFICE*ZonalData->OffEmp[i] +
				DISCR_OFFICE1*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 1) +
				DISCR_OFFICE2*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 2) +
				DISCR_OFFICE3*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				DISCR_OTHER*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i]) +
				DISCR_OTHER1*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 1) +
				DISCR_OTHER2*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 2) +
				DISCR_OTHER3*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i]  > 2) +
				DISCR_TOTAL*ZonalData->TotEmp[i] +
				DISCR_TOTAL1*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 1) +
				DISCR_TOTAL2*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 2) +
				DISCR_TOTAL3*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				DISCR_K12ETOT1*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 1) +
				DISCR_K12ETOT2*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 2) +
				DISCR_K12ETOT3*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i]  > 2) +
				DISCR_UNIV1*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 1) +
				DISCR_UNIV2*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 2) +
				DISCR_UNIV3*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i]  > 2);

			total_ob_size += (*ob_stop_size)[i];
			total_ib_size += (*ib_stop_size)[i];
		}
	}


	// at work purpose
	else if (Ini->PURPOSE_TO_PROCESS == 7) {
		for (i=1; i <= Ini->MAX_TAZS; i++) {
			(*ob_stop_size)[i] =
				ATWORK_POP*ZonalData->HHPop[i] +
				ATWORK_POP1*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 1) +
				ATWORK_POP2*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 2) +
				ATWORK_POP3*ZonalData->HHPop[i]*(ZonalData->UrbanType[i]  > 2) +
				ATWORK_RETAIL*ZonalData->RetEmp[i] +
				ATWORK_RETAIL1*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 1) +
				ATWORK_RETAIL2*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 2) +
				ATWORK_RETAIL3*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				ATWORK_OFFICE*ZonalData->OffEmp[i] +
				ATWORK_OFFICE1*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 1) +
				ATWORK_OFFICE2*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 2) +
				ATWORK_OFFICE3*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				ATWORK_OTHER*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i]) +
				ATWORK_OTHER1*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 1) +
				ATWORK_OTHER2*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 2) +
				ATWORK_OTHER3*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i]  > 2) +
				ATWORK_TOTAL*ZonalData->TotEmp[i] +
				ATWORK_TOTAL1*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 1) +
				ATWORK_TOTAL2*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 2) +
				ATWORK_TOTAL3*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				ATWORK_K12ETOT1*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 1) +
				ATWORK_K12ETOT2*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 2) +
				ATWORK_K12ETOT3*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i]  > 2) +
				ATWORK_UNIV1*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 1) +
				ATWORK_UNIV2*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 2) +
				ATWORK_UNIV3*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i]  > 2);

			(*ib_stop_size)[i] =
				ATWORK_POP*ZonalData->HHPop[i] +
				ATWORK_POP1*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 1) +
				ATWORK_POP2*ZonalData->HHPop[i]*(ZonalData->UrbanType[i] == 2) +
				ATWORK_POP3*ZonalData->HHPop[i]*(ZonalData->UrbanType[i]  > 2) +
				ATWORK_RETAIL*ZonalData->RetEmp[i] +
				ATWORK_RETAIL1*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 1) +
				ATWORK_RETAIL2*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i] == 2) +
				ATWORK_RETAIL3*ZonalData->RetEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				ATWORK_OFFICE*ZonalData->OffEmp[i] +
				ATWORK_OFFICE1*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 1) +
				ATWORK_OFFICE2*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i] == 2) +
				ATWORK_OFFICE3*ZonalData->OffEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				ATWORK_OTHER*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i]) +
				ATWORK_OTHER1*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 1) +
				ATWORK_OTHER2*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i] == 2) +
				ATWORK_OTHER3*(ZonalData->TotEmp[i] - ZonalData->RetEmp[i] - ZonalData->OffEmp[i])*(ZonalData->UrbanType[i]  > 2) +
				ATWORK_TOTAL*ZonalData->TotEmp[i] +
				ATWORK_TOTAL1*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 1) +
				ATWORK_TOTAL2*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i] == 2) +
				ATWORK_TOTAL3*ZonalData->TotEmp[i]*(ZonalData->UrbanType[i]  > 2) +
				ATWORK_K12ETOT1*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 1) +
				ATWORK_K12ETOT2*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i] == 2) +
				ATWORK_K12ETOT3*ZonalData->K12ETot[i]*(ZonalData->UrbanType[i]  > 2) +
				ATWORK_UNIV1*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 1) +
				ATWORK_UNIV2*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i] == 2) +
				ATWORK_UNIV3*ZonalData->UnvEnrol[i]*(ZonalData->UrbanType[i]  > 2);

			total_ob_size += (*ob_stop_size)[i];
			total_ib_size += (*ib_stop_size)[i];
		}
	}

	printf ("total outbound size = %.2f, total inbound size = %.2f\n", total_ob_size, total_ib_size);
}