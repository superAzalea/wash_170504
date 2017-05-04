#ifndef __ADC_H
#define __ADC_H	

/*
#define ADC_CH0  0 //通道0
#define ADC_CH1  1 //通道1
#define ADC_CH2  2 //通道2
#define ADC_CH3  3 //通道3	   
*/

#ifdef __ADC_C

u16 Adc_BatteryVoltag = 0;

void Adc_Init(void);
void Get_Adc(void);  

#else 

extern u16 Adc_BatteryVoltag;

extern void Adc_Init(void);
extern void Get_Adc(void);

#endif 
#endif 


