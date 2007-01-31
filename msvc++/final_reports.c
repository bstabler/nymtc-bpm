#include "md.h"

void final_reports (FILE *fp_rep2, int **mc, double *shares, int **acc_freq, int **tlfreq, double *tldist, int ***dist2dist, float **t_attrs, struct zone_data *ZonalData, int unmet_count) {

	int i, j, m;
	double TotalShare, TotalAcc, TotalDist;
	int *co_zones;
	float *co_attrs;


	co_attrs = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_BPMDIST1+1)*sizeof(float));
	co_zones = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_BPMDIST1+1)*sizeof(int));
//	addRAM ("final_reports", (Ini->NUMBER_BPMDIST1+1)*sizeof(float) + (Ini->NUMBER_BPMDIST1+1)*sizeof(int));

	for (i=1; i <= Ini->MAX_TAZS; i++) {
		co_zones[ZonalData->bpmdist1_index[i]] ++;
		co_zones[Ini->NUMBER_BPMDIST1] ++;
		co_attrs[ZonalData->bpmdist1_index[i]] += t_attrs[Ini->PURPOSE_TO_PROCESS][i];
		co_attrs[Ini->NUMBER_BPMDIST1] += t_attrs[Ini->PURPOSE_TO_PROCESS][i];
	}



	fprintf (fp_rep, "\n\n\n\n\nRandom number generator report\n");
	fprintf (fp_rep,           "------------------------------\n");
	fprintf (fp_rep,           "Random number seed set = %d\n", Ini->RAND_SEED);
	fprintf (fp_rep,           "Random numbers =");
	for (i=0; i < 5; i++)
		fprintf (fp_rep, " %10.6f", (double)rand()/(double)MAX_RANDOM);
	fprintf (fp_rep, "\n\n\n\n\n");


	fprintf (fp_rep, "\n\n\n\n\nFrequency of %s journey destination counties by mode:\n\n", PurposeLabels[Ini->PURPOSE_TO_PROCESS]);
	fprintf (fp_rep, "-------------------------------------------------------------------------------------------------------------------------------------------------\n");
	fprintf (fp_rep, "COUNTY                     DA     SR2     SR3     SR4      WT      DT      WC      DC      TX      NM       SB    TOTAL   SIZ_VAR   DISCR   ZONES\n");
	fprintf (fp_rep, "-------------------------------------------------------------------------------------------------------------------------------------------------\n");
	for (i=1; i <= Ini->NUMBER_BPMDIST1; i++)
		fprintf (fp_rep, "%-3d %-17s %7d %7d %7d %7d %7d %7d %7d %7d %7d %7d %7d %8d %9.0f %7.0f %7d\n",
			i, BPMDist1Labels[i-1], mc[i][0], mc[i][1], mc[i][2], mc[i][3], mc[i][4], mc[i][5], mc[i][6], mc[i][7], mc[i][8], mc[i][9], mc[i][10],
			mc[i][0] + mc[i][1] + mc[i][2] + mc[i][3] + mc[i][4] + mc[i][5] + mc[i][6] + mc[i][7] + mc[i][8] + mc[i][9] + mc[i][10],
			co_attrs[i-1],
			mc[i][0] + mc[i][1] + mc[i][2] + mc[i][3] + mc[i][4] + mc[i][5] + mc[i][6] + mc[i][7] + mc[i][8] + mc[i][9] + mc[i][10] - co_attrs[i-1],
			co_zones[i-1]);
	fprintf (fp_rep, "-------------------------------------------------------------------------------------------------------------------------------------------------\n");
	fprintf (fp_rep, "%3s %-17s %7d %7d %7d %7d %7d %7d %7d %7d %7d %7d %7d %8d %9.0f %7.0f %7d\n",
		"", "Total", mc[0][0], mc[0][1], mc[0][2], mc[0][3], mc[0][4], mc[0][5], mc[0][6], mc[0][7], mc[0][8], mc[0][9], mc[0][10],
		mc[0][0] + mc[0][1] + mc[0][2] + mc[0][3] + mc[0][4] + mc[0][5] + mc[0][6] + mc[0][7] + mc[0][8] + mc[0][9] + mc[0][10],
		co_attrs[Ini->NUMBER_BPMDIST1],
		mc[0][0] + mc[0][1] + mc[0][2] + mc[0][3] + mc[0][4] + mc[0][5] + mc[0][6] + mc[0][7] + mc[0][8] + mc[0][9] + mc[0][10] - co_attrs[Ini->NUMBER_BPMDIST1],
		co_zones[Ini->NUMBER_BPMDIST1]);
	fprintf (fp_rep, "-------------------------------------------------------------------------------------------------------------------------------------------------\n");
	fprintf (fp_rep, "%3s %-17s %6d%% %6d%% %6d%% %6d%% %6d%% %6d%% %6d%% %6d%% %6d%% %6d%% %6d%% %7d%%\n",
		"", "Estimated Shares", (int)(100*mc[0][0]/Ini->NUMBER_JOURNEYS), (int)(100*mc[0][1]/Ini->NUMBER_JOURNEYS), (int)(100*mc[0][2]/Ini->NUMBER_JOURNEYS)
		, (int)(100*mc[0][3]/Ini->NUMBER_JOURNEYS), (int)(100*mc[0][4]/Ini->NUMBER_JOURNEYS), (int)(100*mc[0][5]/Ini->NUMBER_JOURNEYS)
		, (int)(100*mc[0][6]/Ini->NUMBER_JOURNEYS), (int)(100*mc[0][7]/Ini->NUMBER_JOURNEYS), (int)(100*mc[0][8]/Ini->NUMBER_JOURNEYS)
		, (int)(100*mc[0][9]/Ini->NUMBER_JOURNEYS), (int)(100*mc[0][10]/Ini->NUMBER_JOURNEYS),
		(int)(100*(mc[0][0] + mc[0][1] + mc[0][2] + mc[0][3] + mc[0][4] + mc[0][5] + mc[0][6] + mc[0][7] + mc[0][8] + mc[0][9] + mc[0][10])/Ini->NUMBER_JOURNEYS));
	fprintf (fp_rep, "-------------------------------------------------------------------------------------------------------------------------------------------------\n");
	fprintf (fp_rep, "%3s %-17s %6d%% %6d%% %6d%% %6d%% %6d%% %6d%% %6d%% %6d%% %6d%% %6d%% %6d%% %7d%%\n",
		"", "Aggregate Props", (int)(100*shares[0]/Ini->NUMBER_JOURNEYS), (int)(100*shares[1]/Ini->NUMBER_JOURNEYS), (int)(100*shares[2]/Ini->NUMBER_JOURNEYS)
		, (int)(100*shares[3]/Ini->NUMBER_JOURNEYS), (int)(100*shares[4]/Ini->NUMBER_JOURNEYS), (int)(100*shares[5]/Ini->NUMBER_JOURNEYS)
		, (int)(100*shares[6]/Ini->NUMBER_JOURNEYS), (int)(100*shares[7]/Ini->NUMBER_JOURNEYS), (int)(100*shares[8]/Ini->NUMBER_JOURNEYS)
		, (int)(100*shares[9]/Ini->NUMBER_JOURNEYS), (int)(100*shares[10]/Ini->NUMBER_JOURNEYS),
		(int)(100*(shares[0] + shares[1] + shares[2] + shares[3] + shares[4] + shares[5] + shares[6] + shares[7] + shares[8] + shares[9] + shares[10])/Ini->NUMBER_JOURNEYS));
	fflush (fp_rep);




	TotalShare = 0.0;
	TotalAcc = 0.0;
	for (i=0; i < Ini->NUMBER_ALTS; i++) {
		TotalAcc += acc_freq[0][i];
		if (mc[0][i] > 0.0)
			shares[i] = 100.0*acc_freq[0][i]/mc[0][i];
		else
			shares[i] = 0.0;
	}
	TotalShare = 100.0*TotalAcc/Ini->NUMBER_JOURNEYS;

	fprintf (fp_rep, "\n\n\n\n\nFrequency of %s accessible journey destination counties by mode:\n\n", PurposeLabels[Ini->PURPOSE_TO_PROCESS]);
	fprintf (fp_rep, "----------------------------------------------------------------------------------------------------------------------\n");
	fprintf (fp_rep, "COUNTY                     DA     SR2     SR3     SR4      WT      DT      WC      DC      TX      NM      SB    TOTAL\n");
	fprintf (fp_rep, "----------------------------------------------------------------------------------------------------------------------\n");
	for (i=1; i <= Ini->NUMBER_BPMDIST1; i++)
		fprintf (fp_rep, "%-3d %-17s %7d %7d %7d %7d %7d %7d %7d %7d %7d %7d %7d %8d\n",
			i, BPMDist1Labels[i-1], acc_freq[i][0], acc_freq[i][1], acc_freq[i][2], acc_freq[i][3], acc_freq[i][4], acc_freq[i][5], acc_freq[i][6], acc_freq[i][7], acc_freq[i][8], acc_freq[i][9], acc_freq[i][10],
			acc_freq[i][0] + acc_freq[i][1] + acc_freq[i][2] + acc_freq[i][3] + acc_freq[i][4] + acc_freq[i][5] + acc_freq[i][6] + acc_freq[i][7] + acc_freq[i][8] + acc_freq[i][9] + acc_freq[i][10]);
	fprintf (fp_rep, "----------------------------------------------------------------------------------------------------------------------\n");
	fprintf (fp_rep, "%3s %-17s %7d %7d %7d %7d %7d %7d %7d %7d %7d %7d %7d %8d\n",
		"", "Total", acc_freq[0][0], acc_freq[0][1], acc_freq[0][2], acc_freq[0][3], acc_freq[0][4], acc_freq[0][5], acc_freq[0][6], acc_freq[0][7], acc_freq[0][8], acc_freq[0][9],  acc_freq[0][10], 
		acc_freq[0][0] + acc_freq[0][1] + acc_freq[0][2] + acc_freq[0][3] + acc_freq[0][4] + acc_freq[0][5] + acc_freq[0][6] + acc_freq[0][7] + acc_freq[0][8] + acc_freq[0][9] + acc_freq[0][10]);
	fprintf (fp_rep, "----------------------------------------------------------------------------------------------------------------------\n");
	fprintf (fp_rep, "%3s %-17s %6.1f%% %6.1f%% %6.1f%% %6.1f%% %6.1f%% %6.1f%% %6.1f%% %6.1f%% %6.1f%% %6.1f%% %6.1f%% %7.1f%%\n",
		"", "Pct Accessible", shares[0], shares[1], shares[2], shares[3], shares[4], shares[5], shares[6], shares[7], shares[8], shares[9], shares[10], TotalShare);




	
	fprintf (fp_rep, "\n\n\n\n\n%s journey Trip Length Frequency distributions:\n\n", PurposeLabels[Ini->PURPOSE_TO_PROCESS]);
	fprintf (fp_rep, "----------------------------------------------------------------------------------------------------------------------\n");
	fprintf (fp_rep, "FROM         TO            DA     SR2     SR3     SR4      WT      DT      WC      DC      TX      NM      SB    TOTAL\n");
	fprintf (fp_rep, "----------------------------------------------------------------------------------------------------------------------\n");
	for (i=1; i < Ini->TRIP_LENGTH_RANGES; i++)
		fprintf (fp_rep, "%-7d %7d %13d %7d %7d %7d %7d %7d %7d %7d %7d %7d %7d %8d\n",
			(i-1)*Ini->TRIP_LENGTH_WIDTH, (i)*Ini->TRIP_LENGTH_WIDTH, tlfreq[i][0], tlfreq[i][1], tlfreq[i][2], tlfreq[i][3], tlfreq[i][4], tlfreq[i][5], tlfreq[i][6], tlfreq[i][7], tlfreq[i][8], tlfreq[i][9], tlfreq[i][10],
			tlfreq[i][0] + tlfreq[i][1] + tlfreq[i][2] + tlfreq[i][3] + tlfreq[i][4] + tlfreq[i][5] + tlfreq[i][6] + tlfreq[i][7] + tlfreq[i][8] + tlfreq[i][9] + tlfreq[i][10]);
	i = Ini->TRIP_LENGTH_RANGES;
	fprintf (fp_rep, "%-7d %7s %13d %7d %7d %7d %7d %7d %7d %7d %7d %7d %7d %8d\n",
		i*Ini->TRIP_LENGTH_WIDTH, "plus", tlfreq[i][0], tlfreq[i][1], tlfreq[i][2], tlfreq[i][3], tlfreq[i][4], tlfreq[i][5], tlfreq[i][6], tlfreq[i][7], tlfreq[i][8], tlfreq[i][9], tlfreq[i][10],
		tlfreq[i][0] + tlfreq[i][1] + tlfreq[i][2] + tlfreq[i][3] + tlfreq[i][4] + tlfreq[i][5] + tlfreq[i][6] + tlfreq[i][7] + tlfreq[i][8] + tlfreq[i][9] + tlfreq[i][10]);
	fprintf (fp_rep, "----------------------------------------------------------------------------------------------------------------------\n");
	fprintf (fp_rep, "%-7s %7s %13d %7d %7d %7d %7d %7d %7d %7d %7d %7d %7d %8d\n",
		"", "Total", tlfreq[0][0], tlfreq[0][1], tlfreq[0][2], tlfreq[0][3], tlfreq[0][4], tlfreq[0][5], tlfreq[0][6], tlfreq[0][7], tlfreq[0][8], tlfreq[0][9], tlfreq[0][10],
		tlfreq[0][0] + tlfreq[0][1] + tlfreq[0][2] + tlfreq[0][3] + tlfreq[0][4] + tlfreq[0][5] + tlfreq[0][6] + tlfreq[0][7] + tlfreq[0][8] + tlfreq[0][9] + tlfreq[0][10]);
	fprintf (fp_rep, "----------------------------------------------------------------------------------------------------------------------\n");
	TotalDist = 0.0;
	for (i=0; i < Ini->NUMBER_ALTS; i++) {
		if (tlfreq[0][i] > 0.0) {
			shares[i] = tldist[i]/tlfreq[0][i];
			TotalDist += tldist[i];
		}
		else
			shares[i] = 0.0;
	}
	TotalDist /= Ini->NUMBER_JOURNEYS;
	fprintf (fp_rep, "%-17s %3s %7.1f %7.1f %7.1f %7.1f %7.1f %7.1f %7.1f %7.1f %7.1f %7.1f %7.1f %8.1f\n",
		"Mean Trip Length", "", shares[0], shares[1], shares[2], shares[3], shares[4], shares[5], shares[6],
		shares[7], shares[8], shares[9], shares[10], TotalDist);




	
	if (strcmp(Ini->CO_CO_REPORTFILE, "") || strcmp(Ini->SUBAREA_CO_CO_REPORTFILE, "")) {
		fprintf (fp_rep2, "\n\n\n\n\n%s Journey District to District by Mode Frequency Distributions:\n\n", PurposeLabels[Ini->PURPOSE_TO_PROCESS]);
		fprintf (fp_rep2, "-----------------------------------------------\n");
		fprintf (fp_rep2, "ORIG DISTRICT   DEST DISTRICT   MODE   JOURNEYS\n");
		fprintf (fp_rep2, "-----------------------------------------------\n");
		TotalDist = 0.0;
		for (m=1; m <= Ini->NUMBER_ALTS; m++) {
			for (i=1; i <= Ini->NUMBER_BPMDIST1; i++) {
				for (j=1; j <= Ini->NUMBER_BPMDIST1; j++) {
					fprintf (fp_rep2, "%13d %15d %6d %10d\n", i-1, j-1, m, dist2dist[m][i][j]);
					TotalDist += dist2dist[0][i][j];
				}
			}
		}
		fprintf (fp_rep2, "------------------------------------------------\n");
		fprintf (fp_rep2, "%13s %15s %6s %10.0f\n", "Total", "", "", TotalDist);
		fflush (fp_rep2);
		fclose (fp_rep2);
	}

	


	fprintf (fp_rep, "\n\n\n\n\n%s Journey District to District by Mode Frequency Distributions:\n\n", PurposeLabels[Ini->PURPOSE_TO_PROCESS]);
	fprintf (fp_rep, "----------------------------------------------\n");
	fprintf (fp_rep, "ORIG DISTRICT   DEST DISTRICT   TOTAL JOURNEYS\n");
	fprintf (fp_rep, "----------------------------------------------\n");
	TotalDist = 0.0;
	for (i=1; i <= Ini->NUMBER_BPMDIST1; i++) {
		for (j=1; j <= Ini->NUMBER_BPMDIST1; j++) {
			fprintf (fp_rep, "%13d %15d %16d\n", i-1, j-1, dist2dist[0][i][j]);
			TotalDist += dist2dist[0][i][j];
		}
	}
	fprintf (fp_rep, "----------------------------------------------\n");
	fprintf (fp_rep, "%13s %15s %16.0f\n", "Total", "", TotalDist);



	
	printf ("\n\n\n\n%d journeys were selected by pre-mode choice to be non-motorized,\n", unmet_count);
	printf (" but no non-motorized attraction locations were available.\n\n");
	fprintf (fp_rep, "\n\n%d journeys were selected by pre-mode choice to be non-motorized,\n", unmet_count);
	fprintf (fp_rep, " but no non-motorized attraction locations were available.\n\n");

	finish = clock();
	printf ("total MDC execution time:  %.2lf hours.\n\n\n", (((double)finish-start)/CLOCKS_PER_SEC)/3600.0);
	fprintf (fp_rep, "\ntotal execution MDC time:  %.2lf hours.\n\n", (((double)finish-start)/CLOCKS_PER_SEC)/3600.0);



	HeapFree (heapHandle, 0, co_attrs);
	HeapFree (heapHandle, 0, co_zones);
//	relRAM ("final_reports", (Ini->NUMBER_BPMDIST1+1)*sizeof(float) + (Ini->NUMBER_BPMDIST1+1)*sizeof(int));
}