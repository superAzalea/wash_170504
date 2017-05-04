#ifndef __STRUCT_COMM_RECEIVE_H
#define __STRUCT_COMM_RECEIVE_H
#include "sys.h"	 

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

}StructCommReceiveWord;


#ifdef  __STRUCT_COMM_RECEIVE_C

StructCommReceiveWord structCommReceive;

void getCommReceive(void);

#else 
extern StructCommReceiveWord structCommReceive;


extern void getCommReceive(void);

#endif
#endif

