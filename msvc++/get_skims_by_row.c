/*
 *  Takes a TransCAD matrix, highway skims matrices and row number as arguments from the calling routine.
 *  Reads the three TransCAD tables into an array and returns the a row.
 */
 
#include "CaliperMTX.h"
#include "md.h"


void GetSkimsByRow (int rowNo, short core, MATRIX hMatrix, int Index, float *SkimRow)
{
	int j;
	int nrows, ncols, tazID;
	float *RowData;

	// get the dimensions of the TransCAD matrix tables.
	nrows = MATRIX_GetNRows(hMatrix);
	ncols = MATRIX_GetNCols(hMatrix);

	RowData = (float *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, ncols*sizeof(float));

	// Read skims data from TransCAD native format (.mtx file) matrix file.
	MATRIX_SetCore (hMatrix, core);
	MATRIX_CreateCache(hMatrix, SERIAL_CACHE, CACHE_ALL, Ini->CACHE_SIZE);

	// The TransCAD tables are referenced by TAZ ID.
	// The row index is determined by incrementing here by HWY_INDEX_OFFSET.
	tazID = rowNo + Index;
	MATRIX_GetVector (hMatrix, tazID, MATRIX_IsColMajor(hMatrix), MATRIX_GetDataType(hMatrix), RowData);
	for (j=0; j < ncols; j++)
		SkimRow[j+1] = RowData[j];

	MATRIX_DestroyCache(hMatrix);

	HeapFree (heapHandle, 0, RowData);
}
