#ifndef __DEFINE_H
#define __DEFINE_H

// typedef signed          char int8;
// typedef signed short     int int16;
// typedef signed           int int32;
// typedef signed       __int64 int64;	

// typedef unsigned          char u8;
// typedef unsigned short     int u16;
// typedef unsigned           int u32;
// typedef unsigned       __int64 u64;

#define int8	signed char
#define int16	signed short int
#define int32	signed int
#define int64	signed __int64	

#define u8		unsigned char
#define u16		unsigned short int
#define u32		unsigned int
#define u64		unsigned __int64

#define BIT0	0x0001
#define BIT1	0x0002
#define BIT2	0x0004
#define BIT3	0x0008
#define BIT4	0x0010
#define BIT5	0x0020
#define BIT6	0x0040
#define BIT7	0x0080
#define BIT8	0x0100
#define BIT9	0x0200
#define BIT10	0x0400
#define BIT11	0x0800
#define BIT12	0x1000
#define BIT13	0x2000
#define BIT14	0x4000
#define BIT15	0x8000

#define RO1_1	GPIOC->ODR |= BIT12;//PC12
#define RO2_1	GPIOC->ODR |= BIT11;//PC11
#define RO3_1	GPIOC->ODR |= BIT10;//PC10
#define RO4_1	GPIOA->ODR |= BIT15;//PA15
#define RO5_1	GPIOF->ODR |= BIT7;//PF7
#define RO6_1	GPIOF->ODR |= BIT6;//PF6
#define RO7_1	GPIOA->ODR |= BIT11;//PA11
#define RO8_1	GPIOA->ODR |= BIT8;//PA8
#define RO9_1	GPIOC->ODR |= BIT9;//PC9

#define RO1_0	GPIOC->ODR &= ~BIT12;//PC12
#define RO2_0	GPIOC->ODR &= ~BIT11;//PC11
#define RO3_0	GPIOC->ODR &= ~BIT10;//PC10
#define RO4_0	GPIOA->ODR &= ~BIT15;//PA15
#define RO5_0	GPIOF->ODR &= ~BIT7;//PF7
#define RO6_0	GPIOF->ODR &= ~BIT6;//PF6
#define RO7_0	GPIOA->ODR &= ~BIT11;//PA11
#define RO8_0	GPIOA->ODR &= ~BIT8;//PA8
#define RO9_0	GPIOC->ODR &= ~BIT9;//PC9

#define PA9_1	GPIOA->ODR |= BIT9;//PA9
#define PA9_0	GPIOA->ODR &= ~BIT9;//PA9

#define PA10_1	GPIOA->ODR |= BIT10;//PA10
#define PA10_0	GPIOA->ODR &= ~BIT10;//PA10



#endif

