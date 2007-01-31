#include "md.h"

// use bitwise operations to pack all 9 fields into a single integer variable to serve as cluster id number.
//
// bit  0:		1 adult and 1 kid journeys (0,1)		AK_MASK:  '0000 0000 0000 0000 0000 0000 0000 0001'
// bit  1:		2 or more adult journeys (0,1)			AA_MASK:  '0000 0000 0000 0000 0000 0000 0000 0010'
// bit  2-3:	person type (1,2,3)						PT_MASK:  '0000 0000 0000 0000 0000 0000 0000 1100'
// bit  4:		at work mode (0,1)						AW_MASK:  '0000 0000 0000 0000 0000 0000 0001 0000'
// bit  5-6:	auto sufficiency (0,1,2,3)				AS_MASK:  '0000 0000 0000 0000 0000 0000 0110 0000'
// bit  7-8:	income (1,2,3)							IN_MASK:  '0000 0000 0000 0000 0000 0001 1000 0000'
// bit  9:		walk access to transit or CR (0,1)		WA_MASK:  '0000 0000 0000 0000 0000 0010 0000 0000'
// bit  10-12:	purpose (0-7)							PU_MASK:  '0000 0000 0000 0000 0001 1100 0000 0000'
// bit  13-24:	origin zone (1-3586)					OZ_MASK:  '0000 0001 1111 1111 1110 0000 0000 0000'

#define AK_MASK 0x00000001
#define AA_MASK 0x00000002
#define PT_MASK 0x0000000C
#define AW_MASK 0x00000010
#define AS_MASK 0x00000060
#define IN_MASK 0x00000180
#define WA_MASK 0x00000200
#define PU_MASK 0x00001C00
#define OZ_MASK 0x01FFE000



int set_cluster (short orig, short purpose, short walk_orig, short income, short auto_suff, short at_work_mode,
				 short person_type, short ak, short aa)
{
	
	int cluster=0, temp_value=0;


	temp_value = (orig << 13) & OZ_MASK;			// origin
	cluster += temp_value;

	temp_value = (purpose << 10) & PU_MASK;			// purpose
	cluster += temp_value;

	temp_value = (walk_orig << 9) & WA_MASK;		// walk access available
	cluster += temp_value;

	temp_value = (income << 7) & IN_MASK;		// income
	cluster += temp_value;

	temp_value = (auto_suff << 5) & AS_MASK;		// auto sufficiency
	cluster += temp_value;

	temp_value = (at_work_mode << 4) & AW_MASK;		// at work mode
	cluster += temp_value;

	temp_value = (person_type << 2) & PT_MASK;		// person type
	cluster += temp_value;

	temp_value = (aa << 1) & AA_MASK;				// 2 or more adult journeys
	cluster += temp_value;

	temp_value = ak & AK_MASK;						// at least 1 kid and 1 adult journeys
	cluster += temp_value;


	return (cluster);
}



void get_cluster_parts (int cluster, struct cluster_parts ClusterParts)
{

	ClusterParts.orig = (cluster & OZ_MASK) >> 13;
	ClusterParts.purpose = (cluster & PU_MASK) >> 10;
	ClusterParts.walk_orig = (cluster & WA_MASK) >> 9;
	ClusterParts.income = (cluster & IN_MASK) >> 7;
	ClusterParts.auto_suff = (cluster & AS_MASK) >> 5;
	ClusterParts.at_work_mode = (cluster & AW_MASK) >> 4;
	ClusterParts.person_type = (cluster & PT_MASK) >> 2;
	ClusterParts.ak = (cluster & AK_MASK) >> 1;
	ClusterParts.aa = (cluster & AA_MASK);

}

