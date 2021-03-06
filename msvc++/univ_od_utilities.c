/*
 *	University journey mode choice OD related components of systematic utilites for each modal alternative
 *
 *  Calculate the OD portions of systematic utility for each modal alternative to all destination tazs
 *  from the specified origin taz.  These are the lowest level alternatives in the NL structure.
 */

#include "md.h"
#include "univ_coeff.h"

#define UNIV_WT_TRANSFER_WAIT (float)2.0
#define UNIV_WT_TRANSFER_OVTT (float)2.0
#define UNIV_DT_TRANSFER_WAIT (float)2.0
#define UNIV_DT_TRANSFER_OVTT (float)2.0
#define UNIV_WC_TRANSFER_WAIT (float)2.0
#define UNIV_WC_TRANSFER_OVTT (float)2.0
#define UNIV_DC_TRANSFER_WAIT (float)2.0
#define UNIV_DC_TRANSFER_OVTT (float)2.0
	
void univ_da_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct zone_data *ZonalData, int **ranprkcst, float *Utility)
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
		
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + Ovtt)
					 + COST*Ini->CPI_RATIO*(Cost + ParkCost));
		}
	}
}


void univ_sr2_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct zone_data *ZonalData, int **ranprkcst, float *Utility)
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
		
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + Ovtt)
					 + COST*Ini->CPI_RATIO*(Cost + ParkCost)/2.0
					 + UT1_2P*(ZonalData->UrbanType[j] == 1));
		}
	}						 
}


void univ_sr3_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct zone_data *ZonalData, int **ranprkcst, float *Utility)
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
		
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + Ovtt)
					 + COST*Ini->CPI_RATIO*(Cost + ParkCost)/3.0
					 + UT1_3P*(ZonalData->UrbanType[j] == 1));
		}
	}						 
}


void univ_sr4_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct zone_data *ZonalData, int **ranprkcst, float *Utility)
{

	int j;
	float Ivtt, Ovtt, Cost, ParkCost;
	
	for (j=1; j <= ncols; j++) {
		Utility[j] = MISSING;
		if (hwy_time[j] != MISSING && hwy_dist[j] != MISSING && hwy_toll[j] != MISSING) {
			Ivtt = hwy_time[j];
			Ovtt = Ini->TerminalTime[ZonalData->AreaType[i]] + Ini->TerminalTime[ZonalData->AreaType[j]];
			Cost = Ini->HWY_CENTS_PER_MILE*hwy_dist[j] + hwy_toll[j];									// cost in cents
			ParkCost = parking_cost (i, j, ZonalData, ranprkcst);
		
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + Ovtt)
					 + COST*Ini->CPI_RATIO*(Cost + ParkCost)/4.0
					 + UT1_4P*(ZonalData->UrbanType[j] == 1));
		}
	}						 
}


void univ_wt_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct tran_data *TranSkims, struct zone_data *ZonalData, float *Utility)
{

	int j;
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
		if (Ivtt > 0.0) {
			if (TranSkims->Xfrs [j] > 0) {
				if (TranSkims->Xfr_wait[j] != MISSING)
					Xfr_wait = TranSkims->Xfr_wait[j];
				else
					Xfr_wait = TranSkims->Xfrs[j]*UNIV_WT_TRANSFER_WAIT;
//					ExitWithCode (67);
				if (TranSkims->Xfr_ovtt[j] != MISSING)
					Xfr_ovtt = TranSkims->Xfr_ovtt[j];
				else
					Xfr_ovtt = TranSkims->Xfrs[j]*UNIV_WT_TRANSFER_OVTT;
//					ExitWithCode (68);
			}
			Ovtt = TranSkims->First_wait[j] + Xfr_ovtt + Xfr_wait;
			Cost = TranSkims->Fare[j]*(float)100.0;									// cost in cents
		
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + 3.0*(Ovtt))
					 + COST*Ini->CPI_RATIO*(Cost)
					 + UT1_WT*(ZonalData->UrbanType[j] == 1));
		}
		else {
			Utility[j] = MISSING;
		}
	}						 
}


void univ_dt_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct tran_data *TranSkims, struct zone_data *ZonalData, float *Utility)
{

	int j, InfLot;
	float Ivtt, Cost, Ovtt, DrAcc_ovtt, Xfr_wait, Xfr_ovtt;
	
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
		if (Ivtt > 0.0) {
			if (TranSkims->Xfrs [j] > 0) {
				if (TranSkims->Xfr_wait[j] != MISSING)
					Xfr_wait = TranSkims->Xfr_wait[j];
				else
					Xfr_wait = TranSkims->Xfrs[j]*UNIV_DT_TRANSFER_WAIT;
//					ExitWithCode (69);
				if (TranSkims->Xfr_ovtt[j] != MISSING)
					Xfr_ovtt = TranSkims->Xfr_ovtt[j];
				else
					Xfr_ovtt = TranSkims->Xfrs[j]*UNIV_DT_TRANSFER_OVTT;
//					ExitWithCode (70);
			}
			Ovtt = TranSkims->First_wait[j] + Xfr_ovtt + Xfr_wait;
			DrAcc_ovtt = TranSkims->Acc_ovtt[j];
			Cost = TranSkims->Fare[j]*(float)100.0;									// cost in cents
			InfLot = TranSkims->InfLot[j];
		
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + 3.0*Ovtt + 1.5*DrAcc_ovtt)
					 + COST*Ini->CPI_RATIO*(Cost) + INFLOT_DT*(InfLot)
					 + UT1_DT*(ZonalData->UrbanType[j] == 1));
		}
		else {
			Utility[j] = MISSING;
		}
	}						 
}


void univ_wc_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct tran_data *TranSkims, struct zone_data *ZonalData, float *Utility)
{

	int j;
	float Ivtt, Cost, Ovtt, Dist, Xfr_wait, Xfr_ovtt;
	
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
		if (Ivtt > 0.0 && TranSkims->CR_ivtt[j] > 0.0) {
			if (TranSkims->Xfrs [j] > 0) {
				if (TranSkims->Xfr_wait[j] != MISSING)
					Xfr_wait = TranSkims->Xfr_wait[j];
				else
					Xfr_wait = TranSkims->Xfrs[j]*UNIV_WC_TRANSFER_WAIT;
//					ExitWithCode (71);
				if (TranSkims->Xfr_ovtt[j] != MISSING)
					Xfr_ovtt = TranSkims->Xfr_ovtt[j];
				else
					Xfr_ovtt = TranSkims->Xfrs[j]*UNIV_WC_TRANSFER_OVTT;
//					ExitWithCode (72);
			}
			Ovtt = TranSkims->First_wait[j] + Xfr_ovtt + Xfr_wait;
			Cost = TranSkims->Fare[j]*(float)100.0;									// cost in cents
			Dist = hwy_dist[j];
		
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + 3.0*Ovtt)
					 + COST*Ini->CPI_RATIO*(Cost)
					 + UT1_WC*(ZonalData->UrbanType[j] == 1));
		}
		else {
			Utility[j] = MISSING;
		}
	}						 
}


void univ_dc_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct tran_data *TranSkims, struct zone_data *ZonalData, float *Utility)
{

	int j, InfLot;
	float Ivtt, Cost, Ovtt, Dist, DrAcc_ovtt, Xfr_wait, Xfr_ovtt;
	
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
		if (Ivtt > 0.0 && TranSkims->CR_ivtt[j] > 0.0) {
			if (TranSkims->Xfrs [j] > 0) {
				if (TranSkims->Xfr_wait[j] != MISSING)
					Xfr_wait = TranSkims->Xfr_wait[j];
				else
					Xfr_wait = TranSkims->Xfrs[j]*UNIV_DC_TRANSFER_WAIT;
//					ExitWithCode (73);
				if (TranSkims->Xfr_ovtt[j] != MISSING)
					Xfr_ovtt = TranSkims->Xfr_ovtt[j];
				else
					Xfr_ovtt = TranSkims->Xfrs[j]*UNIV_DC_TRANSFER_OVTT;
//					ExitWithCode (74);
			}
			Ovtt = TranSkims->First_wait[j] + Xfr_ovtt + Xfr_wait;
			DrAcc_ovtt = TranSkims->Acc_ovtt[j];
			Cost = TranSkims->Fare[j]*(float)100.0;									// cost in cents
			Dist = hwy_dist[j];
			InfLot = TranSkims->InfLot[j];
		
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + 3.0*Ovtt + 1.5*DrAcc_ovtt)
					 + COST*Ini->CPI_RATIO*(Cost) + INFLOT_DC*(InfLot)
					 + UT1_DC*(ZonalData->UrbanType[j] == 1));
		}
		else {
			Utility[j] = MISSING;
		}
	}						 
}


void univ_tx_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct zone_data *ZonalData, struct taxi_data *TaxiData, float *Utility)
{
	int j;
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
			
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + 10.0*Taxi_wait)
			 		 + COST*Ini->CPI_RATIO*(Taxi_cost + Taxi_toll)*100
					 + UT1_TX*(ZonalData->UrbanType[j] == 1));
		}
	}
}
