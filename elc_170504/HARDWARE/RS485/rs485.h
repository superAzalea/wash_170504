#ifndef __RS485_H
#define __RS485_H			 
#include "sys.h"	 

#define MBS_BufSize 100
#define USART_BufSize 100

//模式控制
#define RS485_TX_EN		PBout(0)	//485模式控制.0,接收;1,发送.
//如果想串口中断接收，请不要注释以下宏定义
#define EN_USART2_RX 	1			//0,不接收;1,接收.
#define EN_USART1_RX 	1			//0,不接收;1,接收.

#ifdef __RS485_C

u8 localAddr = 1;	//单片机控制板的地址
u8 receCount = 0;	//接收到的字节个数
u8 sendPosi = 0;	//发送位置
//接收到的数据长度
u8 RS485_RX_CNT=0;   
u8 RS485_TX_CNT = 0;   			//发送的数据长度
u8 flag_error_usart = 0;
u8 counter_error_usart = 0;
u16 acce_time;
u16 dece_time;
u16 frequency;
u8  fault_clear;

u16 MBS_HoldRegs[MBS_BufSize];
u16 modbus_check[MBS_BufSize];
u8 RS485_TX_BUF[USART_BufSize];
u8 RS485_RX_BUF[USART_BufSize];  	//接收缓冲,最大64个字节.

void RS485_Init(u32 bound);
//void RS485_Send_Data(u8 *buf,u8 len);
void modbus_dataUnpack_slave(void);
void readRegisters(void);
void presetMultipleRegister(void);
void preset_error(u8,u8);
 //--------------------------- 
 
#else

extern u8 localAddr;	//单片机控制板的地址
extern u8 receCount;	//接收到的字节个数
extern u8 sendPosi;	//发送位置
//接收到的数据长度
extern u8 RS485_RX_CNT;   
extern u8 RS485_TX_CNT ;   			//发送的数据长度
extern u8 flag_error_usart;
extern u8 counter_error_usart;
extern u16 acce_time;
extern u16 dece_time;

extern u16 frequency;
extern u8 fault_clear;
extern u16 MBS_HoldRegs[MBS_BufSize];
extern u16 modbus_check[MBS_BufSize];
extern u8 RS485_TX_BUF[USART_BufSize];
extern u8 RS485_RX_BUF[USART_BufSize];  	//接收缓冲,最大64个字节.

extern void RS485_Init(u32 bound);
//extern void RS485_Send_Data(u8 *buf,u8 len);
extern void readRegisters(void);
extern void modbus_dataUnpack_slave(void);
extern void preset_error(u8,u8);
extern void presetMultipleRegister(void);

#endif	   
#endif
















