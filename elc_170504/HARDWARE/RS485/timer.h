
#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
#ifdef __timer_c
u8 flag_200ms_wr = 0;
u8 flag_200ms_rd = 0;
u8 flag_500ms = 0;
u8 flag_1s = 0;
u8 flag_2s = 0;
u8 flag_usart3_wr_500ms = 0;
u8 flag_rd = 0;
u8 flag_wr = 0;
u8 flag_5s = 0;
u8 flag_ad = 0;
u8 flag_usart3 = 0;
void TIM3_Int_Init(u16 arr,u16 psc); 
void TIM3_Configuration(void);
void NVIC_Configuration(void);

#endif
extern u8 flag_200ms_wr;
extern u8 flag_200ms_rd;
extern u8 flag_500ms;
extern u8 flag_1s;
extern u8 flag_2s;
extern u8 flag_5s;
extern u8 flag_ad;
extern u8 flag_usart3;
extern u8 flag_rd;
extern u8 flag_wr;
extern u8 flag_usart3_wr_500ms;
extern void TIM3_Int_Init(u16 arr,u16 psc); 
extern void TIM3_Configuration(void);
extern void NVIC_Configuration(void);

#endif

