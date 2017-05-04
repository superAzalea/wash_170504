/**************************************************************************************************
Project: TTK_ACPR
Filename: timer.c
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
#ifndef __TIMER_C
#define __TIMER_C

#include "stm32f10x.h"
#include "define.h"
#include "SystemInit.h"
#include "Typedef.h"
#include "key.h"
#include "SysPr.h"
#include "Menu.h"
#include "usart.h"
#include "adc.h"
#include "DataHandle.h"
#include "main.h"
#include "pid.h"
#include "SystemWord.h"
#include "LCD_display.h"
#include "timer.h"
#include "buzz.h"
#include "struct_comm.h"
//#include "bit_define.h"

extern unsigned char _manual_mode; //1：手动模式
//extern unsigned char input_door_switch;
extern u8 _enable_display;
extern unsigned int main_cycle_1s_timer;
extern unsigned char flag_fault_display;
extern unsigned char _enable_commu_Inventer;
extern unsigned char usart3_flag_fault_clear;
extern unsigned int acce_time;//加数时间
extern unsigned int dece_time;//减速时间
extern unsigned int run_frequency;
extern unsigned char auto_mode_s;
extern unsigned char auto_mode_m;
extern unsigned char set_num;
extern unsigned char set_num_timer;
extern unsigned char commun_interval;//通讯间隔
extern unsigned char _1s_timer	;
extern unsigned char flag_read_Inv;
extern unsigned char buzzer_timer;
//unsigned char HmiCmd_Run = 0;			//上位机命令字 启动
//extern u8 flag_error_usart;
extern u16 counter_break_usart;
extern unsigned char HMICommuINV_OK;

#define USART1_IRQn		37
#define TIM3_IRQn		29

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//IRQ通道被使能	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器		

}

//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void Timerx_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	TIM_TimeBaseStructure.TIM_Period = (arr-1); //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = (psc-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM2
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
			
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设							 
}

void TIM3_IRQHandler(void)                 //TIM3 1ms中断 exe14us
{
	static u16 counter_rdwr = 0;
	static u16 counter_5s = 0;
	static u16 counter_2s = 0;
	//static u16 counter_fault_display = 0;
	static u16 counter_500ms = 0;
	static u16 counter_holding = 0;
//	static u16 counter_fault_500ms = 0;
	static u16 counter_reset = 0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
		if(flag_InitFinish == 1)//初始化完成后，才开始读写串口
		{
			counter_rdwr ++;
			//if(counter_rdwr == 200)//读elc
			if(counter_rdwr == 500)
			{		
				flag_500ms_rd = 1;
			}
			
			if(counter_rdwr == 800)//写多个elc寄存器
			{	
				flag_wr_more = 1;
			}
			
			if(counter_rdwr == 1200)//写单个elc寄存器
			{
				flag_1s_wr = 1;

			}			
//			if(counter_rdwr == 900)
//			{
//				flag_wr_fault_clear = 1;
//			}
			
			//if(counter_rdwr > 1000)
			if(counter_rdwr > 1800)
			{
				counter_rdwr = 0;
			}
		}
		else
		{
			counter_2s ++;
			if(counter_2s == 2000)
			{
				counter_2s = 0;
				flag_2s_WrSysPr = 1;		
			}
		}	

		if(counter_500ms++ >= 2000)
		{
			counter_500ms = 0;
			flag_500ms = 1;
		}	
		

		 if(buzzer_timer)
		 {
				 buzzer_timer--;
				 if(buzzer && buzzer_timer == 0)
				 {
					 buzzer = 0;      
				 }
			}
		
		KeyScan();//矩阵键盘扫描 exe time 7us
//		if(flag_fault)
//		{
//			if(counter_fault_500ms++ >= 1000)
//			{
//				counter_fault_500ms = 0;
//			}
//			
//			if(counter_fault_500ms < 500)
//			{
//				flag_FaultFlash = 1;//故障信息提示界面闪烁
//			}
//			else
//			{
//				flag_FaultFlash = 0;
//			}
//		}
//		else
//		{
//			counter_fault_500ms = 0;
//			flag_FaultFlash = 0;
//		}
		
		if(KeyAct)
		{
			KeyAct = 0;
			counter_5s = 0;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//如果在变频器回应数据的同时，发数据出去，串口通讯会死掉，此时可以软件复位串口外设。	
		//////////////////////////////////////////////////////////////////////////////////////////////////		

//		if(counter_reset == 0)//复位过程中不计数
//		{
//			counter_break_usart ++;
//			if(counter_break_usart >= 5000)//5s
//			{
//				counter_break_usart = 5001;
//				flag_error_usart = 1;
//			}
//		}
//		if(counter_break_usart == 0)
//		{
//			flag_error_usart = 0;
//		} 
//////////////////////////////////////////////////////////////////////////////////////////////////
//USART 上位机->下位机 写 命令下去
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//USART 上位机->下位机 读 每隔200ms 读21个数据 从地址62开始读21个数据
//////////////////////////////////////////////////////////////////////////////////////////////////		
		if(flag_InitFinish == 1)//初始化完成后，才开始读写串口
		{  	
			if(flag_500ms_rd == 1) //读：1 发握手信息 2收到信息后发读内容的命令
			{
				flag_500ms_rd = 0;
				modbus_dataUnpack_host();//解包通讯返回信息
				if((flag_writing_0x06 == 0)&&(flag_writing_0x10 == 0))
				{
					if(flag_reading_0x03 == 0)
					{
						//flag_reading_0x03 = 1;
						
						if(flag_read_Inv == 1)
						{
							usart_ReadRegister(68,7);//读变频器的值
							flag_read_Inv = 0;
						}
						else
						{
							usart_ReadRegister(58,6);//读AD采样值+下位机状态字+变频器故障码
						    counter_rd ++;
						}
					}
					
					if(flag_reading_0x03 == 0)
					{
						
						if(readINV_handshake)
						{	
							usart_ReadRegister(58,6);//读AD采样值+下位机状态字+变频器故障码
							flag_reading_0x03 = 1;
						}
						else
						{
							usart_ReadRegister(55,8);//发一个握手信息到elc
							flag_reading_0x03 = 1;
						}
						
					}
					else
					{
						if(readINV_handshake)
						{
							if(flag_rw_elc_err == 1 /*&& flag_rd03_fault == 1*/)
							{
								flag_rd03_fault = 0;
								usart_ReadRegister(58,6);
								counter_rd ++;
							}
						}
						else
						{
							if(flag_rw_elc_err == 1)
							{
								//flag_rd03_fault = 0;
								usart_ReadRegister(55,8);
								counter_rd ++;
							}
						}	
					}
					
					if(counter_rd >= 5)
					{
						flag_error_usart = 1;
					}
				}
			}

			if(flag_1s_wr == 1)
			{
				flag_1s_wr = 0;
				modbus_dataUnpack_host();//解包通讯返回信息
				if((flag_reading_0x03 == 0) && (flag_writing_0x10 == 0))//写错误清除与elc_output状态
				{
					if(usart3_flag_fault_clear == 1 )
					{
						if(flag_writing_0x06 == 0)
						{
							flag_writing_0x06 = 1;
							usart_WriteRegister(6,7);//故障清除
							//usart3_flag_fault_clear = 0;
						}
						else
						{
							if(flag_rw_elc_err == 1 /*&& flag_wr_fault == 1*/)
							{
								flag_wr_fault = 0;
								usart_WriteRegister(6,7);//故障清除

							}
						}
					}
					else
					{
						putCommSend();
						if(CommSend_All_old !=structCommSend.all  )
						{
							if(flag_writing_0x06 == 0)
							{
								flag_writing_0x06 = 1;
								usart_WriteRegister(5,structCommSend.all);
							}
							else
							{
								if(flag_rw_elc_err == 1 )
								{
									usart_WriteRegister(5,structCommSend.all);
								}
							}
							
//							if(wash_1 != wash_2)
//							{
//								if(HMICommuINV_OK == 1)
//								{
//									if(flag_writing_0x06 == 0)
//									{
//										flag_writing_0x06 = 1;
//										usart_WriteRegister(5,structCommSend.all);
//									}
//									else
//									{
//										if(flag_rw_elc_err == 1 )
//										{
//											usart_WriteRegister(5,structCommSend.all);
//										}
//									}
//								}
//							}
//							else
//							{
//								
//							}			
						}
					}	
				}
			}				
				
			if(flag_wr_more == 1)
			{
				flag_wr_more = 0;
				modbus_dataUnpack_host();//解包通讯返回信息
				if((flag_reading_0x03 == 0)&&(flag_writing_0x06 == 0))//给变频器写频率、加减速时间
				{
					if(_enable_commu_Inventer == 1)
					{
						//_enable_commu_Inventer = 0;
						if(flag_writing_0x10 == 0)// 01 10 00 02 00 03 06 00 22 01 33 0F A0 4B 36
						{
							flag_writing_0x10 = 1;
							MBS_HoldRegs[2] = acce_time;
							MBS_HoldRegs[3] = dece_time;
							MBS_HoldRegs[4] = (run_frequency / 150) * 100;//最大频率的50%
							usart_WriteMultipleRegister(2,3);	
						}
						else
						{
							if(flag_rw_elc_err == 1/* && flag_wr10_fault == 1*/)
							{
								flag_wr10_fault = 0;
								MBS_HoldRegs[2] = acce_time;
								MBS_HoldRegs[3] = dece_time;
								MBS_HoldRegs[4] = (run_frequency / 150) * 100;//最大频率的50%
								usart_WriteMultipleRegister(2,3);
							}
						}
					
					}		
				}
			}			
				
		}

		sub_InputStates();

//////////////////////////////////////////////////////////////////////////////////////////////////
//USART 实时通讯握手检测
//////////////////////////////////////////////////////////////////////////////////////////////////	
		if(flag_InitFinish == 1)//初始化完成后，才进行握手
		{
			if(modbus_check[1] != MBS_HoldRegs[1])//发握手信号
			{
				if(flag_2s_WrSysPr == 1)//1s发送一次握手信息
				{
					flag_2s_WrSysPr = 0;
					usart_WriteRegister(1,MBS_HoldRegs[1]);//发送握手信号0x5a5a
//					flag_writing = 1;
					flag_holding = 1;
				}
			}
			else
			{
				flag_holding = 0;
				modbus_check[1] = 0;
			}
			//modbus_dataUnpack_host();

		}
		
		if(flag_holding == 1)
		{
			counter_holding ++;
		}
		else
		{
			counter_holding = 0;
		}
		if(counter_holding >= 5000)
		{
			//glFaultWord.bit.usart = 1; //通讯故障		
		}
		else
		{	
			//glFaultWord.bit.usart = 0;
		}
		
		 sub_DataHandle();
		
	}
	
//	if(counter_fault_display)
//	{
//		counter_fault_display--;
//		if(counter_fault_display >= 500 && counter_fault_display <1000)
//		{
//			flag_fault_display = 0;
//			
//		}
//		if(counter_fault_display < 500 )
//		{
//			flag_fault_display = 1;
//			
//		}
//		if(counter_fault_display == 0)
//		{
//			counter_fault_display = 1000;
//			
//		}
//	}		
	if(main_cycle_1s_timer)//1000 2s
	{
		main_cycle_1s_timer--;
		
		 if(main_cycle_1s_timer == 0)
		{
			_enable_display = 1;
			_1s_timer = 1;
			if(flag_fault_display == 0)
			{
				flag_fault_display = 1;
			}
			else
			{
				flag_fault_display = 0;
			}
			//lcd_reset = 1;
			main_cycle_1s_timer = 500;
	//~~~~~~~~~~~~1s~~~~~~~~~~~~~~~~~
			if(_manual_mode)
			{ 
				if(++auto_mode_s > 59)
				{          
					if(auto_mode_m < 99)
					{
					   auto_mode_s = 0;          
					   auto_mode_m++;
					 } 
					 else
					 {
						auto_mode_s--;  
					 }
				}       
			}
		//	_enable_display = 1;
		//	_1s_timer = 1;
			if(set_num != 0)
			{
				set_num_timer--;
				if(set_num_timer == 0)
				{
					set_num = 0;
				}
			}
			if(commun_interval)
			{
				commun_interval--;      
			}
	   }
	}
}

#endif












