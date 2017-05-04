#ifndef __RTC_H
#define __RTC_H	  

typedef struct		//时间结构体 
{
	u8 hour;
	u8 min;
	u8 sec;			
	u16 w_year;
	u8  w_month;
	u8  w_date;
	u8  week;		//公历日月年周		 
}tm;	

#ifdef __RTC_C	

tm timer;   
u8 const mon_table[12];//月份日期数据表

u8 RTC_Init(void);        //初始化RTC,返回0,失败;1,成功;
u8 Is_Leap_Year(u16 year);//平年,闰年判断
u8 RTC_Get(void);         //更新时间   
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//设置时间	  

#else

extern tm timer;   
extern u8 const mon_table[12];//月份日期数据表
	
extern u8 RTC_Init(void);
extern u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);	
	
#endif
#endif
	
	



























 
