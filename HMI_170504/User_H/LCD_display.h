#ifndef __LCD_display_H
#define __LCD_display_H

#define cd  PCout(0)//PC0 
#define rd  PCout(1)//PC1 
#define wr  PCout(2)//PC2 
#define ce  PCout(3)//PC3 
#define rst PCout(4)//PC4 
#define bla  PCout(5)//PC5 

#define DATA_OUT        GPIOA->ODR
#define DATA_IN         GPIOA->IDR
#define PAOutPut   GPIOA->CRL = 0x33333333;    //PA0~7 
#define PAInPut    GPIOA->CRL = 0x88888888;    //PA0~7    

#define DELAY_MS_1000 1000

#define LC_TXTSTART 0x0000

#ifdef __LCD_display_C

unsigned int lcdadress;     
unsigned char lcdh,lcdl;     
unsigned int lcd_grHome;   
unsigned char gewei,shiwei,baiwei,qianwei,wanwei;

char aa[6];			//temporary group
char DisBuff[121];

extern EPASS  passdisp;

//void lcd_test(void);
void lcd_wrstring(u8 uline,u8 ucol,char *addrch);//ÏÔÊ¾×Ö·û´®
void lcd_wrstring_num(u8 uline,u8 ucol,char *addrch,u8 num);

void lcd_wrpicture(u8 uline,u8 ucol,u8 uline_n,u8 ucol_n,char *addrch);//ÏÔÊ¾Í¼Æ¬
void lcd_drawpane(u8 x,u8 y,u8 x_n,u8 y_n,u8 data);//»­·½¿ò
void lcd_wrByte(u8 uline,u8 ucol,char *addrch);//Ð´×Ö½Ú


#else

extern char aa[6];		//temporary group
extern char DisBuff[121];

extern unsigned char gewei,shiwei,baiwei,qianwei,wanwei;
extern void lcd_wrstring(u8 uline,u8 ucol,char *addrch); 
extern void lcd_wrstring_num(u8 uline,u8 ucol,char *addrch,u8 num);
extern unsigned long data_bai,data_shi,data_ge;

extern void init_LCD(void);
extern void init_sap1024(void);
extern void Lcd_MainFun(void);
extern void Lcd_ClearDis(void);
extern void lcd_wrstring(u8 uline,u8 ucol,char *addrch);//ÏÔÊ¾×Ö·û´®
extern void lcd_wrstring_num(u8 uline,u8 ucol,char *addrch,u8 num);
extern void lcd_wrpicture(u8 uline,u8 ucol,u8 uline_n,u8 ucol_n,char *addrch);//ÏÔÊ¾Í¼Æ¬
extern void lcd_drawpane(u8 x,u8 y,u8 x_n,u8 y_n,u8 data);//»­·½¿ò
extern void hex_to_asc(char *array,u32 idat,u8 num,u8 n);
extern void lcd_wrByte(u8 uline,u8 ucol,char *addrch);//Ð´×Ö½Ú
//extern char ClearMenu[121];
//extern void lcd_test(void);
//extern void lcd_ClearAllRam(void);

#endif
#endif

