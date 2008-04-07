/*
 *	At work journey mode choice OD related components of systematic utilites for each modal alternative
 *
 *  Calculate the OD portions of systematic utility for each modal alternative to all destination tazs
 *  from the specified origin taz.  These are the lowest level alternatives in the NL structure.
 */

#include "md.h"
#include "atwork_coeff.h"

#define ATWORK_WT_TRANSFER_WAIT (float)2.0
#define ATWORK_WT_TRANSFER_OVTT (float)2.0

	
void atwork_da_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct zone_data *ZonalData, int **ranprkcst, float *Utility)
{
	int j;
	float Ivtt, Ovtt, Cost, ParkCost;

	for (j=1; j <= ncols; j++) {
		Utility[j] = MISSING;
		if (hwy_time[j] != MISSING && hwy_dist[j] != MISSING && hwy_toll[j] != MISSING) {
			Ivtt = hwy_time[j];
			Ovtt = Ini->TerminalTime[ZonalData->AreaType[i]] + Ini->TerminalTime[ZonalData->AreaType[j]];
			Cost = Ini->HWY_CENTS_PER_MILE*hwy_dist[j] + hwy_toll[j];		// tolls are in cents in the skims table
			ParkCost = parking_cost (i, j, ZonalData, ranprkcst);
		
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + Ovtt
					 + 0.15*Ini->CPI_RATIO*(Cost + ParkCost)));
		}
	}
}


void atwork_sr2_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct zone_data *ZonalData, int **ranprkcst, float *Utility)
{

	int j, Short;
	float Ivtt, Ovtt, Cost, ParkCost;
	
	for (j=1; j <= ncols; j++) {
		Utility[j] = MISSING;
		if (hwy_time[j] != MISSING && hwy_dist[j] != MISSING && hwy_toll[j] != MISSING) {
			Ivtt = hwy_time[j];
			Ovtt = Ini->TerminalTime[ZonalData->AreaType[i]] + Ini->TerminalTime[ZonalData->AreaType[j]];
			Cost = Ini->HWY_CENTS_PER_MILE*hwy_dist[j] + hwy_toll[j];		// tolls are in cents in the skims table
			ParkCost = parking_cost (i, j, ZonalData, ranprkcst);
			Short = (hwy_time[j] <= 7.0);
		
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + Ovtt
					 + 0.015*Ini->CPI_RATIO*(Cost + ParkCost)/2.0) + SHORT_2P*Short);
		}
	}						 
}


void atwork_sr3_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct zone_data *ZonalData, int **ranprkcst, float *Utility)
{

	int j, Short;
	float Ivtt, Ovtt, Cost, ParkCost;
	
	for (j=1; j <= ncols; j++) {
		Utility[j] = MISSING;
		if (hwy_time[j] != MISSING && hwy_dist[j] != MISSING && hwy_toll[j] != MISSING) {
			Ivtt = hwy_time[j];
			Ovtt = Ini->TerminalTime[ZonalData->AreaType[i]] + Ini->TerminalTime[ZonalData->AreaType[j]];
			Cost = Ini->HWY_CENTS_PER_MILE*hwy_dist[j] + hwy_toll[j];		// tolls are in cents in the skims table
			ParkCost = parking_cost (i, j, ZonalData, ranprkcst);
			Short = (hwy_time[j] <= 7.0);
		
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + Ovtt
					 + 0.015*Ini->CPI_RATIO*(Cost + ParkCost)/3.5) + SHORT_3P*Short);
		}
	}						 
}


void atwork_wt_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct tran_data *TranSkims, struct zone_data *ZonalData, float *Utility)
{

	int j, Short;
	float Ivtt, Cost, Ovtt, Xfr_wait, Xfr_ovtt;
	
	for (j=1; j <= ncols; j++) {
		Ivtt = 0.0;
		Xfr_wait = 0.0;
		Xfr_ovtt = 0.0;
		if (TranSkims->Bus_ivtt[j] != MISSING)
			Ivtt += TranSkims->Bus_ivtt[j];
		if (TranSkims->ExpBus_ivtt[j] != MISSING)
			Ivtt += TranSkims->ExpBus_ivtt[j];
		if (TranSkims->Rail_ivtt[j] != MISSING)
			Ivtt += TranSkims->Rail_ivtt[j];
		if (TranSkims->CR_ivtt[j] != MISSING)
			Ivtt += TranSkims->CR_ivtt[j];
		if (TranSkims->Ferry_ivtt[j] != MISSING)
			Ivtt += TranSkims->Ferry_ivtt[j];
		if (Ivtt > 0.0 && hwy_time[j] != MISSING) {
			if (TranSkims->Xfrs [j] > 0) {
				if (TranSkims->Xfr_wait[j] != MISSING)
					Xfr_wait = TranSkims->Xfr_wait[j];
				else
					Xfr_wait = TranSkims->Xfrs[j]*ATWORK_WT_TRANSFER_WAIT;
//					ExitWithCode (99);
				if (TranSkims->Xfr_ovtt[j] != MISSING)
					Xfr_ovtt = TranSkims->Xfr_ovtt[j];
				else
					Xfr_ovtt = TranSkims->Xfrs[j]*ATWORK_WT_TRANSFER_OVTT;
//					ExitWithCode (100);
			}
			Ovtt = TranSkims->First_wait[j] + Xfr_ovtt + Xfr_wait;
			Cost = TranSkims->Fare[j]*(float)100.0;									// cost in cents
			Short = (hwy_time[j] <= 7.0);
		
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + 3.0*(Ovtt)
					 + 0.015*Ini->CPI_RATIO*(Cost)) + SHORT_WT*Short);
		}
		else {
			Utility[j] = MISSING;
		}
	}						 
}


void atwork_tx_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct zone_data *ZonalData, struct taxi_data *TaxiData, float *Utility)
{
	int j, Short;
	float Ivtt, Taxi_cost, Taxi_toll, Taxi_wait;
	
	for (j=1; j <= ncols; j++) {
		Utility[j] = MISSING;
		if (hwy_time[j] != MISSING && hwy_dist[j] != MISSING && hwy_toll[j] != MISSING) {
			Ivtt = hwy_time[j];
			Taxi_wait = TaxiData->wait[ZonalData->County[i]][ZonalData->County[j]];
			Taxi_cost = (TaxiData->pickup[ZonalData->County[i]][ZonalData->County[j]]
					+ TaxiData->cpm[ZonalData->County[i]][ZonalData->County[j]]*hwy_dist[j]
					* TaxiData->surchg[ZonalData->County[i]][ZonalData->County[j]]);
			Taxi_toll = TaxiData->toll[ZonalData->County[i]][ZonalData->County[j]];
			if (Taxi_toll < 0.1)
				Taxi_toll = 2*hwy_toll[j]/100;
			Short = (hwy_time[j] <= 7.0);
			
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + 10.0*Taxi_wait
					+ 0.015*Ini->CPI_RATIO*(Taxi_cost + Taxi_toll)*100) + SHORT_TX*Short);
		}
	}
}
