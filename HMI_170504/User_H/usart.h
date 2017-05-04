#ifndef __USART_H
#define __USART_H

#define USART1_IRQn	37
#define MBS_BufSize 250
#define USART_BufSize 250

#define RS485_RX GPIOB->ODR &= 0xfdff;//RS485置为接收 PB9
#define RS485_TX GPIOB->ODR |= 0x0200;//RS485置为发送 PB9

#ifdef __USART_C	  

u8 localAddr = 1;	//单片机控制板的地址
u8 sendCount = 0;	//发送字节个数
u8 receCount = 0;	//接收到的字节个数
u8 sendPosi = 0;	//发送位置

u8 flag_error_usart = 0;
u8 flag_rw_elc_err = 0;
u8 sendBuf[USART_BufSize];
u8 receBuf[USART_BufSize];
u16 MBS_HoldRegs[MBS_BufSize];
u16 modbus_check[MBS_BufSize];
u8 flag_usart2_read_error = 0;
u8 flag_writing_0x06 = 0;
u8 flag_writing_0x10 = 0;
u8 flag_reading_0x03 = 0;
u8 readINV_handshake = 0;
u8 flag_wr_fault = 0;
u8 flag_wr10_fault = 0;
u8 flag_rd03_fault = 0;
void usart_config(u32 bound);
void modbus_dataUnpack_host(void);
void usart_WriteRegister(u16 data_add,u16 data_num);
void usart_ReadRegister(u16 data_add,u16 data_num);
void usart_WriteMultipleRegister(u16 add,u16 num);

#else

extern u8 localAddr;	//单片机控制板的地址
extern u8 sendCount;	//发送字节个数
extern u8 receCount;	//接收到的字节个数
extern u8 sendPosi;	    //发送位置

extern u8 sendBuf[USART_BufSize];
extern u8 receBuf[USART_BufSize];
extern u16 MBS_HoldRegs[MBS_BufSize];
extern u16 modbus_check[MBS_BufSize];
extern u8 flag_writing_0x06;
extern u8 flag_writing_0x10;
extern u8 flag_reading_0x03;
extern u8 readINV_handshake;
extern u8 flag_wr_fault;
extern u8 flag_rd03_fault;
extern u8 flag_wr10_fault;
extern u8 flag_usart2_read_error;

extern u8 flag_error_usart ;
extern u8 flag_rw_elc_err;
extern void usart_config(u32 bound);
extern void modbus_dataUnpack_host(void);
extern void usart_WriteRegister(u16 data_add,u16 data_num);
extern void usart_ReadRegister(u16 data_add,u16 data_num);
extern void usart_WriteMultipleRegister(u16 add,u16 num);

#endif
#endif

