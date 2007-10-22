// Functions and data stuctures exported by the CaliperMTX dll

#if !defined(MATRIX_H)
#define MATRIX_H

typedef short BOOL;
typedef struct _MATRIX *MATRIX;



typedef enum _AGGR_TYPE
{
    AGGR_NONE,
    AGGR_SUM,
    AGGR_MEAN,
    AGGR_MIN,
    AGGR_MAX,
    AGGR_COUNT

} AGGR_TYPE;

typedef enum _MATRIX_DIM
{
    MATRIX_ROW, 
    MATRIX_COL

} MATRIX_DIM;

typedef enum _MAT_CACHE_TYPE
{
    SERIAL_CACHE, RANDOM_CACHE
} MAT_CACHE_TYPE;

typedef enum _MAT_CACHE_APPLY
{
    CACHE_NONE, CACHE_ONE, CACHE_ALL
} MAT_CACHE_APPLY;

// enum is 1 byte in WATCOM but 4 bytes in MSVC. This causes problem
// in direct IO using fread/fwrite.  Here is a minimum cost work
// arround. (Qi)

// typedef enum _CONTROL_TYPE
enum _CONTROL_TYPE
{
    CONTROL_FALSE,      //  FALSE now, may be changed
    CONTROL_TRUE,       //  TRUE now, may be changed
    CONTROL_NEVER,      //  FALSE, forever
    CONTROL_ALWAYS,     //  TRUE, forever
    CONTROL_AUTOMATIC   //  Use your own logic or default
} ; typedef char CONTROL_TYPE;

#define IsControl(c)    (c == CONTROL_ALWAYS || c == CONTROL_TRUE)
typedef double (*MATRIX_FILLFUNC)(short iCore, long iRow, long iCol,
    void *pFillData);

// maximum file label size

#define _MAX_FLABEL 64
#define _MAX_PATH   260


// typedef enum _data_type
enum _data_type
{
    UNKNOWN_TYPE,
    SHORT_TYPE,
    LONG_TYPE,
    FLOAT_TYPE,
    DOUBLE_TYPE,
    STRING_TYPE,
    ARRAY_TYPE, 
    SPECIAL_TYPE,
    DATE_TYPE     // RF: Not generally supported
} ; typedef char DATA_TYPE;


//  structure for passing information to functions that create a new matrix
//  unspecified elements inherit from other sources
typedef struct _MAT_INFO
{
    char            szFileName[_MAX_PATH];  //  "" = unspecified
    char            szLabel[_MAX_FLABEL];   //  "" = unspecified
    DATA_TYPE       Type;                   //  UNKNOWN_TYPE = unspecified
    CONTROL_TYPE    Sparse;                 //  |
    CONTROL_TYPE    ColMajor;               //  | CONTROL_AUTOMATIC = unspecified
    CONTROL_TYPE    FileBased;              //  |
} MAT_INFO;


typedef struct _MAT_HEADER_INFO
{
    char            szLabel[_MAX_FLABEL];
    short           nCores;         //  number of cores
    long            nRows, nCols;   //  matrix base dimensions
    DATA_TYPE       Type;           //  data type of matrix elements
    CONTROL_TYPE    Sparse;         //  sparse storage flag
    CONTROL_TYPE    ColMajor;       //  column major flag
    CONTROL_TYPE    FileBased;      //  file based flag
    short           nIndices;       //  number of indices
} MAT_HEADER_INFO;



#if !defined(MATRIX_P)
#define MATRIX_P
    

// The following functions are declared in matrix.def

#if defined(__cplusplus)
extern "C" {
#endif 

// function statuses are defined in Status.h
// #define TC_OKAY      0     // normal return, okay

// Returns TC_OKAY if matrix dll has been loaded.
void    InitMatDLL(int *program_tc_status);

short   MATRIX_Done(MATRIX  hMatrix);
    //  Decrement reference count.  If 0, destroy.
        
MATRIX  MATRIX_LoadFromFile(char *szFileName, CONTROL_TYPE FileBased);
    //  Read a matrix from file.
    //  The last argument can be used to force the loading mode of a dense matrix.
    //  If it is passed as CONTROL_AUTOMATIC, the mode is read from the file.
    //  If the stored file was created in memory, it will be read into memory.
    //  A file-based matrix remains in the file (only the header is read).
    
void    MATRIX_GetFileName(MATRIX  hMatrix, char *szFileName);

short   MATRIX_GetNCores(MATRIX  hMatrix);
    //  Returns the number of cores in the matrix.
    
long    MATRIX_GetNRows(MATRIX  hMatrix);
    //  Returns the number of rows in the matrix.
    
long    MATRIX_GetNCols(MATRIX  hMatrix);
    //  Returns the number of columns in the matrix.
    
long    MATRIX_GetBaseNRows(MATRIX hMatrix);
    //  Returns the number of rows in the matrix core.
    
short   MATRIX_GetIDs(MATRIX hMatrix, MATRIX_DIM dim,  long *ids);
    //  Returns an array or index IDs for the matrix.

long    MATRIX_GetBaseNCols(MATRIX hMatrix);
    //  Returns the number of columns in the matrix core.
    
DATA_TYPE   MATRIX_GetDataType(MATRIX hMatrix);
    //  Returns the data type of elements in the matrix.
    
int    MATRIX_IsColMajor(MATRIX hMatrix);
    //  Tests whether the matrix is in column-major order.
    
short MATRIX_SetCore(MATRIX hMatrix, short iCore);
    //  Sets the internal curency to the specified core.
    
short MATRIX_GetCore(MATRIX hMatrix);
    //  Get the index of the current core.
    
short MATRIX_GetElement(MATRIX hMatrix, long idRow, long idCol, DATA_TYPE Type,
    void *p);
    //  Fills p with the value of the cell at (irRow, idCol) converted to Type.
    //  idCol and idRow are lookup values for the corresponding index.
    
short MATRIX_GetBaseVector(MATRIX hMatrix, long iPos, MATRIX_DIM dim,
    DATA_TYPE Type, void *Array);
    //  Fill in an array with a core row or a column.
    //  Arguments:
    //      iPos    -   the core position of the row or the column.
    //      dim     -   dimension to extract: MATRIX_ROW or MATRIX_COL.
    //      Type    -   Requested data type of returned values.
    //      Array   -   Previously allocated array for receiving values.

short MATRIX_GetVector(MATRIX hMatrix, long ID, MATRIX_DIM dim, DATA_TYPE Type,
    void *Array);
    //  Fill in an array with a row or a column.
    //  Arguments:
    //      ID      -   the identifier of the row or the column.
    //      dim     -   dimension to extract: MATRIX_ROW or MATRIX_COL.
    //      Type    -   Requested data type of returned values.
    //      Array   -   Previously allocated array for receiving values.

short MATRIX_SetElement(MATRIX hMatrix, long idRow, long idCol, DATA_TYPE Type,
    void *p);
    //  Set an element of the matrix (see MATRIX_GetElement).
    
short MATRIX_SetBaseVector(MATRIX hMatrix, long iPos, MATRIX_DIM dim,
    DATA_TYPE Type, void *Array);
    //  Fill a core row or a column from an array.
    //  Arguments:
    //      iPos    -   the core position of the row or the column.
    //      dim     -   dimension to set: MATRIX_ROW or MATRIX_COL.
    //      Type    -   data type of provided values.
    //      Array   -   array of values to store.
    
short MATRIX_SetVector(MATRIX hMatrix, long ID, MATRIX_DIM dim, DATA_TYPE Type,
    void *Array);
    //  Fill a row or a column from an array.
    //  Arguments:
    //      ID      -   the identifier of the row or the column.
    //      dim     -   dimension to set: MATRIX_ROW or MATRIX_COL.
    //      Type    -   data type of provided values.
    //      Array   -   array of values to store.

short MATRIX_GetCurrentIndexPos(MATRIX hMatrix, MATRIX_DIM dim);

short MATRIX_SetIndex(MATRIX hMatrix, MATRIX_DIM dim, short iIdx);
    //  Set the current index in the requested dimension to the 'iIdx'th.
    
short MATRIX_CreateCache(MATRIX hMatrix, MAT_CACHE_TYPE Type,
    MAT_CACHE_APPLY apply, long nSize);
    //  For file-based matrices - create a cache, not greater than nSize bytes.
    //  Type is either SERAIL_CACHE or RANDOM_CACHE.
    //  The serial cache is only usefull for a sequential read of the
    //      matrix, in natuaral (base) order.
    //  The random cache is only useful for random access where each element
    //      is accessed several times.  It is WORSE for simple read-through.
    //  apply is either CACHE_ONE or CACHE_ALL.  In case of CACHE_ONE - the
    //      cache is created for the current core, and every time the core is
    //      changed (via MATRIX_SetCore), the old cache is destroyed and a
    //      new one is created.  For CACHE_ALL - a separate cache is created
    //      for each core, the actual size determined by the number of cores.
    //  Note: If called a second time without an call to MATRIX_DestroyCache,
    //      the second call is ignored and returns TC_INVINPUT.
    
void    MATRIX_DestroyCache(MATRIX hMatrix);
    
short MATRIX_DisableCache(MATRIX hMatrix);
    //  Disable the use of the cache.  hMatrix is usefull if the file access
    //      method changes.  Call MATRIX_EnableCache to re-enable the cache.
    
short MATRIX_EnableCache(MATRIX hMatrix);
    //  Enable the cache.  Since the cache is enabled after creation, call
    //      only after disabling.

short MATRIX_RefreshCache(MATRIX hMatrix);
    //  Read back the cache buffer(s) for a matrix.

void    MATRIX_GetLabel(MATRIX this, short iCore, char *szLabel);
    //  Fill szLabel with the label of the specified core.

BOOL    MATRIX_IsSparse(MATRIX this);
    //  Tests whether the matrix is in sparse representation.

BOOL    MATRIX_IsFileBased(MATRIX this);
    //  Tests whether the matrix is in file-based.

#if defined(__cplusplus)
}
#endif

#endif  //  !defined(MATRIX_P)

#endif  //  !defined(MATRIX_H)
