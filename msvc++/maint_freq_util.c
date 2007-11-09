#include "md.h"

#define OB_DENSITY_COEFF				 0.3335
#define OB_HWY_DIST						 0.0000
#define OB_BOTH_LEGS_SUBURBS			 0.2676
#define OB_PERS_MADE_WORK				-0.3812
#define OB_TRANSIT_LOW_MED				-4.3870
#define OB_TRANSIT_HI					-4.9290
#define OB_TRANSIT_IVT					-0.0124
#define OB_AUTO_NOA						-4.2280
#define OB_AUTO_AFW						-5.4370
#define OB_AUTO_ASW						-5.0030
#define OB_AUTO_AMW						-4.9320

#define IB_DENSITY_COEFF				 0.1665
#define IB_HWY_DIST						 0.0000
#define IB_BOTH_LEGS_SUBURBS			 0.2676
#define IB_PERS_MADE_WORK				-0.4004
#define IB_TRANSIT_LOW_MED				-2.7630
#define IB_TRANSIT_HI					-2.2880
#define IB_TRANSIT_IVT					-0.0124
#define IB_AUTO_NOA						-2.9850
#define IB_AUTO_AFW						-3.6260
#define IB_AUTO_ASW						-2.8700
#define IB_AUTO_AMW						-2.6640

#define OBIB_DENSITY_COEFF				 0.2924
#define OBIB_HWY_DIST					-0.0165
#define OBIB_BOTH_LEGS_SUBURBS			 0.8173
#define OBIB_PERS_MADE_WORK				-0.6340
#define OBIB_TRANSIT_LOW_MED			-7.9060
#define OBIB_TRANSIT_HI					-6.7080
#define OBIB_TRANSIT_IVT				-0.0160
#define OBIB_AUTO_NOA					-7.7700
#define OBIB_AUTO_AFW					-8.4740
#define OBIB_AUTO_ASW					-8.1150
#define OBIB_AUTO_AMW					-7.8220


void maint_freq_util (int i, double obDensity, double ibDensity, double ivtt, double hwy_dist,
	int *orig, int *dest, int *mode, struct journey_attribs *JourneyAttribs, struct zone_data *ZonalData,
	double *freqUtil, int *freqAvail, short **hh_pers_freqs, int *hh2id)
{

	int pers_index, autos;




	// set the autos variable for use in auto sufficiency calculation to autos owned,
	// or in the case where an origin or destination is in the restricted area
	// for license plate rationing, to autos available.
	if ( Ini->PCT_LP_RESTRICTION > 0.0 && (ZonalData->lpRestricted[orig[i]] || ZonalData->lpRestricted[dest[i]]) )
		autos = JourneyAttribs->autosAvail[i];
	else
		autos = JourneyAttribs->autos[i];




	// get the pers frequency table index
	pers_index = hh2id[JourneyAttribs->hh[i]]*10 + JourneyAttribs->persno[i];

	// 0 stops
	freqAvail[0] = 1;
	freqUtil[0] = 0.0;

	// 1 out-bound stop, 0 in-bound stops
	freqAvail[1] = 1;
	if (obDensity > 0.0)
		freqUtil[1] = OB_DENSITY_COEFF*log(obDensity)
					+ OB_HWY_DIST*(hwy_dist)
					+ OB_BOTH_LEGS_SUBURBS*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ OB_TRANSIT_LOW_MED*((mode[i] >= 5 && mode[i] <= 8) && JourneyAttribs->income[i] < 3)
					+ OB_TRANSIT_HI*((mode[i] >= 5 && mode[i] <= 8) && JourneyAttribs->income[i] == 3)
					+ OB_AUTO_NOA*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos == 0)
					+ OB_AUTO_AFW*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos <  JourneyAttribs->workers[i])
					+ OB_AUTO_ASW*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos == JourneyAttribs->workers[i])
					+ OB_AUTO_AMW*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos >  JourneyAttribs->workers[i])
					+ OB_TRANSIT_IVT*(ivtt)
					+ OB_PERS_MADE_WORK*(hh_pers_freqs[0][pers_index] > 0)
					+ Ini->OB_FSC;
	else
		freqAvail[1] = 0;

	// 0 out-bound stops, 1 in-bound stop
	freqAvail[2] = 1;
	if (ibDensity > 0.0)
		freqUtil[2] = IB_DENSITY_COEFF*log(ibDensity)
					+ IB_HWY_DIST*(hwy_dist)*((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9)
					+ IB_BOTH_LEGS_SUBURBS*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ IB_TRANSIT_LOW_MED*((mode[i] >= 5 && mode[i] <= 8) && JourneyAttribs->income[i] < 3)
					+ IB_TRANSIT_HI*((mode[i] >= 5 && mode[i] <= 8) && JourneyAttribs->income[i] == 3)
					+ IB_AUTO_NOA*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos == 0)
					+ IB_AUTO_AFW*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos <  JourneyAttribs->workers[i])
					+ IB_AUTO_ASW*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos == JourneyAttribs->workers[i])
					+ IB_AUTO_AMW*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos >  JourneyAttribs->workers[i])
					+ IB_TRANSIT_IVT*(ivtt)
					+ IB_PERS_MADE_WORK*(hh_pers_freqs[0][pers_index] > 0)
					+ Ini->IB_FSC;
	else
		freqAvail[2] = 0;

	// 1 out-bound stop, 1 in-bound stop
	freqAvail[3] = 1;
	if (ibDensity > 0.0 && obDensity > 0.0)
		freqUtil[3] = OBIB_DENSITY_COEFF*(log(obDensity) + log(ibDensity))
					+ OBIB_HWY_DIST*(hwy_dist)
					+ OBIB_BOTH_LEGS_SUBURBS*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ OBIB_TRANSIT_LOW_MED*((mode[i] >= 5 && mode[i] <= 8) && JourneyAttribs->income[i] < 3)
					+ OBIB_TRANSIT_HI*((mode[i] >= 5 && mode[i] <= 8) && JourneyAttribs->income[i] == 3)
					+ OBIB_AUTO_NOA*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos == 0)
					+ OBIB_AUTO_AFW*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos <  JourneyAttribs->workers[i])
					+ OBIB_AUTO_ASW*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos == JourneyAttribs->workers[i])
					+ OBIB_AUTO_AMW*(((mode[i] >= 1 && mode[i] <= 4) || mode[i] == 9) && autos >  JourneyAttribs->workers[i])
					+ OBIB_TRANSIT_IVT*(ivtt)
					+ OBIB_PERS_MADE_WORK*(hh_pers_freqs[0][pers_index] > 0)
					+ Ini->OBIB_FSC;
	else
		freqAvail[3] = 0;


}
