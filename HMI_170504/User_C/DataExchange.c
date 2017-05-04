/**************************************************************************************************
Project: TTK_ACPR
Filename: DataExchange.c
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
#ifndef __DataExchange_C
#define __DataExchange_C

#include "stm32f10x.h"
#include "define.h"
#include "delay.h"
#include "usart.h"
#include "DataExchange.h"
//////////////////////////////////////////////////////////////////////////////////////////////////
//将USART3收的数据，通过USART1，发给上位机显示。
//将AD采样的值读出来，通过USART1，发给上位机显示。
//////////////////////////////////////////////////////////////////////////////////////////////////	
void sub_DataExchange(void)
{
	
}

#endif












