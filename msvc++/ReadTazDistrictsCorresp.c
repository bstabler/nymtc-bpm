#include "md.h"


// input file formats
#define RECORD_LENGTH  1000
#define MAX_NUMBER_OF_DISTRICT_FIELDS 20


// Read index and label for which destination choice constants will be defined in another file.
// The format of the input file must be csv.
void read_flexible_district_corresp (FILE *fp, struct dc_constant_data *dcConstantIndices, struct zone_data *ZonalData)
{
	int i, index, rec_len, n;
	int taz, intValue;
	char InputRecord[RECORD_LENGTH];
	char* temp;
	int* tempIndices;
	int* tempValues;


	// declare temporary arrays for indices and labels, to be discarded after we know how many labels we have
	tempIndices = (int*)malloc( Ini->MAX_TAZS * sizeof(int) );
	tempValues = (int*)malloc( MAX_NUMBER_OF_DISTRICT_FIELDS * sizeof(int) );
	temp = (char*)malloc( LRECL * sizeof(char) );

	// skip 1 header record
	fgets(InputRecord, RECORD_LENGTH, fp);

	while ((fgets(InputRecord, RECORD_LENGTH, fp)) != NULL) {

		rec_len = (int)strlen(InputRecord);

		// read index value
		index = 0;
		i = 0;
		while ( InputRecord[i] != ',' && i < rec_len ) {
			temp[index] = InputRecord[i];
			i++;
			index++;
		}
		temp[index] = '\0';
		taz = atoi(temp);
		tempValues[0] = taz;


		// loop over correspondence fields store values in array
		n = 1;
		while ( i < rec_len ) {

			index = 0;
			i++;
			while ( InputRecord[i] != ',' && i < rec_len ) {
				temp[index] = InputRecord[i];
				i++;
				index++;
			}
			temp[index] = '\0';
			intValue = atoi(temp);
			tempValues[n] = intValue;

			n++;

		}

		if ( Ini->COUNTY_INDICES_FIELD_NUM > 0 )
			ZonalData->county_index[taz] = tempValues[Ini->COUNTY_INDICES_FIELD_NUM];
		if ( Ini->COUNTY_EXTENDED_INDICES_FIELD_NUM > 0 )
			ZonalData->county_extended_index[taz] = tempValues[Ini->COUNTY_EXTENDED_INDICES_FIELD_NUM];
		if ( Ini->DC_CONSTANT_INDICES_FIELD_NUM > 0 )
			ZonalData->dc_constant_index[taz] = tempValues[Ini->DC_CONSTANT_INDICES_FIELD_NUM];
		if ( Ini->REPORT_DIST_TO_DIST_JOURNEYS_FIELD_NUM > 0 )
			ZonalData->dist_to_dist_report_index[taz] = tempValues[Ini->REPORT_DIST_TO_DIST_JOURNEYS_FIELD_NUM];
		if ( Ini->REPORT_MODE_JOURNEYS_FIELD_NUM > 0 )
			ZonalData->mode_report_index[taz] = tempValues[Ini->REPORT_MODE_JOURNEYS_FIELD_NUM];
		if ( Ini->M_MC_ASC_INDEX_FIELD_NUM > 0 )
			ZonalData->m_mc_asc_index[taz] = tempValues[Ini->M_MC_ASC_INDEX_FIELD_NUM];
		if ( Ini->NM_MC_ASC_INDEX_FIELD_NUM > 0 )
			ZonalData->nm_mc_asc_index[taz] = tempValues[Ini->NM_MC_ASC_INDEX_FIELD_NUM];

	}


	// free the heap memory allocated for temporary arrays
	free(tempIndices);
	free(tempValues);
	free(temp);

}

