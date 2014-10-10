#include "md.h"


// input file formats
#define RECORD_LENGTH  1000


// Read index and label for flexible district field definitions.
// The format of the input file must be csv.
void read_motorized_ASC_targets (FILE *fp, struct m_mc_asc_data *mMcAscData)
{
	int i, j, index, rec_len, n, col, maxIndex;
	int intIndex;
	float floattValue;
	char InputRecord[RECORD_LENGTH];
	char* temp;
	char** tempIndices;
	char** tempLabels;
	float** tempValues;
	float* tempRow;
	int tempLength;
	struct district_data *districtData;

	// declare temporary arrays for indices and labels, to be discarded after we know how many labels we have
	tempIndices = (char**)malloc( Ini->MAX_TAZS * sizeof(char *) );
	tempLabels = (char**)malloc( Ini->MAX_TAZS * sizeof(char *) );
	tempValues = (float**)malloc( Ini->MAX_TAZS * sizeof(float *) );
	tempRow = (float*)malloc( Ini->MAX_TAZS * sizeof(float) );

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


		// loop over unknown number of indices and store values in temp array
		col = 0;
		while ( i < rec_len ) {

			index = 0;
			i++;
			while (InputRecord[i] != ',' && InputRecord[i] != '\n' &&InputRecord[i] != '\r' && i < rec_len) {
				temp[index] = InputRecord[i];
				i++;
				index++;
			}
			temp[index] = '\0';
			tempRow[col] = (float)atof(temp);

			col++;

		}

		tempValues[n] = (float*)malloc( col * sizeof(float) );
		for (i=0; i < col; i++)
			tempValues[n][i] = tempRow[i];

		n++;
	}

	districtData = mMcAscData->mMcAscTargets;

	districtData->numValues = n;
	districtData->maxValue = maxIndex;
	districtData->indexIndices = (int*) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (maxIndex+1)*sizeof(int));
	districtData->indexValues = (int*) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, n*sizeof(int));
	districtData->labels = (char**) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, n*sizeof(char *));

	mMcAscData->targets = (float**) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, n*sizeof(float *));

	// the first row of targets are the regional targets
	mMcAscData->regionalShareTargets = (float*) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, col*sizeof(float));
	for (j=0; j < col; j++)
		mMcAscData->regionalShareTargets[j] = tempValues[0][j];

	// the remaining rows are the district targets
	for (i=1; i < n; i++) {
		intIndex = atoi(tempIndices[i]);
		districtData->indexValues[i-1] = intIndex;
		districtData->indexIndices[intIndex] = i-1;
		tempLength = (int)strlen(tempLabels[i]);
		districtData->labels[i-1] = (char*)malloc( (tempLength+1) * sizeof(char));
		strcpy (districtData->labels[i-1], tempLabels[i]);

		mMcAscData->targets[i-1] = (float*) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, col*sizeof(float));
		for (j=0; j < col; j++)
			mMcAscData->targets[i-1][j] = tempValues[i][j];
	}


	// free the heap memory allocated for temporary arrays
	for (i=0; i < n; i++) {
		free(tempIndices[i]);
		free(tempLabels[i]);
		free(tempValues[i]);
	}
	free(tempIndices);
	free(tempLabels);
	free(tempValues);
	free(tempRow);
	free(temp);

}

