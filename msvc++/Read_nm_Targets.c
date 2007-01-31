#include "md.h"


#define RECORD_LENGTH  1000


void read_nm_targets (FILE *fp, struct msc_data *msc)
{
	int i, index, index_value, rec_len, firstRecord, tempRAM;
	char InputRecord[RECORD_LENGTH];
	char temp[LRECL];
	float RegionalTarget;



// Read non-motorized target percentages for auto-calibrating pre-mode choice MSCs
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
		if (firstRecord != 1 && msc->nm_index_used[index_value] != 1) {
			printf ("\nerror in read_nm_targets.\n");
			printf ("\nan index value = %d is specified, but this value was not found in the MSC index table.\n", index_value);
			fprintf (fp_rep, "\nerror in read_nm_targets.\n");
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


		tempRAM += ((int)strlen(temp))*sizeof(char);
		strcpy (msc->nm_labels[index_value], temp);

		// read Targets for motorized pre-mode choice
		index = 0;
		i++;
		while (InputRecord[i] != ',' && i < rec_len) {
			temp[index] = InputRecord[i];
			i++;
			index++;
		}
		temp[index] = '\0';

		if (firstRecord == 1)
			RegionalTarget = (float)atof(temp);
		else
			msc->nm_targets[index_value] = (float)atof(temp);

		firstRecord = 0;

	}

	
	// increment msc->nm_max_index and save the regional non-motorized target proportion in that index
	msc->nm_max_index ++;
	msc->nm_targets[msc->nm_max_index] = RegionalTarget;

//	addRAM ("read_nm_targets", tempRAM);
	
}

