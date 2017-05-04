#ifndef __calendar_C
#define __calendar_C

#include "stm32f10x.h"
#include "Define.h" 
#include "LCD_display.h"
#include "Menu.h"
#include "rtc.h"
#include "calendar.h"

char temp[121] = {
	"万年历                        "
	"                              "
	"                              "                              
	"                            07"};

void calendar(void)
{
	char DisData[4] = {0,0,0,0};

	sendtobuff(0,0,temp);

	hex_to_asc(DisData,timer.w_year,4,0);
	sendtobuff2(1,0,DisData,4);
	sendtobuff(1,4,"年");
	
	hex_to_asc(DisData,timer.w_month,2,0);
	sendtobuff2(1,6,DisData,2);
	sendtobuff(1,8,"月");

	hex_to_asc(DisData,timer.w_date,2,0);
	sendtobuff2(1,10,DisData,2);
	sendtobuff(1,12,"日");
								
	switch(timer.week)			 //发送星期
	{	
		case 0:sendtobuff(2,0,"星期天");break;
		case 1:sendtobuff(2,0,"星期一");break;
		case 2:sendtobuff(2,0,"星期二");break;
		case 3:sendtobuff(2,0,"星期三");break;
		case 4:sendtobuff(2,0,"星期四");break;
		case 5:sendtobuff(2,0,"星期五");break;
		case 6:sendtobuff(2,0,"星期六");break;  		
	}		

	hex_to_asc(DisData,timer.hour,2,0);
	sendtobuff2(3,0,DisData,4);
	sendtobuff(3,2,":");
	
	hex_to_asc(DisData,timer.min,2,0);
	sendtobuff2(3,3,DisData,2);
	sendtobuff(3,5,":");

	hex_to_asc(DisData,timer.sec,2,0);
	sendtobuff2(3,6,DisData,2);
	
	lcd_wrstring(0,0,DisBuff);	//显示Buff里的内容
}









#endif

