#include "md.h"

#define OB_DENSITY_COEFF		 0.2153
#define OB_CHILD_IN_HH			-0.6841
#define OB_PERS_IS_CHILD		 0.0000
#define OB_BOTH_LEGS_SUBURBS	 0.0000
#define OB_TRANSIT				-2.7210
#define OB_AUTO					-3.2430
#define OB_PERS_MADE_WRK_SCH	-0.5188

#define IB_DENSITY_COEFF		 0.1847
#define IB_CHILD_IN_HH			-0.4007
#define IB_PERS_IS_CHILD		 0.0000
#define IB_BOTH_LEGS_SUBURBS	 0.0000
#define IB_TRANSIT				-3.1900
#define IB_AUTO					-2.8970
#define IB_PERS_MADE_WRK_SCH	-0.5413
	
#define OBIB_DENSITY_COEFF		 0.2114
#define OBIB_CHILD_IN_HH		 0.0000
#define OBIB_PERS_IS_CHILD		-0.5419
#define OBIB_BOTH_LEGS_SUBURBS	 2.0910
#define OBIB_TRANSIT			-6.5540
#define OBIB_AUTO				-7.6170
#define OBIB_PERS_MADE_WRK_SCH	-1.1940


void discr_freq_util (int i, double obDensity, double ibDensity, int *orig, int *dest, int *mode,
	struct journey_attribs *JourneyAttribs, struct zone_data *ZonalData, double *freqUtil, int *freqAvail,
	short **hh_pers_freqs, int *hh2id)
{


	int pers_index;


	// get the pers frequency table index
	pers_index = hh2id[JourneyAttribs->hh[i]]*10 + JourneyAttribs->persno[i];

	// 0 stops
	freqAvail[0] = 1;
	freqUtil[0] = 0.0;

	// 1 out-bound stop, 0 in-bound stops
	freqAvail[1] = 1;
	if (obDensity > 0.0)
		freqUtil[1] = OB_DENSITY_COEFF*log(obDensity)
					+ OB_CHILD_IN_HH*(JourneyAttribs->children[i] > 0)
					+ OB_PERS_IS_CHILD*(JourneyAttribs->person_type[i] == 1)
					+ OB_BOTH_LEGS_SUBURBS*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ OB_TRANSIT*(mode[i] >= 5 && mode[i] <= 8)
					+ OB_AUTO*((mode[i] >= 1 && mode[i] <= 4) || (mode[i] == 9))
					+ OB_PERS_MADE_WRK_SCH*(hh_pers_freqs[0][pers_index] > 0)
					+ Ini->OB_FSC;
	else
		freqAvail[1] = 0;

	// 0 out-bound stops, 1 in-bound stop
	freqAvail[2] = 1;
	if (ibDensity > 0.0)
		freqUtil[2] = IB_DENSITY_COEFF*log(ibDensity)
					+ IB_CHILD_IN_HH*(JourneyAttribs->children[i] > 0)
					+ IB_PERS_IS_CHILD*(JourneyAttribs->person_type[i] == 1)
					+ IB_BOTH_LEGS_SUBURBS*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ IB_TRANSIT*(mode[i] >= 5 && mode[i] <= 8)
					+ IB_AUTO*((mode[i] >= 1 && mode[i] <= 4) || (mode[i] == 9))
					+ IB_PERS_MADE_WRK_SCH*(hh_pers_freqs[0][pers_index] > 0)
					+ Ini->IB_FSC;
	else
		freqAvail[2] = 0;

	// 1 out-bound stop, 1 in-bound stop
	freqAvail[3] = 1;
	if (ibDensity > 0.0 && obDensity > 0.0)
		freqUtil[3] = OBIB_DENSITY_COEFF*(log(obDensity) + log(ibDensity))
					+ OBIB_CHILD_IN_HH*(JourneyAttribs->children[i] > 0)
					+ OBIB_PERS_IS_CHILD*(JourneyAttribs->person_type[i] == 1)
					+ OBIB_BOTH_LEGS_SUBURBS*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ OBIB_TRANSIT*(mode[i] >= 5 && mode[i] <= 8)
					+ OBIB_AUTO*((mode[i] >= 1 && mode[i] <= 4) || (mode[i] == 9))
					+ OBIB_PERS_MADE_WRK_SCH*(hh_pers_freqs[0][pers_index] > 0)
					+ Ini->OBIB_FSC;
	else
		freqAvail[3] = 0;


}
