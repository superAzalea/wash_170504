#ifndef __MAIN_H
#define __MAIN_H
#ifdef __MAIN_C
unsigned int main_cycle_1s_timer;
unsigned int  flag_fault_display;

unsigned char _set_mode_symbol;
unsigned char _if_save_data;
unsigned char auto_run_mode;//程序模式
unsigned char usart3_flag_fault_clear;

unsigned char HMICommuINV_OK = 0;
unsigned char _enable_commu_Inventer;

unsigned char _glitter_warm_led;
unsigned char _glitter_heat_led;
unsigned char _glitter_drain_led;
unsigned char _glitter_spin_led;
unsigned char _glitter_wash_led;
unsigned char _glitter_cold_led;
unsigned char back_light_timer;//背光时间

#else


#endif
#endif

