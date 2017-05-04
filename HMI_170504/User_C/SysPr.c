/**************************************************************************************************
Project: TTK_ACPR
Filename: SysPr.c
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
#ifndef __SYSPR_C
#define __SYSPR_C

#include "stm32f10x.h"
#include "define.h"
#include "delay.h" 
#include "Typedef.h"
#include "SystemInit.h" 
#include "IIC.h"
#include "24CXX.h" 
#include "LCD_display.h"
//#include "key.h"
#include "Menu.h"
#include "SysPr.h"

extern unsigned char mode_data[200]; 
extern unsigned auto_run_mode;
extern unsigned char fault[12];
char eepromFault[121] = {
"eeprom故障 请更换eeprom       "
"                              "
"                              "
"                              "
};

void RecoverFactoryPr(void)//恢复出厂参数
{
	u8 i = 0;
	unsigned char a[200];
	unsigned char *z,*j;
	z = a;
	j = (unsigned char *)P01_parameter;
	for( i = 0; i < 136; i++)
	{
			z[i] = j[i];
	}
	for( i = 0; i < 136; i++)
	{
			AT24CXX_WriteOneByte(P01_PARAMETER_E2+i,a[i]);

	}
	
	z = (unsigned char *)PP_parameter;
	j = (unsigned char *)P01_parameter;
	for(i = 0; i < 136; i++)
	{
			z[i] = j[i];
	}
	
	z = (unsigned char *)PP_parameter;
	j = (unsigned char *)Code_I.P1;
	for( i = 0; i <136; i++)
	{
			j[i] = z[i];
	}
	
}

void Init_Procedure(void)  //程序初始化
{
	 u8 i;
	 unsigned char  *P;
	 unsigned char  *z,*j;
	 P = (unsigned char *)program_one;
	
	 for(i=0;i<200;i++)
	 {
			AT24CXX_WriteOneByte(PROGRAM_ONE+i,*P++);

	 }
									 
	 //程序2
	  P = (unsigned char *)program_two;
	 
		for(i=0;i<200;i++)
	 {
			AT24CXX_WriteOneByte(PROGRAM_TWO+i,*P++);

	 }
									 
		 //程序3
	  P = (unsigned char *)program_three;
		for(i=0;i<200;i++)
	 {
			AT24CXX_WriteOneByte(PROGRAM_THREE+i,*P++);

	 }
	 
		 //程序4
	 P = (unsigned char *)program_four;
		for(i=0;i<200;i++)
	 {
			AT24CXX_WriteOneByte(PROGRAM_FOUR+i,*P++);

	 }
	 
		 //程序5
	 P = (unsigned char *)program_five;
		for(i=0;i<200;i++)
	 {
			AT24CXX_WriteOneByte(PROGRAM_FIVE+i,*P++);

	 }
									 
		z = mode_data; //给mode_data赋值
		switch(auto_run_mode){//P12[1]){
		 case 1:
		 j = (unsigned char *)program_one;
		 for(i=0;i<200;i++){
			z[i] = j[i];
		 }
		 break;
		 case 2:
		 j = (unsigned char *)program_two;
		 for(i=0;i<200;i++){
			z[i] = j[i];
		 }
		 break;
		 case 3:
		 j = (unsigned char *)program_three;
			for(i=0;i<200;i++){
			z[i] = j[i];
		 }
		 break;
		 case 4:
		 j = (unsigned char *)program_four;
			for(i=0;i<200;i++){
			z[i] = j[i];
		 }
		 break;
		 default:
		 j = (unsigned char *)program_five;
			for(i=0;i<200;i++){
			z[i] = j[i];
		 }
			 break;          
		 }
}

void init_SoftWareVison(void)
{	
	//AT24CXX_WriteOneByte(NUM_USER_PR*2,SOFTWAREVISION_L);		// 低位存低位，高位存高位		
	//AT24CXX_WriteOneByte(NUM_USER_PR*2 + 1,SOFTWAREVISION_H);	// 软件版本号在eeprom中的地址，存在参数存完的下一个	
}

void init_userPr(void)//将EEPROM中的参数读出来，赋值给用户参数数组
{	
	u8 i = 0;
//	u8 tempData_h = 0;
//	u8 tempData_l = 0;
	unsigned char  *P;
	unsigned char  *Z;
	unsigned char  *J;
	P = (unsigned char *)Code_I.P1;
	//C = (unsigned char *)PP_parameter;
	Z = mode_data;
	J = fault;
	for(i=0;i<Code_Cnt;i++)  
	{
		*P = AT24CXX_ReadOneByte(P01_PARAMETER_E2+i);
		P++;	
	}
	for(i = 0; i < 200;i++)
	{
		Z[i] = AT24CXX_ReadOneByte(PROGRAM_ONE+(auto_run_mode-1)*256+i);
	}
	for(i=0;i<6;i++)
	{
	    passportdata_buffer[i] = AT24CXX_ReadOneByte(ADD_PSWD_USER+i);
	}
	Menu =  MENUCUR0; 
	for(i = 0; i < 6; i++)
	{
		passdisp.dispdata[i] = 0;
	}
	for( i =0; i <12; i++)
	{
		J[i] = AT24CXX_ReadOneByte(ADD_FAULT_E2+i);
	}
	//passdisp.key_data = 0;
	passdisp.dispnum = 0;	
}

void SysPrInit(void)
{
	u8 temp = 0;
	u8 PrInitData = 0x55;//如果需要再次初始化EEPROM，更改此数字 0x55~0xff
	
	temp = AT24CXX_ReadOneByte(ADD_END);	//地址0x1fff，8191 最后一个地址
	if(temp != PrInitData)						//如果地址0x1fff的数据不是0x55，说明此EEPROM没有被初始化过
	{
		AT24CXX_WriteOneByte(ADD_END,PrInitData);	//将0x55写入地址0x1fff
		temp = AT24CXX_ReadOneByte(ADD_END);	//写入之后，再次校验是否写进去
		if(temp == PrInitData)
		{
			RecoverFactoryPr();//恢复出厂参数
		}
		else
		{
			lcd_wrstring(0,0,eepromFault);//显示eeprom故障
			while(1);
		}
	}
	//init_SoftWareVison();
	init_userPr();//初始化系统参数
	
}

#endif




