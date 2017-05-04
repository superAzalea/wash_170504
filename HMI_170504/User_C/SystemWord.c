/**************************************************************************************************
Project: TTK_ACPR
Filename: SystemCmd.c
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
#ifndef __SYSTEMWORD_C
#define __SYSTEMWORD_C

#include "stm32f10x.h"
#include "delay.h" 
#include "SystemInit.h" 
#include "IIC.h"
#include "24CXX.h" 
#include "LCD_display.h"
//#include "key.h"
#include "Menu.h"
#include "SysPr.h"
#include "SystemWord.h"

void SystemWordInit(void)
{
	glSystemCmd.all = 0;
	glFaultWord.all = 0;	
}

void SystemWordProcess(void)
{
	
}


#endif




