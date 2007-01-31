#include "md.h"

int ValidStop (int idist, int jdist, int kdist)
{


	// Rule set 1 Intra
	if ((idist == 11 || idist == 13 || (idist >=16 && idist <= 30)) &&
		(jdist == 11 || jdist == 13 || (jdist >=16 && jdist <= 30))) {
		
			if ((kdist >= 0 && kdist <= 10) || kdist == 12 || kdist == 14 || kdist == 15 || kdist == 29)
				return (0);
	}


	// Rule set 1 Manh
	if (((idist == 11 || idist == 13 || (idist >= 16 && idist <= 30)) && (jdist >= 0 && jdist <= 3)) ||
		((jdist == 11 || jdist == 13 || (jdist >= 16 && jdist <= 30)) && (idist >= 0 && idist <= 3))) {

			if ((kdist >= 4 && kdist <= 9) || kdist == 12 || kdist == 14 || kdist == 15)
				return (0);
	}


	// Rule set 2 Intra
	if (((idist == 8 || idist == 9) && (jdist == 4 || jdist == 6 || jdist == 8 || jdist == 9)) ||
		((idist == 4) && (jdist == 4 || jdist == 8 || jdist == 9)) ||
		((idist == 6) && (jdist == 6 || jdist == 8 || jdist == 9))) {
		
			if ((kdist >= 0 && kdist <= 3) || kdist == 5 || kdist == 7 || (kdist >= 10 && kdist <= 30))
				return (0);
	}


	// Rule set 2 Manh
	if (((idist == 4 || idist == 6 || idist == 8 || idist == 9) && (jdist >= 0 && jdist <= 3)) ||
		((jdist == 4 || jdist == 6 || jdist == 8 || jdist == 9) && (idist >= 0 && idist <= 3))) {
		
			if (kdist >= 10 && kdist <= 30)
				return (0);
	}


	// Rule set 2B
	if ((idist == 4 && jdist == 6) || (jdist == 4 && idist == 6)) {
		
			if (kdist == 5 || kdist == 7 || (kdist >= 9 && kdist <= 30))
				return (0);
	}


	// Rule set 3
	if ((idist >= 0 && idist <= 3) && (jdist >= 0 && jdist <= 3)) {
		
			if (kdist >= 4 && kdist <= 30)
				return (0);
	}


	// Rule set 4
	if (idist == 7 && jdist == 7) {
		
			if ((kdist >= 0 && kdist <= 6) || (kdist >= 8 && kdist <= 30))
				return (0);
	}


	// Rule set 5 Intra
	if (((idist == 10) && (jdist == 12 || jdist == 14 || jdist == 15 || jdist == 29)) ||
		((idist == 12 || idist == 14 || idist == 15 || idist == 29) && (jdist == 10 || jdist == 12 || jdist == 14 || jdist == 15 || jdist == 29))) {
		
			if ((kdist >= 0 && kdist <= 9) || kdist == 11 || kdist == 13 || (kdist >= 16 && kdist <= 28) || kdist == 30)
				return (0);
	}


	// Rule set 5B Intra
	if (idist == 10 && jdist == 10) {
		
			if ((kdist >= 0 && kdist <= 4) || (kdist >= 6 && kdist <= 11) || kdist == 13 || (kdist >= 16 && kdist <= 28) || kdist == 30)
				return (0);
	}


	// Rule set 5B Manh
	if ((idist == 10 || idist == 12 || idist == 14 || idist == 15 || idist == 29) &&
		(jdist >= 0 && jdist <= 3)) {
		
			if ((kdist >= 5 && kdist <= 9) || kdist == 11 || kdist == 13 || (kdist >= 16 && kdist <= 28) || kdist == 30)
				return (0);
	}


	return (1);

}