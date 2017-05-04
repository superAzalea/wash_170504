#include "stm32f10x.h"
#include "led.h"
#include "SystemInit.h"
//初始化PA8和PD2为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{

 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 |GPIO_Pin_12;				 //L_Soap PA.8 L_Heat PA.11 L_Warm PA.12端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 GPIO_SetBits(GPIOA,GPIO_Pin_8 |GPIO_Pin_11 | GPIO_Pin_12);						 //PA.8,11,12 输出高        

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PA端口时钟
	//L_Run PB.1,L_Cold PB.5,L_Stop PB.10,L_Remove PB.11 端口配置
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_5 |GPIO_Pin_10 |GPIO_Pin_11;				
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 GPIO_SetBits(GPIOB,GPIO_Pin_1 | GPIO_Pin_5 |GPIO_Pin_10 |GPIO_Pin_11);	
	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);   //使能PD端口时钟
//	L_Drain PC.6,L_Spin PC.7,L_Wash PC.12 端口配置
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_12 ;	 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
 GPIO_Init(GPIOC, &GPIO_InitStructure);	
 GPIO_SetBits(GPIOC,GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_12); 						 //PD.2 输出高 

}
 
