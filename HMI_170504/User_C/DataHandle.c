/**************************************************************************************************
Project: TTK_ACPR
Filename: DataHandle.c
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
#ifndef __DataHandle_C
#define __DataHandle_C

#include "stm32f10x.h"
#include "delay.h"
#include "define.h"
#include "key.h"
#include "usart.h"
#include "adc.h"
#include "main.h"
#include "pid.h"
#include "SystemWord.h"
#include "LCD_display.h"
#include "timer.h"
#include "DataHandle.h"
//#include "bit_define.h"
//extern unsigned char input_door_switch;
short T[131] = {
	-20,-19,-18,-17,-16,-15,-14,-13,-12,-11,
	-10, -9, -8, -7, -6, -5, -4, -3, -2, -1,
	  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
	 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
     40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
 	 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
     60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
	 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
	 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
	 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
	100,101,102,103,104,105,106,107,108,109,
	110
};

float R[131] = {
	115.266, 108.146, 101.517, 96.3423, 89.5865,  84.219,  79.311,  74.536, 70.1698, 66.0898,
	62.2756, 58.7079, 56.3694, 52.2438, 49.3161, 46.5725,      44, 41.5878, 39.8239, 37.1988,
	35.2024, 33.3269, 31.5635, 29.9058, 28.3459, 26.8778, 25.4954, 24.1932, 22.5662, 21.8094,
	20.7184, 19.6891, 18.7177, 17.8005, 16.9341, 16.1156, 15.3418, 14.6181,  13.918, 13.2631,
	12.6431, 12.0561,    11.5, 10.9731, 10.4736,      10, 9.55074, 9.12445, 8.71983, 8.33566,
    7.97078, 7.62411, 7.29464, 6.98142, 6.68355, 6.40021, 6.13059, 5.87359, 5.62961, 5.39689,
    5.17519, 4.96392, 4.76253,  4.5705, 4.38736, 4.21263, 4.04589, 3.88673, 3.73476, 3.58962,
    3.45097, 3.31847, 3.19183, 3.07075, 2.95896, 2.84421, 2.73823, 2.63682, 2.53973, 2.44677,
    2.35774, 2.27249, 2.19073, 2.11241, 2.03732, 1.96532, 1.89627, 1.83003, 1.76647, 1.70547,
	1.64691, 1.59068, 1.53668, 1.48481, 1.43498, 1.38703, 1.34105, 1.29078, 1.25423,  1.2133,
	1.17393, 1.13604, 1.09958, 1.06448, 1.03069, 0.99815, 0.96681, 0.93662, 0.90753,  0.8795,
	0.85248, 0.82643, 0.80132, 0.77709, 0.75373, 0.73119, 0.70944, 0.68844, 0.66818, 0.64862,
	0.62973, 0.61148, 0.59387, 0.57683, 0.56038, 0.54448, 0.52912, 0.51426, 0.49989,   0.486,
	0.47256
};

int get_T_from_R(float rest)
{
	int i = 0;
	int ret_t = 0;
	int r_array_len = sizeof(R)/sizeof(float);
	float error1,error2 = 0;
	//printf("rest=%.3f/r_array_len=%d\n",rest,r_array_len);
	//printf("rest[%.4f]/R0[%.4f]/R6[%.4f]\n",rest,R[0],R[r_array_len-1]);
	if(rest>R[0])
	{
		rest = 115.266;
	}else
	if(rest<R[r_array_len-1])
	{
		rest = R[r_array_len-1];
	}
	for(i=0;i<sizeof(R)/sizeof(float);i++)
	{
		//printf("i=%d/rest=%.4f/%.4f/%.4f\n",i,rest,R[i],R[i+1]);
		if(rest<R[i]&&rest>=R[i+1])
		{
			if(rest==R[i])
			{
				//printf("-1-ret_t=%d\n",T[i]);

				return T[i];
			}else
			if(rest==R[i+1])
			{
				//printf("-1-ret_t=%d\n",T[i+1]);
				return T[i+1];
			}else{
				error1 = R[i] - rest ;
				error2 = rest - R[i+1];
			//	printf("error1=%.4f/error2=%.4f\n",error1,error2);
				if(error1>=error2)
				{
					ret_t = T[i];
				}else{
					ret_t = T[i+1];
				}
				//printf("-1-ret_t=%d\n",ret_t);
			}
			
			return ret_t;
		}
	}
	return -1;	
}

void sub_DataHandle(void)
{
	 float input_R;		
	 INVCurrent_command = MBS_HoldRegs[68];//变频器当前命令
	 INVCurrent_state   = MBS_HoldRegs[69];//变频器当前状态
	 INVRun_frequency   = MBS_HoldRegs[71];//变频器运行频率
	 INVSet_frequency   = MBS_HoldRegs[72];//变频器设定频率
	 INVBus_voltage     = MBS_HoldRegs[73];//变频器母线电压
	 INVOutput_voltage  = MBS_HoldRegs[74];//变频器输出电压
	 INVOutput_current  = MBS_HoldRegs[75];//变频器输出电流	
	
	if(MBS_HoldRegs[63] != 0xFF)
	{
		INVCurrent_fault   = MBS_HoldRegs[63];//变频器当前故障
	}
	 ad_temp1           = MBS_HoldRegs[59];
	//ad_temp1 = 120;
	 input_R            = (float) 1.8*ad_temp1/(4096 - ad_temp1);
	//--------------------------azalea_test----------------------------------
	 //fact_temp          = get_T_from_R(input_R);//实际温度 	
	 //fact_liquid_height	= MBS_HoldRegs[58];//实际液位高度	
	 fact_liquid_height = 8;
	 fact_temp = 20;
	//--------------------------azalea_test_end----------------------------------

}

void sub_InputStates(void)
{
	SystemState.all = MBS_HoldRegs[62];
	if((SystemState.all & 0x01 )== 0x01)
	{
		input_door_switch = 0;
	}
	else
	{
		input_door_switch = 1;
	}
	
	if((SystemState.all & 0x02 )== 0x02)
	{
		input_shake = 0;
	}
	else
	{
		input_shake = 1;
	}
	
	if((SystemState.all&0x04 )== 0x04)
	{
		input_exigency_stop = 0;
	}
	else
	{
		input_exigency_stop = 1;
	}
	
	if((SystemState.all&0x08 )== 0x08)
	{
		input_low_liquid = 0;
	}
	else
	{
		input_low_liquid = 1;
	}
	
	if((SystemState.all&0x10 )== 0x10)
	{
		input_middle_liquid = 0;
	}
	else
	{
		input_middle_liquid = 1;
	}
	
	if((SystemState.all&0x20 )== 0x20)
	{
		input_high_liquid = 0;
	}
	else
	{
		input_high_liquid = 1;
	}
}

#endif












