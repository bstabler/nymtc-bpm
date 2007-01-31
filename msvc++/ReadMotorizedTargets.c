#include "md.h"


#define RECORD_LENGTH  1000


void read_motorized_targets (FILE *fp, struct msc_data *msc)
{
	int i, index, index_value, rec_len, mode, firstRecord, tempRAM;
	char InputRecord[RECORD_LENGTH];
	char temp[LRECL];
	float *RegionalTargets;
	char RegionalLabel[100];


// Memory allocations
	RegionalTargets = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_ALTS*sizeof(float));
//	addRAM ("read_motorized_targets", Ini->NUMBER_ALTS*sizeof(float));

// Read motorized target percentages for auto-calibrating MSCs
	fgets(InputRecord, RECORD_LENGTH, fp);								// skip 1 header record

	firstRecord = 1;
	tempRAM = 0;
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
		index_value = atoi(temp);


		// check that this index value appears in the O/D district index table
		if (firstRecord != 1 && msc->motorized_index_used[index_value] != 1) {
			printf ("\nerror in read_motorized_targets.\n");
			printf ("\nan index value = %d is specified, but this value was not found in the MSC index table.\n", index_value);
			fprintf (fp_rep, "\nerror in read_motorized_targets.\n");
			fprintf (fp_rep, "\nan index value = %d is specified, but this value was not found in the MSC index table.\n", index_value);
			fflush (stdout);
			fflush (fp_rep);
			exit (-16);
		}


		// read index labels
		index = 0;
		i++;
		while (InputRecord[i] != ',' && i < rec_len) {
			temp[index] = InputRecord[i];
			i++;
			index++;
		}
		temp[index] = '\0';

		if (firstRecord == 1) {
			strcpy (RegionalLabel, temp);
		}
		else {
			tempRAM += ((int)strlen(temp))*sizeof(char);
			strcpy (msc->motorized_labels[index_value], temp);
		}


		// read Targets for all modes (DA, SR2, SR3, SR4+, WT, DT, WC, DC, TX, NM, SB)
		// the first record contains regional targets for reporting purpose
		for (mode=0; mode < Ini->NUMBER_ALTS; mode++) {
			index = 0;
			i++;
			while (InputRecord[i] != ',' && i < rec_len) {
				temp[index] = InputRecord[i];
				i++;
				index++;
			}
			temp[index] = '\0';

			if (firstRecord == 1)
				RegionalTargets[mode] = (float)atof(temp);
			else
				msc->motorized_targets[index_value][mode] = (float)atof(temp);
		}
		firstRecord = 0;
	}
//	addRAM ("read_motorized_targets", tempRAM);

	// increment msc->motorized_max_index and save the regional mode share targets in that index
	msc->motorized_max_index ++;
	for (mode=0; mode < Ini->NUMBER_ALTS; mode++)
		msc->motorized_targets[msc->motorized_max_index][mode] = RegionalTargets[mode];
	strcpy (msc->motorized_labels[msc->motorized_max_index], RegionalLabel);

		
	HeapFree (heapHandle, 0, RegionalTargets);
//	relRAM ("read_motorized_targets", Ini->NUMBER_ALTS*sizeof(float));
}

