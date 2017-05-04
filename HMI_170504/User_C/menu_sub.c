/**************************************************************************************************
Project: TTK_ACPR
Filename: menu_sub.c
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
#ifndef __MENU_SUB_C
#define __MENU_SUB_C

#include "stm32f10x.h"
#include "delay.h"
#include "Menu.h"
#include "IIC.h"
#include "24CXX.h" 
#include "LCD_display.h"
#include "key.h"
#include "SysPr.h"
#include "adc.h"
#include "rtc.h"
#include "faultProcess.h"
#include "main.h"
#include "usart.h"
#include "timer.h"
#include "menu_sub.h"

void ClearDisBuff()
{
	u8 i = 0;
	for(i=0;i<121;i++)	DisBuff[i] = 0x20;	
};
void Menu_SoftwareVersion()
{
	char DisData[2] = {0,0};

	UserPrData_L = AT24CXX_ReadOneByte(NUM_USER_PR*2);		
	UserPrData_H = AT24CXX_ReadOneByte(NUM_USER_PR*2 + 1);	//低位存低位，高位存高位

	hex_to_asc(DisData,UserPrData_H,1,0);	
	sendtobuff_n(2,1,DisData,1);				// 高位
	sendtobuff(2,2,".");					//小数点
	hex_to_asc(DisData,UserPrData_L,2,0);	// 低位
	sendtobuff_n(2,3,DisData,2);	
}
//void Menu_FaultRecord(void)
//{
//	
//}

void M_999(void)//主界面
{
	char DisData[5] = {0,0,0,0,0};
//	static u8 flag_flash = 0;



//===============================================通讯测试界面===============================//

	//sendtobuff(0,10,"通讯测试");
	sendtobuff(0,10,"        ");
	
	//sendtobuff(1,0,"测试时间:");
	sendtobuff(1,0,"        :");
	hex_to_asc(DisData,(counter_rd/5),9,0);//counter_wr
	sendtobuff(1,9,DisData);
	//sendtobuff(1,19,"(秒)");
	sendtobuff(1,19,"(  )");

	sendtobuff(2,0,"write:");
	hex_to_asc(DisData,counter_wr,9,0);//counter_wr
	sendtobuff(2,6,DisData);
	
	sendtobuff(2,16,"ok:");
	hex_to_asc(DisData,counter_wr_ok,9,0);//counter_wr_ok
	sendtobuff(2,19,DisData);

	sendtobuff(3,0,"read :");
	hex_to_asc(DisData,counter_rd,9,0);//counter_rd
	sendtobuff(3,6,DisData);
	
	sendtobuff(3,16,"ok:");
	hex_to_asc(DisData,counter_rd_ok,9,0);//counter_rd_ok
	sendtobuff(3,19,DisData);

//===============================================通讯测试界面===============================//
	
/*	
	if(SystemFault.bit.butttery_lv == 1)// 电池欠压
	{
		flag_flash ^= 1;
		if(flag_flash)	sendtobuff(3,29,"$");//电池电压图标
		else	sendtobuff(3,29,"&");
	}
	else
	{
		sendtobuff(3,29,"&");
	}
*/	
}

void M_14(void)//运行时间动态显示部分
{
	char DisData[2] = {0,0};

	hex_to_asc(DisData,timer.hour,2,0);		//时
	sendtobuff(1,13,DisData);
	sendtobuff(1,15,"小时");
	
	hex_to_asc(DisData,timer.min,2,0);		//分
	sendtobuff(1,19,DisData);
	sendtobuff(1,21,"分");
	
	hex_to_asc(DisData,timer.hour,2,0);		//时
	sendtobuff(2,13,DisData);
	sendtobuff(2,15,"小时");
	
	hex_to_asc(DisData,timer.min,2,0);		//分
	sendtobuff(2,19,DisData);
	sendtobuff(2,21,"分");	
};
void M_15(void)//万年历动态显示部分
{
	char DisData[4] = {0,0,0,0};
	char WeekData[7][3] = {{"天"},{"一"},{"二"},{"三"},{"四"},{"五"},{"六"}};
	char (*p)[3];

	p = WeekData;

	hex_to_asc(DisData,timer.w_year,4,0);	//年
	sendtobuff_n(1,0,DisData,4);
	
	hex_to_asc(DisData,timer.w_month,2,0);	//月
	sendtobuff_n(1,6,DisData,2);

	hex_to_asc(DisData,timer.w_date,2,0);	//日
	sendtobuff_n(1,10,DisData,2);
		
	sendtobuff(2,4,*(p+timer.week));		//星期

	hex_to_asc(DisData,timer.hour,2,0);		//时
	sendtobuff_n(3,0,DisData,2);
	
	hex_to_asc(DisData,timer.min,2,0);		//分
	sendtobuff_n(3,3,DisData,2);

	hex_to_asc(DisData,timer.sec,2,0);		//秒
	sendtobuff_n(3,6,DisData,2);	
	
	lcd_wrstring(0,0,DisBuff);	//显示Buff里的内容
};
void M_16(void)//电池电压
{
	u8 BatteryVoltag = 0;
	char TempArray[3] = {0,0,0};

	BatteryVoltag = (u32)Adc_BatteryVoltag*33 >> 12;
	hex_to_asc(TempArray,BatteryVoltag,2,1);
	
	sendtobuff(1,0,TempArray);
	sendtobuff(1,3,"V");	
}
void Menu_SetCalendar()//设置万年历
{/***
	u8 i = 0;
	u8 Counter = 0;
	char TempArray[2] = {0,0};
	char data_keyin[6] = {0,0,0,0,0,0};
	char data_disply[6] = {0,0,0,0,0,0};
	u8 temp1 = 0,temp2 = 0,temp3 = 0;

	if(MenuIndex == 361)
	{
		temp1 = timer.w_year - 2000;
		temp2 = timer.w_month;
		temp3 = timer.w_date;
	}
	else
	{
		temp1 = timer.hour;
		temp2 = timer.min;
		temp3 = timer.sec;		
	}
	hex_to_asc(TempArray,temp1,2,0);
	sendtobuff_n(1,7,TempArray,2);
	hex_to_asc(TempArray,temp2,2,0);
	sendtobuff_n(1,11,TempArray,2);
	hex_to_asc(TempArray,temp3,2,0);
	sendtobuff_n(1,15,TempArray,2);			//当前值

	while(1)
	{
		if((KeyVal)&&(KeyVal <= 10))//按键1~10代表数字1234567890 设置参数
		{								
			if(KeyVal == KEY_0)	KeyVal = 0;//按键10代表数字0
			data_keyin[Counter] = KeyVal;
			if(++Counter >= 6) Counter = 0;
			KeyVal = 0;
			sendtobuff(3,17,"设定中..");	// 将"设定中.."写入buff
		}
		
		for(i=0;i<6;i++)	data_disply[i] = data_keyin[i] + 0x30;//将数字转换为ASCII码
		
		if(KeyVal == KEY_SET)
		{
			if(MenuIndex == 361)
			{
				timer.w_year = 2000 + data_keyin[0]*10 + data_keyin[1];
				timer.w_month = data_keyin[2]*10 + data_keyin[3];
				timer.w_date = data_keyin[4]*10 + data_keyin[5];
			}
			else
			{
				timer.hour = data_keyin[0]*10 + data_keyin[1];
				timer.min = data_keyin[2]*10 + data_keyin[3];
				timer.sec = data_keyin[4]*10 + data_keyin[5];					
			}
			RTC_Set(timer.w_year,timer.w_month ,timer.w_date,timer.hour,timer.min,timer.sec);  //设置日期、时间
			
			sendtobuff(3,17,"设定完成");	// 将"设定完成"写入buff
			lcd_wrstring(0,0,DisBuff);		// 显示buff里的所有内容
			delay_ms(1000);					// 延时显示"设定完成"
			sendtobuff(3,17,"        ");	// 清除buff里的"设定完成"
			
			Counter = 0;
			MenuIndex = MenuIndex/10;//返回上级菜单
			KeyVal = 0;
			break;
		}											
		if(KeyVal == KEY_ESC)
		{
			Counter = 0;
			MenuIndex = MenuIndex/10;//返回上级菜单
			KeyVal = 0;
			break;			
		}
		switch(Counter)
		{
			case 0:sendtobuff(3,7,"_         ");break;
			case 1:sendtobuff(3,7," _        ");break;
			case 2:sendtobuff(3,7,"    _     ");break;
			case 3:sendtobuff(3,7,"     _    ");break;
			case 4:sendtobuff(3,7,"        _ ");break;
			case 5:sendtobuff(3,7,"         _");break;
		}
		sendtobuff_n(2,7,&data_disply[0],1);
		sendtobuff_n(2,8,&data_disply[1],1);
		sendtobuff_n(2,11,&data_disply[2],1);
		sendtobuff_n(2,12,&data_disply[3],1);
		sendtobuff_n(2,15,&data_disply[4],1);
		sendtobuff_n(2,16,&data_disply[5],1);	//设定值	
		
		lcd_wrstring(0,0,DisBuff);// 显示buff里的内容
	}  ***/
}

void Menu_UserPrPro(void)//用户参数处理
{  /****
	u8 i = 0;
	static u8 Flag_FirstSetPrData = 0;//首次设置参数值标志位   	
	static u16 UserPr_data_z = 0;
	
	UserPr_data_z = UserPr[UserPrIndex][DATA];//在数据刷新前，先保存旧值，以便设定超限时使用旧值。
	
	
	if((KeyVal)&&(KeyVal <= 10))//按键1~10代表数字1234567890 设置参数
	{				
		if(KeyVal == KEY_0)	KeyVal = 0;//按键10代表数字0
		
		flag_prSetting = 1;//设置参数标志位置1
		
		if(Flag_FirstSetPrData == 0)	//第一次进入设置参数值状态
		{
			Flag_FirstSetPrData = 1;	//在离开此状态时，记得清楚该标志位
			Counter_KeyIn = 0;
			for(i=0;i<5;i++)	Data_KeyIn[i] = 0;//首次进入设置参数值状态时，将之前键盘设定的临时值全部清零
								
			switch(UserPr[UserPrIndex][UNIT])
			{
				case U_BIT  :	{UserPrNum = 1;}break;//Bit
				
				case U_Hz :
				case U_C  :
				case U_Per:
				case U_344:	{UserPrNum = 2;}break;//百分比 秒

								 
				case U_MPa:				
				case U_Kw :
				case U_S:
				case U_343:	{UserPrNum = 3;}break;//秒 压力 温度 频率
				
				case U_PI :
				case U_V  :
				case U_A  :
				case U_H  :	{UserPrNum = 4;}break;//KpKi 电压  电流 小时 秒 
				
				default:break;
			}
		}							
		Data_KeyIn_Dis[Counter_KeyIn] = KeyVal;
		Counter_KeyIn++;
		if(Counter_KeyIn >= UserPrNum)	Counter_KeyIn = 0;		
		KeyVal = 0;
	} 
	
	if(KeyVal == KEY_SET)//参数设定状态，按下ENTER
	{	
		if(flag_prSetting)//参数设定状态，按下ENTER
		{	
			Flag_FirstSetPrData = 0;
			flag_prSetting = 0;//按下确认键后，将正在设置参数标志位清0	
			for(i=0;i<5;i++)
			{
				Data_KeyIn[i] = Data_KeyIn_Dis[i];
			}
			for(i=0;i<5;i++)	Data_KeyIn_Dis[i] = 0;//按下确认键后，将记录按键的数组清0
	
			switch(UserPr[UserPrIndex][UNIT])
			{					
				case U_BIT  :UserPr[UserPrIndex][DATA] = Data_KeyIn[0];break;

				case U_Hz :
				case U_C  :
				case U_Per:
				case U_344:	UserPr[UserPrIndex][DATA] = Data_KeyIn[0]*10 + Data_KeyIn[1];break;//Hz 温度 百分比
				
				case U_343:
				case U_MPa:
				case U_Kw :
				case U_S:	UserPr[UserPrIndex][DATA] = Data_KeyIn[0]*100 + Data_KeyIn[1]*10 + Data_KeyIn[2];break;				
				
				case U_PI :
				case U_V  :
				case U_A  :
				case U_H  :	UserPr[UserPrIndex][DATA] = Data_KeyIn[0]*1000 + Data_KeyIn[1]*100 + Data_KeyIn[2]*10 + Data_KeyIn[3];break;				
				
				default:break;					
			}
		
			if((UserPr[UserPrIndex][DATA] <= UserPr[UserPrIndex][MAX])&&(UserPr[UserPrIndex][DATA] >= UserPr[UserPrIndex][MIN]))//参数设定正确
			{			
				Flag_FirstSetPrData = 0;				
				flag_firstInSetPrPage = 0;	
				
				UserPrData = UserPr[UserPrIndex][DATA];	//如果设置值正确，保存起来
				UserPrData_H = (UserPrData>>8)&0xff;//高8位
				UserPrData_L = UserPrData&0xff;//低8位	
				
				MBS_HoldRegs[UserPrIndex] = UserPr[UserPrIndex][DATA];
				
				AT24CXX_WriteOneByte(UserPrIndex*2,UserPrData_L);//写入EEPROM 低8位写入低地址
				AT24CXX_WriteOneByte(UserPrIndex*2+1,UserPrData_H);//写入EEPROM	高8位写入高地址

				MBS_HoldRegs[90] = UserPrIndex;					//90放地址
				MBS_HoldRegs[91] = UserPr[UserPrIndex][DATA];	//91放数据
				flag_writing_0x10 = 1;
				
 				if(UserPrIndex == 59)//厂家参数复位
 				{
					flag_warning = 12;//厂家参数恢复成功
 				}
			}
			else//参数设定超限
			{
				UserPr[UserPrIndex][DATA] = UserPr_data_z;//设定超限时使用旧值
				Flag_FirstSetPrData = 0;				
				flag_firstInSetPrPage = 0;

				flag_warning = 2;//设定超限			
			}
		}
		else//非参数设定状态(为了防止EEPROM数据错误，使得用户参数数组的值在初始化后就是超限的)
		{
			if((UserPr[UserPrIndex][DATA] <= UserPr[UserPrIndex][MAX])&&(UserPr[UserPrIndex][DATA] >= UserPr[UserPrIndex][MIN]))//参数设定正确
			{
				UserPrData = UserPr[UserPrIndex][DATA];	//如果设置值正确，保存起来
				UserPrData_H = (UserPrData>>8)&0xff;//高8位
				UserPrData_L = UserPrData&0xff;//低8位	
				
				MBS_HoldRegs[UserPrIndex] = UserPr[UserPrIndex][DATA];
				
				AT24CXX_WriteOneByte(UserPrIndex*2,UserPrData_L);//写入EEPROM 低8位写入低地址
				AT24CXX_WriteOneByte(UserPrIndex*2+1,UserPrData_H);//写入EEPROM	高8位写入高地址
				
				MBS_HoldRegs[90] = UserPrIndex;					//90放地址
				MBS_HoldRegs[91] = UserPr[UserPrIndex][DATA];	//91放数据
				flag_writing_0x10 = 1;

				flag_warning = 11;//设定正确
			}
			else//参数设定超限
			{
				flag_warning = 2;//设定超限
 				UserPr[UserPrIndex][DATA] = UserPr_data_z;//设定超限时使用旧值(此时旧值为0)
			}
			Flag_FirstSetPrData = 0;				
			flag_firstInSetPrPage = 0;					
		}
		KeyVal = 0;
	}
	if(KeyVal == KEY_ESC) 
	{	
		MenuIndex = MenuIndex/10;//求出上一级菜单				
		Flag_FirstSetPrData = 0;	
		flag_prSetting = 0;//按下ESC键后，将正在设置参数标志位清0				
		flag_firstInSetPrPage = 0;
		for(i=0;i<5;i++)	Data_KeyIn_Dis[i] = 0;//按下ESC键后，将记录按键的数组清0
		KeyVal = 0;	
	}   ****/
}

void Menu_UserPrDis(void)//用户参数显示
{  /****
	char DisTemp[5] = {0,0,0,0,0};
	char temp_data[5] = {0,0,0,0,0};
	char temp_max[5] = {0,0,0,0,0};
	char temp_min[5] = {0,0,0,0,0};
	u8 data_num = 0,data_n = 0,data_dis = 0;//数据位数、小数位数、要显示的数据(带小数点)
 
	if(0 == flag_firstInSetPrPage)
	{
		flag_firstInSetPrPage = 1;
	}			
	if(flag_prSetting == 0)//非正在设置参数状态
	{	
		switch(UserPr[UserPrIndex][UNIT])
		{
			case U_BIT	:{data_num=1;data_n=0,data_dis=data_num;}break;//1 0
			
			case U_Hz	:{data_num=2;data_n=0,data_dis=data_num;}break;
			case U_C	:{data_num=2;data_n=0,data_dis=data_num;}break;			
			case U_Per	:{data_num=2;data_n=0,data_dis=data_num;}break;
			case U_344	:{data_num=2;data_n=0,data_dis=data_num;}break;//2 0

			case U_S	:{data_num=3;data_n=0,data_dis=data_num;}break;
			case U_343	:{data_num=3;data_n=0,data_dis=data_num;}break;//3 0		
			
			case U_Kw	:{data_num=3;data_n=1,data_dis=data_num+1;}break;//3 1
			case U_MPa	:{data_num=3;data_n=2,data_dis=data_num+1;}break;//3 2
			
			case U_PI	:{data_num=4;data_n=0,data_dis=data_num;}break;
			case U_H	:{data_num=4;data_n=0,data_dis=data_num;}break;//4 0
			
			case U_V	:{data_num=4;data_n=1,data_dis=data_num+1;}break;
			case U_A	:{data_num=4;data_n=1,data_dis=data_num+1;}break;//4 1
			
			default:break;			
		}
		
//将数据转换成单个字符
		hex_to_asc(temp_data,UserPr[UserPrIndex][DATA],data_num,data_n);//数据
		hex_to_asc(temp_max,UserPr[UserPrIndex][MAX],data_num,data_n);//最大值
		hex_to_asc(temp_min,UserPr[UserPrIndex][MIN],data_num,data_n);//最小值

//往显示buffer中写入数据
		sendtobuff(0,0,Tab_MenuData[search(Tab_MenuNum,NUM_PAGE,MenuIndex)]);//菜单的静态内容
		sendtobuff(2,7,temp_data);//数据
		if((UserPr[UserPrIndex][UNIT] != U_BIT))//单位是bit的，只有0和1两个值
		{
			sendtobuff(1,7,temp_max);//最大值
			sendtobuff(1,22,temp_min);//最小值
			sendtobuff(2,7+data_dis,Tab_unit[UserPr[UserPrIndex][UNIT]]);//单位
		}

		lcd_wrstring(0,0,DisBuff);//LCD显示函数
	}
	if(flag_prSetting)//正在设置参数状态
	{
		if(flag_shining == 0)//显示参数
		{
			
			switch(UserPr[UserPrIndex][UNIT])
			{
	//1位整数			
				case U_BIT :	
				{
					DisTemp[0] = Data_KeyIn_Dis[0]+0x30;
					sendtobuff(2,7,DisTemp);				
				}break;
	//1位整数 2位小数
				case U_MPa:	//压力
				{
					DisTemp[0] = Data_KeyIn_Dis[0]+0x30;
					DisTemp[1] = 0x2e;//小数点					
					DisTemp[2] = Data_KeyIn_Dis[1]+0x30;
					DisTemp[3] = Data_KeyIn_Dis[2]+0x30;	
					sendtobuff(2,7,DisTemp);
				}break;
	//2位整数			
				case U_Hz:		//频率
				case U_C :		//温度
				case U_344:
				case U_Per:	//百分比
				{
					DisTemp[0] = Data_KeyIn_Dis[0]+0x30;
					DisTemp[1] = Data_KeyIn_Dis[1]+0x30;
					sendtobuff(2,7,DisTemp);					
				}break;				
	//2位整数 1位小数			
				case U_Kw:							
				{
					DisTemp[0] = Data_KeyIn_Dis[0]+0x30;				
					DisTemp[1] = Data_KeyIn_Dis[1]+0x30;
					DisTemp[2] = 0x2e;		//小数点
					DisTemp[3] = Data_KeyIn_Dis[2]+0x30;
					sendtobuff(2,7,DisTemp);				
				}break;	
	//3位整数	
				case U_343:
				case U_S:	//秒 
				{		
					DisTemp[0] = Data_KeyIn_Dis[0]+0x30;
					DisTemp[1] = Data_KeyIn_Dis[1]+0x30;
					DisTemp[2] = Data_KeyIn_Dis[2]+0x30;	
					sendtobuff(2,7,DisTemp);					
				}break;				
	//3位整数 1位小数
				case U_V :	//电压
				case U_A :	//电流
				{
					DisTemp[0] = Data_KeyIn_Dis[0]+0x30;
					DisTemp[1] = Data_KeyIn_Dis[1]+0x30;				
					DisTemp[2] = Data_KeyIn_Dis[2]+0x30;
					DisTemp[3] = 0x2e;		//小数点
					DisTemp[4] = Data_KeyIn_Dis[3]+0x30;		
					sendtobuff(2,7,DisTemp);			
				}break;	
	//4位整数			
				case U_PI:	//PI参数
				case U_Num4://4位数字
				case U_H :	//小时				
				{
					DisTemp[0] = Data_KeyIn_Dis[0]+0x30;
					DisTemp[1] = Data_KeyIn_Dis[1]+0x30;				
					DisTemp[2] = Data_KeyIn_Dis[2]+0x30;
					DisTemp[3] = Data_KeyIn_Dis[3]+0x30;
					sendtobuff(2,7,DisTemp);				
				}break;			
				
				default:break;
			}
		}
		else//显示空白(设定参数时的闪烁效果)
		{
			switch(UserPr[UserPrIndex][UNIT])
			{
	//1位整数			
				case U_BIT :	
				{				
					sendtobuff(2,7," ");				
				}break;
	//1位整数 2位小数
				case U_MPa:	//压力
				{
					switch(Counter_KeyIn)
					{
						case 0:sendtobuff(2,7," ");break;
						case 1:sendtobuff(2,9," ");break;
						case 2:sendtobuff(2,10," ");break;
					}
				}break;
	//2位整数			
				case U_Hz:		//频率
				case U_C :		//温度
				case U_344:
				case U_Per:	//百分比
				{
					switch(Counter_KeyIn)
					{
						case 0:sendtobuff(2,7," ");break;
						case 1:sendtobuff(2,8," ");break;
					}					
				}break;				
	//2位整数 1位小数			
				case U_Kw:							
				{
					DisTemp[0] = Data_KeyIn_Dis[0]+0x30;				
					DisTemp[1] = Data_KeyIn_Dis[1]+0x30;
					DisTemp[2] = 0x2e;		//小数点
					DisTemp[3] = Data_KeyIn_Dis[2]+0x30;
					sendtobuff(2,7,DisTemp);
					switch(Counter_KeyIn)
					{
						case 0:sendtobuff(2,7," ");break;
						case 1:sendtobuff(2,8," ");break;
						case 2:sendtobuff(2,10," ");break;
					}					
				}break;	
	//3位整数	
				case U_343:
				case U_S:	//秒 
				{		
					switch(Counter_KeyIn)
					{
						case 0:sendtobuff(2,7," ");break;
						case 1:sendtobuff(2,8," ");break;
						case 2:sendtobuff(2,9," ");break;
					}					
				}break;				
	//3位整数 1位小数
				case U_V :	//电压
				case U_A :	//电流
				{
					switch(Counter_KeyIn)
					{
						case 0:sendtobuff(2,7," ");break;
						case 1:sendtobuff(2,8," ");break;
						case 2:sendtobuff(2,9," ");break;
						case 3:sendtobuff(2,11," ");break;
					}				
				}break;	
	//4位整数			
				case U_PI:	//PI参数
				case U_Num4://4位数字
				case U_H :	//小时				
				{
					DisTemp[0] = Data_KeyIn_Dis[0]+0x30;
					DisTemp[1] = Data_KeyIn_Dis[1]+0x30;				
					DisTemp[2] = Data_KeyIn_Dis[2]+0x30;
					DisTemp[3] = Data_KeyIn_Dis[3]+0x30;
					sendtobuff(2,7,DisTemp);				
					switch(Counter_KeyIn)
					{
						case 0:sendtobuff(2,7," ");break;
						case 1:sendtobuff(2,8," ");break;
						case 2:sendtobuff(2,9," ");break;
						case 3:sendtobuff(2,10," ");break;
					}				
				}break;			
				
				default:break;
			}		
		}	
		lcd_wrstring(0,0,DisBuff);
	}	  ****/
}

void Menu_FactoryPrPro(void)//厂家参数
{
}
void Menu_DebugPrPro(void)//调试参数
{
}
void Menu_FaultRecordPro(void)//故障记录
{
}
void Menu_UserPasswordPro(void)//用户密码
{
}

void Password_Check(void)//密码校验
{/***
	u8 add = 0,index_CheckOk = 0;
	u16 data[4] = {0,0,0,0};
	static u8 KeyIn[4] = {0,1,2,3};
	char a[4] = {0,0,0,0};
	static u8 counter = 0;
	static u8 flag_firstin= 0;

	while(1)
	{	
		if(flag_firstin == 0)//首次进入
		{		
			flag_firstin = 1;
			KeyVal = 0;
			counter = 0;
			KeyIn[0] = KeyIn[1] = KeyIn[2] = KeyIn[3] = 0;
			ClearDisBuff(); 
			sendtobuff(0,0,"请输入密码");
		}
		
		if((KeyVal>0)&&(KeyVal<=10))
		{
			if(KeyVal==10)	KeyVal = 0;
			
			KeyIn[counter] = KeyVal;
			counter++;
			if(counter >= 4)	counter = 0;		
			KeyVal = 0;
		}
		a[0] = KeyIn[0] + 0x30;
		a[1] = KeyIn[1] + 0x30;
		a[2] = KeyIn[2] + 0x30;
		a[3] = KeyIn[3] + 0x30;
		sendtobuff_n(2,13,a,4);
		sendtobuff_n(3,13,"    ",4);
		sendtobuff(3,13+counter,"_");
		switch(MenuIndex)
		{
			case 2 :{index_CheckOk = 0;add = ADD_PSWD_USER;}break;
			case 3 :{index_CheckOk = 1;add = ADD_PSWD_FACTORY;}break;
			case 51:{index_CheckOk = 2;add = ADD_PSWD_USER;}break;
			case 52:{index_CheckOk = 3;add = ADD_PSWD_FACTORY;}break;
			case 53:{index_CheckOk = 4;add = ADD_PSWD_SPUSER;}break;
			case 54:{index_CheckOk = 5;add = ADD_PSWD_SPFACTORY;}break;
			default:break;
		}				
		if(KeyVal == KEY_SET)
		{
			data[0] = AT24CXX_ReadOneByte(add + 0);
			data[1] = AT24CXX_ReadOneByte(add + 1);	
			data[2] = AT24CXX_ReadOneByte(add + 2);	
			data[3] = AT24CXX_ReadOneByte(add + 3);
			
			if((KeyIn[0] == data[0])&&(KeyIn[1] == data[1])&&(KeyIn[2] == data[2])&&(KeyIn[3] == data[3]))
			{
				lcd_wrstring(3,22,"密码正确");
				CheckOk[index_CheckOk] = 1;
			}
			else
			{
				lcd_wrstring(3,22,"密码错误");								
				CheckOk[index_CheckOk] = 0;
				MenuIndex = MenuIndex/10;	//返回上级
			}	
			delay_ms(1000);
			lcd_wrstring(3,22,"        ");
			KeyVal = 0;
			flag_firstin = 0;
			break;
		}
		if(KeyVal == KEY_ESC)
		{
			KeyVal = 0;
			flag_firstin = 0;	
			MenuIndex = MenuIndex/10;		//返回上级		
			break;
		}	
		lcd_wrstring(0,0,DisBuff);	
	}***/
}
void Password_Set(void)//密码设定
{  /***
	u8 add = 0,index = 0;
	static u8 KeyIn[4] = {0,1,2,3};
	char a[4] = {0,0,0,0};
	static u8 counter = 0;
	static u8 flag_firstin= 0;

	while(1)
	{
		if(flag_firstin==0)
		{		
			flag_firstin = 1;
			counter = 0;
			KeyIn[0] = KeyIn[1] = KeyIn[2] = KeyIn[3] = 0;
		}
		ClearDisBuff(); 
		sendtobuff(0,0,"请输入新密码");

		if((KeyVal>0)&&(KeyVal<=10))
		{
			if(KeyVal==10)	KeyVal = 0;
			
			KeyIn[counter] = KeyVal;
			counter++;
			if(counter >= 4)	counter = 0;  
			
			KeyVal = 0;
		}
		a[0] = KeyIn[0] + 0x30;
		a[1] = KeyIn[1] + 0x30;
		a[2] = KeyIn[2] + 0x30;
		a[3] = KeyIn[3] + 0x30;
		sendtobuff_n(2,13,a,4);
		sendtobuff_n(3,13,"    ",4);
		sendtobuff(3,13+counter,"_");
		
		switch(MenuIndex)
		{
			case 2 :{index = 0;add = ADD_PSWD_USER;}break;
			case 51:{index = 1;add = ADD_PSWD_USER;}break;
			case 3 :{index = 2;add = ADD_PSWD_FACTORY;}break;
			case 52:{index = 3;add = ADD_PSWD_FACTORY;}break;
			case 53:{index = 4;add = ADD_PSWD_SPUSER;}break;
			case 54:{index = 5;add = ADD_PSWD_SPFACTORY;}break;
			default:break;
		}	
		
		if(KeyVal == KEY_SET)
		{
			AT24CXX_WriteOneByte(add + 0,KeyIn[0]);
			AT24CXX_WriteOneByte(add + 1,KeyIn[1]);	
			AT24CXX_WriteOneByte(add + 2,KeyIn[2]);	
			AT24CXX_WriteOneByte(add + 3,KeyIn[3]);
	  
			lcd_wrstring(3,22,"设定完成");
			delay_ms(1000);
			lcd_wrstring(3,22,"        ");
			
			KeyVal = 0;
			flag_firstin = 0; 
			MenuIndex = MenuIndex/10;//返回上级		
			break;			
		}
		if(KeyVal == KEY_ESC)
		{
			KeyVal = 0;
			flag_firstin = 0;			
			MenuIndex = MenuIndex/10;//返回上级		
			break;
		}
		if(flag_5sKeyNoAct)
		{
			flag_5sKeyNoAct = 0;
			CheckOk[index] = 0;
			flag_firstin = 0;
			MenuIndex = 999;
			break;
		}		
		lcd_wrstring(0,0,DisBuff);
	}  ***/
}

void Password_Show(void)
{  /***
	u8 add = 0;
	char data[4] = {0,0,0,0};

	ClearDisBuff(); 
	switch(MenuIndex)
	{
		case 53:add = ADD_PSWD_USER;break;
		case 54:add = ADD_PSWD_FACTORY;break;
		default:break;
	}

	data[0] = AT24CXX_ReadOneByte(add + 0) + 0x30;
	data[1] = AT24CXX_ReadOneByte(add + 1) + 0x30;	
	data[2] = AT24CXX_ReadOneByte(add + 2) + 0x30;	
	data[3] = AT24CXX_ReadOneByte(add + 3) + 0x30;
	
	sendtobuff(0,0,"用户参数密码:");
	sendtobuff_n(0,13,data,4);

	lcd_wrstring(0,0,DisBuff);
	
	if(KeyVal == KEY_ESC)
	{
		KeyVal = 0;
		MenuIndex = 5;
		if(CheckOk[4] == 1)	CheckOk[4] = 0;	//查询密码，不保留校验OK状态
		if(CheckOk[5] == 1)	CheckOk[5] = 0;
	}	  ****/
}

#endif


