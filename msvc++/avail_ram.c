#include "md.h"

int avail_ram ()
{
	char *ram;
	int lo, hi, num;


	ram = (char *) HeapAlloc (heapHandle, 0, AvailMemory);

	if (ram != NULL) {
		HeapFree (heapHandle, 0, ram);
		return (AvailMemory);
	}
	else {
		lo = 0;
		hi = AvailMemory;
		num = (hi - lo)/2;

		while (ram == NULL) {
			if (num < AvailMemory) {
				ram = (char *) HeapAlloc (heapHandle, 0, num);
				while (ram != NULL) {
					HeapFree (heapHandle, 0, ram);
					ram = NULL;
					lo = num;
					num += (hi - num)/2;
					if (hi - lo <= 1)
						break;
					if (num < AvailMemory)
						ram = (char *) HeapAlloc (heapHandle, 0, num);
					else
						hi = num;
				}
			}
			hi = num;
			num -= (num - lo)/2;
			if (hi - lo <= 1)
				break;
		}

		if (ram != NULL)
			HeapFree (heapHandle, 0, ram);

		return (num);

	}

}
