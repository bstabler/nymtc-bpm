#include "md.h"

#define OB_DENSITY_COEFF	 0.1143
#define OB_TRANSIT			-2.8370
#define OB_AUTO				-3.6820
#define OB_INCOME			 0.0000

#define IB_DENSITY_COEFF	 0.1143
#define IB_TRANSIT			-2.4120
#define IB_AUTO				-2.4310
#define IB_INCOME			 0.0000

#define OBIB_DENSITY_COEFF	 0.1329
#define OBIB_TRANSIT		-6.7770
#define OBIB_AUTO			-4.7700
#define OBIB_INCOME			-1.1220


void atwork_freq_util (int i, double obDensity, double ibDensity, int *orig, int *dest, int *mode,
	struct journey_attribs *JourneyAttribs, double *freqUtil, int *freqAvail)
{


	// 0 stops
	freqAvail[0] = 1;
	freqUtil[0] = 0.0;

	// 1 out-bound stop, 0 in-bound stops
	freqAvail[1] = 1;
	if (obDensity > 0.0)
		freqUtil[1] = OB_DENSITY_COEFF*log(obDensity)
					+ OB_TRANSIT*(mode[i] >= 5 && mode[i] <= 8)
					+ OB_AUTO*((mode[i] >= 1 && mode[i] <= 4) || (mode[i] == 9))
					+ OB_INCOME*(JourneyAttribs->income[i] == 3)
					+ Ini->OB_FSC;
	else
		freqAvail[1] = 0;

	// 0 out-bound stops, 1 in-bound stop
	freqAvail[2] = 1;
	if (ibDensity > 0.0)
		freqUtil[2] = IB_DENSITY_COEFF*log(ibDensity)
					+ IB_TRANSIT*(mode[i] >= 5 && mode[i] <= 8)
					+ IB_AUTO*((mode[i] >= 1 && mode[i] <= 4) || (mode[i] == 9))
					+ IB_INCOME*(JourneyAttribs->income[i] == 3)
					+ Ini->IB_FSC;
	else
		freqAvail[2] = 0;

	// 1 out-bound stop, 1 in-bound stop
	freqAvail[3] = 1;
	if (ibDensity > 0.0 && obDensity > 0.0)
		freqUtil[3] = OBIB_DENSITY_COEFF*(log(obDensity) + log(ibDensity))
					+ OBIB_TRANSIT*(mode[i] >= 5 && mode[i] <= 8)
					+ OBIB_AUTO*((mode[i] >= 1 && mode[i] <= 4) || (mode[i] == 9))
					+ OBIB_INCOME*(JourneyAttribs->income[i] == 3)
					+ Ini->OBIB_FSC;
	else
		freqAvail[3] = 0;


}
