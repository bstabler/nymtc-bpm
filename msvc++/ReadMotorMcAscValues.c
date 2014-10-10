#include "md.h"


// input file formats
#define RECORD_LENGTH  2000


// The format of the input file must be csv.
void read_m_mc_asc_values (FILE *fp, struct m_mc_asc_data *mMcAscIndices)
{
	int i, index, rec_len, n;
	int rowIndex;
	char InputRecord[RECORD_LENGTH];
	char* temp;
	int intIndex;
	float floatValue;

	temp = (char*)malloc( LRECL * sizeof(char) );
	mMcAscIndices->constants = (float**) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, mMcAscIndices->mMcAscIndices->numValues*sizeof(float *));
	for ( i=0; i < mMcAscIndices->mMcAscIndices->numValues; i++ )
		mMcAscIndices->constants[i] = (float*) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, mMcAscIndices->mMcAscIndices->numValues*sizeof(float));


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
		rowIndex = mMcAscIndices->mMcAscIndices->indexIndices[intIndex];


		// loop over known number of indices and store values in array
		n = 0;
		while ( i < rec_len ) {

			index = 0;
			i++;
			while ( InputRecord[i] != ',' && i < rec_len ) {
				temp[index] = InputRecord[i];
				i++;
				index++;
			}
			temp[index] = '\0';
			floatValue = (float)atof(temp);

			mMcAscIndices->constants[rowIndex][n] = floatValue;

			n++;

		}

		if ( n != mMcAscIndices->mMcAscIndices->numValues )
			ExitWithCode(175);

	}

	free(temp);

}

