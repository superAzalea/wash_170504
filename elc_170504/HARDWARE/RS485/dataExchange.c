#ifndef __DataExchange_C
#define __DataExchange_C

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "rs485.h"
#include "usart_Frequ.h"
#include "dataExchange.h"
#include "faultProcess.h"
#include "adc.h"
//////////////////////////////////////////////////////////////////////////////////////////////////
//将USART3收的数据，通过USART1，发给上位机显示。
//将AD采样的值读出来，通过USART1，发给上位机显示。
//////////////////////////////////////////////////////////////////////////////////////////////////	
void DataExchange(void)
{
	unsigned char i = 0;
	for(i = 0;i < 5; i++) //AD值
	{
		if(i == 4)
		{
			MBS_HoldRegs[62 + i] = SystemState.all;
		}
		else
		{
			MBS_HoldRegs[62 + i] = adc_result[i]; //test by andy

		}
	}
	
	for( i = 0; i < 8; i++) // 变频器的反馈信息,增加加速时间、减速时间
	{
		MBS_HoldRegs[68 + i] = MBS_HoldRegs3[i];//MBS[73]开始，记录10个变频器反馈信息。73~82
	}	
//	MBS_HoldRegs[83] = SystemState.all;	//下位机系统状态字
}

#endif
