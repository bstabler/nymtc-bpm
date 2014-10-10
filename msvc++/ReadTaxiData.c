#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md.h"

// taxi cost data file format specifiers
#define TAXI_RECORD_LENGTH	80
#define ORIG_CO_START		1
#define ORIG_CO_LENGTH		5
#define DEST_CO_START		6
#define DEST_CO_LENGTH		5
#define TX_WAIT_START		11
#define TX_WAIT_LENGTH		5
#define TX_PICKUP_START		16
#define TX_PICKUP_LENGTH	5
#define TX_CPM_START		21
#define TX_CPM_LENGTH		5
#define TX_SURCHG_START		26
#define TX_SURCHG_LENGTH	5
#define TX_TOLL_START		31
#define TX_TOLL_LENGTH		5



void read_taxi_data (FILE *fp, struct taxi_data *TaxiData)
{
	int i_co, j_co;
	char InputRecord[TAXI_RECORD_LENGTH+2];
	char temp[LRECL];


// Read taxi data records.
	while ((fgets(InputRecord, TAXI_RECORD_LENGTH+5, fp)) != NULL) {

		strncpy (temp, &InputRecord[ORIG_CO_START-1], ORIG_CO_LENGTH);
		temp[ORIG_CO_LENGTH] = '\0';
		i_co = atoi(temp);

		strncpy (temp, &InputRecord[DEST_CO_START-1], DEST_CO_LENGTH);
		temp[DEST_CO_LENGTH] = '\0';
		j_co = atoi(temp);

		strncpy (temp, &InputRecord[TX_WAIT_START-1], TX_WAIT_LENGTH);
		temp[TX_WAIT_LENGTH] = '\0';
		TaxiData->wait[i_co][j_co] = (float)atof(temp);

		strncpy (temp, &InputRecord[TX_PICKUP_START-1], TX_PICKUP_LENGTH);
		temp[TX_PICKUP_LENGTH] = '\0';
		TaxiData->pickup[i_co][j_co] = (float)atof(temp);

		strncpy (temp, &InputRecord[TX_CPM_START-1], TX_CPM_LENGTH);
		temp[TX_CPM_LENGTH] = '\0';
		TaxiData->cpm[i_co][j_co] = (float)atof(temp);

		strncpy (temp, &InputRecord[TX_SURCHG_START-1], TX_SURCHG_LENGTH);
		temp[TX_SURCHG_LENGTH] = '\0';
		TaxiData->surchg[i_co][j_co] = (float)atof(temp);

		strncpy (temp, &InputRecord[TX_TOLL_START-1], TX_TOLL_LENGTH);
		temp[TX_TOLL_LENGTH] = '\0';
		TaxiData->toll[i_co][j_co] = (float)atof(temp);

	}
}

