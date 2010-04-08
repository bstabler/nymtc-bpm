#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **filenames;
extern char *taz_file;
extern char *report_file;
extern char *corresp_file;
extern char *summaryType;
extern int *maxTaz;

void read_ini_control_file (FILE *fp)
{

	char *GetStringFromINI (FILE *fp, char *Section, char *String);
	int GetIntFromINI (FILE *fp, char *Section, char *String);

	char *temp;

// read the file names to use for input data, output data and reports
	if ((temp = GetStringFromINI (fp, "FILES", "MDSC_OUT_1")) != NULL)
		strcpy (filenames[1], temp);
	else
		strcpy (filenames[1], "");

	if ((temp = GetStringFromINI (fp, "FILES", "MDSC_OUT_2")) != NULL)
		strcpy (filenames[2], temp);
	else
		strcpy (filenames[2], "");

	if ((temp = GetStringFromINI (fp, "FILES", "MDSC_OUT_3")) != NULL)
		strcpy (filenames[3], temp);
	else
		strcpy (filenames[3], "");

	if ((temp = GetStringFromINI (fp, "FILES", "MDSC_OUT_4")) != NULL)
		strcpy (filenames[4], temp);
	else
		strcpy (filenames[4], "");

	if ((temp = GetStringFromINI (fp, "FILES", "MDSC_OUT_5")) != NULL)
		strcpy (filenames[5], temp);
	else
		strcpy (filenames[5], "");

	if ((temp = GetStringFromINI (fp, "FILES", "MDSC_OUT_6")) != NULL)
		strcpy (filenames[6], temp);
	else
		strcpy (filenames[6], "");

	if ((temp = GetStringFromINI (fp, "FILES", "MDSC_OUT_7")) != NULL)
		strcpy (filenames[7], temp);
	else
		strcpy (filenames[7], "");

	if ((temp = GetStringFromINI (fp, "FILES", "MDSC_OUT_8")) != NULL)
		strcpy (filenames[8], temp);
	else
		strcpy (filenames[8], "");

	if ((temp = GetStringFromINI (fp, "FILES", "REPORT_FILE")) != NULL)
		strcpy (report_file, temp);
	else
		strcpy (report_file, "");

	if ((temp = GetStringFromINI (fp, "FILES", "TAZ_FILE")) != NULL)
		strcpy (taz_file, temp);
	else
		strcpy (taz_file, "");

	if ((temp = GetStringFromINI (fp, "FILES", "CORRESP_FILE")) != NULL)
		strcpy (corresp_file, temp);
	else
		strcpy (corresp_file, "");

	if ((temp = GetStringFromINI (fp, "FILES", "SUMMARY_TYPE")) != NULL)
		strcpy (summaryType, temp);
	else
		strcpy (summaryType, "");


}
