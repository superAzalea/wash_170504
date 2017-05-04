#ifndef __DELAY_H
#define __DELAY_H
#ifdef __DELAY_C

void delay_init(unsigned char SYSCLK);
void delay_ms(unsigned int nms);
void delay_us(unsigned long nus);
void delay(unsigned int i);

#else

extern void delay_init(unsigned char SYSCLK);
extern void delay_ms(unsigned int nms);
extern void delay_us(unsigned long nus);
extern void delay(unsigned int i);

#endif
#endif






























