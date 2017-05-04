#ifndef __USART3_C
#define __USART3_C

#include "stm32f10x.h"
#include "Define.h" 
#include "usart3.h"

u8 USART_RX_BUF3[64];     //接收缓冲,最大64个字节.
//接收状态
//bit7，接收完成标志
//bit6，接收到0x0d
//bit5~0，接收到的有效字节数目
u8 USART_RX_STA3=0;       //接收状态标记

void uart_init3(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);//USART2 USART3要用RCC_APB1PeriphClockCmd 
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			//USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);				// GPIOB
     
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;			//USART3_RX	  PB.11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    
    GPIO_Init(GPIOB, &GPIO_InitStructure);				// GPIOB  
  
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			//Usart3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART3
  
   //USART 初始化设置
   
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART3, &USART_InitStructure);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断        
   
    USART_Cmd(USART3, ENABLE);                    //使能串口 
}
/*
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
    
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		USART_RX_BUF[USART_RX_STA&0X3F]=Res ;
		USART_RX_STA++;
        if(USART_RX_STA>63)
            USART_RX_STA=0;
    } 
}*/
void USART3_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = USART_ReceiveData(USART3);//读取接收到的数据
		
		if((USART_RX_STA3 & 0x80) == 0)//接收未完成
		{
			if(USART_RX_STA3 & 0x40)//接收到了0x0d
			{
				if(Res!=0x0a)	USART_RX_STA3 = 0;//接收错误,重新开始
				else	USART_RX_STA3 |= 0x80;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)	USART_RX_STA3 |= 0x40;
				else
				{
					USART_RX_BUF3[USART_RX_STA3 & 0X3F] = Res ;
					USART_RX_STA3 ++;
					if(USART_RX_STA3 > 63)	USART_RX_STA3 = 0;//接收数据错误,重新开始接收	  
				}		 
			}
		}
	} 
} 
#endif

