#ifndef __Menu_H
#define __Menu_H

#define NUM_PAGE	106

#ifdef  __Menu_C
#include "bit_define.h"

u16 Tab_MenuNum[NUM_PAGE] = {
0,1,2,3,4,5,6,
11,12,13,14,15,16,
21,22,23,24,25,26,27,
31,32,33,34,35,36,37,	
41,	
51,52,53,54,	
61,62,63,64,65,66,67,68,	
211,212,213,214,215,216,
221,222,223,224,225,
231,232,233,
241,242,243,244,245,246,247,
251,252,253,254,255,256,257,	
261,262,263,264,265,
271,272,273,274,275,
311,312,313,314,315,
321,322,323,
331,332,333,334,335,336,337,338,
341,342,343,344,
351,352,
361,362,
411,412,413,	
999,
};

char Tab_unit[14][3] = {" "," ","V","A","Mp","'C","Kw","h","s","Hz","%"," "," "," "};

char ResetMenu[121] = {
  "        欢  迎  使  用        "
  "      螺  杆  空  压  机      "
  "                              "
  "V1.0                          "};  

char disresetmenu[ ] = {
  "           禾兴电机           "
  "DKC400-LS04 全自动洗脱机控制器"
  "      V1.00 开机中...         "
  "                              "};

char handshakemenu[] = {
  "                              "
  "   握手失败,请检查通讯线路    "
  "                              "
  "                              "
};

char dispMainMenu[] = {
	"    程序                      "
	"设定温度   ℃ 设定液位        "
	"实际温度   ℃ 实际液位        "
	"                              "};//120		


char dispSetMenu[] = {
  "    程序  步                  "
  "设定温度  ℃  设定液位        "
  "                              "
  "                              "};

char disMenu[] = {
  "手动模式       00:00          "
  "实际温度   ℃ 实际液位        "
  "    正在洗涤!                 "
  "                              "};//120
  
char dismodeMenu[] = {
  "  查询方式                    "
  "  修改方式                    "
  "                              "
  "   返回    ∧    ∨    设置   "};

char dismmMenu[] = {
    "                              "
    "             密码             "
    "                              "
    "   返回    +      -    确认   "};//120


  
char dis_main_menu[] = {
  //"          主设置菜单          "
  "  P01 制造商设置              "
  "  P02 功能设置1               "
  "  P03 功能设置2               "
  "  P04 输入定义                "
  "  P05 输出定义                "
  "  P06 液位设置                "
  "  P07 皂液动作时间            "
  "  P08 洗涤时间设置            "
  "  P09 脱水过渡时间            "
  "  P10 脱水结束延时            "
  "  P11 取衣时间设置            "
  "  P12 温度设置                "    
  "  P13 高脱设置                "
  "  P14 速度设置                "
  "  P15 液位高度校准            "
  "  P16 双速电机测试            "};

  
char dis_sub_menu_one[] = {
  "  初始化参数                  "
  "  初始化程序                  "
  "  电机驱动选择                "
  "  机器容量                    "
  "  语言设置                    "
  "  密码设置                    "};
  
char dis_sub_menu_two[] = {
  "  是否自动补热                "
  "  是否自动补水                "
  "  是否可以跳步                "
  "  是否有取衣                  "
  "  用液位传感器                "};

char dis_sub_menu_three[] = {
  "  脱水全程排水                "
  "  自动补水液差                "
  "  热水最低液位                "
  "  超震重试次数                "
  "  变频器速度基数              "};  
	

char dis_sub_menu_four[] = {
  "  门关定义                    "
  "  过载定义                    "
  "  超震定义                    "
  "  紧急停止定义                "
  "  低液位定义                  "
  "  中液位定义                  "
  "  高液位定义                  "};
  
char dis_sub_menu_five[] = {
  "  排水定义                    "
  "  门锁定义                    "};

 
char dis_sub_menu_six[] = {
  "  高液位的高度                "
  "  中液位的高度                "
  "  低液位的高度                "
  "  零基准液位                  "
  "  洗涤最低液位                "
  "  加热最低液位                "
  "  均布排水液位                "
  "  脱水最低液位                "};  
	
 
char dis_sub_menu_seven[] = {
  "  皂液的个数             个   "
  "  皂液1工作时间          秒   "
  "  皂液2工作时间          秒   "
  "  皂液3工作时间          秒   "
  "  加皂液液位                  "
  "  加皂液关冷水                "};

char dis_sub_menu_eight[] = {
  "  洗涤过渡时间           秒   "
  "  洗涤转动时间           秒   "
  "  洗涤停止时间           秒   "};
	
  
char dis_sub_menu_nine[] = {
  "  脱水延时时间           秒   "
  "  单向停留时间           秒   "
  "  均步带水时间           秒   "
  "  均步排水时间           秒   "
  "  中脱停留时间           秒   "
  "  中脱保护时间           分   "
  "  高脱保护时间           分   "};
  
char dis_sub_menu_ten[] = {
  "  单向结束延时           秒   "
  "  均布结束延时           秒   "
  "  中脱结束延时           秒   "
  "  高脱结束延时           秒   "
  "  均步结束刹车           秒   "
  "  中脱结束刹车           秒   "
  "  高脱结束刹车           秒   "};
  
char dis_sub_menu_eleven[] = {
  "  取衣过渡时间           秒   "
  "  取衣正转时间           秒   "
  "  取衣正转停止           秒   "
  "  取衣反转时间           秒   "
  "  取衣反转停止           秒   "};
  
char dis_sub_menu_twelve[] = {
  "  手动默认温度           ℃   "
  "  编程默认温度           ℃   "
  "  最高可设温度           ℃   "
  "  最低可设温度           ℃   "};
  
char dis_sub_menu_thirteen[] = {
  "  高脱速度个数           个   "
  "  高脱1过渡时间          秒   "
  "  高脱2过渡时间          秒   "};
  //"  高脱3过渡时间          秒   "};  

char dis_sub_menu_fourteen[] = {
  "  正转频率               Hz   "
  "  反转频率               Hz   "
  "  均布频率               Hz   "
  "  中脱频率               Hz   "
  "  高脱频率               Hz   "
  "  高脱2频率              Hz   "
  "  高脱3频率              Hz   "
  "  取衣频率               Hz   "};

char dis_sub_menu_fifteen[] = {
  "                              "
  "            无功能            "
  "                              "
  "                              "};
  
char dis_sub_menu_sixteen[] = {
  "                              "
  "            无功能            "
  "                              "
  "                              "};       

char save_data_menu[] = {
  "                              "
  "        是否保存数据?         "
  " 开始键:保存    停止键:不保存 "
  "                              "};
  
char fault_dis_menu[] = {
  "                              "
  " 有故障,请按任意键查询故障表! "
  "                              "
  "                              "};
                                     
char wash_complete_menu[] = {
  "                              "
  "          洗涤完成!           "
  "      请按任意键返回主页      "
  "                              "};

char shake_stop_menu[] = {
  "                              "
  "          超震停机            "
  "      请按任意键返回主页      "
  "                              "};

char instancy_stop_menu[] = {
  "                              "
  "          紧急停机            "
  "      请按任意键返回主页      "
  "                              "};  



typedef void (*UserPrFun) (u8 i);
	
u8 flag_prSetting = 0;			//参数设定中标志位
u8 Counter_KeyIn = 0;			//对键盘输入次数的计数，用来设定参数
u8 UserPrNum = 0;				//系统参数的有效数字
u8 Data_KeyIn_Dis[5] = {0,0,0,0,0};	//在参数设定时，用来记录键盘输入的数组
u8 Data_KeyIn[5] = {0,0,0,0,0};	//为了将反白显示的数组还原为真实的数字，用来计算参数值，引入该数组
u16 UserPrIndex = 0;				//参数索引号
u8 flag_firstInSetPrPage = 0;	//首次进入参数设定界面标志位
u16 UserPrData = 0;				//用来保存参数设定值
u8 UserPrData_H = 0;				//参数设定值的高8位
u8 UserPrData_L = 0;				//低8位
u16 MenuIndex = 0;			//菜单索引号
//u16 MenuIndex_Z = 0;			//上周期的菜单索引号
u8 CheckState = 0;
u8 CheckOk[6] = {0,0,0,0,0,0};
u8 DispMod_Old = 0;

char passportdata[6] = {0,0,0,0,0,0};
char passportdata_buffer[6] = {0,0,0,0,0,0};

void sendtobuff(u8 h,u8 l,char *p);
void Menu_RunPrPro(void);			//运行参数
void Menu_FactoryPrPro(void);		//厂家参数
void Menu_DebugPrPro(void);			//调试参数
void Menu_FaultRecordPro(void);		//故障记录
void Menu_UserPasswordPro(void);	//用户密码

void lcd_mainmenu(void);
///***
void menu_first_page(void);
void menu_second_page(void);
void main_set_menu(void);
void lcd_menu_one(void);
void lcd_menu_two(void);
void lcd_menu_three(void);
void lcd_menu_four(void);
void lcd_menu_five(void);
void lcd_menu_six(void);
void lcd_menu_seven(void);
void lcd_menu_eight(void);
void lcd_menu_nine(void);
void lcd_menu_ten(void);
void lcd_menu_eleven(void);
void lcd_menu_twelve(void);
void lcd_menu_thirteen(void);
void lcd_menu_fourteen(void);
void lcd_menu_fifteen(void);//???
void lcd_menu_sixteen(void);
void lcd_wash_complete_menu(void);
void menu_fault_page(void);
void menu_fault_dis(void);
void menu_handshake_dis(void);
void menu_set_program(void);
void lcd_shake_stop_menu(void);
void lcd_instancy_stop_menu(void);
void menu_save_data(void);//保存数据菜单
void key_mainmenu_handler(void);//dismod0
void key_menu_first_page_handler(void);//dismod1
void key_menu_second_page_handler(void);//dismod2
void key_main_set_menu_handler(void);//dismod3
void key_lcd_menu_one_handler(void);//dismod4_1
void key_lcd_menu_two_handler(void);//dismod4_2
void key_lcd_menu_three_handler(void);//dismod4_3
void key_lcd_menu_four_handler(void);//dismod4_4
void key_lcd_menu_five_handler(void);//dismod4_5
void key_lcd_menu_six_handler(void);//dismod4_6
void key_lcd_menu_seven_handler(void);//dismod4_7
void key_lcd_menu_eight_handler(void);//dismod4_8
void key_lcd_menu_nine_handler(void);//dismod4_9
void key_lcd_menu_ten_handler(void);//dismod4_10
void key_lcd_menu_eleven_handler(void);//dismod4_11
void key_lcd_menu_twelve_handler(void);//dismod4_12
void key_lcd_menu_thirteen_handler(void);//dismod4_13
void key_lcd_menu_fourteen_handler(void);//dismod4_14
void key_lcd_menu_fifteen_handler(void);//dismod4_15
void key_lcd_menu_sixteen_handler(void); // dismod4_16
void key_menu_fault_page_handler(void) ;//dismod5
void key_menu_set_program_handler(void);//dismod6_1
void key_menu_save_data_handler(void) ;         // dismod6_2
void key_wash_complete_menu_handler(void); //dismod7
void key_shake_stop_menu_handler(void);       //dismod8
void key_instancy_stop_menu_handler(void);   //dismod9
void key_menu_fault_dis_handler(void);           //dismod10

void disphex_asc_le(char *array,u32 idat,u8 num);
void kp_menucur(u8 sline,u8 ucol,u8 cur);
void lcd_lastline(u8 g1, u8 g2);
void disphex_asc_be(char *array,u32 idat,u8 num);
void (*RunPrFunTable[16])(void) = {lcd_menu_one,lcd_menu_two,lcd_menu_three,lcd_menu_four,lcd_menu_five,lcd_menu_six,
                                    lcd_menu_seven,lcd_menu_eight,lcd_menu_nine,lcd_menu_ten,lcd_menu_eleven,lcd_menu_twelve,
	                                  lcd_menu_thirteen,lcd_menu_fourteen,lcd_menu_fifteen,lcd_menu_sixteen};

//void 	(*FunProgramTable[2])() = {menu_set_program,menu_save_data};														
																		
//***/
///***
//u8  lcd_submenucur;
//u8 lcd_chosencur;
u8 _new_page;
u8 _bit_set;
//u8 auto_run_mode;
//u8 lcd_chosencur;
u8 _read_data;
u8 _read_function;
u8 lcd_maxcur;
//u8 _display_mode;
//u8 lcd_chosencur_buffer;
u8 run_state;
u8 _enable_display;
u8 set_temp;
u8 lcd_cpage;
u8 lcd_cpage_old;																		

EMENU Menu;
EDISPLAY DispMod;

//****/

//u32 pwsd_factory;

extern void ClearDisBuff(void);
extern void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);
extern u8 AT24CXX_ReadOneByte(u16 ReadAddr);


EPASS  passdisp;
DISMENU  dismenu;

volatile FUNC_CODE_I  Code_I;

#else
extern char handshakemenu[];
extern  char disresetmenu[ ];

extern char dispMainMenu[ ];

extern char dispSetMenu[ ];
extern char disMenu[ ];
extern char dismodeMenu[ ];
extern char dismmMenu[ ];

extern char dis_main_menu[ ];

extern char dis_sub_menu_one[ ];
extern char dis_sub_menu_two[ ];
extern char dis_sub_menu_three[ ];

extern char dis_sub_menu_four[ ];
extern char dis_sub_menu_five[ ];

extern char dis_sub_menu_six[ ];

extern char dis_sub_menu_seven[ ];

extern char dis_sub_menu_eight[ ];
///***
extern char dis_sub_menu_nine[ ];
extern char dis_sub_menu_ten[ ];
extern char dis_sub_menu_eleven[ ];
extern char dis_sub_menu_twelve[ ];
extern char dis_sub_menu_thirteen[ ];
extern char dis_sub_menu_fourteen[ ];
extern char dis_sub_menu_fifteen[ ];
extern char dis_sub_menu_sixteen[ ];
extern char save_data_menu[ ];
extern char fault_dis_menu[ ];
extern char wash_complete_menu[ ];
extern char shake_stop_menu[ ];
extern char instancy_stop_menu[ ];
//****/

//extern u32 pwsd_factory;

extern char ResetMenu[121];
//extern char Tab_MenuData[NUM_PAGE][121];
extern char Tab_unit[14][3];
extern u16 Tab_MenuNum[NUM_PAGE];
extern u8 flag_prSetting;			//参数设定中标志位
extern u8 Counter_KeyIn;			//对键盘输入次数的计数，用来设定参数
extern u8 UserPrNum;				//系统参数的有效数字
extern u8 Data_KeyIn_Dis[5];	//在参数设定时，用来记录键盘输入的数组
extern u8 Data_KeyIn[5];	//为了将反白显示的数组还原为真实的数字，用来计算参数值，引入该数组
extern u16 UserPrIndex;				//参数索引号
//extern u16 MenuIndex_Z;			//上周期的菜单索引号
extern u8 flag_firstInSetPrPage;	//首次进入参数设定界面标志位
extern u16 UserPrData;				//用来保存参数设定值
extern u8 UserPrData_H;				//参数设定值的高8位
extern u8 UserPrData_L;				//低8位
extern u16 MenuIndex;			//菜单索引号
extern u8 CheckState;
extern u8 CheckOk[6];

extern void sendtobuff(u8 h,u8 l,char *p);
extern void Lcd_KEYPro(void);
extern void LCDDisplay(void);
extern void Auto_WashPro(void);
extern void ini_data(void);
extern void fault_dis_pro(void);
extern void handshake_pro(void);
extern void Menu_RunPrPro(void);			//运行参数
extern void Menu_FactoryPrPro(void);		//厂家参数
extern void Menu_DebugPrPro(void);			//调试参数
extern void Menu_FaultRecordPro(void);		//故障记录
extern void Menu_UserPasswordPro(void);	//用户密码

#endif
#endif









