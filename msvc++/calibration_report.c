#include "md.h"

void calibration_report (int iter, FILE *fp_cal, struct m_mc_asc_data mMcAscData, struct nm_mc_asc_data nmMcAscData, float *Mode_Obs,
						 float **m_cal_obs, float **m_cal_est, float *nm_cal_obs, float **nm_cal_est, float cal_scale) {

	int i, j;
	int **m_re;
	float *m_EstTots, *Mode_Est, **m_EstFreq, **nm_EstFreq;
	float m_TotalEst=0.0;
	double mean_error = 0.0;
	char temp1[1000], temp2[1000];


	m_re = (int **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (mMcAscData.mMcAscConstants->numValues+1)*sizeof(int *));
	for (i=0; i < mMcAscData.mMcAscConstants->numValues + 1; i++)
		m_re[i] = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_ALTS+1)*sizeof(int));

	m_EstFreq = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (mMcAscData.mMcAscConstants->numValues+1)*sizeof(float *));
	for (i=0; i < mMcAscData.mMcAscConstants->numValues + 1; i++)
		m_EstFreq[i] = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_ALTS+1)*sizeof(float));

	m_EstTots = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, mMcAscData.mMcAscConstants->numValues*sizeof(float));

	nm_EstFreq = (float **) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (nmMcAscData.nmMcAscConstants->numValues+1)*sizeof(float *));
	for (i=0; i < nmMcAscData.nmMcAscConstants->numValues + 1; i++)
		nm_EstFreq[i] = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, 2*sizeof(float));

	Mode_Est = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_ALTS+1)*sizeof(float));



	// sum up estimated motorized journeys to calculate mode shares by each motorized mode choice stratum
	for (i=0; i < mMcAscData.mMcAscConstants->numValues; i++) {
		for (j=0; j < Ini->NUMBER_ALTS; j++) {
			m_EstFreq[i][j] = m_cal_est[i][j];
			m_EstTots[i] += m_cal_est[i][j];
			m_TotalEst += m_cal_est[i][j];
			Mode_Est[j] += m_cal_est[i][j];
		}
	}

	// compute motorized mode shares and relative errors
	for (i=0; i < mMcAscData.mMcAscConstants->numValues; i++) {

		if (m_cal_obs[i][Ini->NUMBER_ALTS] > 0.0) {
			if (m_cal_est[i][Ini->NUMBER_ALTS] == 0.0)
				m_re[i][Ini->NUMBER_ALTS] = 0;
			else
				m_re[i][Ini->NUMBER_ALTS] = (int)(100.0*(m_cal_est[i][Ini->NUMBER_ALTS] - m_cal_obs[i][Ini->NUMBER_ALTS])/m_cal_obs[i][Ini->NUMBER_ALTS]);
		}

		for (j=0; j < Ini->NUMBER_ALTS; j++) {
			if (m_EstTots[i] > 0.0)
				m_cal_est[i][j] /= m_EstTots[i];
			if (m_cal_obs[i][j] > 0.0) {
				if (m_cal_est[i][j] == 0.0)
					m_re[i][j] = 0;
				else
					m_re[i][j] = (int)(100.0*(m_cal_est[i][j] - m_cal_obs[i][j])/m_cal_obs[i][j]);
			}

			mean_error += fabs(m_re[i][j])*m_EstFreq[i][j];
		}
	}
	mean_error /= m_TotalEst;
	



	// compute shares and relative errors for column margins of the table
	for (j=0; j < Ini->NUMBER_ALTS + 1; j++) {
		if (m_TotalEst > 0.0)
			Mode_Est[j] /= m_TotalEst;
		if (Mode_Obs[j] > 0.0) {
			if (Mode_Est[j] == 0.0)
				m_re[mMcAscData.mMcAscConstants->numValues][j] = 0;
			else
				m_re[mMcAscData.mMcAscConstants->numValues][j] = (int)(100.0*(Mode_Est[j] - Mode_Obs[j])/Mode_Obs[j]);
		}
	}



	// convert non-motorized journey frequency into shares by pre-mode choice stratum
	nm_EstFreq[nmMcAscData.nmMcAscConstants->numValues][0] = 0.0;
	nm_EstFreq[nmMcAscData.nmMcAscConstants->numValues][1] = 0.0;
	for (i=0; i < nmMcAscData.nmMcAscConstants->numValues; i++) {
		nm_EstFreq[nmMcAscData.nmMcAscConstants->numValues][0] += nm_cal_est[i][0];
		nm_EstFreq[nmMcAscData.nmMcAscConstants->numValues][1] += nm_cal_est[i][1];

		if (nm_cal_est[i][1] > 0.0)
			nm_cal_est[i][0] /= nm_cal_est[i][1];
	}




	// print report
	if (fp_cal != NULL) {		
		sprintf (temp1, "CALIBRATION ITERATION %d WITH %d JOURNEYS USING SCALE=%f", iter, Ini->NUMBER_JOURNEYS, cal_scale);
		for (i=0; i < (int)strlen(temp1); i++)
			temp2[i] = '-';
		temp2[i] = '\0';

		fprintf (fp_cal, "%s\n", temp2);
		fprintf (fp_cal, "%s\n", temp1);
		fprintf (fp_cal, "%s\n", temp2);

		fprintf (fp_cal, "\n%s journey MSC Values by motorized mode choice MSC Stratification:\n\n", PurposeLabels[Ini->PURPOSE_TO_PROCESS], iter);
		fprintf (fp_cal, "-----------------------------------------------------------------------------------------------------------------------------------------------\n");
		fprintf (fp_cal, "INDEX                 DESCRIPTION        DA       SR2       SR3       SR4        WT        DT        WC        DC        TX        NM        SB\n");
		fprintf (fp_cal, "-----------------------------------------------------------------------------------------------------------------------------------------------\n");
		for (i=0; i < mMcAscData.mMcAscConstants->numValues; i++) {
			fprintf (fp_cal, "%-5d %27s", mMcAscData.mMcAscConstants->indexValues[i], mMcAscData.mMcAscConstants->labels[i]);
			for (j=0; j < Ini->NUMBER_ALTS; j++) {
				if (mMcAscData.constants[i][j] != MISSING)
					fprintf (fp_cal, " %9.5f", mMcAscData.constants[i][j]);
				else
					fprintf (fp_cal, " %9s", "-99999");
			}
			fprintf (fp_cal, "\n");
		}
		fprintf (fp_cal, "-----------------------------------------------------------------------------------------------------------------------------------------------\n\n\n\n");


		fprintf (fp_cal, "\n%s journey observed shares (in %%) by motorized mode choice MSC Stratification:\n\n", PurposeLabels[Ini->PURPOSE_TO_PROCESS]);
		fprintf (fp_cal, "-----------------------------------------------------------------------------------------------------------------------------------------------\n");
		fprintf (fp_cal, "INDEX                 DESCRIPTION        DA       SR2       SR3       SR4        WT        DT        WC        DC        TX        NM        SB\n");
		fprintf (fp_cal, "-----------------------------------------------------------------------------------------------------------------------------------------------\n");
		for (i=0; i < mMcAscData.mMcAscConstants->numValues; i++) {
			fprintf (fp_cal, "%-5d %27s", mMcAscData.mMcAscConstants->indexValues[i], mMcAscData.mMcAscConstants->labels[i]);
			for (j=0; j < Ini->NUMBER_ALTS; j++)
				fprintf (fp_cal, " %9.1f", 100.0*m_cal_obs[i][j]);
			fprintf (fp_cal, "\n");
		}
		fprintf (fp_cal, "-----------------------------------------------------------------------------------------------------------------------------------------------\n");
		fprintf (fp_cal, "%-33s", "REGIONAL OBSERVED MODE SHARES");
		for (j=0; j < Ini->NUMBER_ALTS; j++)
			fprintf (fp_cal, " %9.1f", 100.0*Mode_Obs[j]);
		fprintf (fp_cal, "\n");
		fprintf (fp_cal, "-----------------------------------------------------------------------------------------------------------------------------------------------\n\n\n\n");


		fprintf (fp_cal, "\n%s journey estimated shares (in %%) by motorized mode choice MSC Stratification:\n\n", PurposeLabels[Ini->PURPOSE_TO_PROCESS]);
		fprintf (fp_cal, "-----------------------------------------------------------------------------------------------------------------------------------------------\n");
		fprintf (fp_cal, "INDEX                 DESCRIPTION        DA       SR2       SR3       SR4        WT        DT        WC        DC        TX        NM        SB\n");
		fprintf (fp_cal, "-----------------------------------------------------------------------------------------------------------------------------------------------\n");
		for (i=0; i < mMcAscData.mMcAscConstants->numValues; i++) {
			fprintf (fp_cal, "%-5d %27s", mMcAscData.mMcAscConstants->indexValues[i], mMcAscData.mMcAscConstants->labels[i]);
			for (j=0; j < Ini->NUMBER_ALTS; j++)
				fprintf (fp_cal, " %9.1f", 100.0*m_cal_est[i][j]);
			fprintf (fp_cal, "\n");
		}
		fprintf (fp_cal, "-----------------------------------------------------------------------------------------------------------------------------------------------\n");
		fprintf (fp_cal, "%-33s", "REGIONAL ESTIMATED MODE SHARES");
		for (j=0; j < Ini->NUMBER_ALTS; j++)
			fprintf (fp_cal, " %9.1f", 100.0*Mode_Est[j]);
		fprintf (fp_cal, "\n");
		fprintf (fp_cal, "-----------------------------------------------------------------------------------------------------------------------------------------------\n\n\n\n");


		fprintf (fp_cal, "\n%s journey relative differences (in %%) between observed and estimated by motorized mode choice MSC Stratification:\n\n", PurposeLabels[Ini->PURPOSE_TO_PROCESS]);
		fprintf (fp_cal, "-----------------------------------------------------------------------------------------------------------------------------------------------\n");
		fprintf (fp_cal, "INDEX                 DESCRIPTION        DA       SR2       SR3       SR4        WT        DT        WC        DC        TX        NM        SB\n");
		fprintf (fp_cal, "-----------------------------------------------------------------------------------------------------------------------------------------------\n");
		for (i=0; i < mMcAscData.mMcAscConstants->numValues; i++) {
			fprintf (fp_cal, "%-5d %27s", mMcAscData.mMcAscConstants->indexValues[i], mMcAscData.mMcAscConstants->labels[i]);
			for (j=0; j < Ini->NUMBER_ALTS; j++) {
				if (m_cal_obs[i][j] > 0.0)
					fprintf (fp_cal, " %9d", m_re[i][j]);
				else
					fprintf (fp_cal, " %9s", "+INF");
			}
			fprintf (fp_cal, "\n");
		}
		fprintf (fp_cal, "-----------------------------------------------------------------------------------------------------------------------------------------------\n");
		fprintf (fp_cal, "%-33s", "REGIONAL RELATIVE ERRORS");
		for (j=0; j < Ini->NUMBER_ALTS; j++)
			fprintf (fp_cal, " %9d", m_re[mMcAscData.mMcAscConstants->numValues][j]);
		fprintf (fp_cal, "\n");
		fprintf (fp_cal, "-----------------------------------------------------------------------------------------------------------------------------------------------\n");

		fprintf (fp_cal, "\nWeighted Mean Absolute Error= %10.2f%%:\n\n\n\n", mean_error);


		fprintf (fp_cal, "\n%s journey shares (in %%) by pre-mode choice MSC Stratification:\n\n", PurposeLabels[Ini->PURPOSE_TO_PROCESS]);
		fprintf (fp_cal, "-------------------------------------------------------------------------\n");
		fprintf (fp_cal, "INDEX                 DESCRIPTION       MSC    OBS NM    EST NM     RE NM\n");
		fprintf (fp_cal, "-------------------------------------------------------------------------\n");
		for (i=0; i < nmMcAscData.nmMcAscConstants->numValues; i++) {
			fprintf (fp_cal, "%-5d %27s %9.5f", mMcAscData.mMcAscConstants->indexValues[i], nmMcAscData.nmMcAscConstants->labels[i], nmMcAscData.constants[i]);
			fprintf (fp_cal, " %9.1f", 100.0*nm_cal_obs[i]);
			fprintf (fp_cal, " %9.1f", 100.0*nm_cal_est[i][0]);
			if (nm_cal_obs[i] > 0.0)
				fprintf (fp_cal, " %9.1f", 100.0*(nm_cal_est[i][0] - nm_cal_obs[i])/nm_cal_obs[i]);
			else
				fprintf (fp_cal, " %9s", "+INF");
			fprintf (fp_cal, "\n");
		}
		fprintf (fp_cal, "-------------------------------------------------------------------------\n");
		fprintf (fp_cal, "%-43s", "REGIONAL VALUES");
		fprintf (fp_cal, " %9.1f", 100.0*nm_cal_obs[nmMcAscData.nmMcAscConstants->numValues]);
		fprintf (fp_cal, " %9.1f", 100.0*nm_EstFreq[nmMcAscData.nmMcAscConstants->numValues][0]/nm_EstFreq[nmMcAscData.nmMcAscConstants->numValues][1]);
		if (nm_cal_obs[nmMcAscData.nmMcAscConstants->numValues] > 0.0)
			fprintf (fp_cal, " %9.1f", 100.0*(nm_EstFreq[nmMcAscData.nmMcAscConstants->numValues][0]/nm_EstFreq[nmMcAscData.nmMcAscConstants->numValues][1] - nm_cal_obs[nmMcAscData.nmMcAscConstants->numValues])/nm_cal_obs[nmMcAscData.nmMcAscConstants->numValues]);
		else
			fprintf (fp_cal, " %9s", "+INF");
		fprintf (fp_cal, "\n");
		fprintf (fp_cal, "-------------------------------------------------------------------------\n\n\n\n\n\n\n\n");
	}

	fflush (fp_cal);



	for (i=0; i < mMcAscData.mMcAscConstants->numValues + 1; i++) {
		HeapFree (heapHandle, 0, m_re[i]);
		HeapFree (heapHandle, 0, m_EstFreq[i]);
	}
	HeapFree (heapHandle, 0, m_re);
	HeapFree (heapHandle, 0, m_EstFreq);

	for (i=0; i < nmMcAscData.nmMcAscConstants->numValues + 1; i++)
		HeapFree (heapHandle, 0, nm_EstFreq[i]);
	HeapFree (heapHandle, 0, nm_EstFreq);

	HeapFree (heapHandle, 0, m_EstTots);
	HeapFree (heapHandle, 0, Mode_Est);

}
