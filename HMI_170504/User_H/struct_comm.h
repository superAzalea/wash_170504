#ifndef STRUCT_COMM_H
#define STRUCT_COMM_H
#include "define.h"
typedef union
{
	struct
	{
		u16  wash_1_1			:    	1; // 变频器继电器 X1
		u16  wash_2_1			:		1;// 变频器继电器 X2
		u16  soap_liquid_3_1		:		1; // 皂液3
		u16	 soap_liquid_2_1	    :		1; // 皂液2
		u16	 soap_liquid_1_1	    :		1; // 皂液1
		u16  cold_water_1	  	: 	    1; // 冷水
		u16  hot_water_1	  	:  	    1; // 热水
		u16  calefaction_1	: 	    1;//加热
		u16  drain_1	 	    : 	    1;  //排水
		u16  skid_1		    : 	    1;//刹车
		u16  door_lock_1	    :  	    1;//门锁
		u16  backup_one_1     :       1;//备用

		u16  backup_two_1     :       1; //备用
		u16  backup_three_1   :       1; //备用
		u16  backup_four_1    :       1; //备用
	    u16  backup_five_1    :       1; //备用
		
		
	}bit;
	u16 all;

}StructCommSendWord;




#ifdef __STRUCT_COMM_C

StructCommSendWord structCommSend;

u8 wash_1 = 0;
u8 wash_2 = 0;
u8 soap_liquid_3 = 0;
u8 soap_liquid_2 = 0;
u8 soap_liquid_1 = 0;
u8 cool_water = 0;
u8 hot_water = 0;
u8 calefaction = 0;
u8 drain = 0;
u8 skid = 0;
u8 door_lock = 0;
u8 backup_one = 0;
u16 CommSend_All = 0;
u16 CommSend_All_old = 0;

void putCommSend(void);

#else

extern StructCommSendWord structCommSend;

extern u8 wash_1;
extern u8 wash_2 ;
extern u8 soap_liquid_3;
extern u8 soap_liquid_2;
extern u8 soap_liquid_1;
extern u8 cool_water ;
extern u8 hot_water;
extern u8 calefaction ;
extern u8 drain ;
extern u8 skid ;
extern u8 door_lock ;
extern u8 backup_one ;
extern u16 CommSend_All;
extern u16 CommSend_All_old;


extern void putCommSend(void);

#endif

#endif
