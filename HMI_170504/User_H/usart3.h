#ifndef __USART3_H
#define __USART3_H

#define USART3_IRQn	39
#define RS485_RX3 GPIOB->ODR &= 0xfffd;//RS485置为接收 PB1
#define RS485_TX3 GPIOB->ODR |= 0x0002;//RS485置为发送 PB1

#ifdef	__usart2_C

void uart_init3(u32 bound);

#else

extern void uart_init3(u32 bound);
extern u8 USART_RX_BUF3[64];     //接收缓冲,最大63个字节.末字节为换行符 
extern u8 USART_RX_STA3;         //接收状态标记	

#endif
#endif

