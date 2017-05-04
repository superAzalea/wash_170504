#ifndef __DataHandle_H
#define __DataHandle_H

typedef union
{
	struct
	{
		u16 X1					:	1;//高液位信号// 33
		u16 X2					:	1;//中液位信号 34
		u16 X3					:	1;// 低液位信号35
		u16 X4					:	1;// 紧急停止信号36
		u16 X5					:	1;// 超震信号37
		u16 X6					:	1;// 门开关信号38
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

#ifdef __DataHandle_C	  


u8 fact_temp;//实际温度
u16 fact_liquid_height;//实际液位
u8 INVCurrent_command;//变频器当前命令
u8 INVCurrent_state;//变频器当前状态
u8 INVCurrent_fault;//变频器当前故障
u16 INVRun_frequency;//变频器运行频率
u16 INVSet_frequency;//变频器设定频率
u16 INVBus_voltage;//变频器母线电压
u16 INVOutput_voltage;//变频器输出电压
u16 INVOutput_current;//变频器输出电流
void sub_DataHandle(void);
u16 SearchTemp(u16 ad_temp);

u8 input_door_switch = 0;//门开关信号
u8 input_shake = 0;//超震信号
u8 input_exigency_stop = 0;//紧急停止信号
u8 input_low_liquid = 0;//低液位信号
u8 input_middle_liquid = 0;//中液位信号
u8 input_high_liquid = 0; // 高液位信号
SystemStateWord SystemState;

void sub_InputStates(void);


#else
extern u8 fact_temp;//实际温度
extern u16 fact_liquid_height;//实际液位
extern u8 INVCurrent_command;//变频器当前命令
extern u8 INVCurrent_state;//变频器当前状态
extern u8 INVCurrent_fault;//变频器当前故障
extern u16 INVRun_frequency;//变频器运行频率
extern u16 INVSet_frequency;//变频器设定频率
extern u16 INVBus_voltage;//变频器母线电压
extern u16 INVOutput_voltage;//变频器输出电压
extern u16 INVOutput_current;//变频器输出电流
extern u16 SearchTemp(u16 ad_temp);
extern void sub_DataHandle(void);

extern u8 input_door_switch;//门开关信号
extern u8 input_shake;//超震信号
extern u8 input_exigency_stop;//紧急停止信号
extern u8 input_low_liquid;//低液位信号
extern u8 input_middle_liquid;//中液位信号
extern u8 input_high_liquid; // 高液位信号
extern SystemStateWord SystemState;

extern void sub_InputStates(void);

#endif
#endif

