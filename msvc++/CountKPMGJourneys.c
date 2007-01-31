#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md.h"

#define JOURNEY_RECORD_LENGTH 44+2
#define SEQ_START 1
#define SEQ_LENGTH 10
#define CLUSTER_START 11
#define CLUSTER_LENGTH 10
#define OTAZ_START 21
#define OTAZ_LENGTH 6
#define WALK_ORIG_START 27
#define WALK_ORIG_LENGTH 2
#define INCOME_START 29
#define INCOME_LENGTH 2
#define PERSONS_START 31
#define PERSONS_LENGTH 2
#define WORKERS_START 33
#define WORKERS_LENGTH 2
#define CHILDREN_START 35
#define CHILDREN_LENGTH 2
#define NWADULTS_START 37
#define NWADULTS_LENGTH 2
#define AUTOS_START 39
#define AUTOS_LENGTH 2
#define HH_JRNYS_START 41
#define HH_JRNYS_LENGTH 4
#define PERSNO_START 1
#define PERSNO_LENGTH 2
#define PERSON_TYPE_START 3
#define PERSON_TYPE_LENGTH 2
#define JRNY_TYPE_START 5
#define JRNY_TYPE_LENGTH 2




void count_kpmg_journey_records (FILE *fp)
{


	int i, k, haj;
	int seq, orig, income, purpose, hh_jrnys, jrny_type;
	char JourneyRecord[JOURNEY_RECORD_LENGTH+1];
	char temp[LRECL];
    double n = 0;




	// Read journey file header record.
	k = 0;
	fgets(JourneyRecord, JOURNEY_RECORD_LENGTH+1, fp);
	haj = 1;

	// Read journey file data records.
	while ((fgets(JourneyRecord, JOURNEY_RECORD_LENGTH+1, fp)) != NULL && k < Ini->NUMBER_JOURNEYS) {
		haj++;

n++;        

		strncpy (temp, &JourneyRecord[SEQ_START-1], SEQ_LENGTH);
		temp[SEQ_LENGTH] = '\0';
		seq = atoi(temp);

		strncpy (temp, &JourneyRecord[OTAZ_START-1], OTAZ_LENGTH);
		temp[OTAZ_LENGTH] = '\0';
		orig = atoi(temp);

		if (orig != STATUE_OF_LIBERTY_TAZ) {

			strncpy (temp, &JourneyRecord[HH_JRNYS_START-1], HH_JRNYS_LENGTH);
			temp[HH_JRNYS_LENGTH] = '\0';
			hh_jrnys = atoi(temp);
		
			strncpy (temp, &JourneyRecord[INCOME_START-1], INCOME_LENGTH);
			temp[INCOME_LENGTH] = '\0';
			income = atoi(temp);
		

			// read the journeys for this household.
			for (i=0; i < hh_jrnys; i++) {
				if (k < Ini->NUMBER_JOURNEYS) {

					fgets(JourneyRecord, JOURNEY_RECORD_LENGTH+2, fp);
n++;        

					strncpy (temp, &JourneyRecord[JRNY_TYPE_START-1], JRNY_TYPE_LENGTH);
					temp[JRNY_TYPE_LENGTH] = '\0';
					jrny_type = atoi(temp);

			
			// define purpose codes
					switch (jrny_type) {
						case (6):					
							purpose = income - 1;	// work (0,1,2)
							break;
						case (1):
						case (2):
						case (3):
							purpose = 3;			// school (3)
							break;
						case (4):
						case (5):
							purpose = 4;			// university (4)
							break;
						case (7):
						case (9):
						case (10):
							purpose = 5;			// maintenance (5)
							break;
						case (11):
						case (12):
						case (13):
							purpose = 6;			// discretionary (6)
							break;
						case (8):
							purpose = 7;			// at work (7)
							break;
					}


					if (purpose == Ini->PURPOSE_TO_PROCESS) {
						k++;

					}
		
				}
			} // end of journeys in HH loop
		}
	}

	Ini->NUMBER_JOURNEYS = k;
}


