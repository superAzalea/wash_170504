#ifndef __INPUT_SIGNAL_H

#define __INPUT_SIGNAL_H

typedef union
{
	struct
	{
		u16 X1					:	1;//门开关信号
		u16 X2					:	1;//超震信号
		u16 X3					:	1;//紧急停止信号
		u16 X4					:	1;//低液位信号
		u16 X5					:	1;//中液位信号
		u16 X6					:	1;//高液位信号
		u16 X7					:	1;// 39
		u16 X8					:	1;// 40
		
		u16 SystemSleep			:	1;//系统休眠
		u16 PhaseMiss			:	1;//输入缺相 26
		u16 PhaseSequenceError	:	1;//相序错误 27
		u16 ElcInv_CommError	:	1;//下位机与变频器通讯故障 28
		u16 HmiElc_CommError	:	1;//上位机与下位机通讯故障 29
		u16 bit5				:	1;// 30
		u16 bit6				:	1;// 31
		u16 bit7				:	1;// 32
		
		


	}bit;
	u16 all;
}SystemStateWord;

#ifdef  __input_signal_c
SystemStateWord SystemState;//全局系统故障字

unsigned char Input_X1 = 0,Input_X2 = 0,Input_X3 = 0,Input_X4 = 0,Input_X5 = 0,Input_X6 = 0,Input_X7 = 0,Input_X8 = 0;
unsigned int Input_All = 0;

void getInput(void);

#else 
extern unsigned char Input_X1,Input_X2,Input_X3 ,Input_X4 ,Input_X5 ,Input_X6 ,Input_X7 ,Input_X8 ;
extern unsigned int Input_All;

extern void getInput(void);
extern SystemStateWord SystemState;//全局系统故障字

#endif
#endif

