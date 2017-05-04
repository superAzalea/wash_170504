/**************************************************************************************************
Project: TTK_ACPR
Filename: X.c
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
#ifndef __buzz_C
#define __buzz_C

#include "stm32f10x.h"
#include "define.h"
#include "SystemInit.h"
#include "key.h"
#include "SysPr.h"
#include "Menu.h"
#include "usart.h"
#include "adc.h"
#include "main.h"
#include "pid.h"
#include "SystemWord.h"
#include "LCD_display.h"
#include "timer.h"
//#include "bit_define.h"
//#include "maintain.h"
//#include "warning.h"
#include "buzz.h"

//extern u8 flag_buzz;
extern unsigned char buzzer_num;
extern unsigned char buzzer_timer;
extern unsigned char _buzzer_run;
extern unsigned char _tell_alarm;
extern unsigned char _tell_stop;
extern unsigned char tell_stop_num;//提示停止的次数

//void sub_buzz(void)//维护参数
//{
//	static u8 counter_buzz_25ms = 0;
//	static u8 counter_buzz_30ms = 0;
//	static u8 counter_time = 0;
//	
//	if(KeyBuzz > 0)
//	{
//		flag_buzz = 1;
//		counter_buzz_25ms ++;
//		if(counter_buzz_25ms >= 25)
//		{
//			counter_buzz_25ms = 0;
//			flag_buzz = 0;
//			KeyBuzz = 0;
//		}
//	}
//	
//	if(_tell_alarm != 0)
//	{
//		if(counter_time < 5)
//		{
//			counter_buzz_30ms ++;
//			if(counter_buzz_30ms >= 30)
//			{
//				counter_buzz_30ms = 0;
//				counter_time ++;
//			}
//		}
//		
//		if(counter_time == 2)
//		{
//			flag_buzz = 1;//蜂鸣器响
//		}
//		else if(counter_time == 3)
//		{
//			flag_buzz = 0;
//		}
//		else if(counter_time == 4)
//		{
//			flag_buzz = 1;
//		}	
//		else if(counter_time == 5)
//		{
//			flag_buzz = 0;
//		}			
//	}
//	else
//	{
//		counter_buzz_30ms = 0;
//		counter_time = 0;
//	}
//	
//	if(flag_buzz == 1)
//	{
//		buzzer = 1;
//	}
//	else
//	{
//		buzzer = 0;
//	}
//}


void sub_buzz(void)
{ 
 if(_buzzer_run){
   if(_tell_alarm || _tell_stop){
     _tell_alarm = 0;
     _tell_stop = 0;
     buzzer_num = 0;
	   buzzer = 0;
     buzzer_timer = 0;    
   } 
   else if(buzzer_num == 3 && buzzer_timer == 0){
	   buzzer_num = 2;
	   buzzer = 1;
     buzzer_timer = 55;
   }
   else if(buzzer_num == 2 && buzzer_timer == 0){
	   buzzer_num =1;
	   buzzer = 0;
     buzzer_timer = 10;
   }
   else if(buzzer_num == 1 && buzzer_timer == 0){
     buzzer_num = 0;
     buzzer = 1;
     buzzer_timer = 55;
   } 
   else if(buzzer_num == 0 && buzzer_timer == 0){
     buzzer = 0;
     _buzzer_run = 0;
   }
 } 
 else if(_tell_alarm){
   if(buzzer_timer == 0 && buzzer_num == 0){
     buzzer = 0;
     buzzer_num = 3;
     buzzer_timer = 200;
   } 
   else if(buzzer_timer == 0 && buzzer_num == 3){
     buzzer = 1;     
     buzzer_timer = 30;
     buzzer_num = 2;   
   } 
   else if(buzzer_timer == 0 && buzzer_num == 2){
     buzzer = 0;     
     buzzer_timer = 15;
     buzzer_num = 1;
   }
   else if(buzzer_timer == 0 && buzzer_num == 1){
     buzzer = 1;     
     buzzer_timer = 70;
     buzzer_num = 0;
   }   
 }     
//停止
 if(_tell_stop){
   if(tell_stop_num == 0){
     _tell_stop = 0;
     tell_stop_num = 0;    
   }
   /*if(buzzer_timer == 0 && buzzer_num == 0){
     buzzer = 0;
     buzzer_num = 8;
     buzzer_timer = 2500;
   }*/ 
   if(buzzer_timer == 0){
     switch(buzzer_num){
       case 8:
         buzzer = 1;     
         buzzer_timer = 85;
         buzzer_num = 7;   
         break;
       case 7:
         buzzer = 0;     
         buzzer_timer = 35;
         buzzer_num = 6;
         break;
       case 6:
         buzzer = 1;     
         buzzer_timer = 85;//55;
         buzzer_num = 5;
         break;
       case 5:
         buzzer = 0;     
         buzzer_timer = 35;//20;
         buzzer_num = 4;
         break;
       case 4:
         buzzer = 1;     
         buzzer_timer = 85;
         buzzer_num = 3;
         break;
       case 3:
         buzzer = 0;     
         buzzer_timer = 35;
         buzzer_num = 2;
         break;
       case 2:
         buzzer = 1;     
         buzzer_timer = 85;
         buzzer_num = 1;
         break;
       default:
         buzzer = 0;     
         buzzer_timer = 35;
         buzzer_num = 0;
         tell_stop_num--;
         break;      
     }
   }
 } 
 if(_tell_stop == 0 && _tell_alarm == 0 && _buzzer_run == 0){
   buzzer = 0;
   buzzer_num = 0; 
 } 
}

#endif












