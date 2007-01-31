#include "md.h"


// input file formats
#define RECORD_LENGTH  1000


void read_bpmdist1_coeffs (FILE *fp, struct bpmdist1_coeff_data BPMDist1)
{
	int i, idist, jdist, index, rec_len, tempRAM;
	char InputRecord[RECORD_LENGTH];
	char temp[LRECL];


// Read intra county and county to county dummy variable coefficients.
	tempRAM = 0;
	fgets(InputRecord, RECORD_LENGTH, fp);								// skip 2 header records
	fgets(InputRecord, RECORD_LENGTH, fp);
	while ((fgets(InputRecord, RECORD_LENGTH, fp)) != NULL) {
		rec_len = (int)strlen(InputRecord);

		// read origin district
		index = 0;
		i = 0;
			while (InputRecord[i] != ',' && i < rec_len) {
			temp[index] = InputRecord[i];
			i++;
			index++;
		}
		temp[index] = '\0';
		idist = atoi(temp);


		// read origin district label (nothing done with this currently)
		index = 0;
		i++;
		while (InputRecord[i] != ',' && i < rec_len) {
			temp[index] = InputRecord[i];
			i++;
			index++;
		}
		temp[index] = '\0';
		BPMDist1.labels[idist] = (char *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (int)strlen(temp)*sizeof(char));
		tempRAM += ((int)strlen(temp))*sizeof(char);
		strcpy (BPMDist1.labels[idist], temp);


		// read intra-district coefficient
		index = 0;
		i++;
		while (InputRecord[i] != ',' && i < rec_len) {
			temp[index] = InputRecord[i];
			i++;
			index++;
		}
		temp[index] = '\0';
		BPMDist1.intra_county[idist] = (float)atof(temp);

		// read all destination district coefficients for orig_dist
		for (jdist=0; jdist < Ini->NUMBER_BPMDIST1; jdist++) {
			index = 0;
			i++;
			while (InputRecord[i] != ',' && i < rec_len) {
				temp[index] = InputRecord[i];
				i++;
				index++;
			}
			temp[index] = '\0';
			BPMDist1.co_co_coeffs[idist][jdist] = (float)atof(temp);
		}
	}
//	addRAM ("read_bpmdist1_coeffs", tempRAM);
}

