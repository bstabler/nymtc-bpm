// Data structures and function prototypes exported by the CaliperMTX dll

#if !defined(MATRIX_H)
#define MATRIX_H

//typedef short BOOL;
typedef int BOOL;
typedef struct _MATRIX *MATRIX;

typedef enum _MATRIX_DIM
{
    MATRIX_ROW, 
    MATRIX_COL

} MATRIX_DIM;


// Type of matrix cache.
typedef enum _MAT_CACHE_TYPE
{
    SERIAL_CACHE, RANDOM_CACHE
} MAT_CACHE_TYPE;

// Indicates whether the matrix cache is active or not, and whether it is used
// for all the cores in the matrix or only one
typedef enum _MAT_CACHE_APPLY
{
    CACHE_NONE, CACHE_ONE, CACHE_ALL
} MAT_CACHE_APPLY;


enum _CONTROL_TYPE
{
    CONTROL_FALSE,      
    CONTROL_TRUE,       
    CONTROL_NEVER,      
    CONTROL_ALWAYS,     
    CONTROL_AUTOMATIC   
} ; typedef char CONTROL_TYPE;

#define IsControl(c)    (c == CONTROL_ALWAYS || c == CONTROL_TRUE)
typedef double (*MATRIX_FILLFUNC)(short iCore, long iRow, long iCol,
    void *pFillData);


// Maximum size of a matrix label
#define _MAX_FLABEL 64

// Maximum size of a matrix file name
#define _MAX_PATH   260


// Data types
enum _data_type
{
    UNKNOWN_TYPE,
    SHORT_TYPE,
    LONG_TYPE,
    FLOAT_TYPE,
    DOUBLE_TYPE
} ; typedef char DATA_TYPE;


#if !defined(MATRIX_P)
#define MATRIX_P
    

#if defined(__cplusplus)
extern "C" {
#endif 


// Loads the matrix dll into memory
void    InitMatDLL(int *program_tc_status);

// Decrements the matrix reference count and closes the matrix if the 
//      reference count is zero
short   MATRIX_Done(MATRIX  hMatrix);

//  Read a matrix from a file.
//  The last argument can be used to force the loading mode of a dense matrix.
//  If it is passed as CONTROL_AUTOMATIC, the mode is read from the file.
//  If the stored file was created in memory, it will be read into memory.
//  A file-based matrix remains in the file (only the header is read).
MATRIX  MATRIX_LoadFromFile(char *szFileName, CONTROL_TYPE FileBased);
    
// Returns the matrix file name.
void    MATRIX_GetFileName(MATRIX  hMatrix, char *szFileName);

//  Returns the number of cores in the matrix.
short   MATRIX_GetNCores(MATRIX  hMatrix);
    
//  Returns the number of rows in the matrix.
long    MATRIX_GetNRows(MATRIX  hMatrix);
    
//  Returns the number of columns in the matrix.
long    MATRIX_GetNCols(MATRIX  hMatrix);
    
//  Returns the number of rows in the matrix core.
long    MATRIX_GetBaseNRows(MATRIX hMatrix);
    
//  Returns an array of index IDs for the matrix.
short   MATRIX_GetIDs(MATRIX hMatrix, MATRIX_DIM dim,  long *ids);

//  Returns the number of columns in the matrix core.
long    MATRIX_GetBaseNCols(MATRIX hMatrix);
    
//  Returns the data type of the elements in the matrix.
DATA_TYPE   MATRIX_GetDataType(MATRIX hMatrix);
    
//  Tests whether the matrix is in column-major order.
int    MATRIX_IsColMajor(MATRIX hMatrix);
    
//  Sets the internal currency to the specified core.
short MATRIX_SetCore(MATRIX hMatrix, short iCore);
    
//  Gets the index of the current core.
short MATRIX_GetCore(MATRIX hMatrix);
    
//  Fills p with the value of the cell at (idRow, idCol) converted to Type.
short MATRIX_GetElement(MATRIX hMatrix, long idRow, long idCol, DATA_TYPE Type, void *p);
    
//  Reads a row or column of a matrix using the base index.
//  Arguments:
//      iPos    -   the core position of the row or the column.
//      dim     -   dimension to extract: MATRIX_ROW or MATRIX_COL.
//      Type    -   Requested data type of returned values.
//      Array   -   Previously allocated array for receiving values.
short MATRIX_GetBaseVector(MATRIX hMatrix, long iPos, MATRIX_DIM dim, DATA_TYPE Type, void *Array);

short MATRIX_GetVector(MATRIX hMatrix, long ID, MATRIX_DIM dim, DATA_TYPE Type, void *Array);
//  Reads a row or column of a matrix using the current index.
//      ID      -   the identifier of the row or the column.
//      dim     -   dimension to extract: MATRIX_ROW or MATRIX_COL.
//      Type    -   Requested data type of returned values.
//      Array   -   Previously allocated array for receiving values.

//  Sets an element of the matrix 
short MATRIX_SetElement(MATRIX hMatrix, long idRow, long idCol, DATA_TYPE Type, void *p);
 
//  Sets a row or column of a matrix using the base index.
//  Arguments:
//      iPos    -   the core position of the row or the column.
//      dim     -   dimension to set: MATRIX_ROW or MATRIX_COL.
//      Type    -   data type of provided values.
//      Array   -   array of values to store.
short MATRIX_SetBaseVector(MATRIX hMatrix, long iPos, MATRIX_DIM dim, DATA_TYPE Type, void *Array);
    
//  Sets a row or a column  of a matrix using the current index.
//  Arguments:
//      ID      -   the identifier of the row or the column.
//      dim     -   dimension to set: MATRIX_ROW or MATRIX_COL.
//      Type    -   data type of provided values.
//      Array   -   array of values to store.
short MATRIX_SetVector(MATRIX hMatrix, long ID, MATRIX_DIM dim, DATA_TYPE Type, void *Array);

// Returns the position of the current index.
short MATRIX_GetCurrentIndexPos(MATRIX hMatrix, MATRIX_DIM dim);

//  Sets the current matrix index for the the requested dimension.
short MATRIX_SetIndex(MATRIX hMatrix, MATRIX_DIM dim, short iIdx);
    
//  For file-based matrices - creates a cache, not greater than nSize bytes.
//  Type is either SERIAL_CACHE or RANDOM_CACHE.
//  The serial cache is only useful for a sequential read of the
//      matrix, in natural (base) order.
//  The random cache is only useful for random access where each element
//      is accessed several times.
//  Apply is either CACHE_ONE or CACHE_ALL.  In case of CACHE_ONE - the
//      cache is created for the current core, and every time the core is
//      changed (via MATRIX_SetCore), the old cache is destroyed and a
//      new one is created.  For CACHE_ALL - a separate cache is created
//      for each core, the actual size determined by the number of cores.
//  Note: If called a second time without an call to MATRIX_DestroyCache,
//      the second call is ignored and the status of TC_INVINPUT is returned.
short MATRIX_CreateCache(MATRIX hMatrix, MAT_CACHE_TYPE Type, MAT_CACHE_APPLY apply, long nSize);

// Destroys a matrix cache.
void    MATRIX_DestroyCache(MATRIX hMatrix);
    
//  Disables the use of the cache.  
short MATRIX_DisableCache(MATRIX hMatrix);
    
//  Enables the cache.  
short MATRIX_EnableCache(MATRIX hMatrix);

//  Reads back the cache buffer(s) for a matrix.
short MATRIX_RefreshCache(MATRIX hMatrix);

//  Fills szLabel with the label of the specified core.
void    MATRIX_GetLabel(MATRIX _this, short iCore, char *szLabel);

//  Tests whether the matrix is in sparse representation.
BOOL    MATRIX_IsSparse(MATRIX _this);

//  Tests whether the matrix is in file-based.
BOOL    MATRIX_IsFileBased(MATRIX hMatrix);

// Increases the reference count of the matrix. This call should be used to increase access speed.
// It must be called in pair with MATRIX_CloseFile.
short MATRIX_OpenFile(MATRIX hMatrix, BOOL fRead);

// Decreases the reference count of the matrix.
short MATRIX_CloseFile(MATRIX hMatrix);

// Clears the contents of the matrix.
short MATRIX_Clear(MATRIX hMatrix);

// Checks whether the matrix is read only.
BOOL MATRIX_IsReadOnly(MATRIX _this);

//  Saves the matrix into a named file.  Do not call for file-based matrices.
short   MATRIX_SaveToFile(MATRIX hMatrix, char *szFileName);


#if defined(__cplusplus)
}
#endif

#endif  //  !defined(MATRIX_P)

#endif  //  !defined(MATRIX_H)
