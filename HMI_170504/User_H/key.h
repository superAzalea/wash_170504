#ifndef __KEY_H
#define __KEY_H


#define  Wash    10 //洗涤
#define  Cold    4  //冷水
#define  Warm    8  //热水
#define  Heat    13 //加热
#define  Soap    1  //皂液
#define  Drain   5  //排水
#define  Spin    9  //脱水
#define  Remove  15 //取衣
#define  Return  12 //返回
#define  UP      14 // 增加
#define  Down    3  //减少
#define  Setup   7  //设置

#define  Setup_C   20  //设置
#define  STOP    11 //停止
#define  RUN     16 //运行



#ifdef __KEY_C

unsigned char KeyVal = 0;
unsigned char KeyAct = 0;
unsigned char KeyCmd = 0;
unsigned char KeyBuzz = 0;
//unsigned char KeyCode = 0;
//unsigned char key_flag = 0;

void KeyScan(void);

#else

extern unsigned char KeyVal;
extern unsigned char KeyAct;
extern unsigned char KeyCmd;
extern unsigned char KeyBuzz;
//extern unsigned char key_flag;

extern void KeyScan(void);
//extern void KeyScancode(void);

#endif
#endif


