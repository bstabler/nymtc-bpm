#include "md.h"

void stops_reports (int *ob_mode_freqs, int *ib_mode_freqs, int *obib_mode_freqs, int *none_mode_freqs, int **ob_area_freqs, int **ib_area_freqs, int **obib_area_freqs, int **none_area_freqs, int *ob_district_freqs, int *ib_district_freqs, double *ob_district_size, double *ib_district_size)
{

	int i, j;
	int ob_total_freqs=0, ib_total_freqs=0;
	int obib_total_freqs=0, none_total_freqs=0;
	int total;
	double ob_total_size=0.0, ib_total_size=0.0;




	fprintf (fp_rep, "\n\n\n\n\n%s Stop Frequency by Mode:\n\n", PurposeLabels[Ini->PURPOSE_TO_PROCESS]);
	fprintf (fp_rep, "-----------------------------------------------------------------------------------------\n");
	fprintf (fp_rep, "MODE                         NO STOPS     OB STOPS     IB STOPS   BOTH STOPS        TOTAL\n");
	fprintf (fp_rep, "-----------------------------------------------------------------------------------------\n");
	ob_total_freqs = 0;
	ib_total_freqs = 0;
	obib_total_freqs = 0;
	none_total_freqs = 0;
	for (i=1; i <= Ini->NUMBER_ALTS; i++) {
		fprintf (fp_rep, "%-24s %12d %12d %12d %12d %12d\n", ModeLabel[i-1],
			none_mode_freqs[i], ob_mode_freqs[i], ib_mode_freqs[i], obib_mode_freqs[i],
			none_mode_freqs[i] + ob_mode_freqs[i] + ib_mode_freqs[i] + obib_mode_freqs[i]);
		ob_total_freqs += ob_mode_freqs[i];
		ib_total_freqs += ib_mode_freqs[i];
		obib_total_freqs += obib_mode_freqs[i];
		none_total_freqs += none_mode_freqs[i];
	}
	fprintf (fp_rep, "-----------------------------------------------------------------------------------------\n");
	fprintf (fp_rep, "%-24s %12d %12d %12d %12d %12d\n", "Total",
		none_total_freqs, ob_total_freqs, ib_total_freqs, obib_total_freqs,
		none_total_freqs + ob_total_freqs + ib_total_freqs + obib_total_freqs);




	fprintf (fp_rep, "\n\n\n\n\n%s Percent Stop Frequency by Mode:\n\n", PurposeLabels[Ini->PURPOSE_TO_PROCESS]);
	fprintf (fp_rep, "--------------------------------------------------------------------------------------------\n");
	fprintf (fp_rep, "MODE                       %% NO STOPS   %% OB STOPS   %% IB STOPS   %% BOTH STOPS      %% TOTAL\n");
	fprintf (fp_rep, "--------------------------------------------------------------------------------------------\n");
	ob_total_freqs = 0;
	ib_total_freqs = 0;
	obib_total_freqs = 0;
	none_total_freqs = 0;
	for (i=1; i <= Ini->NUMBER_ALTS; i++) {
		ob_total_freqs += ob_mode_freqs[i];
		ib_total_freqs += ib_mode_freqs[i];
		obib_total_freqs += obib_mode_freqs[i];
		none_total_freqs += none_mode_freqs[i];
	}
	for (i=1; i <= Ini->NUMBER_ALTS; i++) {
		total = none_mode_freqs[i] + ob_mode_freqs[i] + ib_mode_freqs[i] + obib_mode_freqs[i];
		fprintf (fp_rep, "%-24s %12.1f %12.1f %12.1f %15.1f %12.1f\n", ModeLabel[i-1],
			(total > 0.0 ? 100.0*none_mode_freqs[i]/total : 0.0),
			(total > 0.0 ? 100.0*ob_mode_freqs[i]/total : 0.0),
			(total > 0.0 ? 100.0*ib_mode_freqs[i]/total : 0.0),
			(total > 0.0 ? 100.0*obib_mode_freqs[i]/total : 0.0),
			(total > 0.0 ? 100.0 : 0.0));
	}
	total =	none_total_freqs + ob_total_freqs + ib_total_freqs + obib_total_freqs;
	fprintf (fp_rep, "--------------------------------------------------------------------------------------------\n");
	fprintf (fp_rep, "%-24s %12.1f %12.1f %12.1f %15.1f %12.1f\n", "Total",
		(total > 0.0 ? 100.0*none_total_freqs/total : 0.0),
		(total > 0.0 ? 100.0*ob_total_freqs/total : 0.0),
		(total > 0.0 ? 100.0*ib_total_freqs/total : 0.0),
		(total > 0.0 ? 100.0*obib_total_freqs/total : 0.0),
		(total > 0.0 ? 100.0 : 0.0));




	fprintf (fp_rep, "\n\n\n\n\n%s Stop Frequency by Orig/Dest Urban Type:\n\n", PurposeLabels[Ini->PURPOSE_TO_PROCESS]);
	fprintf (fp_rep, "-------------------------------------------------------------------------------------------\n");
	fprintf (fp_rep, "ORIGIN       DESTINATION       NO STOPS     OB STOPS     IB STOPS   BOTH STOPS        TOTAL\n");
	fprintf (fp_rep, "-------------------------------------------------------------------------------------------\n");
	ob_total_freqs = 0;
	ib_total_freqs = 0;
	obib_total_freqs = 0;
	none_total_freqs = 0;
	for (i=1; i <= 4; i++) {
		for (j=1; j <= 4; j++) {
			fprintf (fp_rep, "%-13s%-13s %12d %12d %12d %12d %12d\n", UrbanTypeLabels[i-1], UrbanTypeLabels[j-1],
				none_area_freqs[i][j], ob_area_freqs[i][j], ib_area_freqs[i][j], obib_area_freqs[i][j],
				none_area_freqs[i][j] + ob_area_freqs[i][j] + ib_area_freqs[i][j] + obib_area_freqs[i][j]);
			ob_total_freqs += ob_area_freqs[i][j];
			ib_total_freqs += ib_area_freqs[i][j];
			obib_total_freqs += obib_area_freqs[i][j];
			none_total_freqs += none_area_freqs[i][j];
		}
	}
	fprintf (fp_rep, "-------------------------------------------------------------------------------------------\n");
	fprintf (fp_rep, "%-13s%-13s %12d %12d %12d %12d %12d\n", "Total", "Total",
		none_total_freqs, ob_total_freqs, ib_total_freqs, obib_total_freqs,
		none_total_freqs + ob_total_freqs + ib_total_freqs + obib_total_freqs);




	fprintf (fp_rep, "\n\n\n\n\n%s Percent Stop Frequency by Orig/Dest Urban Type:\n\n", PurposeLabels[Ini->PURPOSE_TO_PROCESS]);
	fprintf (fp_rep, "----------------------------------------------------------------------------------------------\n");
	fprintf (fp_rep, "ORIGIN       DESTINATION     %% NO STOPS   %% OB STOPS   %% IB STOPS    %% BOTH STOPS      %% TOTAL\n");
	fprintf (fp_rep, "----------------------------------------------------------------------------------------------\n");
	ob_total_freqs = 0;
	ib_total_freqs = 0;
	obib_total_freqs = 0;
	none_total_freqs = 0;
	for (i=1; i <= 4; i++) {
		for (j=1; j <= 4; j++) {
			ob_total_freqs += ob_area_freqs[i][j];
			ib_total_freqs += ib_area_freqs[i][j];
			obib_total_freqs += obib_area_freqs[i][j];
			none_total_freqs += none_area_freqs[i][j];
		}
	}
	for (i=1; i <= 4; i++) {
		for (j=1; j <= 4; j++) {
			total =	none_area_freqs[i][j] + ob_area_freqs[i][j] + ib_area_freqs[i][j] + obib_area_freqs[i][j];
			fprintf (fp_rep, "%-13s%-13s %12.1f %12.1f %12.1f %15.1f %12.1f\n", UrbanTypeLabels[i-1], UrbanTypeLabels[j-1],
				(total > 0.0 ? 100.0*none_area_freqs[i][j]/total : 0.0),
				(total > 0.0 ? 100.0*ob_area_freqs[i][j]/total : 0.0),
				(total > 0.0 ? 100.0*ib_area_freqs[i][j]/total : 0.0),
				(total > 0.0 ? 100.0*obib_area_freqs[i][j]/total : 0.0),
				(total > 0.0 ? 100.0 : 0.0));
		}
	}
	total =	none_total_freqs + ob_total_freqs + ib_total_freqs + obib_total_freqs;
	fprintf (fp_rep, "----------------------------------------------------------------------------------------------\n");
	fprintf (fp_rep, "%-13s%-13s %12.1f %12.1f %12.1f %15.1f %12.1f\n", "Total", "Total",
		(total > 0.0 ? 100.0*none_total_freqs/total : 0.0),
		(total > 0.0 ? 100.0*ob_total_freqs/total : 0.0),
		(total > 0.0 ? 100.0*ib_total_freqs/total : 0.0),
		(total > 0.0 ? 100.0*obib_total_freqs/total : 0.0),
		(total > 0.0 ? 100.0 : 0.0));




	fprintf (fp_rep, "\n\n\n\n\n%s Stop Locations by District:\n\n", PurposeLabels[Ini->PURPOSE_TO_PROCESS]);
	fprintf (fp_rep, "-----------------------------------------------------------------------------------------------------------------\n");
	fprintf (fp_rep, "                                               OUTBOUND                                     INBOUND\n");
	fprintf (fp_rep, "     STOP DISTRICT               STOPS           SIZE     STOPS/SIZE          STOPS           SIZE     STOPS/SIZE\n");
	fprintf (fp_rep, "-----------------------------------------------------------------------------------------------------------------\n");
	ob_total_freqs = 0;
	ib_total_freqs = 0;
	ob_total_size = 0.0;
	ib_total_size = 0.0;
	for (i=0; i < Ini->NUMBER_BPMDIST1; i++) {
		fprintf (fp_rep, "%3d  %-18s %14d %14.0f %14.4f %14d %14.0f %14.4f\n", i, BPMDist1Labels[i],
			ob_district_freqs[i], ob_district_size[i], (ob_district_size[i] > 0.0 ? ob_district_freqs[i]/ob_district_size[i] : 0.0),
			ib_district_freqs[i], ib_district_size[i], (ib_district_size[i] > 0.0 ? ib_district_freqs[i]/ib_district_size[i] : 0.0));
		ob_total_freqs += ob_district_freqs[i];
		ib_total_freqs += ib_district_freqs[i];
		ob_total_size += ob_district_size[i];
		ib_total_size += ib_district_size[i];
	}
	fprintf (fp_rep, "-----------------------------------------------------------------------------------------------------------------\n");
	fprintf (fp_rep, "     %-18s %14d %14.0f %14.4f %14d %14.0f %14.4f\n", "Total",
		ob_total_freqs, ob_total_size, ob_total_freqs/ob_total_size,
		ib_total_freqs, ib_total_size, ib_total_freqs/ib_total_size);

}