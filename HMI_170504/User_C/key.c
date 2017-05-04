/**************************************************************************************************
Project: TTK_ACPR
Filename: Key.c
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
#ifndef __KEY_C
#define __KEY_C

#include "stm32f10x.h"
#include "SystemInit.h"

#include "key.h"
#include "Typedef.h"
//#include  "bit_define.h"
#include "stdio.h"
extern EDISPLAY DispMod;
unsigned char scan_key_timer_6s;
extern unsigned char _key_pro_ok_2;
extern unsigned char back_light_timer;//背光时间
/*unsigned char InData[] = {0x00,		//1~16数据有效
0x11,0x12,0x14,0x18,
0x21,0x22,0x24,0x28,
0x41,0x42,0x44,0x48,
0x81,0x82,0x84,0x88};
*/

unsigned char InData[] = {0x00,		//1~16数据有效
0x11,0x21,0x41,0x81,
0x12,0x22,0x42,0x82,
0x14,0x24,0x44,0x84,
0x18,0x28,0x48,0x88};


unsigned char KeyModify[17] = {0,10,4,8,13,1,5,9,15,2,6,12,14,3,7,11,16};

void KeyScan(void)
{ 	
    unsigned char PCIn = 0;
    unsigned char KeyCode = 0;
	  static unsigned char OutputNum = 0;  
    static unsigned char PBOut = 0;
	  static unsigned char KeyCount[17]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned char i = 0;
    unsigned char PortState_KeyUp = 0;
    static unsigned char Num_KeyDown = 0;
	static unsigned char KeyVal_temp = 0;		

    PCIn = (GPIOC->IDR & 0x00000f00)>>8;//get PC8 9 10 11 
    KeyCode = (PCIn<<4) | PBOut;  

    if(Num_KeyDown==0)  //上次按键弹起后再进行下次检测
    {    	
			 for(i = 1; i <= 16; i++)
				{
						if(KeyCode == InData[i])//±?à′ê??a?ùμ?
					
						{
								Num_KeyDown = i; 
								break;
						}

				}
					
    }
    else
    {
        switch(Num_KeyDown)
        {
            case 1:case 2:case 3:case 4:	PortState_KeyUp = 0x01;	break;  
					case 5:case 6:case 7:case 8:case 20:	PortState_KeyUp = 0x02;	break;
            case 11:case 12:case 9:case 10:	PortState_KeyUp = 0x04;	break;
            case 13:case 14:case 15:case 16:PortState_KeyUp = 0x08;	break; 
            default:break;               
        }		

        if(KeyCount[Num_KeyDown] < 255)   KeyCount[Num_KeyDown]++;  //防止数据溢出
        if((KeyCount[Num_KeyDown] >= 10) && (KeyCode == PortState_KeyUp))    //10ms延时防抖动 高4位是0按键已经弹起		
        {
            KeyVal_temp = Num_KeyDown;
						KeyCount[Num_KeyDown] = 0;
						Num_KeyDown = 0;
						KeyVal_temp = KeyModify[KeyVal_temp];//修正因硬件改动引起的按键返回值的变化	 ?
						KeyVal = KeyVal_temp;
						KeyAct = KeyVal_temp;		//用来记录多久时间没有进行键盘操作
						KeyCmd = KeyVal_temp;
						KeyBuzz = KeyVal_temp;

        }      
    }	
	 
    if(OutputNum++ == 3)    OutputNum = 0;
    switch(OutputNum)
    {
        case 0:
        {
            PBout(15) = 0;
            PBout(14) = 0;
            PBout(13) = 0;
            PBout(12) = 1;  
            PBOut = 0x01;            
        }break;
        case 1:
        {
            PBout(15) = 0;
            PBout(14) = 0;
            PBout(13) = 1;
            PBout(12) = 0; 
            PBOut = 0x02;
        }break;   
        case 2:
        {
            PBout(15) = 0;
            PBout(14) = 1;
            PBout(13) = 0;
            PBout(12) = 0; 
            PBOut = 0x04;
        }break; 
        case 3:
        {
            PBout(15) = 1;
            PBout(14) = 0;
            PBout(13) = 0;
            PBout(12) = 0; 
            PBOut = 0x08;
        }break;
        default:break;
    }
		
		if(KeyVal)
		{
				back_light_timer = 30;
		}
}


#endif




