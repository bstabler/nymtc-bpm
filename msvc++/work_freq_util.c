#include "md.h"

#define OB_DENSITY_COEFF				 0.0000
#define OB_BOTH_LEGS_URBAN				-0.2162
#define OB_BOTH_LEGS_SUBURBS			 0.0000
#define OB_DENSITY_BOTH_LEGS_SUBURBS	 0.0105
#define OB_AUTO_LOW_MED		 			 0.0000
#define OB_AUTO_HI						 0.0000
#define OB_TRANSIT_LOW_MED				-2.2380
#define OB_TRANSIT_HI					-2.0420
#define OB_AUTO_NOA						-2.1130
#define OB_AUTO_AFW						-0.9435
#define OB_AUTO_ASW						-0.7482
#define OB_AUTO_AMW						-0.3501
#define OB_TRANSIT_IVT					 0.0000
#define OB_AUTO_DA_OR_TX				-1.8060
#define OB_HH_NWADULTS_GT_0				-0.5007
#define OB_HH_MADE_SCHOOL				 0.4628
#define OB_PERS_MADE_ATWORK				 0.6493

#define IB_DENSITY_COEFF				 0.0362
#define IB_BOTH_LEGS_URBAN				 0.0000
#define IB_BOTH_LEGS_SUBURBS			 0.1723
#define IB_DENSITY_BOTH_LEGS_SUBURBS	 0.0000
#define IB_AUTO_LOW_MED		 			-1.7040
#define IB_AUTO_HI						-1.6120
#define IB_TRANSIT_LOW_MED				-1.4850
#define IB_TRANSIT_HI					-1.3700
#define IB_AUTO_NOA						 0.0000
#define IB_AUTO_AFW						 0.0000
#define IB_AUTO_ASW						 0.0000
#define IB_AUTO_AMW						 0.0000
#define IB_TRANSIT_IVT					-0.0116
#define IB_AUTO_DA_OR_TX				 0.0000
#define IB_HH_NWADULTS_GT_0				-0.3097
#define IB_HH_MADE_SCHOOL				 0.0000
#define IB_PERS_MADE_ATWORK				 0.3834

#define OBIB_DENSITY_COEFF				 0.0362
#define OBIB_BOTH_LEGS_URBAN			 0.1706
#define OBIB_BOTH_LEGS_SUBURBS			 0.0000
#define OBIB_DENSITY_BOTH_LEGS_SUBURBS	 0.0105
#define OBIB_AUTO_LOW_MED		 		-0.7494
#define OBIB_AUTO_HI					-0.6676
#define OBIB_TRANSIT_LOW_MED			-2.8020
#define OBIB_TRANSIT_HI					-2.3030
#define OBIB_AUTO_NOA					 0.0000
#define OBIB_AUTO_AFW					 0.0000
#define OBIB_AUTO_ASW					 0.0000
#define OBIB_AUTO_AMW					 0.0000
#define OBIB_TRANSIT_IVT				 0.00426
#define OBIB_AUTO_DA_OR_TX				-2.2780
#define OBIB_HH_NWADULTS_GT_0			-0.7971
#define OBIB_HH_MADE_SCHOOL				 0.4285
#define OBIB_PERS_MADE_ATWORK			 0.5938


void work_freq_util (int i, double obDensity, double ibDensity, double ivtt, int *orig, int *dest, int *mode,
	struct journey_attribs *JourneyAttribs, struct zone_data *ZonalData, double *freqUtil, int *freqAvail,
	short *hh_freqs, short **hh_pers_freqs, int *hh2id)
{

	int hh_index, pers_index, autos;



	// set the autos variable for use in auto sufficiency calculation to autos owned,
	// or in the case where an origin or destination is in the restricted area
	// for license plate rationing, to autos available.
	if ( Ini->PCT_LP_RESTRICTION > 0.0 && (ZonalData->lpRestricted[orig[i]] || ZonalData->lpRestricted[dest[i]]) )
		autos = JourneyAttribs->autosAvail[i];
	else
		autos = JourneyAttribs->autos[i];



	// get the pers frequency table index
	hh_index = hh2id[JourneyAttribs->hh[i]];
	pers_index = hh2id[JourneyAttribs->hh[i]]*10 + JourneyAttribs->persno[i];

	// 0 stops
	freqAvail[0] = 1;
	freqUtil[0] = 0.0;

	// 1 out-bound stop, 0 in-bound stops
	freqAvail[1] = 1;
	if (obDensity > 0.0)
		freqUtil[1] = OB_DENSITY_COEFF*log(obDensity)
					+ OB_BOTH_LEGS_URBAN*(ZonalData->UrbanType[orig[i]] < 3 && ZonalData->UrbanType[dest[i]] < 3)
					+ OB_BOTH_LEGS_SUBURBS*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ OB_DENSITY_BOTH_LEGS_SUBURBS*log(obDensity)*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ OB_AUTO_LOW_MED*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && JourneyAttribs->income[i] < 3)
					+ OB_AUTO_HI*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && JourneyAttribs->income[i] == 3)
					+ OB_TRANSIT_LOW_MED*((mode[i] >= 5 && mode[i] <= 8) && JourneyAttribs->income[i] < 3)
					+ OB_TRANSIT_HI*((mode[i] >= 5 && mode[i] <= 8) && JourneyAttribs->income[i] == 3)
					+ OB_AUTO_NOA*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos == 0)
					+ OB_AUTO_AFW*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos <  JourneyAttribs->workers[i])
					+ OB_AUTO_ASW*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos == JourneyAttribs->workers[i])
					+ OB_AUTO_AMW*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos >  JourneyAttribs->workers[i])
					+ OB_TRANSIT_IVT*(ivtt)
					+ OB_AUTO_DA_OR_TX*(mode[i] == 1 || mode[i] == 9)
					+ OB_HH_NWADULTS_GT_0*(JourneyAttribs->nwas[i] > 0)
					+ OB_HH_MADE_SCHOOL*(hh_freqs[hh_index] > 0)
					+ OB_PERS_MADE_ATWORK*(hh_pers_freqs[0][pers_index] > 0)
					+ Ini->OB_FSC;
	else
		freqAvail[1] = 0;

	// 0 out-bound stops, 1 in-bound stop
	freqAvail[2] = 1;
	if (ibDensity > 0.0)
		freqUtil[2] = IB_DENSITY_COEFF*log(ibDensity)
					+ IB_BOTH_LEGS_URBAN*(ZonalData->UrbanType[orig[i]] < 3 && ZonalData->UrbanType[dest[i]] < 3)
					+ IB_BOTH_LEGS_SUBURBS*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ IB_DENSITY_BOTH_LEGS_SUBURBS*log(ibDensity)*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ IB_AUTO_LOW_MED*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && JourneyAttribs->income[i] < 3)
					+ IB_AUTO_HI*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && JourneyAttribs->income[i] == 3)
					+ IB_TRANSIT_LOW_MED*((mode[i] >= 5 && mode[i] <= 8) && JourneyAttribs->income[i] < 3)
					+ IB_TRANSIT_HI*((mode[i] >= 5 && mode[i] <= 8) && JourneyAttribs->income[i] == 3)
					+ IB_AUTO_NOA*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos == 0)
					+ IB_AUTO_AFW*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos <  JourneyAttribs->workers[i])
					+ IB_AUTO_ASW*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos == JourneyAttribs->workers[i])
					+ IB_AUTO_AMW*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos >  JourneyAttribs->workers[i])
					+ IB_TRANSIT_IVT*(ivtt)
					+ IB_AUTO_DA_OR_TX*(mode[i] == 1 || mode[i] == 9)
					+ IB_HH_NWADULTS_GT_0*(JourneyAttribs->nwas[i] > 0)
					+ IB_HH_MADE_SCHOOL*(hh_freqs[hh_index] > 0)
					+ IB_PERS_MADE_ATWORK*(hh_pers_freqs[0][pers_index] > 0)
					+ Ini->IB_FSC;
	else
		freqAvail[2] = 0;

	// 1 out-bound stop, 1 in-bound stop
	freqAvail[3] = 1;
	if (ibDensity > 0.0 && obDensity > 0.0)
		freqUtil[3] = OBIB_DENSITY_COEFF*(log(obDensity) + log(ibDensity))
					+ OBIB_BOTH_LEGS_URBAN*(ZonalData->UrbanType[orig[i]] < 3 && ZonalData->UrbanType[dest[i]] < 3)
					+ OBIB_BOTH_LEGS_SUBURBS*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ OBIB_DENSITY_BOTH_LEGS_SUBURBS*(log(obDensity) + log(ibDensity))*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ OBIB_AUTO_LOW_MED*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && JourneyAttribs->income[i] < 3)
					+ OBIB_AUTO_HI*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && JourneyAttribs->income[i] == 3)
					+ OBIB_TRANSIT_LOW_MED*((mode[i] >= 5 && mode[i] <= 8) && JourneyAttribs->income[i] < 3)
					+ OBIB_TRANSIT_HI*((mode[i] >= 5 && mode[i] <= 8) && JourneyAttribs->income[i] == 3)
					+ OBIB_AUTO_NOA*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos == 0)
					+ OBIB_AUTO_AFW*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos <  JourneyAttribs->workers[i])
					+ OBIB_AUTO_ASW*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos == JourneyAttribs->workers[i])
					+ OBIB_AUTO_AMW*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos >  JourneyAttribs->workers[i])
					+ OBIB_TRANSIT_IVT*(ivtt)
					+ OBIB_AUTO_DA_OR_TX*(mode[i] == 1 || mode[i] == 9)
					+ OBIB_HH_NWADULTS_GT_0*(JourneyAttribs->nwas[i] > 0)
					+ OBIB_HH_MADE_SCHOOL*(hh_freqs[hh_index] > 0)
					+ OBIB_PERS_MADE_ATWORK*(hh_pers_freqs[0][pers_index] > 0)
					+ Ini->OBIB_FSC;
	else
		freqAvail[3] = 0;


}
