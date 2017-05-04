#ifndef __BIT_DEFINE_H
#define __BIT_DEFINE_H

unsigned char _manual_mode;
//unsigned char input_door_switch;
unsigned char run_state;
//unsigned char _enable_display;

unsigned int run_frequency;//运行频率
unsigned int acce_time;//加数时间
unsigned int dece_time;//减速时间

unsigned char set_liquid_height;//设定液位高度
unsigned char _1s_timer	;
unsigned char flag_read_Inv;
unsigned char _key_pro_ok_2;
extern unsigned char _set_mode_symbol;
extern unsigned char _if_save_data;
//unsigned char set_bit;//b0--设置时间分  b1--设置时间秒 b2--设置设定温度 b3--设置液位高度
unsigned char set_num;//设置符号 0--无 1--分 2--秒 3--温度 4--水位
unsigned char set_num_timer;//间隔时间
unsigned char wash_run_time;//洗涤运行时间
unsigned char wash_stop_time;//洗涤停止时间
unsigned char _wash_direction;
//unsigned char soap_liquid_run_time;//皂液运行时间
//unsigned char dehydration_state;//脱水状态
//unsigned char auto_mode_run_step;//自动模式运行步
//unsigned char wait_state;//水的状态  //d5代表脱水, d4代表排水, d3代表皂液, d2代表加热, d1代表热水,d0代表冷水,
//unsigned char auto_mode_liquid_height;//自动设定模式液位
//unsigned char auto_mode_wash_dehydration;//自动模式洗涤和脱水
unsigned char auto_mode_s;//自动模式运行时间秒
unsigned char auto_mode_m;//自动模式运行时间分
//unsigned char auto_mode_temp;//自动模式设定温度
//unsigned char auto_mode_soap_liquid;//自动模式皂液
//unsigned char add_water_dehydration;//加水到适当液位脱水
unsigned char soap_liquid_num;//第几个皂液
unsigned char dehydration_num;//第几个脱水

unsigned char mode_data[200]; //数据地址
unsigned char mode_data_buffer[200];
unsigned char fault[12];
unsigned char *data_group2;



//unsigned char trans_data_num;//传输第几组数据
//unsigned char tran_num;//传输第几个数据

unsigned char program_step_num;//运行时的程序步数
unsigned char soap_liquid_time;//皂液运行时间             
unsigned char tell_stop_num;//提示停止的次数
//unsigned char receive_num;//接受数据的数量
//unsigned int error_for_commu_num;//通讯错误故障记数
unsigned char new_fault;//新故障
//unsigned char check_fault_num;//检查故障计数
//unsigned char commun_state;//通讯状态
unsigned char commun_interval;//通讯间隔
extern unsigned char back_light_timer;//背光时间
extern unsigned char auto_run_mode;//自动运行模式


unsigned char wash_provide_timer;       //洗涤开始前的准备时间

unsigned char single_wash_have_water_time;     //均布带水时间
unsigned char single_wash_drain_time;          //均布排水时间
unsigned char single_wash_complete_delay_time; //均布结束延时
unsigned char single_wash_complete_skid;       //均布结束刹车

unsigned char dehydration_provide_timer;        //脱水开始前的准备时间
unsigned char middle_dehydration_settle_timer;  //中脱和高脱之间的停留时间
unsigned int middle_dehydration_max_run_timer; //中脱最长不超过的运行时间
unsigned int high_dehydration_max_run_timer;   //高脱最长不超过的运行时间
unsigned char middle_dehydration_stop_wait_timer;//中脱程序结束等待时间
unsigned char high_dehydration_stop_wait_timer;  //高脱程序结束等待时间 
unsigned char middle_dehydration_stop_skid_time; //中脱结束刹车时间
unsigned char high_dehydration_stop_skid_time;   //高脱结束刹车时间 
//unsigned char scan_dehydration_num;//扫描用脱水值
unsigned char trans_num;//重复发送的次数
unsigned char wait_trans_time;//重复发送的间隔时间
unsigned char shake_num;//超震计数
unsigned int rxe_receive_timer;//通讯间隔计时
unsigned int ad_fault_num;//AD转换故障计时

unsigned char buzzer_timer;
unsigned char buzzer_num;
unsigned char _buzzer_run;
unsigned char _tell_alarm;
unsigned char _tell_stop;
//unsigned char data_number;//数据个数
//unsigned char data_address;//数据地址
//unsigned char data_page;
//unsigned char checkout;
//unsigned int run_anyone_step_timer;
extern unsigned int main_cycle_1s_timer;
extern unsigned int flag_fault_display;
extern unsigned char _glitter_warm_led;
extern unsigned char _glitter_heat_led;
extern unsigned char _glitter_drain_led;
extern unsigned char _glitter_spin_led;
extern unsigned char _glitter_wash_led;
extern unsigned char _glitter_cold_led;

//unsigned char manual_mode_m;//手动模式运行时间分
//unsigned char manual_mode_s;//手动模式运行时间秒
//unsigned char manual_mode_temp;//手动模式设定温度
//unsigned char manual_mode_liquid_height;//手动设定模式液位

unsigned char step_num;

unsigned char get_clothes_provide_timer; // azalea???
unsigned char deasil_run_time; // azalea???
unsigned char deasil_stop_time; // azalea???
unsigned char anticlockwise_run_time; // azalea???
unsigned char anticlockwise_stop_time;
unsigned char lcd_chosen_sub_step; //azalea???
//unsigned char receive_delay;
//unsigned int timer_800ms;
//unsigned char data_num;
unsigned char all_time_h;
unsigned char all_time_m;
unsigned char all_time_s;

typedef union {
  unsigned char BYTE;
  struct {
    unsigned char b0    :1;
    unsigned char b1    :1;
    unsigned char b2    :1;
    unsigned char b3    :1;
    unsigned char b4    :1;
    unsigned char b5    :1;
    unsigned char b6    :1;
    unsigned char b7    :1;
  }bool;
}FLAGBYTE;

volatile FLAGBYTE DATA1; 
#define data1 DATA1.BYTE
#define _clear_for_stop			  DATA1.bool.b0
#define _dis_open_close_door 	DATA1.bool.b1
#define _get_clothes_function DATA1.bool.b2
#define _enable_single_wash		DATA1.bool.b3  //允许均布
#define _had_single_way_wash  DATA1.bool.b4  //已经有均布
#define _all_time  					  DATA1.bool.b5
#define _error_commu          DATA1.bool.b6   //crc校验错误，超过45次报错
#define _clear_for_start			DATA1.bool.b7  //机器当前状态  1-运行  0-停止

volatile FLAGBYTE DATA2; 
#define data2 DATA2.BYTE
#define _have_wash			 		 	DATA2.bool.b0 //洗涤标志
#define _enable_wash 					DATA2.bool.b1	//运行洗涤
#define _have_cool_water			DATA2.bool.b2 //有冷水
#define _add_cool_water_stop 	DATA2.bool.b3 //停止添加冷水
#define _have_hot_water 			DATA2.bool.b4	
#define _add_hot_water_stop		DATA2.bool.b5 //停止添加热水	
#define _have_calefaction   	DATA2.bool.b6
#define _had_drain_double_dis DATA2.bool.b7 //azalea???

volatile FLAGBYTE DATA3; 
#define data3 DATA3.BYTE
#define _start_set_program        DATA3.bool.b0
#define	_wash_enable 				   	  DATA3.bool.b1  //洗涤
#define	_cold_enable              DATA3.bool.b2  //冷水
#define	_warm_enable 					  	DATA3.bool.b3  //热水
#define	_heat_enable  						DATA3.bool.b4  //加热
#define	_soap_enable 						  DATA3.bool.b5  //皂液
#define _drain_enable 						DATA3.bool.b6  //排水
#define _spin_enable  						DATA3.bool.b7  //脱水 
	
volatile FLAGBYTE DATA4; 
#define data4 DATA4.BYTE
#define _had_soap     					  DATA4.bool.b0  //已经有皂液了
#define _had_drain    					  DATA4.bool.b1 //已经有排水
#define _had_spin     					  DATA4.bool.b2 //已经有脱水 
#define _num_4        					  DATA4.bool.b3 // azalea???
#define _num_3        					  DATA4.bool.b4 // azalea???
#define _num_2        					  DATA4.bool.b5 // azalea???
#define _num_1       						  DATA4.bool.b6 // azalea???
#define _had_wash    						  DATA4.bool.b7 //已经有洗涤标志了
	
volatile FLAGBYTE DATA5; 
#define data5 DATA5.BYTE
#define _enable_scan_one 					DATA5.bool.b0//允许按键扫描
#define _enable_scan_two 					DATA5.bool.b1//允许按键扫描
#define _enable_scan_three 				DATA5.bool.b2//允许按键扫描
#define _enable_scan_four 				DATA5.bool.b3//允许按键扫描
//#define _buzzer_run 							DATA5.bool.b4//设置数据
#define _have_fault								DATA5.bool.b5
#define _sign_13 									DATA5.bool.b6
#define _wash_txd									DATA5.bool.b7 //洗涤发送标志

volatile FLAGBYTE DATA6; 
#define data6 DATA6.BYTE
#define _enable_ini_program 			DATA6.bool.b0 //运行初始化
#define _high_dehydration_txd     DATA6.bool.b2 //有效震动标志
#define _enable_dehydration				DATA6.bool.b3 //运行脱水
//#define _enable_commu_clear_fault DATA6.bool.b5
#define _dehydration_stop_pro     DATA6.bool.b6
#define _middle_dehydration_txd   DATA6.bool.b7
	
volatile FLAGBYTE DATA7; 
#define data7 DATA7.BYTE
#define _auto_mode_run_stop       DATA7.bool.b0 //自动模式结束运行标志
#define _single_way_wash_stop_pro DATA7.bool.b1 //均布结束和刹车处理标志
#define _single_wash_dis          DATA7.bool.b2 //均布显示
#define _single_wash_txd					DATA7.bool.b3 //均布通讯
#define _soap_liquid_1						DATA7.bool.b4
#define _soap_liquid_2						DATA7.bool.b5	
#define _soap_liquid_3						DATA7.bool.b6	
#define _drain										DATA7.bool.b7
	
volatile FLAGBYTE DATA8; 
#define data8 DATA8.BYTE 
#define _shake_bit								DATA8.bool.b0
#define _new_step							  	DATA8.bool.b1
#define _wait_trans							  DATA8.bool.b2 //等待再次发送标志
#define _fault_pro_complete				DATA8.bool.b3 
#define _key_pro_ok								DATA8.bool.b3
#define _positive_reverse_dis     DATA8.bool.b4 //有高脱标志
#define _ini_program_ok           DATA8.bool.b5 //主程序初始化完成
#define _error_door_open					DATA8.bool.b6
#define _no_read_fault						DATA8.bool.b7

volatile FLAGBYTE DATA9; 
#define data9 DATA9.BYTE 
//#define _tell_stop 								DATA9.bool.b0 //停止标志
#define _error_shake							DATA9.bool.b1 //震动故障
#define _soap_liquid_pro					DATA9.bool.b2 //皂液处理标志
//#define _tell_alarm								DATA9.bool.b5
//#define _init_param								DATA9.bool.b6 //初始化参数标志
//#define _init_proce								DATA9.bool.b7	//初始化程序标志
#endif



