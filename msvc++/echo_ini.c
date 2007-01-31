#include "md.h"

void echo_ini (FILE *fp_in, FILE *fp_out, char *filename)
{
	char InputRecord[LRECL];
	char temp[LRECL];
	int i;

// echo ini file to the report file.
	sprintf (temp, "Contents of input file: %s", filename);
	fprintf (fp_out, "\n%s\n", temp);
	for (i=0; i < (int)strlen(temp); i++)
		fprintf (fp_out, "_");
	fprintf (fp_out, "\n");

	while ((fgets(InputRecord, LRECL, fp_in)) != NULL)
		fprintf (fp_out, "%s", InputRecord);

	sprintf (temp, "End of %s", filename);
	for (i=0; i < (int)strlen(temp); i++)
		fprintf (fp_out, "_");
	fprintf (fp_out, "\n%s\n", temp);

	fprintf (fp_out, "\n\n\n\n");

	fflush (fp_out);
}


