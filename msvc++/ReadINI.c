#include "GetFromINI.h"
#include "md.h"

/****
      Rules for .ini files:
		The left bracket of [SECTION] must always begin in character position 1.
		By convention, SECTION names are uppercase.
		By convention, SECTIONs are separated by at least one blank line.
		There may be only one 'String=Value' pair per line.
		'String=Value' pairs must have an '=' seoparate the Value from the String.
		Strings and values may not contain blanks or ';'.  Any other character ok.
		Strings and values are case sensitive.
		Any text to the right of a ';' is considered a comment and is ignored.
		A ';' can appear in any character position.
****/


/****
      Return Values:
		GetIntFromINI returns (int)0xffffffff if requested String= not found in .ini file.
		GetFloatFromINI returns (float)0xffffffff if requested String= not found in .ini file.
		GetDoubleFromINI returns (double)0xffffffff if requested String= not found in .ini file.
		GetStringFromINI returns NULL if requested String= not found in .ini file.
****/


int GetIntFromINI (FILE *fp, char *Section, char *String)
{
	char InputLine[LRECL];
	char TempString[LRECL];
	char Value[LRECL];
	int i, n;

	rewind (fp);

	while ((fgets(InputLine, LRECL, fp)) != NULL) {
		if (InputLine[0] == '[') {
			// start of a section; see if it's the section we're looking for.
			i = 1;
			n = 0;
			while (InputLine[i] != ']')
				TempString[n++] = InputLine[i++];
			TempString[n] = '\0';
		}
		if (!strcmp(TempString, Section)) {
			// start of the section we're looking for, so read the string on LHS of '='.
			while ((fgets(InputLine, LRECL, fp)) != NULL) {
				if (InputLine[0] == '[')
					// start of next section.  didn't find String, so return -1;
					return (0xffffffff);
				else {
					// determine variable name.
					i = 0;
					n = 0;
					while (InputLine[i] != '=' && InputLine[i] != ';' && InputLine[i] != '\n') {
						if (InputLine[i] != ' ' && InputLine[i] != '\t')		// white space is ignored
							TempString[n++] = InputLine[i];
						i++;
					}
					TempString[n] = '\0';
					if (!strcmp(TempString, String)) {
						// it's the string we're looking for, so find position of character following equal sign.
						while (InputLine[i] != '=')
							i++;
						i++;
						// move to character position of first non-blank after equal sign.
						while (InputLine[i] == ' ')
							i++;
						// determine variable's value.
						n = 0;
						while (InputLine[i] != ' ' && InputLine[i] != ';' && InputLine[i] != '\n')
							Value[n++] = InputLine[i++];
						Value[n] = '\0';
						return ((int) atoi (Value));
					}
				}
			}
		}
	}
	return (0xffffffff);
}


float GetFloatFromINI (FILE *fp, char *Section, char *String)
{
	char InputLine[LRECL];
	char TempString[LRECL];
	char Value[LRECL];
	int i, n;

	rewind (fp);

	while ((fgets(InputLine, LRECL, fp)) != NULL) {
		if (InputLine[0] == '[') {
			// start of a section; see if it's the section we're looking for.
			i = 1;
			n = 0;
			while (InputLine[i] != ']')
				TempString[n++] = InputLine[i++];
			TempString[n] = '\0';
		}
		if (!strcmp(TempString, Section)) {
			// start of the section we're looking for, so read the string on LHS of '='.
			while ((fgets(InputLine, LRECL, fp)) != NULL) {
				if (InputLine[0] == '[')
					// start of next section.  didn't find String, so return -1;
					return ((float)0xffffffff);
				else {
					// determine variable name.
					i = 0;
					n = 0;
					while (InputLine[i] != '=' && InputLine[i] != ';' && InputLine[i] != '\n') {
						if (InputLine[i] != ' ' && InputLine[i] != '\t')		// white space is ignored
							TempString[n++] = InputLine[i];
						i++;
					}
					TempString[n] = '\0';
					if (!strcmp(TempString, String)) {
						// it's the string we're looking for, so find position of character following equal sign.
						while (InputLine[i] != '=')
							i++;
						i++;
						// move to character position of first non-blank after equal sign.
						while (InputLine[i] == ' ')
							i++;
						// determine variable's value.
						n = 0;
						while (InputLine[i] != ' ' && InputLine[i] != ';' && InputLine[i] != '\n')
							Value[n++] = InputLine[i++];
						Value[n] = '\0';
						return ((float) atof (Value));
					}
				}
			}
		}
	}
	return ((float)0xffffffff);
}


double GetDoubleFromINI (FILE *fp, char *Section, char *String)
{
	char InputLine[LRECL];
	char TempString[LRECL];
	char Value[LRECL];
	int i, n;

	rewind (fp);

	while ((fgets(InputLine, LRECL, fp)) != NULL) {
		if (InputLine[0] == '[') {
			// start of a section; see if it's the section we're looking for.
			i = 1;
			n = 0;
			while (InputLine[i] != ']')
				TempString[n++] = InputLine[i++];
			TempString[n] = '\0';
		}
		if (!strcmp(TempString, Section)) {
			// start of the section we're looking for, so read the string on LHS of '='.
			while ((fgets(InputLine, LRECL, fp)) != NULL) {
				if (InputLine[0] == '[')
					// start of next section.  didn't find String, so return -1;
					return ((double)0xffffffff);
				else {
					// determine variable name.
					i = 0;
					n = 0;
					while (InputLine[i] != '=' && InputLine[i] != ';' && InputLine[i] != '\n') {
						if (InputLine[i] != ' ' && InputLine[i] != '\t')		// white space is ignored
							TempString[n++] = InputLine[i];
						i++;
					}
					TempString[n] = '\0';
					if (!strcmp(TempString, String)) {
						// it's the string we're looking for, so find position of character following equal sign.
						while (InputLine[i] != '=')
							i++;
						i++;
						// move to character position of first non-blank after equal sign.
						while (InputLine[i] == ' ')
							i++;
						// determine variable's value.
						n = 0;
						while (InputLine[i] != ' ' && InputLine[i] != ';' && InputLine[i] != '\n')
							Value[n++] = InputLine[i++];
						Value[n] = '\0';
						return ((double) atof (Value));
					}
				}
			}
		}
	}
	return ((double)0xffffffff);
}


char *GetStringFromINI (FILE *fp, char *Section, char *String)
{
	char InputLine[LRECL];
	char *Value;
	char TempString[LRECL];
	int i, n;

	rewind (fp);

	while ((fgets(InputLine, LRECL, fp)) != NULL) {
		if (InputLine[0] == '[') {
			// start of a section; see if it's the section we're looking for.
			i = 1;
			n = 0;
			while (InputLine[i] != ']')
				TempString[n++] = InputLine[i++];
			TempString[n] = '\0';
		}
		if (!strcmp(TempString, Section)) {
			// start of the section we're looking for, so read the string on LHS of '='.
			while ((fgets(InputLine, LRECL, fp)) != NULL) {
				if (InputLine[0] == '[')
					// start of next section.  didn't find String, so return -1;
					return (NULL);
				else {
					// determine variable name.
					i = 0;
					n = 0;
					while (InputLine[i] != '=' && InputLine[i] != ';' && InputLine[i] != '\n') {
						if (InputLine[i] != ' ' && InputLine[i] != '\t')		// white space is ignored
							TempString[n++] = InputLine[i];
						i++;
					}
					TempString[n] = '\0';
					if (!strcmp(TempString, String)) {
						// it's the string we're looking for, so find position of character following equal sign.
						while (InputLine[i] != '=')
							i++;
						i++;
						// move to character position of first non-blank after equal sign.
						while (InputLine[i] == ' ')
							i++;
						// determine the number of character positions in the variable's value.
						n = 0;
						while (InputLine[i] != ' ' && InputLine[i] != ';' && InputLine[i] != '\t' && InputLine[i] != '\n') {
							n++;
							i++;
						}
						Value = (char *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (n+1)*sizeof(char));
//						addRAM ("GetStringFromINI", (n+1)*sizeof(char));
						// determine variable's value.
						i -= n;
						n = 0;
						while (InputLine[i] != ' ' && InputLine[i] != ';' && InputLine[i] != '\t' && InputLine[i] != '\n')
							Value[n++] = InputLine[i++];
						Value[n] = '\0';
						return (Value);
					}
				}
			}
		}
	}
	return (NULL);
}
