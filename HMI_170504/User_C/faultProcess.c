/**************************************************************************************************
Project: TTK_ACPR
Filename: faultProcess.c
Description: time interruput functions for air compressor.
Complier: Keil uVision5.
***************************************************************************************************
Copyright (c) 2014 TTK.
All rights reserved.
***************************************************************************************************
DESCRIPTION:
Revision History:
Original Author:JesonsSun
First edition Time:20140601
Last Review Time:20140701 11:11;V1.0
**************************************************************************************************/
#ifndef __FAULTPROCESS_C
#define __FAULTPROCESS_C

#include "stm32f10x.h"
#include "define.h"
#include "delay.h"
#include "SysPr.h"
#include "adc.h"
#include "faultProcess.h"
																   
void fault_Init(void)
{ 	
//	SystemFault.all = 0;
}	

void faultProcess(void)   
{	
//	if(Adc_BatteryVoltag < 4000)//2.3V
//	{
//		SystemFault.bit.butttery_lv = 1;
//	}
//	else
//	{
//		SystemFault.bit.butttery_lv = 0;
//	}
}

#endif




























