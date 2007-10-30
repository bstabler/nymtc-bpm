#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Labels.h"

#define TAZS 3586
#define DISTRICTS 31
#define PURPOSES 8
#define MODES 11
#define MAJOR_PURPOSES 3
#define MAJOR_MODES 6
#define SR7_REGIONS 7
#define SAD14_REGIONS 14
#define STOPS 4

#define JOURNEYS 20000000



char **filenames;
char *report_file;
char *corresp_file;
char *summaryType;

char **SubregionLabels;
int numSubRegions;

int main (int argc, char *argv[]) {

	void read_dist_region_data (FILE *fp, char *subregion, int *taz2dist, int *taz2reg, int tazs);
	void print_table (FILE *fp, int **table, char *tableHeading, char *rowHeading,
		char *rowLabels[], char *colLabels[], int rows, int cols);
	void read_ini_control_file (FILE *fp);



	FILE *fp[PURPOSES+1];
	FILE *fp_rep;
	FILE *fp_ini;
	char temp[1000];
	
	int i, j, k;
	int odist, ddist, oreg, dreg, majPurp, majMode, stop, numJourneys;
	int osdist, isdist, numStops;
	
	int taz2dist[TAZS+1];
	int taz2reg[TAZS+1];
	int majorPurpose[] = { 0, 1, 1, 1, 2, 2, 3, 3, 3 };
	int majorMode[] = { 0, 1, 2, 2, 2, 4, 4, 5, 5, 3, 6, 6 };
	
	int **table1;
	int ***table2a;
	int ***table2b;
	int ***table3;
	int ***table4;
	int ***table5;
	int ***table6;
	int ***table7;
	int ***table8;
	
	int *seq, *orig, *dest, *purp, *mode, *oloc, *iloc;



	filenames = (char **) calloc (PURPOSES+1, sizeof (char *));
	for (i=0; i < PURPOSES+1; i++)
		filenames[i] = (char *) calloc (256, sizeof (char));

	report_file = (char *) calloc (256, sizeof (char));
	corresp_file = (char *) calloc (256, sizeof (char));
	summaryType = (char *) calloc (256, sizeof (char));



// open and read .ini control file
	if (argc != 2) {
		printf ("PostSum.exe expecting one argument, the control file.\n");
		printf ("Use syntax: PostSum PostSum.ini\n");
		exit (-5);
	}
	else {
		if ((fp_ini = fopen (argv[1], "r")) == NULL) {
			printf ("file not found or other error opening control file: %s\n", argv[1]);
			exit (-6);
		}
		else {
			read_ini_control_file (fp_ini);
		}
	}



	if ( strcmp(summaryType, "SR7") == 0 ) {
		numSubRegions = SR7_REGIONS;
		SubregionLabels = SR7_Labels;
	}
	else if ( strcmp(summaryType, "SAD14") == 0 ) {
		numSubRegions = SAD14_REGIONS;
		SubregionLabels = SAD14_Labels;
	}
	else {
		printf ("invalid summary type value = %s - should be SR7 or SAD14.\n", summaryType);
		exit (-1);
	}
	


// allocate memory for summary tables
	table1 = (int **) calloc (DISTRICTS+1, sizeof(int *));
	for (i=0; i < DISTRICTS+1; i++)
		table1[i] = (int *) calloc (PURPOSES+1, sizeof(int));
	
	table2a = (int ***) calloc (PURPOSES+1, sizeof(int **));
	table2b = (int ***) calloc (PURPOSES+1, sizeof(int **));
	for (i=0; i < PURPOSES+1; i++) {
		table2a[i] = (int **) calloc (DISTRICTS+1, sizeof(int *));
		table2b[i] = (int **) calloc (DISTRICTS+1, sizeof(int *));
		for (j=0; j < DISTRICTS+1; j++) {
			table2a[i][j] = (int *) calloc (MODES+1, sizeof(int));
			table2b[i][j] = (int *) calloc (MODES+1, sizeof(int));
		}
	}

	table3 = (int ***) calloc (MAJOR_PURPOSES+1, sizeof(int **));
	for (i=0; i < MAJOR_PURPOSES+1; i++) {
		table3[i] = (int **) calloc (numSubRegions+1, sizeof(int *));
		for (j=0; j < numSubRegions+1; j++)
			table3[i][j] = (int *) calloc (numSubRegions+1, sizeof(int));
	}

	table4 = (int ***) calloc (MAJOR_MODES+1, sizeof(int **));
	for (i=0; i < MAJOR_MODES+1; i++) {
		table4[i] = (int **) calloc (numSubRegions+1, sizeof(int *));
		for (j=0; j < numSubRegions+1; j++)
			table4[i][j] = (int *) calloc (numSubRegions+1, sizeof(int));
	}

	table5 = (int ***) calloc (STOPS+1, sizeof(int **));
	for (i=0; i < STOPS+1; i++) {
		table5[i] = (int **) calloc (numSubRegions+1, sizeof(int *));
		for (j=0; j < numSubRegions+1; j++)
			table5[i][j] = (int *) calloc (numSubRegions+1, sizeof(int));
	}

	table6 = (int ***) calloc (PURPOSES+1, sizeof(int **));
	for (i=0; i < PURPOSES+1; i++) {
		table6[i] = (int **) calloc (STOPS+1, sizeof(int *));
		for (j=0; j < STOPS+1; j++)
			table6[i][j] = (int *) calloc (MODES+1, sizeof(int));
	}

	table7 = (int ***) calloc (PURPOSES+1, sizeof(int **));
	for (i=0; i < PURPOSES+1; i++) {
		table7[i] = (int **) calloc (DISTRICTS+1, sizeof(int *));
		for (j=0; j < DISTRICTS+1; j++)
			table7[i][j] = (int *) calloc (MODES+1, sizeof(int));
	}

	table8 = (int ***) calloc (MAJOR_MODES+1, sizeof(int **));
	for (i=0; i < MAJOR_MODES+1; i++) {
		table8[i] = (int **) calloc (numSubRegions+1, sizeof(int *));
		for (j=0; j < numSubRegions+1; j++)
			table8[i][j] = (int *) calloc (numSubRegions+1, sizeof(int));
	}



	seq  = (int *) calloc (JOURNEYS, sizeof(int));
	orig = (int *) calloc (JOURNEYS, sizeof(int));
	dest = (int *) calloc (JOURNEYS, sizeof(int));
	purp = (int *) calloc (JOURNEYS, sizeof(int));
	mode = (int *) calloc (JOURNEYS, sizeof(int));
	oloc = (int *) calloc (JOURNEYS, sizeof(int));
	iloc = (int *) calloc (JOURNEYS, sizeof(int));
	


	// open output report file
	if ((fp_rep = fopen (report_file, "w")) == NULL) {
		printf ("error opening report file for writing: %s\n", report_file);
		exit (-1);
	}

	
	// open sub-region correspondence file
	if ((fp[0] = fopen (corresp_file, "r")) == NULL) {
		printf ("error opening TAZ/SUB-REGION correspondence file: %s\n", corresp_file);
		exit (-1);
	}
	for (i=0; i <= TAZS; i++) {
		taz2dist[i] = 0;
		taz2reg[i] = 0;
	}
	read_dist_region_data (fp[0], summaryType, taz2dist, taz2reg, TAZS);
	fclose (fp[0]);


	// summarize results from mdsc_out for each purpose
	for (i=1; i <= PURPOSES; i++) {
	
		// open MDSC output file for purpose i
		if ((fp[i] = fopen (filenames[i], "r")) == NULL) {
			printf ("error opening purpose %d MDSC output file: %s\n", i, filenames[i]);
			exit (-1);
		}
		
		// read MDSC output file for purpose i
		k = 0;
		while ((fscanf (fp[i], "%d %d %d %d %d %d %d", &seq[k], &orig[k], &dest[k], &purp[k], &mode[k], &oloc[k], &iloc[k])) != EOF) {
	
			// outbound and inbound stop locations for DT and DC may be negative to indicate stop is closer to dest than orig.
			if (oloc[k] < 0)
				oloc[k] *= -1;
			if (iloc[k] < 0)
				iloc[k] *= -1;
			
			purp[k] = i;
			k++;

			if (k == JOURNEYS) {
				printf ("This program was compiled assuming a maximum of %d paired journeys.\n", JOURNEYS);
				printf ("The MDSC output file being read, %s, has exceeded this maximum.\n", filenames[i]);
				printf ("Change the #define JOURNEYS statement in PostSum.c to raise the maximum to a more \n");
				printf ("approriate value and re-build PostSum.exe.\n");
				fflush (stdout);
				exit (-3);
			}
		}
			
		numJourneys = k;

		// close MDSC output file for purpose i
		fclose (fp[i]);
		
		// summarize MDSC output file data for purpose i
		for (k=0; k < numJourneys; k++) {

			if (purp[k] != i) {
				printf ("record number %d of %d in file %s has purpose code of %d but should be %d.\n", k, numJourneys, filenames[i], purp[k], i);
				exit (-2);
			}
			

			// get summary level values
			odist = taz2dist[orig[k]];
			ddist = taz2dist[dest[k]];
			osdist = taz2dist[oloc[k]];
			isdist = taz2dist[iloc[k]];
			oreg = taz2reg[orig[k]];
			dreg = taz2reg[dest[k]];
			majPurp = majorPurpose[purp[k]];
			majMode = majorMode[mode[k]];
			if (oloc[k] > 0) {
				if (iloc[k] > 0)
					stop = 4;
				else
					stop = 2;
			}
			else {
				if (iloc[k] > 0)
					stop = 3;
				else
					stop = 1;
			}

			if (stop == 4)
				numStops = 2;
			else if (stop == 2 || stop == 3)
				numStops = 1;
			else
				numStops = 0;


				
			// tabulate tables - first cell, then row margin, column margin, and finally table margin.
			table1[ddist][i] ++;
			table1[ddist][0] ++;
			table1[0][i] ++;
			table1[0][0] ++;

			table2a[i][odist][mode[k]] ++;
			table2a[i][odist][0] ++;
			table2a[i][0][mode[k]] ++;
			table2a[i][0][0] ++;
			table2a[0][odist][mode[k]] ++;
			table2a[0][odist][0] ++;
			table2a[0][0][mode[k]] ++;
			table2a[0][0][0] ++;

			table2b[i][ddist][mode[k]] ++;
			table2b[i][ddist][0] ++;
			table2b[i][0][mode[k]] ++;
			table2b[i][0][0] ++;
			table2b[0][ddist][mode[k]] ++;
			table2b[0][ddist][0] ++;
			table2b[0][0][mode[k]] ++;
			table2b[0][0][0] ++;

			table3[majPurp][oreg][dreg] ++;
			table3[majPurp][oreg][0] ++;
			table3[majPurp][0][dreg] ++;
			table3[majPurp][0][0] ++;
			table3[0][oreg][dreg] ++;
			table3[0][oreg][0] ++;
			table3[0][0][dreg] ++;
			table3[0][0][0] ++;

			table4[majMode][oreg][dreg] ++;
			table4[majMode][oreg][0] ++;
			table4[majMode][0][dreg] ++;
			table4[majMode][0][0] ++;
			table4[0][oreg][dreg] ++;
			table4[0][oreg][0] ++;
			table4[0][0][dreg] ++;
			table4[0][0][0] ++;

			table5[stop][oreg][dreg] ++;
			table5[stop][oreg][0] ++;
			table5[stop][0][dreg] ++;
			table5[stop][0][0] ++;
			table5[0][oreg][dreg] ++;
			table5[0][oreg][0] ++;
			table5[0][0][dreg] ++;
			table5[0][0][0] ++;

			table6[i][stop][mode[k]] ++;
			table6[i][stop][0] ++;
			table6[i][0][mode[k]] ++;
			table6[i][0][0] ++;
			table6[0][stop][mode[k]] ++;
			table6[0][stop][0] ++;
			table6[0][0][mode[k]] ++;
			table6[0][0][0] ++;

			if (osdist > 0) {
				table7[i][osdist][mode[k]] ++;
				table7[i][osdist][0] ++;
				table7[i][0][mode[k]] ++;
				table7[i][0][0] ++;
				table7[0][osdist][mode[k]] ++;
				table7[0][osdist][0] ++;
				table7[0][0][mode[k]] ++;
				table7[0][0][0] ++;
			}
			if (isdist > 0) {
				table7[i][isdist][mode[k]] ++;
				table7[i][isdist][0] ++;
				table7[i][0][mode[k]] ++;
				table7[i][0][0] ++;
				table7[0][isdist][mode[k]] ++;
				table7[0][isdist][0] ++;
				table7[0][0][mode[k]] ++;
				table7[0][0][0] ++;
			}

			table8[majMode][oreg][dreg] += numStops;
			table8[majMode][oreg][0] += numStops;
			table8[majMode][0][dreg] += numStops;
			table8[majMode][0][0] += numStops;
			table8[0][oreg][dreg] += numStops;
			table8[0][oreg][0] += numStops;
			table8[0][0][dreg] += numStops;
			table8[0][0][0] += numStops;

		} // end loop over journeys

	} // end loop over purposes
	


	print_table (fp_rep, table1, "Total Paired Journeys -- Destination Districts by Purpose",
		"Destination Districts", DistrictLabels, PurposeLabels, DISTRICTS, PURPOSES);

	
	for (i=1; i <= PURPOSES; i++) {
		strcpy (temp, PurposeLabels[i-1]);
		strcat (temp, " Paired Journeys -- Origin Districts by Modes");
		print_table (fp_rep, table2a[i], temp, "Origin Districts", DistrictLabels, ModeLabels, DISTRICTS, MODES);
	}
	strcpy (temp, "Total");
	strcat (temp, " Paired Journeys -- Origin Districts by Modes");
	print_table (fp_rep, table2a[0], temp, "Origin Districts", DistrictLabels, ModeLabels, DISTRICTS, MODES);


	for (i=1; i <= PURPOSES; i++) {
		strcpy (temp, PurposeLabels[i-1]);
		strcat (temp, " Paired Journeys -- Destination Districts by Modes");
		print_table (fp_rep, table2b[i], temp, "Destination Districts", DistrictLabels, ModeLabels, DISTRICTS, MODES);
	}
	strcpy (temp, "Total");
	strcat (temp, " Paired Journeys -- Destination Districts by Modes");
	print_table (fp_rep, table2b[0], temp, "Destination Districts", DistrictLabels, ModeLabels, DISTRICTS, MODES);


	for (i=1; i <= MAJOR_PURPOSES; i++) {
		strcpy (temp, MajorPurposeLabels[i-1]);
		strcat (temp, " Paired Journeys -- O/D Sub-regions");
		print_table (fp_rep, table3[i], temp, "Origin Sub-region", SubregionLabels, SubregionLabels, numSubRegions, numSubRegions);
	}
	strcpy (temp, "Total");
	strcat (temp, " Paired Journeys -- O/D Sub-regions");
	print_table (fp_rep, table3[0], temp, "Origin Sub-region", SubregionLabels, SubregionLabels, numSubRegions, numSubRegions);


	for (i=1; i < MAJOR_MODES; i++) {
		strcpy (temp, MajorModeLabels[i-1]);
		strcat (temp, " Paired Journeys -- O/D Sub-regions");
		print_table (fp_rep, table4[i], temp, "Origin Sub-region", SubregionLabels, SubregionLabels, numSubRegions, numSubRegions);
	}
	strcpy (temp, "Total");
	strcat (temp, " Paired Journeys -- O/D Sub-regions");
	print_table (fp_rep, table4[0], temp, "Origin Sub-region", SubregionLabels, SubregionLabels, numSubRegions, numSubRegions);


	for (i=1; i <= STOPS; i++) {
		strcpy (temp, StopsMadeLabels[i-1]);
		strcat (temp, " Paired Journeys -- O/D Sub-regions");
		print_table (fp_rep, table5[i], temp, "Origin Sub-region", SubregionLabels, SubregionLabels, numSubRegions, numSubRegions);
	}
	strcpy (temp, "Total");
	strcat (temp, " Paired Journeys -- O/D Sub-regions");
	print_table (fp_rep, table5[0], temp, "Origin Sub-region", SubregionLabels, SubregionLabels, numSubRegions, numSubRegions);


	for (i=1; i <= PURPOSES; i++) {
		strcpy (temp, PurposeLabels[i-1]);
		strcat (temp, " Paired Journeys -- Stops Made by Mode");
		print_table (fp_rep, table6[i], temp, "Stops Made", StopsMadeLabels, ModeLabels, STOPS, MODES);
	}
	strcpy (temp, "Total");
	strcat (temp, " Paired Journeys -- Stops Made by Mode");
	print_table (fp_rep, table6[0], temp, "Stops Made", StopsMadeLabels, ModeLabels, STOPS, MODES);


	for (i=1; i <= PURPOSES; i++) {
		strcpy (temp, PurposeLabels[i-1]);
		strcat (temp, " Paired Journey Stops -- Stop District by Mode");
		print_table (fp_rep, table7[i], temp, "Stop District", DistrictLabels, ModeLabels, DISTRICTS, MODES);
	}
	strcpy (temp, "Total");
	strcat (temp, " Paired Journey Stops -- Stop District by Mode");
	print_table (fp_rep, table7[0], temp, "Stop District", DistrictLabels, ModeLabels, DISTRICTS, MODES);


	for (i=1; i < MAJOR_MODES; i++) {
		strcpy (temp, MajorModeLabels[i-1]);
		strcat (temp, " Paired Journey Stops -- O/D Sub-regions");
		print_table (fp_rep, table8[i], temp, "Origin Sub-region", SubregionLabels, SubregionLabels, numSubRegions, numSubRegions);
	}
	strcpy (temp, "Total");
	strcat (temp, " Paired Journey Stops -- O/D Sub-regions");
	print_table (fp_rep, table8[0], temp, "Origin Sub-region", SubregionLabels, SubregionLabels, numSubRegions, numSubRegions);


} // end of main





void print_table (FILE *fp, int **table, char *tableHeading, char *rowHeading,
	char *rowLabels[], char *colLabels[], int rows, int cols)
{
	
/*  assumes table is an integer array dimensioned as: int table[rows+1][cols+1] where
    table[1,...,rows][0] has the row margins, table[0][1,...,cols] has the column margins,
    and table[0][0] has the table margin.
*/    

	void print_underline (FILE *fp, int length);
  
	int i, j;
	int tableWidth, indexWidth, rowLabelWidth, cellWidth, spacingWidth;
	char cellFormat[100];
	char colHeadingFormat[100];
	char rowHeadingFormat[100];
	char rowTotHeadingFormat[100];

	rowLabelWidth = 0;
	for (i=0; i <= rows; i++) {
		if ((int)strlen(rowLabels[i]) > rowLabelWidth)
			rowLabelWidth = (int)strlen(rowLabels[i]);
	}
	if ((int)strlen(rowHeading) > rowLabelWidth)
		rowLabelWidth = (int)strlen(rowHeading);
	rowLabelWidth += 2;

	cellWidth = 8;
	for (i=0; i <= cols; i++) {
		if ((int)strlen(colLabels[i]) > cellWidth)
			cellWidth = (int)strlen(colLabels[i]);
	}
	cellWidth += 2;

	indexWidth = (int)(log10(rows) + 1);
	spacingWidth = rowLabelWidth - (int)strlen(rowHeading) + indexWidth + 1;
	tableWidth = rowLabelWidth + indexWidth + 2 + cols*(cellWidth+1) + cellWidth;
	
	sprintf (colHeadingFormat, " %%%ds", cellWidth);
	sprintf (rowHeadingFormat, "%%-%dd %%-%ds", indexWidth, rowLabelWidth);
	sprintf (rowTotHeadingFormat, "%%-%ds %%-%ds", indexWidth, rowLabelWidth);
	sprintf (cellFormat, " %%%dd", cellWidth);
	
	
	fprintf (fp, "%s:\n", tableHeading);
	print_underline (fp, tableWidth);
	fprintf (fp, "%s", rowHeading);
	for (i=0; i < spacingWidth; i++)
		fprintf (fp, "%c", ' ');
	for (i=0; i <= cols; i++)
		fprintf (fp, colHeadingFormat, colLabels[i]);
	fprintf (fp, "\n");
	print_underline (fp, tableWidth);

	for (i=1; i <= rows; i++) {
		fprintf (fp, rowHeadingFormat, i, rowLabels[i-1]);
		for (j=1; j <= cols; j++)
			fprintf (fp, cellFormat, table[i][j]);
		fprintf (fp, cellFormat, table[i][0]);
		fprintf (fp, "\n");
	}
	print_underline (fp, tableWidth);
	fprintf (fp, rowTotHeadingFormat, " ", "TOTAL");
	for (j=1; j <= cols; j++)
		fprintf (fp, cellFormat, table[0][j]);
	fprintf (fp, cellFormat, table[0][0]);
	fprintf (fp, "\n");
	print_underline (fp, tableWidth);

	fprintf (fp, "\n\n\n\n\n");
	fflush (fp);
}


void print_underline (FILE *fp, int length) {
	
	int i;
	
	for (i=0; i < length; i++)
		fprintf (fp, "-");
	fprintf (fp, "\n");
}
