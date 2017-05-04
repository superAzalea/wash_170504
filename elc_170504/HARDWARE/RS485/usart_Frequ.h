#ifndef __USART_FREQU_H
#define __USART_FREQU_H	
#include "sys.h"
#include "stdio.h"

#define MBS_BufSize3 20
#define USART_BufSize3 250
#define RS485_TX_EN3		PBout(1)	//485模式控制.0,接收;1,发送.

enum{
	ERR_INIT = 0,ERR_NONE=1,ERR_TIMEOUT=2,ERR_CHECKSUM=3
};

#ifdef __USART_FREQU_C

u8 sendPosi3 = 0;		//发送位置
u8 localAddr3 = 1;	//单片机控制板的地址
u8 RS485_RX_CNT3;  	//接收到的数据长度
u8 RS485_TX_CNT3;		//发送的数据长度
u8 ReadINV_usart3_start = 0;
u8 RS485_RX_BUF3[USART_BufSize3]; 		//接收缓冲,最大64个字节
u8 RS485_TX_BUF3[USART_BufSize3];  	//发送缓冲,最大64个字节.
u16 modbus_check3[MBS_BufSize3];
u8 ReadINV_usart3_ok = 0;
u8 WriteINV_usart3_ok = 0;

u8 trans_data_num =0;//读变频器第几组数据
u8 trans_data_num_old = 0;

volatile u16 MBS_HoldRegs3[MBS_BufSize3];

u8 flag_usart3_read_error = 0;
u8 flag_usart3_write_error = 0;
u8 flag_usart3_error = 0;
u8 flag_error_usart3 = 0;
u8 flag_writing3 = 0;
u8 flag_wr_accetime = 0;//收到加速时间标记
u8 flag_wr_decetime = 0;//收到减速时间标记
u8 flag_wr_freqency = 0;
u8 flag_reading3 = 0;
u8 flag_rd3_timeout = 0;
u8 flag_wr3_timeout = 0;
u8 flag_rw_err = 0;
u8 counter_rw_err = 0;
//模式控制
//如果想串口中断接收，请不要注释以下宏定义
//#define EN_USART3_RX 	1			//0,不接收;1,接收.
//#define EN_USART1_RX 	1			//0,不接收;1,接收.

void RS485_Init3(u32 bound);
void usart_ReadRegister3(u16 data_add,u16 data_num);
void usart_WriteRegister3(u16 data_add,u16 data_num);
u8 modbus_dataUnpack_2(void);
u8 presetReadINV_Adress(void);
u8 presetWriteINV_Adress(void);
u8 presetWrite10INV_Adress(void);
void ELC2INV_Read_write(void);


#else

extern u8 sendPosi3;		//发送位置
extern u8 localAddr3;	//单片机控制板的地址
extern u8 RS485_RX_CNT3;  	//接收到的数据长度
extern u8 RS485_TX_CNT3;		//发送的数据长度
extern u8 ReadINV_usart3_start;
extern u8 RS485_RX_BUF3[USART_BufSize3]; 		//接收缓冲,最大64个字节
extern u8 RS485_TX_BUF3[USART_BufSize3];  	//发送缓冲,最大64个字节.
extern volatile u16 MBS_HoldRegs3[MBS_BufSize3];

extern u8 flag_usart3_read_error;
extern u8 flag_usart3_write_error;
extern u8 flag_usart3_error;
extern u8 flag_error_usart3;
extern u8 flag_writing3;
extern u8 flag_wr_accetime;
extern u8 flag_wr_decetime;
extern u8 flag_wr_freqency;
extern u8  flag_reading3;
extern u8 flag_rd3_timeout;
extern u8 flag_wr3_timeout;
extern u8 flag_rw_err;
extern u8 counter_rw_err;
extern u8 trans_data_num;//读变频器第几组数据
extern u8 trans_data_num_old;
extern u8 ReadINV_usart3_ok ;
extern u8 WriteINV_usart3_ok;
extern void RS485_Init3(u32 bound);
extern void usart_ReadRegister3(u16 data_add,u16 data_num);
extern void usart_WriteRegister3(u16 data_add,u16 data_num);
extern u8 modbus_dataUnpack_2(void);
extern u8 presetReadINV_Adress(void);
extern u8 presetWriteINV_Adress(void);
extern u8 presetWrite10INV_Adress(void);

extern void ELC2INV_Read_write(void);

#endif

#endif
