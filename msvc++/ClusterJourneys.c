#include "md.h"

void cluster_journeys (struct journey_attribs *JourneyAttribs, int **p_sort, int **p_start)
{

//	int i, j, k, p, old_p, count, temp_len;
	int i, j, k, p, old_p, count;
	int packet_number, packets, packet_size, remainder, extra;
	int *packet, *temp_start;
	int max_size=0, no_matches=0;
	short auto_suff;
//	char temp[1024];

// memory allocations
	packet = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(int));
	temp_start = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_JOURNEYS+1)*sizeof(int));
	*p_sort = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, Ini->NUMBER_JOURNEYS*sizeof(int));
//	addRAM ("cluster_journeys", 3*(Ini->NUMBER_JOURNEYS+1)*sizeof(int));

	printf ("assigning cluster codes...\n");

// assign cluster codes to journeys
	for (k=0; k < Ini->NUMBER_JOURNEYS; k++) {

		if (JourneyAttribs->autos[k] == 0)
			auto_suff = 0;
		else if (JourneyAttribs->autos[k] < JourneyAttribs->workers[k])
			auto_suff = 1;
		else if (JourneyAttribs->autos[k] == JourneyAttribs->workers[k])
			auto_suff = 2;
		else
			auto_suff = 3;

		packet[k] = set_cluster (JourneyAttribs->orig[k], JourneyAttribs->purpose[k], JourneyAttribs->walk_orig[k],
			JourneyAttribs->income[k], auto_suff, JourneyAttribs->at_work_mode[k], JourneyAttribs->person_type[k],
			JourneyAttribs->ak[k], JourneyAttribs->aa[k]);

		// assign a large packet number for at-work journeys with no matching work journey.
		// these will get sorted into the last packet, and we'll eliminate this last packet from further processing.
		// keep track of the number of journeys in this final packet.
		if (JourneyAttribs->at_work_mode[k] < 0) {
			no_matches++;
			packet[k] = 0x0ffffff;
		}

		(*p_sort)[k] = k;

//		sprintf (temp, "assigning cluster codes %3d%% complete", 100*k/Ini->NUMBER_JOURNEYS);
//		temp_len = (int)strlen(temp);
//		for (j=0; j < temp_len; j++)
//			sprintf (&temp[temp_len + j], "\b");
//		printf ("%s", temp);
	}
	printf ("assigning cluster codes complete\n");


// create a set of indices for journeys sorted by packet number
	isort (packet, *p_sort, Ini->NUMBER_JOURNEYS);


	printf ("numbering packets...\n");

// go through the journeys, count packet frequency, and assign packet numbers, keep track of start journey of packets
	packet_number = 0;
	p = packet[(*p_sort)[0]];
	old_p = p;
	count = 1;
	temp_start[0] = 0;
	for (k=1; k < Ini->NUMBER_JOURNEYS; k++) {
		if (k == Ini->NUMBER_JOURNEYS - 1) {
			k = Ini->NUMBER_JOURNEYS - 1;
		}
		p = packet[(*p_sort)[k]];
		if (p != old_p) {
			// set the journey number of the start of the next packet in temp_start.
			// allocate journeys within packets as uniformly as possible.
			packets = (int)(count/Ini->MAX_PACKET);
			if ((count - packets*Ini->MAX_PACKET) > 0)
				packets++;
			packet_size = (int)(count/packets);
			remainder = count - packets*packet_size;
			for (j=0; j < packets; j++) {
				extra = 0;
				packet_number++;
				if (j < remainder)
					extra = 1;
				temp_start[packet_number] = temp_start[packet_number - 1] + packet_size + extra;

				// assign packet number to the journeys
				for (i=temp_start[packet_number - 1]; i < temp_start[packet_number]; i++)
					JourneyAttribs->packet[(*p_sort)[i]] = packet_number - 1;
			}
			if (count > max_size)
				max_size = count;
			old_p = p;
			count = 1;
		}
		else {
			count++;
		}
//		sprintf (temp, "numbering packets %3d%% complete", 100*k/Ini->NUMBER_JOURNEYS);
//		temp_len = (int)strlen(temp);
//		for (j=0; j < temp_len; j++)
//			sprintf (&temp[temp_len + j], "\b");
//		printf ("%s", temp);
	}
	if (count >= 1) {
		packets = (int)(count/Ini->MAX_PACKET);
		if ((count - packets*Ini->MAX_PACKET) > 0)
			packets++;
		packet_size = (int)(count/packets);
		remainder = count - packets*packet_size;
		for (j=0; j < packets; j++) {
			extra = 0;
			packet_number++;
			if (j < remainder)
				extra = 1;
			temp_start[packet_number] = temp_start[packet_number - 1] + packet_size + extra;

			// assign packet number to the journeys
			for (i=temp_start[packet_number - 1]; i < temp_start[packet_number]; i++)
				JourneyAttribs->packet[(*p_sort)[i]] = packet_number - 1;
		}
	}
	// eliminate the packet with at-work journeys that had no matching work journey.
	if (no_matches > 0) {
		packet_number--;
	}
	Ini->NUMBER_PACKETS = packet_number;

	printf ("numbering packets complete\n");

// allocate correct amount of memory for the packets determined and store start of packet locations
	*p_start = (int *) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, (Ini->NUMBER_PACKETS+1)*sizeof(int));
//	addRAM ("cluster_journeys", (Ini->NUMBER_PACKETS+1)*sizeof(int));
	for (i=0; i <= Ini->NUMBER_PACKETS; i++)
		(*p_start)[i] = temp_start[i];



	if (Calibrating == 0)
		fprintf (fp_rep, "\nstrata with highest frequency had %d journeys\n\n", max_size);


	HeapFree (heapHandle, 0, packet);
	HeapFree (heapHandle, 0, temp_start);
//	relRAM ("cluster_journeys", 2*(Ini->NUMBER_JOURNEYS+1)*sizeof(int));
}