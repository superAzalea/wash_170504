#ifndef __INVERTER_C
#define __INVERTER_C

#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "rs485.h"
#include "usart_Frequ.h"
#include "faultProcess.h"
#include "Inverter.h"
#include "time.h"

unsigned long counter_RunDelay = 0;
unsigned long counter_SleepJudge = 0;
void Inverter_control(void)
{
	//HMI发送的MBS_HoldRegs[61]，来控制电机正反转
	if((MBS_HoldRegs[61]&0x0f) == 0x01)
	{
			HmiCmd_Run = 1;  //正转
	}
	else if((MBS_HoldRegs[61]&0x0f) == 0x02)
	{
			HmiCmd_Run = 2; //反转
	}
	else if((MBS_HoldRegs[61]&0x0f) == 0x10)
	{
			HmiCmd_Run = 0;  //停止
	}
	
	if(HmiCmd_Run == 0)
	{
		OUT_X1 = 0;
		OUT_X2 = 0;
	}
	else if(HmiCmd_Run == 1)
	{
		OUT_X1 = 0;
		OUT_X2 = 1;
	}
	else if(HmiCmd_Run == 2)
	{
		OUT_X1 = 1;
		OUT_X2 = 0;
	}

}

#endif
