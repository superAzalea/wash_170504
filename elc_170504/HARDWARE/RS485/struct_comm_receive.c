#ifndef __STRUCT_COMM_RECEIVE_C
#define __STRUCT_COMM_RECEIVE_C

#include "struct_comm_receive.h"
#include "rs485.h"
void getCommReceive(void)
{
	structCommReceive.all = MBS_HoldRegs[5];
	if((structCommReceive.all&0x01 )== 0x01)
	{
		OUT_X1 = 0;
	}
	else
	{
		OUT_X1 = 1;

	}
    if((structCommReceive.all&0x02) == 0x02)
	{
		OUT_X2 = 0;
	}
	else
	{
		OUT_X2 = 1;
	}
	if((structCommReceive.all&0x04) == 0x04)
	{
		OUT_SOAP3 =0;
	}
	else
	{
		OUT_SOAP3 = 1;
	}
	if((structCommReceive.all&0x08) == 0x08)
	{
		OUT_SOAP2 = 0;
	}
	else
	{
		OUT_SOAP2 = 1;
	}
	
	if((structCommReceive.all&0x10 )== 0x10)
	{
		OUT_SOAP1 =0;
	}
	else
	{
		OUT_SOAP1 = 1;
	}
	
	if((structCommReceive.all&0x20) == 0x20)
	{
		OUT_COOL_WATER = 0;
	}
	else
	{
		OUT_COOL_WATER = 1;
	}
	
	if((structCommReceive.all&0x40 )== 0x40)
	{
		OUT_HOT_WATER = 0;
	}
	else
	{
		OUT_HOT_WATER = 1;
	}
	if((structCommReceive.all&0x80 )== 0x80)
	{
		OUT_CALEFACTION = 0;
	}
	else
	{
		OUT_CALEFACTION = 1;
	}
	
	if((structCommReceive.all&0x100) == 0x100)
	{
		OUT_DRAIN = 0;
	}
	else
	{
		OUT_DRAIN = 1;
	}
	
	if((structCommReceive.all&0x200) == 0x200)
	{
		OUT_SKID = 0;
	}
	else
	{
		OUT_SKID = 1;
	}
	
	if((structCommReceive.all&0x400) == 0x400)
	{
		OUT_DOOR_LOCK = 0;
	}
	else
	{
		OUT_DOOR_LOCK = 1;
	}
	
	if((structCommReceive.all&0x800) == 0x800)
	{
		OUT_BACK =0;
	}
	else
	{
		OUT_BACK = 1;
	}
}

#endif
