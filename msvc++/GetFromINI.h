#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/****
      Rules for .ini files:
		The left bracket of [SECTION] must always begin in character position 1.
		By convention, SECTION names are uppercase.
		By convention, SECTIONs are separated by at least on blank line.
		There may be only one 'String=Value' pair per line.
		'String=Value' pairs must have an '=' seoparate the Value form String.
		Strings and values may not contain blanks or ';'.  Any other character ok.
		Any text to the right of a ';' is considered a comment.
		A ';' can appear in any character position.
****/


/****
      Return Values:
		GetIntFromINI returns (int)0xffffffff if requested String= not found in .ini file.
		GetFloatFromINI returns (float)0xffffffff if requested String= not found in .ini file.
		GetDoubleFromINI returns (double)0xffffffff if requested String= not found in .ini file.
		GetStringFromINI returns NULL if requested String= not found in .ini file.
****/


#define LRECL 1024

int    GetIntFromINI    (FILE *, char *, char *);
float  GetFloatFromINI  (FILE *, char *, char *);
double GetDoubleFromINI (FILE *, char *, char *);
char  *GetStringFromINI (FILE *, char *, char *);
