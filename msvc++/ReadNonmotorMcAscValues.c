#include "md.h"


// input file formats
#define RECORD_LENGTH  2000


// The format of the input file must be csv.
void read_nm_mc_asc_values (FILE *fp, struct nm_mc_asc_data *nmMcAscIndices)
{
	int i, index, rec_len;
	int rowIndex;
	char InputRecord[RECORD_LENGTH];
	char* temp;
	int intIndex;
	float floatValue;

	temp = (char*)malloc( LRECL * sizeof(char) );
	nmMcAscIndices->constants = (float*) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, nmMcAscIndices->nmMcAscIndices->numValues*sizeof(float));


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
		intIndex = atoi(temp);
		rowIndex = nmMcAscIndices->nmMcAscIndices->indexIndices[intIndex];


		index = 0;
		i++;
		while ( InputRecord[i] != ',' && i < rec_len ) {
			temp[index] = InputRecord[i];
			i++;
			index++;
		}
		temp[index] = '\0';
		floatValue = (float)atof(temp);

		nmMcAscIndices->constants[rowIndex] = floatValue;

	}

	free(temp);

}

