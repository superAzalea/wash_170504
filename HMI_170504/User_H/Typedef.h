#ifndef __TYPEDEF_H
#define __TYPEDEF_H


typedef enum 
{
   dismod0,
   dismod1,
   dismod2,
   dismod3,
   dismod4,
   dismod5,
   dismod6,
   dismod7,
   dismod8,
   dismod9,
   dismod10,
   dismod11,

}EDISPLAY;


typedef enum 
{
	MENUCUR0,//主菜单
	MENUCUR1,//查询、修改选择菜单
	MENUCUR2,//设置密码界面
	MENUCUR3,//功能列表界面
	MENUCUR4,
	MENUCUR5,
	MENUCUR6,
	MENUCUR7,
	MENUCUR8,
	MENUCUR9,
	MENUCUR10,
	MENUCUR11,
      
}EMENU;

typedef struct
{
	unsigned char dispnum;
	unsigned char pst_rev_dis;// 有高脱标记
	unsigned char set_rev_flg;//反显标记
	unsigned char disp_flg; //刷屏标志
	unsigned char disp_cir;
	char dispdata[6];
}EPASS;


typedef struct
{
	unsigned char dis_mode;
	unsigned char data_mode;
	unsigned char sub_menucur;// 二级菜单指针   azalea ?
	unsigned char sub_chosecur;//选择指针    azalea ?
	unsigned char sub_chosecurbuf;

}DISMENU;



typedef struct 
{
	unsigned char P1[8];//制造商设置
	unsigned char P2[8];//功能设置1
	unsigned char P3[8];//功能设置2
    unsigned char P4[8];//输入定义
	unsigned char P5[8];//输出定义
	unsigned char P6[8];//液位设置
	unsigned char P7[8];//皂液动作时间
	unsigned char P8[8];//洗涤时间设置
	unsigned char P9[8];//脱水过度时间
	unsigned char P10[8];//脱水结束延时
	unsigned char P11[8];//取衣时间设置
	unsigned char P12[8];//温度设置
	unsigned char P13[8];//高脱设置
	unsigned char P14[8];//速度设置
	unsigned char P15[8];//液位高度校准
	unsigned char P16[8];//双速电机测试
	unsigned char P17[8];
}FUNC_CODE_I ;

#endif
