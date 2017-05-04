#ifndef __INVERTER_H
#define __INVERTER_H

#ifdef __INVERTER_C
unsigned char HmiCmd_Run = 0;			//上位机命令字 启动
//unsigned char SystemState_Run = 0;		//系统状态字 运行
unsigned char INV_fault_code;
void Inverter_control(void);
//void sub_fun(void);
#else
extern unsigned char HmiCmd_Run;			//上位机命令字 启动
//extern unsigned char SystemState_Run = 0;		//系统状态字 运行
extern unsigned char INV_fault_code;
extern void Inverter_control(void);
//extern void sub_fun(void);

#endif
#endif
