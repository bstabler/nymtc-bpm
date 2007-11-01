#include "md.h"



#define OB_CONSTANT					 0.0000
#define OB_HWY_DIST					 0.0000
#define OB_TRANSIT_OB				-3.8380
#define OB_TRANSIT_IB				 0.0000
#define OB_AUTO_SOV					-4.0780
#define OB_AUTO_HOV					-3.5070
#define OB_AUTO_NOA					 0.0000
#define OB_AUTO_AUTOS				 0.0000
#define OB_DENSITY_URBAN			 0.0000
#define OB_DENSITY_SUBURBAN			 0.0000
#define OB_WORKER_IN_HH				 0.7226
#define OB_HH_NONWORKERS			 0.1609
#define OB_PERS_MADE_WORK			 0.5878
#define OB_HH_OTHER_SCHOOL_OB		 0.1751
#define OB_HH_OTHER_SCHOOL_IB		 0.0000

#define IB_CONSTANT					 0.0000
#define IB_HWY_DIST					 0.0000
#define IB_TRANSIT_OB				 0.0000
#define IB_TRANSIT_IB				-1.4550
#define IB_AUTO_SOV					 0.0000
#define IB_AUTO_HOV					 0.0000
#define IB_AUTO_NOA					-2.5740
#define IB_AUTO_AUTOS				-1.4950
#define IB_DENSITY_URBAN			 0.0534
#define IB_DENSITY_SUBURBAN			 0.0938
#define IB_WORKER_IN_HH				 0.0000
#define IB_HH_NONWORKERS			-0.2045
#define IB_PERS_MADE_WORK			-1.3230
#define IB_HH_OTHER_SCHOOL_OB		 0.0000
#define IB_HH_OTHER_SCHOOL_IB		-0.3295

#define OBIB_CONSTANT				 2.0100
#define OBIB_HWY_DIST				 0.0101
#define OBIB_TRANSIT_OB				-3.8380
#define OBIB_TRANSIT_IB				-1.4550
#define OBIB_AUTO_SOV				-4.0780
#define OBIB_AUTO_HOV				-3.5070
#define OBIB_AUTO_NOA				-2.5740
#define OBIB_AUTO_AUTOS				-1.4950
#define OBIB_DENSITY_URBAN			 0.0534
#define OBIB_DENSITY_SUBURBAN		 0.0938
#define OBIB_WORKER_IN_HH			 0.3850
#define OBIB_HH_NONWORKERS			-0.5100
#define OBIB_PERS_MADE_WORK			-0.7352
#define OBIB_HH_OTHER_SCHOOL_OB		 0.1751
#define OBIB_HH_OTHER_SCHOOL_IB		-0.3295


void school_freq_util (int i, double obDensity, double ibDensity, double hwy_dist, int *orig, int *dest, int *mode,
	struct journey_attribs *JourneyAttribs, struct zone_data *ZonalData, double *freqUtil, int *freqAvail,
	short *hh_freqs, short **hh_pers_freqs, int *hh2id)
{


	int hh_index, pers_index, autos;



	// set the autos variable for use in auto sufficiency calculation to autos owned,
	// or in the case where an origin or destination is in the restricted area
	// for license plate rationing, to autos available.
	if ( ZonalData->lpRestricted[orig[i]] || ZonalData->lpRestricted[dest[i]] )
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
		freqUtil[1] = OB_CONSTANT
					+ OB_HWY_DIST*(hwy_dist)
					+ OB_TRANSIT_OB*(mode[i] >= 5 && mode[i] <= 8)
					+ OB_TRANSIT_IB*(mode[i] >= 5 && mode[i] <= 8)
					+ OB_AUTO_SOV*(mode[i] == 1 || mode[i] == 9)
					+ OB_AUTO_HOV*(mode[i] >= 2 && mode[i] <= 4)
					+ OB_AUTO_NOA*(autos == 0)
					+ OB_AUTO_AUTOS*(autos > 0)
					+ OB_DENSITY_URBAN*log(obDensity)*(ZonalData->UrbanType[orig[i]] < 3 && ZonalData->UrbanType[dest[i]] < 3)
					+ OB_DENSITY_SUBURBAN*log(obDensity)*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ OB_WORKER_IN_HH*(JourneyAttribs->workers[i] > 0)
					+ OB_HH_NONWORKERS*(JourneyAttribs->nwas[i] > 0)
					+ OB_PERS_MADE_WORK*(hh_pers_freqs[0][pers_index] > 0)
					+ OB_HH_OTHER_SCHOOL_OB*((hh_freqs[hh_index] - hh_pers_freqs[1][pers_index]) > 0)
					+ OB_HH_OTHER_SCHOOL_IB*((hh_freqs[hh_index] - hh_pers_freqs[1][pers_index]) > 0)
					+ Ini->OB_FSC;
	else
		freqAvail[1] = 0;

	// 0 out-bound stops, 1 in-bound stop
	freqAvail[2] = 1;
	if (ibDensity > 0.0)
		freqUtil[2] = IB_CONSTANT
					+ IB_HWY_DIST*(hwy_dist)
					+ IB_TRANSIT_OB*(mode[i] >= 5 && mode[i] <= 8)
					+ IB_TRANSIT_IB*(mode[i] >= 5 && mode[i] <= 8)
					+ IB_AUTO_SOV*(mode[i] == 1 || mode[i] == 9)
					+ IB_AUTO_HOV*(mode[i] >= 2 && mode[i] <= 4)
					+ IB_AUTO_NOA*(autos == 0)
					+ IB_AUTO_AUTOS*(autos > 0)
					+ IB_DENSITY_URBAN*log(ibDensity)*(ZonalData->UrbanType[orig[i]] < 3 && ZonalData->UrbanType[dest[i]] < 3)
					+ IB_DENSITY_SUBURBAN*log(obDensity)*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ IB_WORKER_IN_HH*(JourneyAttribs->workers[i] > 0)
					+ IB_HH_NONWORKERS*(JourneyAttribs->nwas[i] > 0)
					+ IB_PERS_MADE_WORK*(hh_pers_freqs[0][pers_index] > 0)
					+ IB_HH_OTHER_SCHOOL_OB*((hh_freqs[hh_index] - hh_pers_freqs[1][pers_index]) > 0)
					+ IB_HH_OTHER_SCHOOL_IB*((hh_freqs[hh_index] - hh_pers_freqs[1][pers_index]) > 0)
					+ Ini->IB_FSC;
	else
		freqAvail[2] = 0;

	// 1 out-bound stop, 1 in-bound stop
	freqAvail[3] = 1;
	if (ibDensity > 0.0 && obDensity > 0.0)
		freqUtil[3] = OBIB_CONSTANT
					+ OBIB_HWY_DIST*(hwy_dist)
					+ OBIB_TRANSIT_OB*(mode[i] >= 5 && mode[i] <= 8)
					+ OBIB_TRANSIT_IB*(mode[i] >= 5 && mode[i] <= 8)
					+ OBIB_AUTO_SOV*(mode[i] == 1 || mode[i] == 9)
					+ OBIB_AUTO_HOV*(mode[i] >= 2 && mode[i] <= 4)
					+ OBIB_AUTO_NOA*(autos == 0)
					+ OBIB_AUTO_AUTOS*(autos > 0)
					+ OBIB_DENSITY_URBAN*(log(obDensity) + log(ibDensity))*(ZonalData->UrbanType[orig[i]] < 3 && ZonalData->UrbanType[dest[i]] < 3)
					+ OBIB_DENSITY_SUBURBAN*log(obDensity)*(ZonalData->UrbanType[orig[i]] >= 3 && ZonalData->UrbanType[dest[i]] >= 3)
					+ OBIB_WORKER_IN_HH*(JourneyAttribs->workers[i] > 0)
					+ OBIB_HH_NONWORKERS*(JourneyAttribs->nwas[i] > 0)
					+ OBIB_PERS_MADE_WORK*(hh_pers_freqs[0][pers_index] > 0)
					+ OBIB_HH_OTHER_SCHOOL_OB*((hh_freqs[hh_index] - hh_pers_freqs[1][pers_index]) > 0)
					+ OBIB_HH_OTHER_SCHOOL_IB*((hh_freqs[hh_index] - hh_pers_freqs[1][pers_index]) > 0)
					+ Ini->OBIB_FSC;
	else
		freqAvail[3] = 0;


}
