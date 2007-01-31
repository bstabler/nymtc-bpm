#include "md.h"


#define RECORD_LENGTH  1000


void read_motorized_MSCs (FILE *fp, struct msc_data *msc)
{
	int i, index, index_value, rec_len, mode;
	char InputRecord[RECORD_LENGTH];
	char temp[LRECL];



// Read motorized MSC index value and corresponding MSC
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
		if (msc->motorized_index_used[index_value] != 1) {
			printf ("\nerror in read_motorized_MSCs.\n");
			printf ("\nan index value = %d is specified, but this value was not found in the MSC index table.\n", index_value);
			fprintf (fp_rep, "\nerror in read_motorized_MSCs.\n");
			fprintf (fp_rep, "\nan index value = %d is specified, but this value was not found in the MSC index table.\n", index_value);
			fflush (stdout);
			fflush (fp_rep);
			exit (-15);
		}

		// read index label  (nothing done with these; index label is taken from targets file.
		index = 0;
		i++;
		while (InputRecord[i] != ',' && i < rec_len) {
			temp[index] = InputRecord[i];
			i++;
			index++;
		}
		temp[index] = '\0';


		// read MSCs for all modes (DA, SR2, SR3, SR4+, WT, DT, WC, DC, TX, NM, SB)
		for (mode=0; mode < Ini->NUMBER_ALTS; mode++) {
			index = 0;
			i++;
			while (InputRecord[i] != ',' && i < rec_len) {
				temp[index] = InputRecord[i];
				i++;
				index++;
			}
			temp[index] = '\0';

			if ((float)atof(temp) < -99990)
				msc->MSC[index_value][mode] = MISSING;
			else
				msc->MSC[index_value][mode] = (float)atof(temp);
		}

	}

}

