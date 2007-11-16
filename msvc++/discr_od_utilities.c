/*
 *	Discretionary journey mode choice OD related components of systematic utilites for each modal alternative
 *
 *  Calculate the OD portions of systematic utility for each modal alternative to all destination tazs
 *  from the specified origin taz.  These are the lowest level alternatives in the NL structure.
 */

#include "md.h"
#include "discr_coeff.h"


#define DISCR_WT_TRANSFER_WAIT (float)2.0
#define DISCR_WT_TRANSFER_OVTT (float)2.0
#define DISCR_DT_TRANSFER_WAIT (float)2.0
#define DISCR_DT_TRANSFER_OVTT (float)2.0
#define DISCR_WC_TRANSFER_WAIT (float)2.0
#define DISCR_WC_TRANSFER_OVTT (float)2.0
#define DISCR_DC_TRANSFER_WAIT (float)2.0
#define DISCR_DC_TRANSFER_OVTT (float)2.0
	
	
void discr_da_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct zone_data *ZonalData, int **ranprkcst, float *Utility)
{
	int j;
	float Ivtt, Ovtt, Cost, ParkCost;
	
	for (j=1; j <= ncols; j++) {
		Utility[j] = MISSING;
		if (hwy_time[j] != MISSING && hwy_dist[j] != MISSING && hwy_toll[j] != MISSING) {
			Ivtt = hwy_time[j];
			Ovtt = Ini->TerminalTime[ZonalData->AreaType[i]] + Ini->TerminalTime[ZonalData->AreaType[j]];
			Cost = Ini->HWY_CENTS_PER_MILE*hwy_dist[j] + hwy_toll[j];		// tolls are in cents in the skims table
			ParkCost = nw_parking_cost (i, j, ZonalData, ranprkcst);
		
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + Ovtt)
					 + COST*(Cost + ParkCost)
					 + UT1_DA*(ZonalData->UrbanType[j] == 1));
		}
	}
}


void discr_sr2_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct zone_data *ZonalData, int **ranprkcst, float *Utility)
{

	int j;
	float Ivtt, Ovtt, Cost, ParkCost;
	
	for (j=1; j <= ncols; j++) {
		Utility[j] = MISSING;
		if (hwy_time[j] != MISSING && hwy_dist[j] != MISSING && hwy_toll[j] != MISSING) {
			Ivtt = hwy_time[j];
			Ovtt = Ini->TerminalTime[ZonalData->AreaType[i]] + Ini->TerminalTime[ZonalData->AreaType[j]];
			Cost = Ini->HWY_CENTS_PER_MILE*hwy_dist[j] + hwy_toll[j];		// tolls are in cents in the skims table
			ParkCost = nw_parking_cost (i, j, ZonalData, ranprkcst);
		
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + Ovtt)
					 + COST*(Cost + ParkCost)/2.0
					 + UT1_2P*(ZonalData->UrbanType[j] == 1));
		}
	}						 
}


void discr_sr3_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct zone_data *ZonalData, int **ranprkcst, float *Utility)
{

	int j;
	float Ivtt, Ovtt, Cost, ParkCost;
	
	for (j=1; j <= ncols; j++) {
		Utility[j] = MISSING;
		if (hwy_time[j] != MISSING && hwy_dist[j] != MISSING && hwy_toll[j] != MISSING) {
			Ivtt = hwy_time[j];
			Ovtt = Ini->TerminalTime[ZonalData->AreaType[i]] + Ini->TerminalTime[ZonalData->AreaType[j]];
			Cost = Ini->HWY_CENTS_PER_MILE*hwy_dist[j] + hwy_toll[j];		// tolls are in cents in the skims table
			ParkCost = nw_parking_cost (i, j, ZonalData, ranprkcst);
		
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + Ovtt)
					 + COST*(Cost + ParkCost)/3.0
					 + UT1_3P*(ZonalData->UrbanType[j] == 1));
		}
	}						 
}


void discr_sr4_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct zone_data *ZonalData, int **ranprkcst, float *Utility)
{

	int j;
	float Ivtt, Ovtt, Cost, ParkCost;
	
	for (j=1; j <= ncols; j++) {
		Utility[j] = MISSING;
		if (hwy_time[j] != MISSING && hwy_dist[j] != MISSING && hwy_toll[j] != MISSING) {
			Ivtt = hwy_time[j];
			Ovtt = Ini->TerminalTime[ZonalData->AreaType[i]] + Ini->TerminalTime[ZonalData->AreaType[j]];
			Cost = Ini->HWY_CENTS_PER_MILE*hwy_dist[j] + hwy_toll[j];									// cost in cents
			ParkCost = nw_parking_cost (i, j, ZonalData, ranprkcst);
		
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + Ovtt)
					 + COST*(Cost + ParkCost)/4.0
					 + UT1_4P*(ZonalData->UrbanType[j] == 1));
		}
	}						 
}


void discr_wt_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct tran_data *TranSkims, struct zone_data *ZonalData, float *Utility)
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
					Xfr_wait = TranSkims->Xfrs[j]*DISCR_WT_TRANSFER_WAIT;
//					ExitWithCode (83);
				if (TranSkims->Xfr_ovtt[j] != MISSING)
					Xfr_ovtt = TranSkims->Xfr_ovtt[j];
				else
					Xfr_ovtt = TranSkims->Xfrs[j]*DISCR_WT_TRANSFER_OVTT;
//					ExitWithCode (84);
			}
			Ovtt = TranSkims->First_wait[j] + Xfr_ovtt + Xfr_wait;
			Cost = TranSkims->Fare[j]*(float)100.0;									// cost in cents
		
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + 2.5*(Ovtt))
					 + COST*(Cost)
					 + UT1_WT*(ZonalData->UrbanType[j] == 1));
		}
		else {
			Utility[j] = MISSING;
		}
	}						 
}


void discr_dt_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct tran_data *TranSkims, struct zone_data *ZonalData, float *Utility)
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
					Xfr_wait = TranSkims->Xfrs[j]*DISCR_DT_TRANSFER_WAIT;
//					ExitWithCode (85);
				if (TranSkims->Xfr_ovtt[j] != MISSING)
					Xfr_ovtt = TranSkims->Xfr_ovtt[j];
				else
					Xfr_ovtt = TranSkims->Xfrs[j]*DISCR_DT_TRANSFER_OVTT;
//					ExitWithCode (86);
			}
			Ovtt = TranSkims->First_wait[j] + Xfr_ovtt + Xfr_wait;
			DrAcc_ovtt = TranSkims->Acc_ovtt[j];
			Cost = TranSkims->Fare[j]*(float)100.0;									// cost in cents
			InfLot = TranSkims->InfLot[j];
		
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + 2.5*Ovtt + 1.5*DrAcc_ovtt)
					 + COST*(Cost) + INFLOT_DT*(InfLot)
					 + UT1_DT*(ZonalData->UrbanType[j] == 1));
		}
		else {
			Utility[j] = MISSING;
		}
	}						 
}


void discr_wc_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct tran_data *TranSkims, struct zone_data *ZonalData, float *Utility)
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
					Xfr_wait = TranSkims->Xfrs[j]*DISCR_WC_TRANSFER_WAIT;
//					ExitWithCode (87);
				if (TranSkims->Xfr_ovtt[j] != MISSING)
					Xfr_ovtt = TranSkims->Xfr_ovtt[j];
				else
					Xfr_ovtt = TranSkims->Xfrs[j]*DISCR_WC_TRANSFER_OVTT;
//					ExitWithCode (88);
			}
			Ovtt = TranSkims->First_wait[j] + Xfr_ovtt + Xfr_wait;
			Cost = TranSkims->Fare[j]*(float)100.0;									// cost in cents
			Dist = hwy_dist[j];
		
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + 2.5*Ovtt)
					 + COST*(Cost)
					 + CR_DIST*(Dist)
					 + UT1_WC*(ZonalData->UrbanType[j] == 1));
		}
		else {
			Utility[j] = MISSING;
		}
	}						 
}


void discr_dc_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct tran_data *TranSkims, struct zone_data *ZonalData, float *Utility)
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
					Xfr_wait = TranSkims->Xfrs[j]*DISCR_DC_TRANSFER_WAIT;
//					ExitWithCode (89);
				if (TranSkims->Xfr_ovtt[j] != MISSING)
					Xfr_ovtt = TranSkims->Xfr_ovtt[j];
				else
					Xfr_ovtt = TranSkims->Xfrs[j]*DISCR_DC_TRANSFER_OVTT;
//					ExitWithCode (90);
			}
			Ovtt = TranSkims->First_wait[j] + Xfr_ovtt + Xfr_wait;
			DrAcc_ovtt = TranSkims->Acc_ovtt[j];
			Cost = TranSkims->Fare[j]*(float)100.0;									// cost in cents
			Dist = hwy_dist[j];
			InfLot = TranSkims->InfLot[j];
		
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + 2.5*Ovtt + 1.5*DrAcc_ovtt)
					 + COST*(Cost) + INFLOT_DC*(InfLot)
					 + CR_DIST*(Dist)
					 + UT1_DC*(ZonalData->UrbanType[j] == 1));
		}
		else {
			Utility[j] = MISSING;
		}
	}						 
}


void discr_tx_od_util (int i, int ncols, float *hwy_time, float *hwy_dist, float *hwy_toll, struct zone_data *ZonalData, struct taxi_data *TaxiData, float *Utility)
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
			
			Utility[j] = (float)(IN_VEH_TIM*(Ivtt + 8.0*Taxi_wait)
					+ COST*(Taxi_cost + Taxi_toll)*100
					 + UT1_TX*(ZonalData->UrbanType[j] == 1));
		}
	}
}
