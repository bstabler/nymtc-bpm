#include "md.h"

// use bitwise operations to pack orig, dest, and mode fields into a single integer variable to serve as cluster id number.
//
// bit  0-3:	mode (1-13)								MD_MASK:  '0000 0000 0000 0000 0000 0000 0000 1111'
// bit  4-15:	origin zone (1-3586)					OZ_MASK:  '0000 0000 0000 0000 1111 1111 1111 0000'
// bit 16-27:	destination zone (1-3586)				DZ_MASK:  '0000 1111 1111 1111 0000 0000 0000 0000'

#define MD_MASK 0x0000000F
#define OZ_MASK 0x0000FFF0
#define DZ_MASK 0x0FFF0000



int set_stops_cluster (int orig, int dest, int mode)
{
	
	int cluster=0, temp_value=0;


	temp_value = (dest << 16) & DZ_MASK;			// destination
	cluster += temp_value;

	temp_value = (orig << 4) & OZ_MASK;				// origin
	cluster += temp_value;

	temp_value = mode  & MD_MASK;					// mode
	cluster += temp_value;


	return (cluster);
}
