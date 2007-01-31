#include "md.h"


#define RECORD_LENGTH  1000


void read_nm_MSCs (FILE *fp, struct msc_data *msc)
{
	int i, index, index_value, rec_len;
	char InputRecord[RECORD_LENGTH];
	char temp[LRECL];



// Read non-motorized MSC index value and corresponding MSC
	fgets(InputRecord, RECORD_LENGTH, fp);								// skip 1 header record
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
		if (msc->nm_index_used[index_value] != 1) {
			printf ("\nerror in read_nm_MSCs.\n");
			printf ("\nan index value = %d is specified, but this value was not found in the MSC index table.\n", index_value);
			fprintf (fp_rep, "\nerror in read_nm_MSCs.\n");
			fprintf (fp_rep, "\nan index value = %d is specified, but this value was not found in the MSC index table.\n", index_value);
			fflush (stdout);
			fflush (fp_rep);
			exit (-15);
		}


		// read index label (nothing done with these; index label is taken from targets file.
		index = 0;
		i++;
		while (InputRecord[i] != ',' && i < rec_len) {
			temp[index] = InputRecord[i];
			i++;
			index++;
		}
		temp[index] = '\0';


		// read MSC for the non-motorized utility in pre-mode choice
		index = 0;
		i++;
		while (InputRecord[i] != ',' && i < rec_len) {
			temp[index] = InputRecord[i];
			i++;
			index++;
		}
		temp[index] = '\0';

		if ((float)atof(temp) < -99990)
			msc->nmMSC[index_value] = MISSING;
		else
			msc->nmMSC[index_value] = (float)atof(temp);
	}
}

