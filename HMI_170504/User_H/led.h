#ifndef __LED_H
#define __LED_H	 

#include "stm32f10x.h"
#include "SystemInit.h"
/**********************************************************************
LED控制代码
工作平台：LYS3-A（STM32）小板
作者：林洋电子-单片机
网站：lydz8.taobao.com
***********************************************************************/

//LED端口定义
//#define L_Wash 		PCout(12);
//#define L_Cold 		PBout(5);
//#define L_Warm 		PAout(12);
//#define L_Heat 		PAout(11);
//#define L_Soap 		PAout(8);
//#define L_Drain 	PCout(6);
//#define L_Spin 		PCout(7);
//#define L_Remove 	PBout(11);
//#define L_Run 		PBout(1);
//#define L_Stop 		PBout(10);

void LED_Init(void);//初始化

		 				    
#endif
