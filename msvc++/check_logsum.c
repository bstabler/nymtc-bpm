#include "md.h"

void logsum_check (int i, int journey, int orig, int dest, double util_value, double return_value, double THETA151, double THETA152,
				   float *OD_Utility, float *SE_Utility, float *ASC) {

	int j;

	printf ("Error found in calculating logsums for mode %s:\n", ModeLabel[i]);
	printf ("%s utility times nesting coefficients=%18.10e should be less than or equal to logsum=%18.10e.\n", ModeLabel[i], util_value*THETA151*THETA152, return_value);
	printf ("journey record = %d\n", journey);
	printf ("orig,dest = %d,%d\n", orig, dest);
	printf ("lowest level nesting coefficient = %7.5f\n", THETA151);
	printf ("next lowest level nesting coefficient = %7.5f\n", THETA152);
	for (j=0; j < Ini->NUMBER_ALTS; j++)
		printf ("%s od util= %16.6e, se util= %16.6e, msc= %16.6e, total util= %16.6e\n", ModeLabel[j], OD_Utility[j], SE_Utility[j], ASC[j], OD_Utility[j] + SE_Utility[j] + ASC[j]);
	printf ("exiting program.\n\n\n");
	fprintf (fp_rep, "Error found in calculating logsums for mode %s:\n", ModeLabel[i]);
	fprintf (fp_rep, "%s utility times nesting coefficients=%18.10e should be less than or equal to logsum=%18.10e.\n", ModeLabel[i], util_value*THETA151*THETA152, return_value);
	fprintf (fp_rep, "journey record = %d\n", journey);
	fprintf (fp_rep, "orig,dest = %d,%d\n", orig, dest);
	fprintf (fp_rep, "lowest level nesting coefficient = %7.5f\n", THETA151);
	fprintf (fp_rep, "next lowest level nesting coefficient = %7.5f\n", THETA152);
	for (j=0; j < Ini->NUMBER_ALTS; j++)
		fprintf (fp_rep, "%s od util= %16.6e, se util= %16.6e, msc= %16.6e, total util= %16.6e\n", ModeLabel[j], OD_Utility[j], SE_Utility[j], ASC[j], OD_Utility[j] + SE_Utility[j] + ASC[j]);
	fprintf (fp_rep, "exiting program.\n\n\n");
	fflush (stdout);
	fflush (fp_rep);

	exit (-15);
}

