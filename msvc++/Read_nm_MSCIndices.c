#include "md.h"


// input file formats
#define RECORD_LENGTH  1000


void read_nm_MSC_indices (FILE *fp, struct msc_data *msc)
{
	int i, idist, index, index_value, rec_len;
	char InputRecord[RECORD_LENGTH];
	char temp[LRECL];


	msc->nm_max_index = 0;


// Read district to district MSC index table.
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


		// read the nm coefficient index for the origin district
		index = 0;
		i++;
		while (InputRecord[i] != ',' && i < rec_len) {
			temp[index] = InputRecord[i];
			i++;
			index++;
		}
		temp[index] = '\0';
		index_value = (int)atoi(temp);

		if (index_value > msc->nm_max_index)
			msc->nm_max_index = index_value;
		msc->nm_indices[idist] = index_value;
		msc->nm_index_used[index_value] = 1;
	}
	
}

