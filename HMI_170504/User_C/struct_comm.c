#ifndef __STRUCT_COMM_C
#define __STRUCT_COMM_C

#include "struct_comm.h"
void putCommSend(void)
{
	 CommSend_All_old = structCommSend.all;
	 structCommSend.bit.wash_1_1		   = wash_1;
	 structCommSend.bit.wash_2_1 	  	   = wash_2;
	 structCommSend.bit.soap_liquid_3_1    = soap_liquid_3;
	 structCommSend.bit.soap_liquid_2_1    = soap_liquid_2;
	 structCommSend.bit.soap_liquid_1_1    = soap_liquid_1 ;
	 structCommSend.bit.cold_water_1       = cool_water;
	 structCommSend.bit.hot_water_1        = hot_water;
	 structCommSend.bit.calefaction_1      = calefaction;
	 structCommSend.bit.drain_1            = drain;
	 structCommSend.bit.skid_1		       = skid ;
	 structCommSend.bit.door_lock_1 	   = door_lock;
	 structCommSend.bit.backup_one_1	   = backup_one;
	 CommSend_All = (wash_1<<0) + (wash_2<<1) + (soap_liquid_3<<2) + (soap_liquid_2<<3) 
			 + (soap_liquid_1<<4) + (cool_water<<5) + (hot_water<<6) + (calefaction<<7)
			 + (drain<<8) + (skid<<9) + (door_lock<<10) + (backup_one<<11);
	structCommSend.all = CommSend_All;

}


#endif
