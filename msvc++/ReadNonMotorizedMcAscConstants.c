#include "md.h"


// input file formats
#define RECORD_LENGTH  1000


// Read index and label for flexible district field definitions.
// The format of the input file must be csv.
void read_non_motorized_ASC_constants (FILE *fp, struct nm_mc_asc_data *nmMcAscIndices)
{
	int i, index, rec_len, n, maxIndex;
	int intIndex;
	char InputRecord[RECORD_LENGTH];
	char* temp;
	char** tempIndices;
	char** tempLabels;
	float* tempValues;
	int tempLength;
	struct district_data *districtData;

	// declare temporary arrays for indices and labels, to be discarded after we know how many labels we have
	tempIndices = (char**)malloc( Ini->MAX_TAZS * sizeof(char *) );
	tempLabels = (char**)malloc( Ini->MAX_TAZS * sizeof(char *) );
	tempValues = (float*)malloc( Ini->MAX_TAZS * sizeof(float) );

	temp = (char*)malloc( LRECL * sizeof(char) );

	// skip 1 header record
	fgets(InputRecord, RECORD_LENGTH, fp);

	n = 0;
	maxIndex = 0;
	while ((fgets(InputRecord, RECORD_LENGTH, fp)) != NULL) {

		rec_len = (int)strlen(InputRecord);

		// read index value
		index = 0;
		i = 0;
		while (InputRecord[i] != ',' && i < rec_len) {
			temp[index] = InputRecord[i];
			i++;
			index++;
		}
		temp[index] = '\0';
		tempLength = (int)strlen(temp);
		tempIndices[n] = (char*)malloc( (tempLength+1) * sizeof(char));
		strcpy (tempIndices[n], temp);
		
		intIndex = atoi(temp);
		if ( intIndex > maxIndex )
			maxIndex = intIndex;


		// read associated label
		index = 0;
		i++;
		while (InputRecord[i] != ',' && InputRecord[i] != '\n' &&InputRecord[i] != '\r' && i < rec_len) {
			temp[index] = InputRecord[i];
			i++;
			index++;
		}
		temp[index] = '\0';
		tempLength = (int)strlen(temp);
		tempLabels[n] = (char*)malloc( (tempLength+1) * sizeof(char));
		strcpy (tempLabels[n], temp);


		// read constant value
		index = 0;
		i++;
		while (InputRecord[i] != ',' && InputRecord[i] != '\n' &&InputRecord[i] != '\r' && i < rec_len) {
			temp[index] = InputRecord[i];
			i++;
			index++;
		}
		temp[index] = '\0';
		tempValues[n] = (float)atof(temp);

		n++;
	}

	districtData = nmMcAscIndices->nmMcAscConstants;

	districtData->numValues = n;
	districtData->maxValue = maxIndex;
	districtData->indexIndices = (int*) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (maxIndex+1)*sizeof(int));
	districtData->indexValues = (int*) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, n*sizeof(int));
	districtData->labels = (char**) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, n*sizeof(char *));

	nmMcAscIndices->constants = (float*) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, n*sizeof(float));

	for (i=0; i < n; i++) {
		intIndex = atoi(tempIndices[i]);
		districtData->indexValues[i] = intIndex;
		districtData->indexIndices[intIndex] = i;
		tempLength = (int)strlen(tempLabels[i]);
		districtData->labels[i] = (char*)malloc( (tempLength+1) * sizeof(char));
		strcpy (districtData->labels[i], tempLabels[i]);

		nmMcAscIndices->constants[i] = tempValues[i];
	}


	// free the heap memory allocated for temporary arrays
	for (i=0; i < n; i++) {
		free(tempIndices[i]);
		free(tempLabels[i]);
	}
	free(tempIndices);
	free(tempLabels);
	free(tempValues);
	free(temp);

}

