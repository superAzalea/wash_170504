/**************************************************************************************************
Project: TTK_ACPR
Filename: SystemInit.c
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
#ifndef __SystemInit_C
#define __SystemInit_C

#include "stm32f10x.h"
#include "SystemInit.h"

void GPIOInit(void)
{
	
    RCC->APB2ENR |= RCC_APB2Periph_GPIOA;//PA   
    GPIOA->CRL = 0x33333333;    //LCDÊý¾Ý¿ÚPA0~7=0  (ÍÆÍìÊä³ö£¬×î´óËÙ¶È50mhz)
    GPIOA->CRH &= 0x000FF00F;   //
    GPIOA->CRH |= 0x00033003;   //3:Êä³ö	LCDÊý¾Ý¿ÚPA0~7=0£¬LEDµÆPA8 PA11 PA12
	GPIOA->ODR = 0x1900;		//LCDÊý¾Ý¿ÚPA0~7=0£¬LEDµÆPA8 PA11 PA12=0£¬Ã»ÓÃµ½PA9 PA10 PA14 PA15 = 0
	//	GPIOA->ODR = 0xFFFF;
    
    RCC->APB2ENR |= RCC_APB2Periph_GPIOB;//PB
    //GPIOB->CRL &= 0xFF0FFF00;   //PB0:ADÊäÈëë
    //GPIOB->CRL |= 0x00300038;   //PB1:rs485Ê¹ÄÜ½Å£¬PB5 output
	  GPIOB->CRL &= 0xFF0FFF00;   //PB0:FSÊä³ö ()
    GPIOB->CRL |= 0x00300033;   //PB1:L-RUNÊä³ö£¬PB5 output
    //GPIOB->CRH &= 0x0000FF00;   //
    //GPIOB->CRH |= 0x33330033;   //3:Êä³ö	PB12 13 14 15:¾ØÕó¼üÅÌµÄÊä³ö²¿·Ö£¬PB9:rs485Ê¹ÄÜ½Å£¬PB8:Buzz
	  GPIOB->CRH &= 0x00000000;   //
	GPIOB->CRH |= 0x33333333;   //3:Êä³ö	PB12 13 14 15:¾ØÕó¼üÅÌµÄÊä³ö²¿·Ö£¬PB9:rs485Ê¹ÄÜ½Å£¬PB8:Buzz,PB10,PB11:29,30½ÅµÆÊä³ö¡£
    GPIOB->ODR = 0x0D22;        //
     // GPIOB->ODR = 0xFFFF;  
  
  RCC->APB2ENR |= RCC_APB2Periph_GPIOC;//PC
   // GPIOC->CRL = 0x88333333;    //PC0~5:CD RD WR CE RST BLAÊä³ö PC6 PC7:ÆÕÍ¨°´¼üÊäÈë
	  GPIOC->CRL = 0x33333333;    //PC0~5:CD RD WR CE RST BLAÊä³ö PC6 PC7:LedµÆ
    GPIOC->CRH = 0x33338888;    //PC8 9 10 11:¾ØÕó¼üÅÌµÄÊäÈë²¿·Ö PC12:FSÊä³ö 	[PC13 14 15Ê±ÖÓ²¿·Ö£¬´ý¶¨ÎªÊä³ö]
      GPIOC->ODR = 0x10C0;        //
//GPIOC->ODR = 0xFFFF;

    RCC->APB2ENR |= RCC_APB2Periph_GPIOD;//PD
    GPIOD->CRL &= 0xFFFFF0FF;
    GPIOD->CRL |= 0x00000300;	//PD2 WP EEPROMµÄ¶ÁÐ´±£»¤½Å
    GPIOD->ODR &= 0xFFFB;		//PD2
	
}

#endif

