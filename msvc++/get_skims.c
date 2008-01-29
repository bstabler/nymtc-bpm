/*
 *  Takes a TransCAD matrix and highway skims matrices as arguments from the calling routine.
 *  Reads the three TransCAD tables into and returns the full matrices.
 */
 
#include "CaliperMTX.h"
#include "md.h"


void GetSkims (char *label, int Count, int TotCount, short core, MATRIX hMatrix, int Index, float **SkimData)
{
//	int i, j, k, temp_len;
	int i, j;
	int nrows, ncols, tazID;
	float *RowData;
//	char temp[120];

//	if (Ini->RAM_DEBUG_LEVEL == 2)
//		addRAM ("GetSkims 0", 0);

	// get the dimensions of the TransCAD matrix tables.
	nrows = MATRIX_GetNRows(hMatrix);

//	if (Ini->RAM_DEBUG_LEVEL == 2)
//		addRAM ("GetSkims 1", 0);

	ncols = MATRIX_GetNCols(hMatrix);
	
//	if (Ini->RAM_DEBUG_LEVEL == 2)
//		addRAM ("GetSkims 2", 0);

	RowData = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, ncols*sizeof(float));
//	if (Ini->RAM_DEBUG_LEVEL == 2)
//		addRAM ("GetSkims", ncols*sizeof(float));

// Initialize arrays
	for (i=0; i <= nrows; i++)
		for (j=0; j <= ncols; j++)
			SkimData[i][j] = 0.0;

// Read skims data from TransCAD native format (.mtx file) matrix file.
	MATRIX_SetCore (hMatrix, core);
	MATRIX_CreateCache(hMatrix, SERIAL_CACHE, CACHE_ALL, Ini->CACHE_SIZE);
	for (i=1; i <= nrows; i++) { 
		// The TransCAD tables are referenced by TAZ ID.
		// The row index is determined by incrementing here by HWY_INDEX_OFFSET.
		tazID = i + Index;

		//MATRIX_GetVector (hMatrix, tazID, MATRIX_IsColMajor(hMatrix), MATRIX_GetDataType(hMatrix), RowData);
		MATRIX_GetVector (hMatrix, tazID, MATRIX_ROW, FLOAT_TYPE, RowData);
		
		for (j=0; j < ncols; j++)
			SkimData[i][j+1] = RowData[j];

//		sprintf (temp, "reading %s skims %3d%% complete", label, 100*(Count+i)/TotCount);
//		temp_len = (int)strlen(temp);
//		for (k=0; k < temp_len; k++)
//			sprintf (&temp[temp_len + k], "\b");
//		printf ("%s", temp);
	}
	MATRIX_DestroyCache(hMatrix);

	HeapFree (heapHandle, 0, RowData);
//	if (Ini->RAM_DEBUG_LEVEL == 2)
//		relRAM ("GetSkims", ncols*sizeof(float));
}



