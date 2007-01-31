#include "md.h"

#define OB_BOTH_LEGS_SUBURBS_OB		 0.6502
#define OB_BOTH_LEGS_SUBURBS_IB		 0.0000
#define OB_CHILD_IN_HH_OB			 0.5342
#define OB_CHILD_IN_HH_IB			 0.0000
#define OB_WORKER_IN_HH				 0.0000
#define OB_PERS_MADE_WORK			-1.1220
#define OB_TRANSIT					-2.3890
#define OB_AUTO						-2.0700
#define OB_HH_OTHER_UNIV			 0.0000

#define IB_BOTH_LEGS_SUBURBS_OB		 0.0000
#define IB_BOTH_LEGS_SUBURBS_IB		 0.2555
#define IB_CHILD_IN_HH_OB			 0.0000
#define IB_CHILD_IN_HH_IB			 0.4669
#define IB_WORKER_IN_HH				 0.0000
#define IB_PERS_MADE_WORK			-1.4410
#define IB_TRANSIT					-1.8290
#define IB_AUTO						-1.2190
#define IB_HH_OTHER_UNIV			 0.0000

#define OBIB_BOTH_LEGS_SUBURBS_OB	 0.6502
#define OBIB_BOTH_LEGS_SUBURBS_IB	 0.2555
#define OBIB_CHILD_IN_HH_OB			 0.5342
#define OBIB_CHILD_IN_HH_IB			 0.4669
#define OBIB_WORKER_IN_HH			-1.1560
#define OBIB_PERS_MADE_WORK			-0.9232
#define OBIB_TRANSIT				-1.6150
#define OBIB_AUTO					-2.7140
#define OBIB_HH_OTHER_UNIV			-0.8325


void univ_freq_util (int i, double obDensity, double ibDensity, int *orig, int *dest, int *mode,
	struct journey_attribs *JourneyAttribs, struct zone_data *ZonalData, double *freqUtil, int *freqAvail,
	short *hh_freqs, short **hh_pers_freqs, int *hh2id)
{

	int hh_index, pers_index;


	// get the pers frequency table index
	hh_index = hh2id[JourneyAttribs->hh[i]];
	pers_index = hh2id[JourneyAttribs->hh[i]]*10 + JourneyAttribs->persno[i];

	// 0 stops
	freqAvail[0] = 1;
	freqUtil[0] = 0.0;

	// 1 out-bound stop, 0 in-bound stops
	freqAvail[1] = 1;
	if (obDensity > 0.0)
		freqUtil[1] = OB_BOTH_LEGS_SUBURBS_OB*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ OB_BOTH_LEGS_SUBURBS_IB*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ OB_CHILD_IN_HH_OB*(JourneyAttribs->children[i] > 0)
					+ OB_CHILD_IN_HH_IB*(JourneyAttribs->children[i] > 0)
					+ OB_WORKER_IN_HH*(JourneyAttribs->workers[i] > 0)
					+ OB_PERS_MADE_WORK*(hh_pers_freqs[0][pers_index] > 0)
					+ OB_TRANSIT*(mode[i] >= 5 && mode[i] <= 8)
					+ OB_AUTO*((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9)
					+ OB_HH_OTHER_UNIV*((hh_freqs[hh_index] - hh_pers_freqs[1][pers_index]) > 0)
					+ Ini->OB_FSC;
	else
		freqAvail[1] = 0;

	// 0 out-bound stops, 1 in-bound stop
	freqAvail[2] = 1;
	if (ibDensity > 0.0)
		freqUtil[2] = IB_BOTH_LEGS_SUBURBS_OB*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ IB_BOTH_LEGS_SUBURBS_IB*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ IB_CHILD_IN_HH_OB*(JourneyAttribs->children[i] > 0)
					+ IB_CHILD_IN_HH_IB*(JourneyAttribs->children[i] > 0)
					+ IB_WORKER_IN_HH*(JourneyAttribs->workers[i] > 0)
					+ IB_PERS_MADE_WORK*(hh_pers_freqs[0][pers_index] > 0)
					+ IB_TRANSIT*(mode[i] >= 5 && mode[i] <= 8)
					+ IB_AUTO*((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9)
					+ IB_HH_OTHER_UNIV*((hh_freqs[hh_index] - hh_pers_freqs[1][pers_index]) > 0)
					+ Ini->IB_FSC;
	else
		freqAvail[2] = 0;

	// 1 out-bound stop, 1 in-bound stop
	freqAvail[3] = 1;
	if (ibDensity > 0.0 && obDensity > 0.0)
		freqUtil[3] = OBIB_BOTH_LEGS_SUBURBS_OB*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ OBIB_BOTH_LEGS_SUBURBS_IB*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ OBIB_CHILD_IN_HH_OB*(JourneyAttribs->children[i] > 0)
					+ OBIB_CHILD_IN_HH_IB*(JourneyAttribs->children[i] > 0)
					+ OBIB_WORKER_IN_HH*(JourneyAttribs->workers[i] > 0)
					+ OBIB_PERS_MADE_WORK*(hh_pers_freqs[0][pers_index] > 0)
					+ OBIB_TRANSIT*(mode[i] >= 5 && mode[i] <= 8)
					+ OBIB_AUTO*((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9)
					+ OBIB_HH_OTHER_UNIV*((hh_freqs[hh_index] - hh_pers_freqs[1][pers_index]) > 0)
					+ Ini->OBIB_FSC;
	else
		freqAvail[3] = 0;


}
