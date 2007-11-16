/*
 *	Work journey mode choice OD related components of systematic utilites for each modal alternative.
 *  Called by OD_Utility() if the prupose is work.
 *
 *  Calculate the OD portions of systematic utility for each modal alternative.
 *  These are the lowest level alternatives in the NL structure.
 */

#include "md.h"
#include "work_coeff.h"


#define WORK_WT_TRANSFER_WAIT (float)2.0
#define WORK_WT_TRANSFER_OVTT (float)2.0
#define WORK_DT_TRANSFER_WAIT (float)2.0
#define WORK_DT_TRANSFER_OVTT (float)2.0
#define WORK_WC_TRANSFER_WAIT (float)2.0
#define WORK_WC_TRANSFER_OVTT (float)2.0
#define WORK_DC_TRANSFER_WAIT (float)2.0
#define WORK_DC_TRANSFER_OVTT (float)2.0

void work_da_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct zone_data *ZonalData, int **ranprkcst, float *Utility)
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
		
			Utility[j] = (float)(IN_VEH_TIME*(Ivtt + Ovtt)
					 + COST*(Cost + ParkCost));
		}
	}
}


void work_sr2_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct zone_data *ZonalData, int **ranprkcst, float *Utility)
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
		
			Utility[j] = (float)(IN_VEH_TIME*(Ivtt + Ovtt)
					 + COST*(Cost + ParkCost)/2.0
					 + UA_2P*(ZonalData->UrbanType[j] == 1));
		}
	}						 
}


void work_sr3_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct zone_data *ZonalData, int **ranprkcst, float *Utility)
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
		
			Utility[j] = (float)(IN_VEH_TIME*(Ivtt + Ovtt)
					 + COST*(Cost + ParkCost)/3.0
					 + UA_3P*(ZonalData->UrbanType[j] == 1));
		}
	}						 
}


void work_sr4_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct zone_data *ZonalData, int **ranprkcst, float *Utility)
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
		
			Utility[j] = (float)(IN_VEH_TIME*(Ivtt + Ovtt)
					 + COST*(Cost + ParkCost)/4.0
					 + UA_4P*(ZonalData->UrbanType[j] == 1));
		}
	}						 
}


void work_wt_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct tran_data *TranSkims, struct zone_data *ZonalData, float *Utility)
{

	int j;
	float Ivtt, Cost, Wait_le7, Wait_gt7, Xfr_wait, Xfr_ovtt;
	
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
			Wait_le7 = (TranSkims->First_wait[j] <= 7.0)*TranSkims->First_wait[j];
			Wait_gt7 = (TranSkims->First_wait[j] > 7.0)*TranSkims->First_wait[j];
			if (TranSkims->Xfrs[j] > 0) {
				if (TranSkims->Xfr_wait[j] != MISSING)
					Xfr_wait = TranSkims->Xfr_wait[j];
				else
					Xfr_wait = TranSkims->Xfrs[j]*WORK_WT_TRANSFER_WAIT;
				if (TranSkims->Xfr_ovtt[j] != MISSING)
					Xfr_ovtt = TranSkims->Xfr_ovtt[j];
				else
					Xfr_ovtt = TranSkims->Xfrs[j]*WORK_WT_TRANSFER_OVTT;
			}
			Cost = TranSkims->Fare[j]*(float)100.0;									// cost in cents
		
			Utility[j] = (float)(IN_VEH_TIME*(Ivtt)
					 + FW_LE7_TR*(Wait_le7) + FW_GT7_TR*(Wait_gt7) + XFER_WAIT*(Xfr_wait)
					 + WALK*(Xfr_ovtt)
					 + COST*(Cost)
					 + UA_WT*(ZonalData->UrbanType[j] == 1));
		}
		else {
			Utility[j] = MISSING;
		}
	}						 
}


void work_dt_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct tran_data *TranSkims, struct zone_data *ZonalData, float *Utility)
{

	int j, InfLot;
	float Ivtt, Cost, Wait_le7, Wait_gt7, Xfr_ovtt, Xfr_wait, DrAcc_ovtt;
	
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
			Wait_le7 = (TranSkims->First_wait[j] <= 7.0)*TranSkims->First_wait[j];
			Wait_gt7 = (TranSkims->First_wait[j] > 7.0)*TranSkims->First_wait[j];
			if (TranSkims->Xfrs[j] > 0) {
				if (TranSkims->Xfr_wait[j] != MISSING)
					Xfr_wait = TranSkims->Xfr_wait[j];
				else
					Xfr_wait = TranSkims->Xfrs[j]*WORK_DT_TRANSFER_WAIT;
//					ExitWithCode (61);
				if (TranSkims->Xfr_ovtt[j] != MISSING)
					Xfr_ovtt = TranSkims->Xfr_ovtt[j];
				else
					Xfr_ovtt = TranSkims->Xfrs[j]*WORK_DT_TRANSFER_OVTT;
//					ExitWithCode (62);
			}
			DrAcc_ovtt = TranSkims->Acc_ovtt[j];
			Cost = TranSkims->Fare[j]*(float)100.0;									// cost in cents
			InfLot = TranSkims->InfLot[j];
		
			Utility[j] = (float)(IN_VEH_TIME*(Ivtt + 2.0*DrAcc_ovtt)
					 + FW_LE7_TR*(Wait_le7) + FW_GT7_TR*(Wait_gt7) + XFER_WAIT*(Xfr_wait)
					 + WALK*(Xfr_ovtt)
					 + COST*(Cost) + INFORMLOT_TR*(InfLot)
					 + UA_DT*(ZonalData->UrbanType[j] == 1));
		}
		else {
			Utility[j] = MISSING;
		}
	}						 
}


void work_wc_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct tran_data *TranSkims, struct zone_data *ZonalData, float *Utility)
{

	int j;
	float Ivtt, Cost, Wait_le7, Wait_gt7, Xfr_wait, Xfr_ovtt, Dist;
	
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
			Wait_le7 = (TranSkims->First_wait[j] <= 7.0)*TranSkims->First_wait[j];
			Wait_gt7 = (TranSkims->First_wait[j] > 7.0)*TranSkims->First_wait[j];
			if (TranSkims->Xfrs [j] > 0) {
				if (TranSkims->Xfr_wait[j] != MISSING)
					Xfr_wait = TranSkims->Xfr_wait[j];
				else
					Xfr_wait = TranSkims->Xfrs[j]*WORK_WC_TRANSFER_WAIT;
//					ExitWithCode (63);
				if (TranSkims->Xfr_ovtt[j] != MISSING)
					Xfr_ovtt = TranSkims->Xfr_ovtt[j];
				else
					Xfr_ovtt = TranSkims->Xfrs[j]*WORK_WC_TRANSFER_OVTT;
//					ExitWithCode (64);
			}
			Cost = TranSkims->Fare[j]*(float)100.0;									// cost in cents
			Dist = hwy_dist[j];
		
			Utility[j] = (float)(IN_VEH_TIME*(Ivtt)
					 + FW_LE7_CR*(Wait_le7) + FW_GT7_CR*(Wait_gt7) + XFER_WAIT*(Xfr_wait)
					 + WALK*(Xfr_ovtt)
					 + COST*(Cost)
					 + CR_DIST*(Dist)
					 + UA_WC*(ZonalData->UrbanType[j] == 1));
		}
		else {
			Utility[j] = MISSING;
		}
	}						 
}


void work_dc_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct tran_data *TranSkims, struct zone_data *ZonalData, float *Utility)
{

	int j, InfLot;
	float Ivtt, Cost, Wait_le7, Wait_gt7, Xfr_wait, Xfr_ovtt, Dist, DrAcc_ovtt;
	
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
			Wait_le7 = (TranSkims->First_wait[j] <= 7.0)*TranSkims->First_wait[j];
			Wait_gt7 = (TranSkims->First_wait[j] > 7.0)*TranSkims->First_wait[j];
			if (TranSkims->Xfrs [j] > 0) {
				if (TranSkims->Xfr_wait[j] != MISSING)
					Xfr_wait = TranSkims->Xfr_wait[j];
				else
					Xfr_wait = TranSkims->Xfrs[j]*WORK_DC_TRANSFER_WAIT;
//					ExitWithCode (65);
				if (TranSkims->Xfr_ovtt[j] != MISSING)
					Xfr_ovtt = TranSkims->Xfr_ovtt[j];
				else
					Xfr_ovtt = TranSkims->Xfrs[j]*WORK_DC_TRANSFER_OVTT;
//					ExitWithCode (66);
			}
			DrAcc_ovtt = TranSkims->Acc_ovtt[j];
			Cost = TranSkims->Fare[j]*(float)100.0;									// cost in cents
			Dist = hwy_dist[j];
			InfLot = TranSkims->InfLot[j];
		
			Utility[j] = (float)(IN_VEH_TIME*(Ivtt + 2.0*DrAcc_ovtt)
					 + FW_LE7_CR*(Wait_le7) + FW_GT7_CR*(Wait_gt7) + XFER_WAIT*(Xfr_wait)
					 + WALK*(Xfr_ovtt)
					 + COST*(Cost) + INFORMLOT_CR*(InfLot)
					 + CR_DIST*(Dist)
					 + UA_DC*(ZonalData->UrbanType[j] == 1));
		}
		else {
			Utility[j] = MISSING;
		}
	}						 
}


void work_tx_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct zone_data *ZonalData, struct taxi_data *TaxiData, float *Utility)
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
			
			Utility[j] = (float)(IN_VEH_TIME*(Ivtt)
					+ COST*(Taxi_cost + Taxi_toll)*100
					+ WAIT_TX*(Taxi_wait)
				    + UA_TX*(ZonalData->UrbanType[j] == 1));
		}
		else {
			Utility[j] = MISSING;
		}
	}
}
