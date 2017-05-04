#include "sys.h"
#include "stm32f10x.h"
void sub_gpio_init(void)
{
//3输出 8输入
//PA	
    RCC->APB2ENR |= RCC_APB2Periph_GPIOA;//使能PA时钟   
	
   // GPIOA->CRL = 0x88888888;  //PA0~6 AD输入 PA7 IO输入
	
    GPIOA->CRH &= 0xFFF00FF0;   //
	GPIOA->CRH |= 0x00033003;//PA8（RO11） PA11(RO12) PA12(RO10)输出
	GPIOA->ODR = 0xFFFF;		//输出高电平

//PB    
    RCC->APB2ENR |= RCC_APB2Periph_GPIOB;//使能PB时钟
	
    GPIOB->CRL &= 0XFF0FFF00;   //
    GPIOB->CRL |= 0x00808033;   //PB0 PB1:输出(rs485使能脚) PB3输入 PB5输入
	
    GPIOB->CRH &= 0x0000FF00;   //
    GPIOB->CRH |= 0x33330088;   //PB8 9输入 PB12 13 14 15:输出
	
    GPIOB->ODR = 0xFFFF;        //

//PC	
    RCC->APB2ENR |= RCC_APB2Periph_GPIOC;//使能PC时钟
		
    GPIOC->CRL &= 0x00FFFF00;   //
    GPIOC->CRL |= 0x33000088;    //
	GPIOC->CRH &= 0xFFF00000;
    GPIOC->CRH |= 0x00088333;    //
    GPIOC->ODR = 0xFFFF;        //


    RCC->APB2ENR |= RCC_APB2Periph_GPIOD;//PD 新版电路图PD2为X4输入
    GPIOD->CRL &= 0XFFFFF0FF;
    GPIOD->CRL |= 0X00000800;	//PD2输入(WP EEPROM的读写保护脚)
    GPIOD->ODR &= 0xFFFF;		//PD2


}

void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
