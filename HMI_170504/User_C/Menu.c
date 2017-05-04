/**************************************************************************************************
Project: TTK_ACPR
Filename: menu.c
Description: time interruput functions for air compressor.
Complier: Keil uVision5.
***************************************************************************************************
Copyright (c) 2014 TTK.
All rights reserved.
***************************************************************************************************
DESCRIPTION:
Revision History:
Original Author:JesonsSun
First edition Time:20140601
Last Review Time:20140701 11:11;V1.0
**************************************************************************************************/
#ifndef __Menu_C
#define __Menu_C
#include "stm32f10x.h"
#include "SystemInit.h"
#include  "define.h"
#include "Typedef.h"
#include "LCD_display.h"
#include "key.h"
#include "SysPr.h"
#include "calendar.h"
#include "menu_sub.h"
#include "timer.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "Menu.h"
#include "string.h"
#include "DataHandle.h"
#include "struct_comm.h"
#include "ziku.h"
extern unsigned char auto_run_mode;
extern unsigned char _enable_commu_Inventer;
extern unsigned char usart3_flag_fault_clear;
extern unsigned char flag_read_Inv;

u16 Counter_DisBuff = 0;
u8 fault_count_back = 0;
void main_program_pro(void);
void course_auto_manual_mode(void);

//*************************
void ClearDisBuff()
{
	u8 i = 0;
	for(i=0;i<121;i++)	DisBuff[i] = 0x20;	
};

void sendtobuff(u8 h,u8 l,char *p)
{
	Counter_DisBuff = h*30 + l;
	while(*p != '\0')// 直到字符串的结束
	{
		DisBuff[Counter_DisBuff] = *p++;
		Counter_DisBuff++;
	}	
}; 
void Lcd_KEYPro(void)
{
     switch(Menu)
	 {
		 case MENUCUR0: 
			DispMod = dismod0;			 
			key_mainmenu_handler();
		 break;		
		 case MENUCUR1://查看方式、修改方式选择界面
			DispMod = dismod1;	 
			key_menu_first_page_handler();
		 break;				 
		 case MENUCUR2://设置密码界面
			DispMod = dismod2;
     		key_menu_second_page_handler();//dismod2
		 break;
		 case MENUCUR3://功能列表界面
			DispMod = dismod3;
			key_main_set_menu_handler();
		 break;	
		 case MENUCUR4://各功能界面
			DispMod = dismod4;
			switch(dismenu.sub_menucur)
			{
				case 1: //P01 
				   key_lcd_menu_one_handler();
			    break;
				case 2:// P02 功能设置1
				   key_lcd_menu_two_handler();
				break;
				case 3://P03 功能设置2
				   key_lcd_menu_three_handler();
				   break;				
				case 4://P04 输入定义
				   key_lcd_menu_four_handler();
				break;				
				case 5:// P05 输出定义
				   key_lcd_menu_five_handler();
				break;
				case 6:// P06 液位设置
				   key_lcd_menu_six_handler();
				break;
				case 7://P07 皂液动作时间
				   key_lcd_menu_seven_handler();
				break;
				case 8://P08 洗涤时间设置
				   key_lcd_menu_eight_handler();
				break;
				case 9://P09 脱水过渡时间
				   key_lcd_menu_nine_handler();
				break;
				case 10:// P10 脱水结束延时
				   key_lcd_menu_ten_handler();
				break;
				case 11:// P11取衣时间设置
				   key_lcd_menu_eleven_handler();
				break;		
			    case 12://P12 温度设置
				   key_lcd_menu_twelve_handler();
			    break;
				case 13://P13 高脱设置
				   key_lcd_menu_thirteen_handler();
			    break;
				case 14:// P14 速度设置
				   key_lcd_menu_fourteen_handler();
			    break;
				case 15:	//P15 液位高度校准
				   key_lcd_menu_fifteen_handler();
				break;
				default:// P16双速电机测试
				   key_lcd_menu_sixteen_handler();
			    break;    	
			}
		break;						
		case  MENUCUR5:
		   DispMod = dismod5;
		   key_menu_fault_page_handler();
		break;						
		case MENUCUR6: 
		   DispMod = dismod6;
		   switch(dismenu.sub_menucur)
		   {
			   case 0:
				  key_menu_set_program_handler();
			   break;
			   case 1:
				  key_menu_save_data_handler();
			   break;
			   default:
			   break;
			} 
		break;								
		case MENUCUR7:
		   DispMod = dismod7;
		   key_wash_complete_menu_handler();
		break; 				          
		case MENUCUR8:
			DispMod = dismod8;
			key_shake_stop_menu_handler();
		break;	
	    case MENUCUR9:
			DispMod = dismod9;
			key_instancy_stop_menu_handler();
		break;							
		case MENUCUR10:
		DispMod = dismod10;
		key_menu_fault_dis_handler();
		break;
		default:
		break;			
	 }	 
	 KeyVal = 0;
}

 void LCDDisplay(void)
{
	if(DispMod_Old !=DispMod)
	{
		 if(passdisp.disp_flg) //刷屏标志
		 {
				passdisp.disp_flg = 0;
		 }
		 DispMod_Old = DispMod;
	}
	switch(DispMod)  //
	{
		case dismod0:
			lcd_mainmenu();//主菜单 
		break;

		case dismod1:
			menu_first_page();//（查找方式，修改方式）
		break;

		case dismod2:
			menu_second_page();//(密码设置)
		break;
		case dismod3:
			main_set_menu();//设置菜单
		break;
		case dismod4:
			if((dismenu.sub_menucur>0)&&(dismenu.sub_menucur<17))//（P01~P16）个查询与修改菜单
			{
				(*RunPrFunTable[dismenu.sub_menucur - 1])();	
			}
		break;
		case dismod5:
		 menu_fault_page();//具体故障显示界面
		break;
		case dismod6:
		switch(dismenu.sub_menucur)
		{
			case 0:
				menu_set_program();//dismod6_1
				break;
			default:
				menu_save_data(); 
				break;	
		}
		break;
		case dismod7:
			lcd_wash_complete_menu();
		break;
		case dismod8:
			lcd_shake_stop_menu();
		break;
		case dismod9:
			lcd_instancy_stop_menu();
		break;
		case dismod10:
			menu_fault_dis();
		break;
		case dismod11:
			menu_handshake_dis();
		break;
		default:
		break;	 
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lcd_mainmenu(void)////主菜单
{
	char aa[6];
//	char c;
	int  i;
	for(i = 0;i <6; i++)//刷新密码
	{
		passdisp.dispdata[i] = 0;
	}
//------------------程序-------------------- 
	 if(_manual_mode)
	 {
		lcd_wrstring(0,0,"手动");				 
	 } 
	 else
	 {  	
		switch(auto_run_mode)
		{  //P12[1]){
		case 1:
			lcd_wrstring(0,0,"标准");
		 break;
		case 2:
			lcd_wrstring(0,0,"床单");
		 break;
		case 3:
			lcd_wrstring(0,0,"台布");
		 break;
		case 4:
			lcd_wrstring(0,0,"衬衣");
		 break;
		default:
			lcd_wrstring(0,0,"毛巾");
		 break;
		}
	 }

	 if(passdisp.disp_flg==0)
	 {
		 sendtobuff(0,0,dispMainMenu);
		 lcd_wrstring(0,0,DisBuff);
		 passdisp.disp_flg++;
	 }
		 
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 	
	if(run_state == 0 && _manual_mode == 0)//_manual_mode = 0 洗衣停止且自动模式
	 {
		 lcd_wrstring(3, 3,"故障");
		 lcd_wrstring(3,10,"程序");
		 lcd_wrstring(3,16,"编程");
		 lcd_wrstring(3,23,"设置");
			
		 if(input_door_switch ==0)
		{
			 lcd_wrstring(1,28,"憨");

		}
		else
		{
			 lcd_wrstring(1,28,"翰");

		}
			
		 if(_enable_display && _manual_mode == 0 )
		 {//每1秒置1
			_enable_display = 0;
			if(fact_temp > 100)	
			{
				fact_temp = fact_temp - 100;
				if(fact_temp > 100)
				{
					fact_temp = fact_temp - 100;

				}
			}	

			disphex_asc_le(aa,fact_temp,3);//实际温度
			if(aa[0] == '0')
			{
				aa[0] = ' ';
			}
			if(aa[1] == '0')
			{
				aa[1] = ' ';
			}
			lcd_wrstring_num(2,8,aa,3);
			
			disphex_asc_le(aa,fact_liquid_height,4);//实际液位
			if(aa[0] == '0')
			{
				aa[0] = ' ';
			}
			lcd_wrstring_num(2,23,aa,4); 
		 }		
		//////////设定液位/////////////////////////
		 if(run_state && _manual_mode == 0)
		 {
			 disphex_asc_le(aa,set_liquid_height,2);
			 if(aa[0] == '0')
			 {
				 aa[0] = ' ';
			 }
			 lcd_wrstring_num(1,23,aa,2);
		 }
		 else
		 {
			 disphex_asc_le(aa,Code_I.P6[1],2);//中液位高度 
			 if(aa[0] == '0')
			 {
				 aa[0] = ' ';
			 }
			 lcd_wrstring_num(1,23,aa,2);
		 }
				 
		//////////////设定温度//////////////////////// 
		 if(run_state && _manual_mode == 0)
		 {
				disphex_asc_le(aa,set_temp,3);
		 }
		 else
		 {
				 disphex_asc_le(aa,Code_I.P12[0],3);//手动默认温度
				 set_temp = Code_I.P12[0];
		 }
		 if(aa[0] == '0')
		 {
				aa[0] = ' ';
		 }
		 if(aa[1] == '0')
		 {
				aa[1] = ' ';
		 }
		 lcd_wrstring_num(1,8,aa,3);

//		 if(_set_mode_symbol)
//		 {
//				dehydration_num = 0;
//				_set_mode_symbol = 0;//没有进入设定模式
//		 }				 				
		 _dis_open_close_door = 1;  //显示开关门标志
		 
		 if(input_door_switch ==0) // azalea_wait_for_modify 门开关 JP5
		 {
			 lcd_wrstring(0,12,"请关门  ");
		 }
		 else if((single_wash_complete_delay_time || single_wash_complete_skid
		    || middle_dehydration_stop_wait_timer || middle_dehydration_stop_skid_time
		    || high_dehydration_stop_wait_timer   || high_dehydration_stop_skid_time)
			                && _get_clothes_function == 0)
		 {//均布与中脱，高脱结束或刹车时间
				lcd_wrstring(0,12,"刹车延时");// azalea???
		 }
		 else if( single_wash_complete_delay_time == 0 && single_wash_complete_skid == 0
			&& middle_dehydration_stop_wait_timer == 0 && middle_dehydration_stop_skid_time == 0 
		    &&   high_dehydration_stop_wait_timer == 0 && high_dehydration_stop_skid_time == 0 
							&&   _get_clothes_function == 0)
		 {
				lcd_wrstring(0,12,"可以开门"); 
		 }
		 else if(_get_clothes_function)
		 {
				lcd_wrstring(0,12,"运行取衣"); 
		 }
		 else
		 {
				lcd_wrstring(0,12,"        ");
		 }
				 
	 }
	 else  //运行状态  azalea ???
	 { 	 
		if(_clear_for_start)
		{
			  _clear_for_start = 0;
			  lcd_wrstring(3,0,"                              ");
		}
		////////////////////////////////////////////////////   
		 if(input_door_switch ==0)
		{
			 lcd_wrstring(1,28,"憨");

		}
		else
		{
			 lcd_wrstring(1,28,"翰");

		}
		if(_manual_mode == 0) //非手动模式
		{
			lcd_wrstring(0,9,"步");
			disphex_asc_le(aa,program_step_num+1,2); 
			if(aa[0] == '0')
			{
				aa[0] = ' ';
			}	
			lcd_wrstring_num(0,11,aa,2);
				 
		}
		else
		{
			lcd_wrstring_num(0,9,"  ",2);
			lcd_wrstring_num(0,11,"  ",2);
		}
			///////////////////时间///////////////////////////
		if(_dis_open_close_door)
		{
			 _dis_open_close_door = 0;
			 lcd_wrstring(0,12,"            ");
		}
		if((middle_dehydration_stop_wait_timer || middle_dehydration_stop_skid_time 
		   || high_dehydration_stop_wait_timer || high_dehydration_stop_skid_time 
		   ||  single_wash_complete_delay_time || single_wash_complete_skid) 
								 && run_state)
		{  
			//azalea???  反显机制不一样--- azlea_wait_for_modify(高脱标志)
					//passdisp.set_rev_flg = (passdisp.pst_rev_dis)?0:1;
			passdisp.set_rev_flg = (passdisp.set_rev_flg)?0:1;
		} 
		else
		{
					passdisp.set_rev_flg = 0;    
		}
		// 自动模式运行时间
		 disphex_asc_le(aa,auto_mode_m,2);//自动模式运行时间 分
		 lcd_wrstring_num(0,14,aa,2);
		 lcd_wrstring_num(0,16,":",1);
		 disphex_asc_le(aa,auto_mode_s,2);//自动模式运行时间 秒
		 lcd_wrstring_num(0,17,aa,2);
		 passdisp.set_rev_flg = 0; 
		
		//////////////////////////////////////////////////
		
		if(_have_wash)
		{	
			  if(_enable_wash)
			  {
						
					lcd_wrstring(3,0,"洗涤");
					 LED_WASH = 0;
			  }
			  else
			  {
					passdisp.set_rev_flg = 1; //azalea.du 17.03.15
					lcd_wrstring(3,0,"洗涤");
					passdisp.set_rev_flg = 0;
					//LED_WASH = 0;
					_glitter_wash_led = 1;
			  }
		}	
		else if(run_state)
		{
			if(_enable_single_wash)
			{
				if(_had_single_way_wash && wash_1 != wash_2)// azalea_wait_for_modify 变频器继电器x1,x2  JP7
				{
						lcd_wrstring(3,0,"均布");
						LED_WASH = 0;
				}
				else
				{
						passdisp.set_rev_flg = 1; // azalea.du 170315
						lcd_wrstring(3,0,"均布"); 
						passdisp.set_rev_flg = 0; 
						//	LED_WASH = 0;
						_glitter_wash_led = 1;

				}
			}
			else
			{
				_glitter_wash_led = 0;
				LED_WASH = 1;
				lcd_wrstring(3,0,"    "); 
			}
		}
		else
		{
		   LED_WASH = 1;
			_glitter_wash_led = 0;
			lcd_wrstring(3,0,"    ");
		}	
		///冷水ok///////////////////////////////////////////////
		if(_have_cool_water == 0 && cool_water == 0) // azalea_wait_for_modify  下位机传过来的的冷水 JP6
		{
			 lcd_wrstring(3,5,"    "); 
			 LED_COLD = 1;
			 _glitter_cold_led = 0;
		} 
		else
		{
			 if(_add_cool_water_stop)
			 {
				 passdisp.set_rev_flg = 1; // azalea.du 170315
				 lcd_wrstring(3,5,"冷水");
				 passdisp.set_rev_flg = 0;
				_glitter_cold_led = 1;							

			 } 
			 else
			 {
				lcd_wrstring(3,5,"冷水");
				LED_COLD = 0;						 
			 } 
			 
		}	
		///热水///////////////////////////////////////////////
		if(_have_hot_water == 0 && hot_water == 0) //azalea_wait_for_modify 热水 JP6
		{
			 lcd_wrstring(3,10,"    ");
			 LED_WARM = 1;
			 _glitter_warm_led = 0;
		} 
		else
		{
			 if(_add_hot_water_stop || fact_liquid_height < Code_I.P3[2]) //实际液位<自动补热温差
			 {
				 passdisp.set_rev_flg = 1; // azalea.du 170315
				 lcd_wrstring(3,10,"热水");
				 passdisp.set_rev_flg = 0; 
				_glitter_warm_led = 1;								
			 } 
			 else
			 {
				 LED_WARM = 0;
				 lcd_wrstring(3,10,"热水");      
			 } 
									 
		}   
		///加热ok///////////////////////////////////////////////
		if(_have_calefaction == 0 && calefaction == 0) // azalea_wait_for_modify 加热 JP6
		{
			lcd_wrstring(3,15,"    ");
			LED_HEAT = 1;					 
			 _glitter_heat_led = 0;
		} 
		else
		{
			if(calefaction == 0)
			{
				 passdisp.set_rev_flg = 1; // azalea.du 170315
				 lcd_wrstring(3,15,"加热");
				 passdisp.set_rev_flg = 0; 
					_glitter_heat_led = 1;
					
			} 
			else
			{      
				lcd_wrstring(3,15,"加热");
				LED_HEAT = 0;						 
			}  
		 }   
		////////////////////////////////////////////////// 
		if(_soap_enable == 0 && _had_soap)
		{  //皂液功能已去除但皂液还显示
			 _had_soap = 0;//清皂液
			 lcd_wrstring(3,20,"    ");
			 lcd_wrstring(3,24," "); 
		 }
		 if(_drain_enable == 0 && _had_drain )
		 { //排水功能已去除但排水还显示
				_had_drain = 0;//请排水     
				lcd_wrstring(3,23,"    ");
		 }
		 if(_spin_enable == 0 && _had_spin)
		 {  //脱水功能已去除但脱水还显示
				_had_spin = 0;//清脱水
				lcd_wrstring(3,26,"    ");
		 }
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
		if(_soap_enable)
		{
			 _had_soap = 1;//已经有皂液了
			 lcd_wrstring(3,20,"皂液");
				
			 switch(soap_liquid_num)
			 {
			 case 1:
				 lcd_wrstring_num(3,24,"1",1);
				 break;
			 case 2:
				 lcd_wrstring_num(3,24,"2",1);
				 break;
			 default:
				 lcd_wrstring_num(3,24,"3",1);
				 break;
			 } 
			 LED_SOAP = 0;
		  }
		  else
		  {
				LED_SOAP = 1;
		  }					
			 ///排水////////////////////////////////////////////////
		 if(_manual_mode == 0)
		 {
			 if(_drain_enable)
			 {
				 LED_DRAIN = 0;
				 _had_drain = 1;//已经有皂液
			
				 lcd_wrstring(3,23,"排水");
				 _had_drain_double_dis = 0; // azalea???
			 }    
		 }
		 else
		 {
			 if(_enable_single_wash)
			 {
				 if(_had_single_way_wash == 0)
				 {
					// passdisp.set_rev_flg = 1; 
					 lcd_wrstring(3,23,"排水");
					 _had_drain_double_dis = 1;
					// passdisp.set_rev_flg = 0; 
					_glitter_drain_led = 1;	   
				 }
				 else
				 {
					 _had_drain = 1;//已经有皂液
					 lcd_wrstring(3,23,"排水");
					 _had_drain_double_dis = 0;  
					 LED_DRAIN = 0;	     
				 } 
			 } 
			 else if(_had_drain_double_dis)
			 {
					 _had_drain_double_dis = 0;
					 lcd_wrstring(3,23,"    ");
				 LED_DRAIN = 1;
				   _glitter_drain_led =0;
			 }
	    }
		 ///////////////////////////////////////////////////
		 if(_spin_enable)
		 {
				 _had_spin = 1;//有脱水 
				 lcd_wrstring(3,28,"脱");
				 if(dehydration_num == 'Z') //第几个脱水
				 {
					 lcd_wrstring(3,26,"中");
					 LED_SPIN = 0;
				 }
				 else
				 {
					 lcd_wrstring(3,26,"高");
					// LED_SPIN = 0;
					_glitter_spin_led = 1;		
				 }
		 }
		 else
		 {
			LED_SPIN = 1;
			_glitter_spin_led = 0;

		 }					

//----------------------手动模式下显示实际温度、液位；设定温度、液位-------------------------------------
		if(_enable_display && _manual_mode == 1)
		{//每1秒置1
			 _enable_display = 0;
			
			
			 disphex_asc_le(aa,13,3);//实际温度
			 if(aa[0] == '0'){
				 aa[0] = ' ';
			 }
			 if(aa[1] == '0'){
				 aa[1] = ' ';
			 }
			 lcd_wrstring_num(2,8,aa,3);
	//			 if(fact_temp > 100)
	//			 {
	//					 c = fact_temp  - 100;
	//					// disphex_asc_le(aa,c,3);//实际温度
	//					 disphex_asc_le(aa,12,3);//实际温度

	//					 if(aa[0] != '0'){
	//						 aa[0] = '-';
	//					 } 
	//					 else{
	//						aa[1] = ' ';
	//						aa[1] = '-';
	//					 }
	//					 lcd_wrstring_num(2,8,aa,3); 
	//			 } 
	//			 else
	//			 {
	//					 //disphex_asc_le(aa,fact_temp,3);//实际温度
	//				 disphex_asc_le(aa,13,3);//实际温度
	//					 if(aa[0] == '0'){
	//						 aa[0] = ' ';
	//					 }
	//					 if(aa[1] == '0'){
	//						 aa[1] = ' ';
	//					 }
	//					 lcd_wrstring_num(2,8,aa,3);
	//			 } 
			 disphex_asc_le(aa,fact_liquid_height,2);//实际液位
			 if(aa[0] == '0'){
				 aa[0] = ' ';
			 }
			 lcd_wrstring_num(2,23,aa,2); 
		}
				
	
		 //////////设定液位/////////////////////////
		 if(run_state && _manual_mode == 1)
		 {
				 disphex_asc_le(aa,set_liquid_height,2);
				 if(aa[0] == '0'){
					 aa[0] = ' ';
				 }
				 lcd_wrstring_num(1,23,aa,2);
		 }
		 else
		 {
				 disphex_asc_le(aa,Code_I.P6[1],2);//中液位高度 
				 if(aa[0] == '0'){
					 aa[0] = ' ';
				 }
				 //_lcd_setrevflag = 1;
				 lcd_wrstring_num(1,23,aa,2);
				 //_lcd_setrevflag = 0;
		 }
			//////////////设定温度//////////////////////// 
		 if(run_state && _manual_mode == 1)
		 {
				disphex_asc_le(aa,set_temp,3);
		 }
		 else
		 {
				 disphex_asc_le(aa,Code_I.P12[0],3);//手动默认温度
				 set_temp = Code_I.P12[0];
		 }
		 if(aa[0] == '0'){
				aa[0] = ' ';
		 }
		 if(aa[1] == '0'){
				aa[1] = ' ';
		 }
		 lcd_wrstring_num(1,8,aa,3);
		 if(_set_mode_symbol)
		 {
				dehydration_num = 0;
				_set_mode_symbol = 0;//没有进入设定模式
		 }
		////////////运行总时间//////////////
		if(run_state)
		{
				if(_all_time == 0)
				{
						_all_time = 1;
						//lcd_wrstring(0,20,"[  :  :  ]");
						lcd_wrstring_num(0,20,"[  :  :  ]",10);

				}
				disphex_asc_le(aa,all_time_h,2);
				lcd_wrstring_num(0,21,aa,2);

				disphex_asc_le(aa,all_time_m,2);
				lcd_wrstring_num(0,24,aa,2);

				disphex_asc_le(aa,all_time_s,2);
				lcd_wrstring_num(0,27,aa,2); 
		} 
		else
		{
				_all_time = 0;
		}
			 
			 
	 }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void menu_first_page(void)//（查找方式，修改方式）
{
	u8 t;
	lcd_maxcur = (sizeof(dismodeMenu)/30-3);
	if(passdisp.disp_flg==0)
    {
		sendtobuff(0,0,dismodeMenu);
		lcd_wrstring(0,0,DisBuff);
		passdisp.disp_flg++;
	}
	t = (u8)(dismenu.sub_chosecur%3);
	kp_menucur(0,0,t);//》光标设置
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void menu_second_page(void)//(密码设置) azalea???
{
	 u8  i;
	 if(passdisp.disp_flg==0)
	 {
		 sendtobuff(0,0,dismmMenu);
		 lcd_wrstring(0,0,DisBuff);
		 passdisp.disp_flg++;
	 }
	 /* 需要刷新一下密码，以解决有光标的那一位数据保留以前值得问题   azalea??? */
	  passportdata[passdisp.dispnum] = passdisp.dispdata[passdisp.dispnum];
	// passportdata[passdisp.dispnum]=passdisp.key_data; //增加键值，密码相应位会增加  azalea???

	 for(i=0;i<6;i++)
	 {
		passportdata[i]= passportdata[i]+0x30;//转化成ASCII码
	 }
	
	 if(passdisp.dispnum==0) 
	 {		
		if(passdisp.pst_rev_dis)	
		{			 
			passdisp.set_rev_flg = 1;
		}
	 }
	 lcd_wrstring_num(2,12,&passportdata[0],1);  
	 passdisp.set_rev_flg = 0;

	 if(passdisp.dispnum==1) 
	 {
		if(passdisp.pst_rev_dis)
		{
			passdisp.set_rev_flg = 1;  
		}
	}
	lcd_wrstring_num(2,13,&passportdata[1],1);
	passdisp.set_rev_flg = 0;
	if(passdisp.dispnum==2)
	{
		if(passdisp.pst_rev_dis)
		{
			passdisp.set_rev_flg = 1;  
		}
	}	
	lcd_wrstring_num(2,14,&passportdata[2],1);
	passdisp.set_rev_flg = 0;		
	
	if(passdisp.dispnum==3) 
	{
		if(passdisp.pst_rev_dis)
		{
			passdisp.set_rev_flg = 1;  
		}
	}		
	lcd_wrstring_num(2,15,&passportdata[3],1);
	passdisp.set_rev_flg = 0;	
	
	if(passdisp.dispnum==4)
	{
		if(passdisp.pst_rev_dis)
		{
			passdisp.set_rev_flg = 1;  
		}		
	}
	lcd_wrstring_num(2,16,&passportdata[4],1);
	passdisp.set_rev_flg = 0;	
	
	if(passdisp.dispnum==5)
	{
		if(passdisp.pst_rev_dis)
		{
			passdisp.set_rev_flg = 1; 
		}
	}
	lcd_wrstring_num(2,17,&passportdata[5],1);
	passdisp.set_rev_flg = 0;		
	
	for(i=0;i<6;i++)
	{
		passportdata[i]= passportdata[i]-0x30;
	}

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void main_set_menu(void)//设置菜单
{
	u8 i;
	u8 t;

	lcd_maxcur = (sizeof(dis_main_menu)/30)-1;//16-1
	lcd_cpage = dismenu.sub_chosecur/3;
	i = (u8)((lcd_cpage < lcd_maxcur/3)?3:(lcd_maxcur%3 + 1));
	if(i==1)
	{
		ClearDisBuff();	
	}
	(void)strncpy(DisBuff,dis_main_menu+90*lcd_cpage,30*i);
	 if(lcd_cpage_old != lcd_cpage)
	 {
		passdisp.disp_flg = 0;
	 }
	 if(passdisp.disp_flg==0)
	 {
		 lcd_wrstring(0,0,DisBuff);
		 passdisp.disp_flg++;
	 }
	 lcd_cpage_old = lcd_cpage;	 

	 t = (dismenu.dis_mode)?1:0;
     lcd_lastline(1,t);

     t = (u8)(dismenu.sub_chosecur%3);
     kp_menucur(0,0,t);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lcd_menu_one(void) //P01
{
	 u8 i,t;
	 char aa[6];
	 if(_new_page)  // azalea???
	 {
		_new_page = 0; 
	 }
	 lcd_maxcur = (sizeof(dis_sub_menu_one)/30)-1;//4-1  行
	 lcd_cpage = dismenu.sub_chosecur/3;//3个是一页

	 i = (u8)((lcd_cpage < lcd_maxcur/3)?3:(lcd_maxcur%3 + 1));
	 (void)strncpy(DisBuff,dis_sub_menu_one+90*lcd_cpage,30*i);
	 if(lcd_cpage_old != lcd_cpage)
	 {
		passdisp.disp_flg = 0;
	 }
	 if(passdisp.disp_flg==0)
	 {
		 lcd_wrstring(0,0,DisBuff);
		 passdisp.disp_flg++;
	 }
	 lcd_cpage_old = lcd_cpage;
	 i = (dismenu.data_mode)?2:1;
	 t = (dismenu.dis_mode)?1:0;  
	 lcd_lastline(i,t);
	 t = (u8)(dismenu.sub_chosecur%3);
	 kp_menucur(0,0,t); 

	 if(dismenu.sub_chosecur <= 2)
	 {
		if(dismenu.sub_chosecur == 0)//初始化参数
		{
			if(dismenu.dis_mode)//1 修改方式
			{
				if(dismenu.data_mode )
				{
					passdisp.set_rev_flg = 1; 
					lcd_wrstring(0,22,"运行");
					passdisp.set_rev_flg = 0; 
				}
				else
				{
					lcd_wrstring(0,22,"    ");
				}
			}
			else
			{
				lcd_wrstring(0,22,"    ");
			}
		}
		if(dismenu.sub_chosecur == 1)//初始化程序
		{
			if(dismenu.dis_mode)
			{
				 if(dismenu.data_mode)//设置数据
				 { 
					 passdisp.set_rev_flg = 1; 
					 lcd_wrstring(1,22,"运行");
					 passdisp.set_rev_flg = 0; 
				 }
				 else
				 {
					 lcd_wrstring(1,22,"    ");
				 } 
			} 
			else
			{
				lcd_wrstring(1,22,"    ");
			}	 
		} 
		lcd_wrstring(2,18,"TTK变频器");
	 }
	 else
	 {
		 if(dismenu.sub_chosecur == 3 && dismenu.dis_mode && dismenu.data_mode) //机器容量，修改模式，设置数据
		 {
			 passdisp.set_rev_flg = 1;//设置参数标记
		 }
		 if(Code_I.P1[1] == 0)    //azalea???
		 {
			 lcd_wrstring(0,20,"未设置");
		 }
		 else
		 {
			 disphex_asc_le(aa,Code_I.P1[3],3);
			 
			 if(aa[0] == '0')
			 {
				 aa[0] = ' ';
				 if(aa[1] == '0')
				 {
						aa[1] = ' ';
				 }
			 }
			 lcd_wrstring_num(0,19,aa,3);
			 lcd_wrstring(0,22,"公斤");
		 }
		 passdisp.set_rev_flg = 0;
		 lcd_wrstring(1,19,"简体中文");
		 if(dismenu.dis_mode)
		 {
			for(i=0;i<6;i++)
			{
					passportdata[i]= passportdata[i]+0x30;//显示密码
			}
			if(passdisp.dispnum==0&& passdisp.pst_rev_dis&& dismenu.data_mode&&dismenu.sub_chosecur==5)
			{		 
					passdisp.set_rev_flg = 1;// 反显标记
			
			}
			lcd_wrstring_num(2,19,&passportdata[0],1);  
			passdisp.set_rev_flg = 0;    
			if(passdisp.dispnum==1&& passdisp.pst_rev_dis&& dismenu.data_mode&&dismenu.sub_chosecur==5)//passdisp.pst_rev_dis
			{
					passdisp.set_rev_flg = 1;  
			}
			lcd_wrstring_num(2,20,&passportdata[1],1);
			passdisp.set_rev_flg = 0;
			if(passdisp.dispnum==2&& passdisp.pst_rev_dis&& dismenu.data_mode&&dismenu.sub_chosecur==5)
			{		
					passdisp.set_rev_flg = 1;  
			}	
			lcd_wrstring_num(2,21,&passportdata[2],1);
			passdisp.set_rev_flg = 0;		
			
			if(passdisp.dispnum==3&& passdisp.pst_rev_dis&& dismenu.data_mode&&dismenu.sub_chosecur==5) 
			{	
					passdisp.set_rev_flg = 1;  
			}		
			lcd_wrstring_num(2,22,&passportdata[3],1);
			passdisp.set_rev_flg = 0;	
			
			if(passdisp.dispnum==4&& passdisp.pst_rev_dis&& dismenu.data_mode&&dismenu.sub_chosecur==5)
			{
				
					passdisp.set_rev_flg = 1;  
		
			}
			lcd_wrstring_num(2,23,&passportdata[4],1);
			passdisp.set_rev_flg = 0;	
			
			if(passdisp.dispnum==5&& passdisp.pst_rev_dis&& dismenu.data_mode&&dismenu.sub_chosecur==5)
			{

					passdisp.set_rev_flg = 1; 

			}
			lcd_wrstring_num(2,24,&passportdata[5],1);
			passdisp.set_rev_flg = 0;		
			
			for(i=0;i<6;i++)
			{
					passportdata[i]= passportdata[i]-0x30;
			}
		}
		else
		{ 
			 lcd_wrstring(2,19,"******");		
		}
	}  
	
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lcd_menu_two(void)// P02
{
	 u8 i,t;
	 if(_new_page){
	   _new_page = 0;  
	 }
	 lcd_maxcur = (sizeof(dis_sub_menu_two)/30)-1;//4-1
	 lcd_cpage = dismenu.sub_chosecur/3;
	 i = (u8)((lcd_cpage < lcd_maxcur/3)?3:(lcd_maxcur%3 + 1));
     if(i==2)
	 {
	    ClearDisBuff();	
	 }
	 (void)strncpy(DisBuff,dis_sub_menu_two+90*lcd_cpage,30*i);
		
	 if(lcd_cpage_old != lcd_cpage)
	 {
		passdisp.disp_flg = 0;
	 }
	 if(passdisp.disp_flg==0)
	 {
		 lcd_wrstring(0,0,DisBuff);
		 passdisp.disp_flg++;
	 }
     lcd_cpage_old = lcd_cpage;  
	 
	 i = (dismenu.data_mode)?2:1;
	 t = (dismenu.dis_mode)?1:0;
		
	 lcd_lastline(i,t);
	 t = (u8)(dismenu.sub_chosecur%3);
	 kp_menucur(0,0,t);
		
	 if(dismenu.sub_chosecur <= 2)
	 {
		if(dismenu.sub_chosecur == 0 && dismenu.dis_mode && dismenu.data_mode)
		{
			passdisp.set_rev_flg = 1;			
		}
		if(Code_I.P2[0] == 'n')
		{ 
			lcd_wrstring(0,24,"否");
		}
		else
	    {
			lcd_wrstring(0,24,"是");
		}
        passdisp.set_rev_flg = 0;
   //////////////////////////////
		 if(dismenu.sub_chosecur == 1 && dismenu.dis_mode && dismenu.data_mode)
		 {
			 passdisp.set_rev_flg = 1;			
		 }

	   if(Code_I.P2[1] == 'n')
	   { 
		 lcd_wrstring(1,24,"否");
			 
	   }
	   else
	   {
		 lcd_wrstring(1,24,"是");
	   }
      passdisp.set_rev_flg = 0;			
   //////////////////////////////
	 if(dismenu.sub_chosecur == 2 && dismenu.dis_mode && dismenu.data_mode)
	 {
		passdisp.set_rev_flg = 1;			
     }

	   if(Code_I.P2[2] == 'n'){ 
		 lcd_wrstring(2,24,"否");
	   }
	   else{
		 lcd_wrstring(2,24,"是");
	   }
        passdisp.set_rev_flg = 0;	
   
	}
	else
	{
		 if(dismenu.sub_chosecur == 3 && dismenu.dis_mode && dismenu.data_mode)
		 {
			 passdisp.set_rev_flg = 1;			
		 }
		 
		 if(Code_I.P2[3] == 'n'){ 
		lcd_wrstring(0,24,"否");
   }
   else{
     lcd_wrstring(0,24,"是");
   }
	 passdisp.set_rev_flg = 0;
	 

   /////////////////////////////////
	 if(dismenu.sub_chosecur == 4 && dismenu.dis_mode && dismenu.data_mode)
	 {
		passdisp.set_rev_flg = 1;			
	 }
	 
	 if(Code_I.P2[4] == 'n'){ 
		 lcd_wrstring(1,24,"否");
	 }
	 else{
		 lcd_wrstring(1,24,"是");
	 }
	 passdisp.set_rev_flg = 0;
   }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lcd_menu_three(void)// P03
{
 u8 i,t;
 if(_new_page){
   _new_page = 0;
 }
	 lcd_maxcur = (sizeof(dis_sub_menu_three)/30)-1;//7-1
	 lcd_cpage = dismenu.sub_chosecur/3;
	 i = (u8)((lcd_cpage < lcd_maxcur/3)?3:(lcd_maxcur%3 + 1));
    if(i==2)
		{
		  ClearDisBuff();	
		}
	 (void)strncpy(DisBuff,dis_sub_menu_three+90*lcd_cpage,30*i);
	 if(lcd_cpage_old != lcd_cpage)
	 {
	  passdisp.disp_flg = 0;
	 }
   if(passdisp.disp_flg==0)
	 {
			 lcd_wrstring(0,0,DisBuff);
			 passdisp.disp_flg++;
	 }
   lcd_cpage_old = lcd_cpage; 
 i = (dismenu.data_mode)?2:1;
 t = (dismenu.dis_mode)?1:0;    
 lcd_lastline(i,t);
 t = (u8)(dismenu.sub_chosecur%3);
 kp_menucur(0,0,t);
		
  if(dismenu.sub_chosecur <= 2)
		{
			if(Code_I.P3[0] == 'n'){ 
     lcd_wrstring(0,23,"否");
   }
   else{
     lcd_wrstring(0,23,"是");
   }

   //////////////////////////////
	 
	 if(dismenu.sub_chosecur == 1 && dismenu.dis_mode && dismenu.data_mode)
	 {
				passdisp.set_rev_flg = 1;			
   }

   disphex_asc_le(aa,Code_I.P3[1],2);
   if(aa[0] == '0'){
     aa[0] = ' ';
   }
   lcd_wrstring_num(1,23,aa,2);
	 passdisp.set_rev_flg = 0;
	 
	 if(dismenu.sub_chosecur == 2 && dismenu.dis_mode && dismenu.data_mode)
	 {
				passdisp.set_rev_flg = 1;			
   }
	 
   disphex_asc_le(aa,Code_I.P3[2],2);
   if(aa[0] == '0'){
     aa[0] = ' ';
   }
   lcd_wrstring_num(2,23,aa,2);
	 passdisp.set_rev_flg = 0;

 }
 else if(dismenu.sub_chosecur >= 3)
	 {

	 if(dismenu.dis_mode && dismenu.data_mode)
	 {
			passdisp.set_rev_flg = 1;			
   }
   if(Code_I.P3[3] == 0)
   { 
     lcd_wrstring(0,21,"不重试");
		 passdisp.set_rev_flg = 0;	
   }
   else
    {
			
     disphex_asc_le(aa,Code_I.P3[3],1);
   
      lcd_wrstring_num(0,24,aa,1);
			passdisp.set_rev_flg = 0;
			lcd_wrstring(0,21,"   ");
			lcd_wrstring(0,25,"次");
    }
   } 
 } 
 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lcd_menu_four(void) // P04
{
 u8 i,t;
 if(_new_page){
   _new_page = 0;  
 }
	 lcd_maxcur = (sizeof(dis_sub_menu_four)/30)-1;//7-1
	 lcd_cpage = dismenu.sub_chosecur/3;
	 i = (u8)((lcd_cpage < lcd_maxcur/3)?3:(lcd_maxcur%3 + 1));
    if(i==1)
		{
		  ClearDisBuff();	
		}
	 (void)strncpy(DisBuff,dis_sub_menu_four+90*lcd_cpage,30*i);
	 if(lcd_cpage_old != lcd_cpage)
	 {
				passdisp.disp_flg = 0;
	 }
   if(passdisp.disp_flg==0)
	 {
			 lcd_wrstring(0,0,DisBuff);
			 passdisp.disp_flg++;
	 }
   lcd_cpage_old = lcd_cpage;  
 i = (dismenu.data_mode)?2:1;
 t = (dismenu.dis_mode)?1:0;    
 lcd_lastline(i,t);
 t = (u8)(dismenu.sub_chosecur%3);
 kp_menucur(0,0,t);	 

 if(dismenu.sub_chosecur <= 2)
	 {
		  if(Code_I.P4[0] == 'c'){ 
     lcd_wrstring(0,23,"常闭");
   }
   else{
     lcd_wrstring(0,23,"常开");
   }

   //////////////////////////////

  /* if(Code_I.P4[1] == 'c'){ 
     lcd_wrstring(1,23,"常闭");
   }
   else{
     lcd_wrstring(1,23,"常开");
   }
*/
   //////////////////////////////

   if(Code_I.P4[2] == 'c'){ 
     lcd_wrstring(2,23,"常闭");
   }
   else{
     lcd_wrstring(2,23,"常开");
   }  

 }
 else if(dismenu.sub_chosecur>2 && dismenu.sub_chosecur <= 5)
	 {
		 if(Code_I.P4[3] == 'c'){ 
     lcd_wrstring(0,23,"常闭");
   }
   else{
     lcd_wrstring(0,23,"常开");
   }

   /////////////////////////////////  
 }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lcd_menu_five(void) // P05
{
 u8 i,t;
 if(_new_page){
   _new_page = 0;  
 }
	 lcd_maxcur = (sizeof(dis_sub_menu_five)/30)-1;//2-1
	 lcd_cpage = dismenu.sub_chosecur/3;
	 i = (u8)((lcd_cpage < lcd_maxcur/3)?3:(lcd_maxcur%3 + 1));
    if(i==2)
		{
		  ClearDisBuff();	
		}
	 (void)strncpy(DisBuff,dis_sub_menu_five+90*lcd_cpage,30*i);
	 if(passdisp.disp_flg==0)
	 {
			 lcd_wrstring(0,0,DisBuff);
			 passdisp.disp_flg++;
	 }
 
 i = (dismenu.data_mode)?2:1;
 t = (dismenu.dis_mode)?1:0;    
 lcd_lastline(i,t);
 t = (u8)(dismenu.sub_chosecur%3);
 kp_menucur(0,0,t);
 
	if(Code_I.P5[0] == 'o'){ 
   lcd_wrstring(0,23,"常开");
 }
 else{
   lcd_wrstring(0,23,"常闭");
 }

//////////////////////////////

 if(Code_I.P5[1] == 'o'){ 
   lcd_wrstring(1,23,"常开");
 }
 else{
   lcd_wrstring(1,23,"常闭");
 }

	
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lcd_menu_six(void) //P06
{
 u8 i,t;
 if(_new_page){
   _new_page = 0;  
 }
	 lcd_maxcur = (sizeof(dis_sub_menu_six)/30)-1;//7-1
	 lcd_cpage = dismenu.sub_chosecur/3;
	 i = (u8)((lcd_cpage < lcd_maxcur/3)?3:(lcd_maxcur%3 + 1));
    if(i==2)
		{
		  ClearDisBuff();	
		}
	 (void)strncpy(DisBuff,dis_sub_menu_six+90*lcd_cpage,30*i);
	 if(lcd_cpage_old != lcd_cpage)
	 {
	  passdisp.disp_flg = 0;
	 }
   if(passdisp.disp_flg==0)
	 {
			 lcd_wrstring(0,0,DisBuff);
			 passdisp.disp_flg++;
	 }
   lcd_cpage_old = lcd_cpage; 
		i = (dismenu.data_mode)?2:1; 
		t = (dismenu.dis_mode)?1:0;  
		
	 lcd_lastline(i,t);
	 t = (u8)(dismenu.sub_chosecur%3);
	 kp_menucur(0,0,t);

  if(dismenu.sub_chosecur <= 2)
		{
			if(dismenu.sub_chosecur == 0 && dismenu.dis_mode && dismenu.data_mode){
     passdisp.set_rev_flg = 1; 
   }
   disphex_asc_le(aa,Code_I.P6[0],2);
   if(aa[0] == '0'){
     aa[0] = ' ';
   }
   lcd_wrstring_num(0,24,aa,2);
	 passdisp.set_rev_flg = 0;

   //////////////////////////////
if(dismenu.sub_chosecur == 1 && dismenu.dis_mode && dismenu.data_mode){
     passdisp.set_rev_flg = 1; 
   }
   disphex_asc_le(aa,Code_I.P6[1],2);
   if(aa[0] == '0'){
     aa[0] = ' ';
   }
   lcd_wrstring_num(1,24,aa,2);
	 passdisp.set_rev_flg = 0;

   //////////////////////////////
if(dismenu.sub_chosecur == 2 && dismenu.dis_mode && dismenu.data_mode){
     passdisp.set_rev_flg = 1; 
   }
   disphex_asc_le(aa,Code_I.P6[2],2);
   if(aa[0] == '0'){
     aa[0] = ' ';
   }
   lcd_wrstring_num(2,24,aa,2);
	 passdisp.set_rev_flg = 0;

  
 }
 else if(dismenu.sub_chosecur>2 && dismenu.sub_chosecur <= 5)
 {
   if(dismenu.sub_chosecur == 3 && dismenu.dis_mode && dismenu.data_mode){
     passdisp.set_rev_flg = 1; 
   }   
		 disphex_asc_le(aa,Code_I.P6[3],2);
     if(aa[0] == '0'){
       aa[0] = ' ';
     }
		 lcd_wrstring_num(0,24,aa,2);
		 passdisp.set_rev_flg = 0;

   /////////////////////////////////
		 if(dismenu.sub_chosecur == 4 && dismenu.dis_mode && dismenu.data_mode){
     passdisp.set_rev_flg = 1; 
   }
     disphex_asc_le(aa,Code_I.P6[4],2);
     if(aa[0] == '0'){
       aa[0] = ' ';
     }
		 lcd_wrstring_num(1,24,aa,2);
		 passdisp.set_rev_flg = 0;
   ///////////////////////////////////
		 if(dismenu.sub_chosecur == 5 && dismenu.dis_mode && dismenu.data_mode){
     passdisp.set_rev_flg = 1; 
   }
      disphex_asc_le(aa,Code_I.P6[5],2);
     if(aa[0] == '0'){
       aa[0] = ' ';
     }
		 lcd_wrstring_num(2,24,aa,2);
		 passdisp.set_rev_flg = 0;

 }
 else
 {
	 if(dismenu.sub_chosecur == 7 && dismenu.dis_mode && dismenu.data_mode){  // 脱水最低液位 azalea P06
     passdisp.set_rev_flg = 1; 
   }
		 disphex_asc_le(aa,Code_I.P6[7],2);
     if(aa[0] == '0'){
       aa[0] = ' ';
     }
		 lcd_wrstring_num(1,24,aa,2);
		 passdisp.set_rev_flg = 0;
 }
 
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lcd_menu_seven(void) //P07
{
 u8 i,t;
 if(_new_page){
   _new_page = 0;  
 }
	 lcd_maxcur = (sizeof(dis_sub_menu_seven)/30)-1;//7-1
	 lcd_cpage = dismenu.sub_chosecur/3;
	 i = (u8)((lcd_cpage < lcd_maxcur/3)?3:(lcd_maxcur%3 + 1));
	 if( i == 2)
	 {
			ClearDisBuff();
	 }
	 (void)strncpy(DisBuff,dis_sub_menu_seven+90*lcd_cpage,30*i);
		if(lcd_cpage_old != lcd_cpage)
	 {
	  passdisp.disp_flg = 0;
	 }
   if(passdisp.disp_flg==0)
	 {
			 lcd_wrstring(0,0,DisBuff);
			 passdisp.disp_flg++;
	 }
	 
   lcd_cpage_old = lcd_cpage; 
		i = (dismenu.data_mode)?2:1; 
		t = (dismenu.dis_mode)?1:0;  
		
	 lcd_lastline(i,t);
	 t = (u8)(dismenu.sub_chosecur%3);
	 kp_menucur(0,0,t);
 
  if(dismenu.sub_chosecur <= 2)
		{			
			disphex_asc_le(aa,Code_I.P7[0],1);
			if(aa[0] == '0'){
			aa[0] = ' ';}
			lcd_wrstring_num(0,24,aa,1);
				
			if(dismenu.sub_chosecur == 1 && dismenu.dis_mode && dismenu.data_mode)
			{
					passdisp.set_rev_flg = 1;
			}
			disphex_asc_le(aa,Code_I.P7[1],2);
      if(aa[0] == '0'){
       aa[0] = ' ';
      }
			lcd_wrstring_num(1,23,aa,2);
			passdisp.set_rev_flg = 0;
   //////////////////////////////


 }
 else if(dismenu.sub_chosecur>2 && dismenu.sub_chosecur <= 5)
	 {

   ///////////////////////////////////		 
		 if(Code_I.P7[5] == 0)
		 { 
    // sendtobuff(2,24,"否");
			  lcd_wrstring(2,25,"否");
     }
     else
		 {
    // sendtobuff(2,24,"是");
			  lcd_wrstring(2,25,"是");
     }
		 
  
 }

	
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lcd_menu_eight(void) //P08
{
 u8 i,t;
 if(_new_page){
   _new_page = 0;  
 }
	 lcd_maxcur = (sizeof(dis_sub_menu_eight)/30)-1;//7-1
	 lcd_cpage = dismenu.sub_chosecur/3;
	 i = (u8)((lcd_cpage < lcd_maxcur/3)?3:(lcd_maxcur%3 + 1));
   if(i == 2)
	 {
				ClearDisBuff();
	 }
	 (void)strncpy(DisBuff,dis_sub_menu_eight+90*lcd_cpage,30*i);  
	 if(passdisp.disp_flg == 0)
	 {
			lcd_wrstring(0,0,DisBuff);
			passdisp.disp_flg++;
	 }
	 i = (dismenu.data_mode)?2:1;
	 t = (dismenu.dis_mode)?1:0;    
	 lcd_lastline(i,t);
	 t = (u8)(dismenu.sub_chosecur%3);
	 kp_menucur(0,0,t);  

	 if(dismenu.sub_chosecur == 0 && dismenu.dis_mode && dismenu.data_mode)
	 {
				passdisp.set_rev_flg = 1;
	 }
	 disphex_asc_le(aa,Code_I.P8[0],2);
	 if(aa[0] == '0'){
		 aa[0] = ' ';
	 }
	 lcd_wrstring_num(0,23,aa,2);
	 passdisp.set_rev_flg = 0;
	 if(dismenu.sub_chosecur == 1 && dismenu.dis_mode && dismenu.data_mode)
	 {
				passdisp.set_rev_flg = 1;
	 }
	 disphex_asc_le(aa,Code_I.P8[1],3);
	 if(aa[0] == '0'){
				aa[0] = ' ';
				if(aa[1] == '0'){
						aa[1] = ' ';
				}
		}
   lcd_wrstring_num(1,22,aa,3);
	 passdisp.set_rev_flg = 0;
	 if(dismenu.sub_chosecur == 2 && dismenu.dis_mode && dismenu.data_mode)
	 {
				passdisp.set_rev_flg = 1;
	 }
	  disphex_asc_le(aa,Code_I.P8[2],3);
	 if(aa[0] == '0'){
				aa[0] = ' ';
				if(aa[1] == '0'){
						aa[1] = ' ';
				}
	 }

	  lcd_wrstring_num(2,22,aa,3);
	 passdisp.set_rev_flg = 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lcd_menu_nine(void) //P09
{
	 u8 i,t;
	 
	 if(_new_page){
   _new_page = 0;  
 }
	 lcd_maxcur = (sizeof(dis_sub_menu_nine)/30)-1;//7-1
	 lcd_cpage = dismenu.sub_chosecur/3;
	 i = (u8)((lcd_cpage < lcd_maxcur/3)?3:(lcd_maxcur%3 + 1));
   if(i == 1)
	 {
				ClearDisBuff();
	 }
	 (void)strncpy(DisBuff,dis_sub_menu_nine+90*lcd_cpage,30*i); 
		if(lcd_cpage_old != lcd_cpage)
		{
				passdisp.disp_flg = 0;
		}
	 if(passdisp.disp_flg == 0)
	 {
			lcd_wrstring(0,0,DisBuff);
			passdisp.disp_flg++;
	 }
	 lcd_cpage_old = lcd_cpage;
	 i = (dismenu.data_mode)?2:1;
	 t = (dismenu.dis_mode)?1:0;    
	 lcd_lastline(i,t);
	 t = (u8)(dismenu.sub_chosecur%3);
	 kp_menucur(0,0,t);  

 		 if(dismenu.sub_chosecur <= 2)
		 {
			 if(dismenu.sub_chosecur == 0 && dismenu.dis_mode && dismenu.data_mode)
				{
							passdisp.set_rev_flg = 1;
			  }	
				disphex_asc_le(aa,Code_I.P9[0],3);
			  if(aa[0] == '0'){
				 aa[0] = ' ';
				 if(aa[1] == '0'){
					 aa[1] = ' ';
				 }
			 }
				 lcd_wrstring_num(0,22,aa,3);
				passdisp.set_rev_flg = 0;
				
				if(dismenu.sub_chosecur == 2 && dismenu.dis_mode && dismenu.data_mode)
				{
							passdisp.set_rev_flg = 1;
			  }	
				disphex_asc_le(aa,Code_I.P9[2],3);
			 if(aa[0] == '0'){
				 aa[0] = ' ';
				 if(aa[1] == '0'){
					 aa[1] = ' ';
				 }
			 }
			 lcd_wrstring_num(2,22,aa,3);
				passdisp.set_rev_flg = 0;
				
		 }
		 else if(dismenu.sub_chosecur>2 && dismenu.sub_chosecur <= 5)
		 {
			 
			 if(dismenu.sub_chosecur == 3 && dismenu.dis_mode && dismenu.data_mode)
				{
							passdisp.set_rev_flg = 1;
			  }	
				disphex_asc_le(aa,Code_I.P9[3],3);
			  if(aa[0] == '0'){
				 aa[0] = ' ';
				 if(aa[1] == '0'){
					 aa[1] = ' ';
				 }
			 }
				 lcd_wrstring_num(0,22,aa,3);
				passdisp.set_rev_flg = 0;
				
				if(dismenu.sub_chosecur == 4 && dismenu.dis_mode && dismenu.data_mode)
				{
							passdisp.set_rev_flg = 1;
			  }	
				disphex_asc_le(aa,Code_I.P9[4],3);
			 if(aa[0] == '0'){
				 aa[0] = ' ';
				 if(aa[1] == '0'){
					 aa[1] = ' ';
				 }
			 }
			 lcd_wrstring_num(1,22,aa,3);
				passdisp.set_rev_flg = 0;
				
				if(dismenu.sub_chosecur == 5 && dismenu.dis_mode && dismenu.data_mode)
				{
							passdisp.set_rev_flg = 1;
			  }	
				 disphex_asc_le(aa,Code_I.P9[5],2);
			 if(aa[0] == '0'){
				 aa[0] = ' ';
			 }
			 lcd_wrstring_num(2,23,aa,2);
			 passdisp.set_rev_flg = 0;
			
		 }
		 else
		 {
				 if(dismenu.sub_chosecur == 6 && dismenu.dis_mode && dismenu.data_mode)
					{
								passdisp.set_rev_flg = 1;
					}	
					disphex_asc_le(aa,Code_I.P9[6],2);
				 if(aa[0] == '0'){
					 aa[0] = ' ';
				 }
				 lcd_wrstring_num(0,23,aa,2);
				 passdisp.set_rev_flg = 0;

		 }  
			 
			
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lcd_menu_ten(void) //P10
{
 u8 i,t;
	if(_new_page){
   _new_page = 0;  
 }
	 lcd_maxcur = (sizeof(dis_sub_menu_ten)/30)-1;//7-1
	 lcd_cpage = dismenu.sub_chosecur/3;
	 i = (u8)((lcd_cpage < lcd_maxcur/3)?3:(lcd_maxcur%3 + 1));
   if(i == 1)
	 {
				ClearDisBuff();
	 }
	 (void)strncpy(DisBuff,dis_sub_menu_ten+90*lcd_cpage,30*i); 
		if(lcd_cpage_old != lcd_cpage)
		{
				passdisp.disp_flg = 0;
		}
	 if(passdisp.disp_flg == 0)
	 {
			lcd_wrstring(0,0,DisBuff);
			passdisp.disp_flg++;
	 }
	 lcd_cpage_old = lcd_cpage;
	 i = (dismenu.data_mode)?2:1;
	 t = (dismenu.dis_mode)?1:0;    
	 lcd_lastline(i,t);
	 t = (u8)(dismenu.sub_chosecur%3);
	 kp_menucur(0,0,t);  
	 
	 if(dismenu.sub_chosecur <= 2){
  
   //////////////////////////////
		 
		 if(dismenu.sub_chosecur == 1 && dismenu.dis_mode && dismenu.data_mode)
		 {
					passdisp.set_rev_flg = 1;
		 }   
 
		 disphex_asc_le(aa,Code_I.P10[1],3);
		 if(aa[0] == '0'){
				 aa[0] = ' ';
				 if(aa[1] == '0'){
					 aa[1] = ' ';
				 }
     }
			lcd_wrstring_num(1,22,aa,3);
      passdisp.set_rev_flg = 0;

   //////////////////////////////
		  if(dismenu.sub_chosecur == 2 && dismenu.dis_mode && dismenu.data_mode)
			{
					passdisp.set_rev_flg = 1;
			}   

		 disphex_asc_le(aa,Code_I.P10[2],3);
		 if(aa[0] == '0'){
				 aa[0] = ' ';
				 if(aa[1] == '0'){
					 aa[1] = ' ';
				 }
			}
	  lcd_wrstring_num(2,22,aa,3);
		passdisp.set_rev_flg = 0;
 }
 else if(dismenu.sub_chosecur>2 && dismenu.sub_chosecur <= 5){
     if(dismenu.sub_chosecur == 3 && dismenu.dis_mode && dismenu.data_mode)
			{
					passdisp.set_rev_flg = 1;
			} 
	   disphex_asc_le(aa,Code_I.P10[3],3);
			if(aa[0] == '0'){
				 aa[0] = ' ';
				 if(aa[1] == '0'){
					 aa[1] = ' ';
				 }
			}
    lcd_wrstring_num(0,22,aa,3);
	 	passdisp.set_rev_flg = 0;
   /////////////////////////////////
		if(dismenu.sub_chosecur == 4 && dismenu.dis_mode && dismenu.data_mode)
		{
				passdisp.set_rev_flg = 1;
		} 
	 disphex_asc_le(aa,Code_I.P10[4],3);
	 if(aa[0] == '0'){
     aa[0] = ' ';
     if(aa[1] == '0'){
       aa[1] = ' ';
     }
   }
   lcd_wrstring_num(1,22,aa,3);
	 	passdisp.set_rev_flg = 0;
   ///////////////////////////////////
	if(dismenu.sub_chosecur == 5 && dismenu.dis_mode && dismenu.data_mode)
		{
				passdisp.set_rev_flg = 1;
		} 
	  disphex_asc_le(aa,Code_I.P10[5],3);
	 if(aa[0] == '0'){
     aa[0] = ' ';
     if(aa[1] == '0'){
       aa[1] = ' ';
     }
   }
   lcd_wrstring_num(2,22,aa,3);
	 	 	passdisp.set_rev_flg = 0;

 }
 else{
	 if(dismenu.sub_chosecur == 6 && dismenu.dis_mode && dismenu.data_mode)
		{
				passdisp.set_rev_flg = 1;
		} 
    disphex_asc_le(aa,Code_I.P10[6],3);
	 if(aa[0] == '0'){
     aa[0] = ' ';
     if(aa[1] == '0'){
       aa[1] = ' ';
     }
   }
	 lcd_wrstring_num(0,22,aa,3);
  	passdisp.set_rev_flg = 0;

 }  
 
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lcd_menu_eleven(void) //P11
{
 u8 i,t;
	
	if(_new_page){
   _new_page = 0;  
 }
	 lcd_maxcur = (sizeof(dis_sub_menu_eleven)/30)-1;//7-1
	 lcd_cpage = dismenu.sub_chosecur/3;
	 i = (u8)((lcd_cpage < lcd_maxcur/3)?3:(lcd_maxcur%3 + 1));
   if(i == 2)
	 {
				ClearDisBuff();
	 }
	 (void)strncpy(DisBuff,dis_sub_menu_eleven+90*lcd_cpage,30*i); 
		if(lcd_cpage_old != lcd_cpage)
		{
				passdisp.disp_flg = 0;
		}
	 if(passdisp.disp_flg == 0)
	 {
			lcd_wrstring(0,0,DisBuff);
			passdisp.disp_flg++;
	 }
	 lcd_cpage_old = lcd_cpage;
	 i = (dismenu.data_mode)?2:1;
	 t = (dismenu.dis_mode)?1:0;    
	 lcd_lastline(i,t);
	 t = (u8)(dismenu.sub_chosecur%3);
	 kp_menucur(0,0,t);   
		
	 if(dismenu.sub_chosecur <= 2){

		  if(dismenu.sub_chosecur == 0 && dismenu.dis_mode && dismenu.data_mode)
			{
						passdisp.set_rev_flg = 1;
			}

			disphex_asc_le(aa,Code_I.P11[0],2);
			 if(aa[0] == '0'){
			 aa[0] = ' ';
		 }
			 lcd_wrstring_num(0,23,aa,2);	 
				passdisp.set_rev_flg = 0;

   //////////////////////////////
		  if(dismenu.sub_chosecur ==1 && dismenu.dis_mode && dismenu.data_mode)
			{
						passdisp.set_rev_flg = 1;
			}

    disphex_asc_le(aa,Code_I.P11[1],3);
	 if(aa[0] == '0'){
     aa[0] = ' ';
     if(aa[1] == '0'){
       aa[1] = ' ';
     }
   }
      lcd_wrstring_num(1,22,aa,3);
		passdisp.set_rev_flg = 0;	 
   //////////////////////////////
		if(dismenu.sub_chosecur ==2 && dismenu.dis_mode && dismenu.data_mode)
			{
						passdisp.set_rev_flg = 1;
			}
	 disphex_asc_le(aa,Code_I.P11[2],2);
	 if(aa[0] == '0'){
     aa[0] = ' ';
   }
	    lcd_wrstring_num(2,23,aa,2);	 
      passdisp.set_rev_flg = 0;	 
 }
 else if(dismenu.sub_chosecur>2 && dismenu.sub_chosecur <= 4){
	 if(dismenu.sub_chosecur ==3 && dismenu.dis_mode && dismenu.data_mode)
			{
						passdisp.set_rev_flg = 1;
			}
	 disphex_asc_le(aa,Code_I.P11[3],3);
	 if(aa[0] == '0'){
     aa[0] = ' ';
     if(aa[1] == '0'){
       aa[1] = ' ';
     }
   }
    lcd_wrstring_num(0,22,aa,3);
      passdisp.set_rev_flg = 0;	 
	 
   /////////////////////////////////
		if(dismenu.sub_chosecur ==4 && dismenu.dis_mode && dismenu.data_mode)
			{
						passdisp.set_rev_flg = 1;
			}
	 disphex_asc_le(aa,Code_I.P11[4],2);
	 if(aa[0] == '0'){
     aa[0] = ' ';
   }
    lcd_wrstring_num(1,23,aa,2);
      passdisp.set_rev_flg = 0;	 
	 
 }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lcd_menu_twelve(void) //P12
{
 u8 i,t;
	
	if(_new_page){
   _new_page = 0;  
 }
	 lcd_maxcur = (sizeof(dis_sub_menu_twelve)/30)-1;//7-1
	 lcd_cpage = dismenu.sub_chosecur/3;
	 i = (u8)((lcd_cpage < lcd_maxcur/3)?3:(lcd_maxcur%3 + 1));
   if(i == 1)
	 {
				ClearDisBuff();
	 }
	 (void)strncpy(DisBuff,dis_sub_menu_twelve+90*lcd_cpage,30*i); 
		if(lcd_cpage_old != lcd_cpage)
		{
				passdisp.disp_flg = 0;
		}
	 if(passdisp.disp_flg == 0)
	 {
			lcd_wrstring(0,0,DisBuff);
			passdisp.disp_flg++;
	 }
	 lcd_cpage_old = lcd_cpage;
	 i = (dismenu.data_mode)?2:1;
	 t = (dismenu.dis_mode)?1:0;    
	 lcd_lastline(i,t);
	 t = (u8)(dismenu.sub_chosecur%3);
	 kp_menucur(0,0,t);  
	
		 if(dismenu.sub_chosecur <= 2)
		{
			 if(dismenu.sub_chosecur == 0 && dismenu.dis_mode && dismenu.data_mode)
		  	{
							passdisp.set_rev_flg = 1;
				} 

			 disphex_asc_le(aa,Code_I.P12[0],2);
			 if(aa[0] == '0'){
				 aa[0] = ' ';
			 }
			 lcd_wrstring_num(0,23,aa,2);
				passdisp.set_rev_flg = 0;
			 //////////////////////////////
			 if(dismenu.sub_chosecur == 1 && dismenu.dis_mode && dismenu.data_mode)
		  	{
							passdisp.set_rev_flg = 1;
				} 
			 disphex_asc_le(aa,Code_I.P12[1],2);
			 if(aa[0] == '0'){
				 aa[0] = ' ';
			 }
			 lcd_wrstring_num(1,23,aa,2);
				passdisp.set_rev_flg = 0;

			 //////////////////////////////
				if(dismenu.sub_chosecur == 2 && dismenu.dis_mode && dismenu.data_mode)
		  	{
							passdisp.set_rev_flg = 1;
				} 
			 disphex_asc_le(aa,Code_I.P12[2],2);
			 if(aa[0] == '0'){
				 aa[0] = ' ';
			 }
			 lcd_wrstring_num(2,23,aa,2);
				passdisp.set_rev_flg = 0;

 }
 else if(dismenu.sub_chosecur == 3){
				if(dismenu.sub_chosecur == 3 && dismenu.dis_mode && dismenu.data_mode)
				{
							passdisp.set_rev_flg = 1;
				} 
			 disphex_asc_le(aa,Code_I.P12[3],2);
			 if(aa[0] == '0'){
				 aa[0] = ' ';
			 }
			 lcd_wrstring_num(0,23,aa,2);
				passdisp.set_rev_flg = 0;

 } 
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lcd_menu_thirteen(void) //P13
{
 u8 i,t;
	
	if(_new_page){
   _new_page = 0;  
 }
	 lcd_maxcur = (sizeof(dis_sub_menu_thirteen)/30)-1;//7-1
	 lcd_cpage = dismenu.sub_chosecur/3;
	 i = (u8)((lcd_cpage < lcd_maxcur/3)?3:(lcd_maxcur%3 + 1));
   if(i == 1)
	 {
				ClearDisBuff();
	 }
	 (void)strncpy(DisBuff,dis_sub_menu_thirteen+90*lcd_cpage,30*i); 
		if(lcd_cpage_old != lcd_cpage)
		{
				passdisp.disp_flg = 0;
		}
	 if(passdisp.disp_flg == 0)
	 {
			lcd_wrstring(0,0,DisBuff);
			passdisp.disp_flg++;
	 }
	 lcd_cpage_old = lcd_cpage;
	 i = (dismenu.data_mode)?2:1;
	 t = (dismenu.dis_mode)?1:0;    
	 lcd_lastline(i,t);
	 t = (u8)(dismenu.sub_chosecur%3);
	 kp_menucur(0,0,t);  
	
 if(dismenu.sub_chosecur <= 2)
	 {	 
		 disphex_asc_le(aa,Code_I.P13[0],2);
		 if(aa[0] == '0'){
     aa[0] = ' ';
     if(aa[1] == '0'){
       aa[1] = ' ';
     }
   }
     //sendtobuff_n(0,23,aa,2);
	// lcd_wrstring(0,0,DisBuff);
	  lcd_wrstring(0,24,"1");
   //////////////////////////////
 }
 else if(dismenu.sub_chosecur == 3)
 {

 } 
 
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lcd_menu_fourteen(void) //P14
{
 u8 i,t;
 if(_new_page){
   _new_page = 0;  
 }
	 lcd_maxcur = (sizeof(dis_sub_menu_fourteen)/30)-1;//7-1
	 lcd_cpage = dismenu.sub_chosecur/3;
	 i = (u8)((lcd_cpage < lcd_maxcur/3)?3:(lcd_maxcur%3 + 1));
   if(i==2)
		{
		  ClearDisBuff();	
		}
	 (void)strncpy(DisBuff,dis_sub_menu_fourteen+90*lcd_cpage,30*i); 
			if(lcd_cpage_old != lcd_cpage)
			{
					passdisp.disp_flg = 0;
			}
			if(passdisp.disp_flg == 0)
			{
					lcd_wrstring(0,0,DisBuff);
				  passdisp.disp_flg++;
			}
			lcd_cpage_old = lcd_cpage;
		 i = (dismenu.data_mode)?2:1;
		 t = (dismenu.dis_mode)?1:0;    
		 lcd_lastline(i,t);
		 t = (u8)(dismenu.sub_chosecur%3);
		 kp_menucur(0,0,t);
		
  if(dismenu.sub_chosecur <= 2)
		{
			 if(dismenu.sub_chosecur == 0 && dismenu.dis_mode && dismenu.data_mode)
			{
						passdisp.set_rev_flg = 1;
			} 

			disphex_asc_le(aa,Code_I.P14[0],2);
			if(aa[0] == '0'){
					 aa[0] = ' ';
					 if(aa[1] == '0'){
							 aa[1] = ' ';
					 }
       }

			lcd_wrstring_num(0,22,aa,2);
			passdisp.set_rev_flg = 0;

   //////////////////////////////
			if(dismenu.sub_chosecur == 1 && dismenu.dis_mode && dismenu.data_mode)
			{
						passdisp.set_rev_flg = 1;
			}
		 disphex_asc_le(aa,Code_I.P14[1],2);
		 if(aa[0] == '0'){
			 aa[0] = ' ';
			 if(aa[1] == '0'){
				 aa[1] = ' ';
			 }
		 }
		 lcd_wrstring_num(1,22,aa,2);
			passdisp.set_rev_flg = 0;

   //////////////////////////////
		if(dismenu.sub_chosecur == 2 && dismenu.dis_mode && dismenu.data_mode)
			{
						passdisp.set_rev_flg = 1;
			}
   disphex_asc_le(aa,Code_I.P14[2],2);
   if(aa[0] == '0'){
     aa[0] = ' ';
     if(aa[1] == '0'){
       aa[1] = ' ';
     }
   }
   lcd_wrstring_num(2,22,aa,2);
	 passdisp.set_rev_flg = 0;

 }
 else if(dismenu.sub_chosecur>2 && dismenu.sub_chosecur <= 5){
   if(dismenu.sub_chosecur == 3 && dismenu.dis_mode && dismenu.data_mode)
			{
						passdisp.set_rev_flg = 1;
			}
	 disphex_asc_le(aa,Code_I.P14[3],2);
   if(aa[0] == '0'){
     aa[0] = ' ';
     if(aa[1] == '0'){
       aa[1] = ' ';
     }
   }
   lcd_wrstring_num(0,22,aa,2);
	 passdisp.set_rev_flg = 0;

   /////////////////////////////////
		if(dismenu.sub_chosecur == 4 && dismenu.dis_mode && dismenu.data_mode)
			{
						passdisp.set_rev_flg = 1;
			}
   disphex_asc_le(aa,Code_I.P14[4],3);
   if(aa[0] == '0'){
     aa[0] = ' ';
     if(aa[1] == '0'){
       aa[1] = ' ';
     }
   }
   lcd_wrstring_num(1,21,aa,3);
	 passdisp.set_rev_flg = 0;

 }
 else{
	 if(dismenu.sub_chosecur == 7 && dismenu.dis_mode && dismenu.data_mode)
	 {
		passdisp.set_rev_flg = 1;
	 }
	 disphex_asc_le(aa,Code_I.P14[7],2);
   if(aa[0] == '0'){
     aa[0] = ' ';
     if(aa[1] == '0'){
       aa[1] = ' ';
     }
   }
   lcd_wrstring_num(1,22,aa,2);
	 passdisp.set_rev_flg = 0;
 }  
}
//~~~~~~~~~~~~~~~~~~~~~~??~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lcd_menu_fifteen(void) // P15
{
 u8 i,t;
 if(_new_page){
   _new_page = 0;
 }
	 lcd_maxcur = (sizeof(dis_sub_menu_fifteen)/30)-1;//7-1
	 lcd_cpage = dismenu.sub_chosecur/3;
	 i = (u8)((lcd_cpage < lcd_maxcur/3)?3:(lcd_maxcur%3 + 1));
	 (void)strncpy(DisBuff,dis_sub_menu_fifteen+90*lcd_cpage,30*i);
		if(i==1)
		{
		  ClearDisBuff();	
		} 
 i = (dismenu.data_mode)?2:1;
 t = (dismenu.dis_mode)?1:0;    
 lcd_lastline(i,t);
 t = (u8)(dismenu.sub_chosecur%3);
 kp_menucur(0,0,t);
 
 
 lcd_wrstring(0,0,DisBuff); 
 
}
//~~~~~~~~~~~~~~~~~~~~~~??~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lcd_menu_sixteen(void)// P16
{
 u8 i,t;

 if(_new_page){
   _new_page = 0; 
 }
	 lcd_maxcur = (sizeof(dis_sub_menu_sixteen)/30)-1;//7-1
	 lcd_cpage = dismenu.sub_chosecur/3;
	 i = (u8)((lcd_cpage < lcd_maxcur/3)?3:(lcd_maxcur%3 + 1));
	 (void)strncpy(DisBuff,dis_sub_menu_sixteen+90*lcd_cpage,30*i); 
		if(i==1)
		{
		  ClearDisBuff();	
		} 
 i = (dismenu.data_mode)?2:1;
 t = (dismenu.dis_mode)?1:0;    
 lcd_lastline(i,t);
 t = (u8)(dismenu.sub_chosecur%3);
 kp_menucur(0,0,t);
 lcd_wrstring(0,0,DisBuff);  
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void lcd_wash_complete_menu(void)
{
	 if(_new_page)
	 {
       _new_page = 0; 
     }
	 if(passdisp.disp_flg==0)
	 {
		 sendtobuff(0,0,wash_complete_menu);
		 lcd_wrstring(0,0,DisBuff);
		 passdisp.disp_flg++;
	 }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lcd_shake_stop_menu(void)//????? 
{
 
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lcd_instancy_stop_menu(void)//????
{
 
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void menu_fault_page(void)//dismod5 显示故障界面
{
	unsigned char a,i,j,t;
	unsigned char *P;
	P = fault;
	if(_new_page)
	{
		_new_page = 0;		
	}
	for(i = 0;i < 12; i++) //显示12个故障
	{
		 P[i] = AT24CXX_ReadOneByte(ADD_FAULT_E2+i);
		 //P++;
	}
	for( i = 0; i < 12; i++)
	{
		if(P[i] == 0)
		{
			break;
		}
	}	
	fault_count_back = i;
  //lcd_cpage = dismenu.sub_chosecur/3;
//	if(lcd_cpage_old != lcd_cpage)
//	{
//		passdisp.disp_flg = 0;
//	}
//		
	if(passdisp.disp_flg == 0 ||lcd_cpage != dismenu.sub_chosecur/3)//azalea_wait_for_modify
	{
		ClearDisBuff();  
		lcd_wrstring(0,0,DisBuff);
		if(i == 0)
		{ 
			 lcd_wrstring(0,2,"无历史故障");
		}
		else
		{
			 lcd_maxcur = i-1;//0--9最大指针
			 lcd_cpage = dismenu.sub_chosecur/3;
			 t = (lcd_cpage < lcd_maxcur/3)?3:(lcd_maxcur%3 + 1);//0 1 2 第几页
	//			 if(t == 3)
	//			 {
	//				 ClearDisBuff();   
	//			 }	
			 for(a=0;t>0;a++,t--)
			 {	      
				j = (lcd_cpage * 3) + a;
				switch(fault[j])
				{
					case 0x01:
						lcd_wrstring(a,2,"变频器加速过电流(ocA)");
						break;
					case 0x02:
						lcd_wrstring(a,2,"变频器减速过电流(ocd)");
						break;
					case 0x03:
						lcd_wrstring(a,2,"变频器恒速过电流(ocn)");
						break;
					case 0x04:
						lcd_wrstring(a,2,"变频器逆变单元U相保护(Sc1)");
						break;
					case 0x05:
						lcd_wrstring(a,2,"变频器逆变单元V相保护(Sc2)");
						break;
					case 0x06:
						lcd_wrstring(a,2,"变频器逆变单元W相保护(Sc3)");
						break;
					case 0x07:
						lcd_wrstring(a,2,"变频器加速过电压(ouA)");
						break;
					case 0x08:
						lcd_wrstring(a,2,"变频器减速过电压(oud)");
						break;
					case 0x09:
						lcd_wrstring(a,2,"变频器恒速过电压(oun)");
						break;
					case 0x0a:
						lcd_wrstring(a,2,"变频器电机过载(OL1)");
						break;														
					case 0x0b:
						lcd_wrstring(a,2,"变频器过载(OL2)");
						break;
					case 0x0c:
						lcd_wrstring(a,2,"变频器电子热过载(OL3)");
						break;
					case 0x0d:
						lcd_wrstring(a,2,"变频器母线欠压故障(Uu)");
						break;
					case 0x0e:
						lcd_wrstring(a,2,"变频器逆变模块过热(OH1)");
						break;
					case 0x0f:
						lcd_wrstring(a,2,"变频器散热器过热故障(OH2)");
						break; 
					case 0x10:
						lcd_wrstring(a,2,"变频器输入侧缺相(PHL)");
						break; 
					case 0x11:
						lcd_wrstring(a,2,"变频器输出侧缺相(PHO)");
						break;
					case 0x12:
						lcd_wrstring(a,2,"变频器电机自学习故障(ErtUn)");
						break;
					case 0x13:
						lcd_wrstring(a,2,"变频器EEPROM故障(CF)");
						break;
					case 0x14:
						lcd_wrstring(a,2,"变频器PID反馈断线故障(PI.DE)");
						break;						
					case 0x15:
						lcd_wrstring(a,2,"变频器外部故障(EF)");
						break;
					case 0x17:
						lcd_wrstring(a,2,"变频器电流检测故障(ItE)");
						break;
					case 0x18:
						lcd_wrstring(a,2,"变频器制动单元故障(bCE)");
						break;
					case 0x19:
						lcd_wrstring(a,2,"变频器接触器未吸合(FE)");
						break;  
						
					case 0x1a:
						lcd_wrstring(a,2,"上下位机通讯故障");
						break;
					case 0x1b:
						lcd_wrstring(a,2,"超震故障");
						break;
					case 0x1c:
						lcd_wrstring(a,2,"液位传感器故障");
						break;
					case 0x1d:
						lcd_wrstring(a,2,"温度传感器故障");
						break;
					case 0x1e:
						lcd_wrstring(a,2,"门开故障");
						break;
					
					case 0x1f:
						lcd_wrstring(a,2,"下位机与变频器通讯故障");
						break;
					default:
						lcd_wrstring(a,2,"无故障");
						break;
				}      
			} 
		} 
		passdisp.disp_flg++;
	 }
		  
	// lcd_cpage_old = lcd_cpage;
	 lcd_lastline(1,0);
	 t = (dismenu.sub_chosecur%3);
	 kp_menucur(0,0,t);  	
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void menu_set_program(void)
{ 
	unsigned char  a,i;//a,i,t;
  unsigned char *j,*k;//*j,*k;
	char aa[30];
	
	j = mode_data;
	k = mode_data_buffer; 
	
	if(_read_data == 0)
	{	//读e2prom中的数据mode_data
		_read_data = 1;
		for(i = 0; i<200; i++)
		{
				j[i] =  AT24CXX_ReadOneByte(PROGRAM_ONE+(auto_run_mode-1)*256+i);
		}
		for(i=0;i<200;i++)
		{
			  *(k + i) = *(j + i);
		}
	// delay(60000);
	} // azalea_modify_161128
	
	
	if(_read_function == 0)
	{
	    _read_function = 1;
		lcd_wrstring(2,0,"                              ");
	    if(*(k + step_num*8+3) == 'X')//洗涤
		{
			_wash_enable = 1;
			_enable_single_wash = 0;    
	    }
	    else if(*(k + step_num*8+3) == 'J')//均布
		{
			_wash_enable = 0;
			_enable_single_wash = 1;
	    }
	    else
		{    
			_wash_enable = 0;
			_enable_single_wash = 0;
	    }   
        //_wash_enable = (*(k + step_num*8+3) == 'X')?1:0;//洗涤   
	    _cold_enable = (*(k + step_num*8+1) & 0x01)?1:0;//冷水
	    _warm_enable = (*(k + step_num*8+1) & 0x02)?1:0;//热水
	    _heat_enable = (*(k + step_num*8+1) & 0x04)?1:0;//加热
	    _drain_enable = (*(k + step_num*8+1) & 0x08)?1:0;//排水
   
		if(*(k + step_num*8+3) != 'X' && *(k + step_num*8+3) != 0 && *(k + step_num*8+3) != 'J')
		{
			 _spin_enable = 1;//脱水
			 if(*(k + step_num*8+3) == 'Z')
			 {
					dehydration_num = 1;
			 }
			 else if(*(k + step_num*8+3) == 'G')
			 {
					dehydration_num = 2;
			 }
	   }
	   else
	   {
			_spin_enable = 0;//脱水
			dehydration_num = 0;
	   }
	   if(*(k + step_num*8+7) != 0)
	   {
			_soap_enable = 1;//皂液
			soap_liquid_num = *(k + step_num*8+7);//皂液数量
	   }
	   else
	   {
			_soap_enable = 0;//皂液
			soap_liquid_num = 0;
	   }
	 
	}

	if(passdisp.disp_flg == 0)
	{//定时整屏刷新
			sendtobuff(0,0,dispSetMenu);
			lcd_wrstring(0,0,DisBuff);
			passdisp.disp_flg++;
	}   
	lcd_lastline(3,1);//最下边一行
    //disphex_asc_le(aa,program_num,1);
    //lcd_stringsnum(0,5,aa,1);
////////////////////////////////////////////////// 
	passdisp.set_rev_flg = (lcd_chosen_sub_step == 0)?1:0; //下一步
	a = step_num + 1;
	disphex_asc_le(aa,a,2);
	if(aa[0] == '0')
	{
		aa[0] = ' ';
	}
	lcd_wrstring_num(0,12,aa,2);//10
	passdisp.set_rev_flg = 0;
//////////////////////////////////////////////////
	passdisp.set_rev_flg = (lcd_chosen_sub_step == 1)?1:0;
	disphex_asc_le(aa,*(k + step_num*8+4),2);//分
	lcd_wrstring_num(0,18,aa,2);//17
	passdisp.set_rev_flg = 0;
////////////////////////////////////////////////// 
	lcd_wrstring_num(0,20,":",1);//19
//////////////////////////////////////////////////
	passdisp.set_rev_flg = (lcd_chosen_sub_step == 2)?1:0; 
	disphex_asc_le(aa,k[step_num * 8 + 5],2);//秒
	lcd_wrstring_num(0,21,aa,2);//20
	passdisp.set_rev_flg = 0;
////////////////////////////////////////////////// 
	passdisp.set_rev_flg = (lcd_chosen_sub_step == 3)?1:0;
	disphex_asc_le(aa,*(k + step_num*8+6),2);//设定温度
	if(aa[0] == '0'){
		aa[0] = ' ';
	}
	lcd_wrstring_num(1,8,aa,2);
	passdisp.set_rev_flg = 0;
//////////////////////////////////////
	passdisp.set_rev_flg = (lcd_chosen_sub_step == 4)?1:0;
	disphex_asc_le(aa,*(k + step_num*8+2),2);//设定液位
	if(aa[0] == '0'){
		aa[0] = ' ';
	}
	lcd_wrstring_num(1,22,aa,2);
	passdisp.set_rev_flg = 0;
 
 //////////////////////////////////////////////////
	if(_wash_enable)
	{
		lcd_wrstring(2,0,"洗涤");
	} 
	else if(_enable_single_wash)
	{
		lcd_wrstring(2,0,"均布");  
	}
	else
	{
		lcd_wrstring(2,0,"    ");
	}
 //////////////////////////////////////////////////
	if(_cold_enable)
	{
		lcd_wrstring(2,5,"冷水");
	}
	else
	{
		lcd_wrstring(2,5,"    ");
	}
 //////////////////////////////////////////////////
	if(_warm_enable)
	{ 
		lcd_wrstring(2,10,"热水");
	}
	else
	{
		lcd_wrstring(2,10,"    ");
	}
 //////////////////////////////////////////////////
	if(_heat_enable)
	{
		lcd_wrstring(2,15,"加热");
	}
	else
	{
		lcd_wrstring(2,15,"    ");
	}
 ////////////////////////////////////////////////// 
	if(_soap_enable == 0 && _had_soap)
	{//皂液功能已去除但皂液还显示
		_had_soap = 0;
		lcd_wrstring(2,20,"    ");//清皂液
		lcd_wrstring(2,24," "); 
	}
	if(_drain_enable == 0 && _had_drain){//皂掖
		_had_drain = 0;//请排水
		lcd_wrstring(2,23,"    ");
	}
	if(_spin_enable == 0 && _had_spin)
	{
		_had_spin = 0;
		lcd_wrstring(2,26,"    ");
	}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
	if(_soap_enable)
	{
		_had_soap = 1;//已经有皂液了
		lcd_wrstring(2,20,"皂液");
	    switch(soap_liquid_num)
		{
		 case 1:
		   lcd_wrstring_num(2,24,"1",1);
		   break;
		 case 2:
		   lcd_wrstring_num(2,24,"2",1);
		   break;
		 default:
		   lcd_wrstring_num(2,24,"3",1);
		   break;
	   }
	}
 ///////////////////////////////////////////////////
	 if(_drain_enable){
	   _had_drain = 1;//已经有paishui
	   lcd_wrstring(2,23,"排水");
	 }        
 ///////////////////////////////////////////////////
	 if(_spin_enable){
	   _had_spin = 1;//有脱水 
	   lcd_wrstring(2,28,"脱");
	   if(dehydration_num == 1){
		 lcd_wrstring(2,26,"中");
	   }
	   else{
		 lcd_wrstring(2,26,"高");
	   }
	 }
	 switch(auto_run_mode){//P12[1]){
	   case 1:
		 lcd_wrstring(0,0,"标准");
		 break;
	   case 2:
		 lcd_wrstring(0,0,"床单");
		 break;
	   case 3:
		 lcd_wrstring(0,0,"台布");
		 break;
	   case 4:
		 lcd_wrstring(0,0,"衬衣");
		 break;
	   default:
		 lcd_wrstring(0,0,"毛巾");
		 break;
	 }
	 _set_mode_symbol = 1;//进入设定模式 

		
}   
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void menu_save_data(void)
{
	//	u8 i;
	 if(_new_page){
       _new_page = 0; 
     }
		if(passdisp.disp_flg==0)
		 {
				 sendtobuff(0,0,save_data_menu);
				 lcd_wrstring(0,0,DisBuff);
				 passdisp.disp_flg++;
		 }
		 
		 _if_save_data = 1;
		 
		 
		 
}
void menu_handshake_dis(void)
{
	 if(_new_page)
	 {
 		  _new_page = 0; 
	 }
	 
	 if(passdisp.disp_flg==0)
	 {
		 sendtobuff(0,0,handshakemenu);
		 lcd_wrstring(0,0,DisBuff);
		 passdisp.disp_flg++;
	 }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//void menu_fault_dis(void)
//{
//  //	u8 i;
//	 if(_new_page)
//	 {
//		_new_page = 0; 
//	 }
//	 if(passdisp.disp_flg==0)
//	 {
//		 sendtobuff(0,0,fault_dis_menu);
//		 lcd_wrstring(0,0,DisBuff);
//		 passdisp.disp_flg++;
//	 }

//}

void menu_fault_dis(void)
{	 
	char aa[6];
//------------------程序-------------------- 
	if(_new_page)
	 {
		_new_page = 0; 
	 }	
	if(_manual_mode)
	 {
		lcd_wrstring(0,0,"手动");				 
	 } 
	 else
	 {  	
		switch(auto_run_mode)
		{  
		case 1:
			lcd_wrstring(0,0,"标准");
		 break;
		case 2:
			lcd_wrstring(0,0,"床单");
		 break;
		case 3:
			lcd_wrstring(0,0,"台布");
		 break;
		case 4:
			lcd_wrstring(0,0,"衬衣");
		 break;
		default:
			lcd_wrstring(0,0,"毛巾");
		 break;
		}
	 }

	 if(passdisp.disp_flg==0)
	 {
		 sendtobuff(0,0,dispMainMenu);
		 lcd_wrstring(0,0,DisBuff);
		 passdisp.disp_flg++;
	 }
	 if(flag_fault_display == 1){
		 lcd_wrstring(0,28,"■");
		 LED_STOP = 0;

	  }
	  else
	  {
		lcd_wrstring(0,28,"  ");
		 LED_STOP = 1;

	  }	 
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 	
	if(run_state == 0 && _manual_mode == 0)//_manual_mode = 0 洗衣停止且自动模式
	 {
		 lcd_wrstring(3, 3,"故障");
		 lcd_wrstring(3,10,"程序");
		 lcd_wrstring(3,16,"编程");
		 lcd_wrstring(3,23,"设置");
			 
		  if(input_door_switch ==0)
			{
				 lcd_wrstring(1,28,"憨");

			}
			else
			{
				 lcd_wrstring(1,28,"翰");

			}
		 if(_enable_display && _manual_mode == 0 )
		 {//每1秒置1
			_enable_display = 0;
			 
			if(fact_temp > 100)	
			{
				fact_temp = fact_temp - 100;
				if(fact_temp > 100)
				{
					fact_temp = fact_temp - 100;

				}
			}			
			disphex_asc_le(aa,fact_temp,3);//实际温度
			if(aa[0] == '0')
			{
				aa[0] = ' ';
			}
			if(aa[1] == '0')
			{
				aa[1] = ' ';
			}
			lcd_wrstring_num(2,8,aa,3);
			
			disphex_asc_le(aa,fact_liquid_height,4);//实际液位
			if(aa[0] == '0')
			{
				aa[0] = ' ';
			}
			lcd_wrstring_num(2,23,aa,4); 
		 }		
		//////////设定液位/////////////////////////
		 if(run_state && _manual_mode == 0)
		 {
			 disphex_asc_le(aa,set_liquid_height,2);
			 if(aa[0] == '0')
			 {
				 aa[0] = ' ';
			 }
			 lcd_wrstring_num(1,23,aa,2);
		 }
		 else
		 {
			 disphex_asc_le(aa,Code_I.P6[1],2);//中液位高度 
			 if(aa[0] == '0')
			 {
				 aa[0] = ' ';
			 }
			 lcd_wrstring_num(1,23,aa,2);
		 }
				 
		//////////////设定温度//////////////////////// 
		 if(run_state && _manual_mode == 0)
		 {
				disphex_asc_le(aa,set_temp,3);
		 }
		 else
		 {
				 disphex_asc_le(aa,Code_I.P12[0],3);//手动默认温度
				 set_temp = Code_I.P12[0];
		 }
		 if(aa[0] == '0')
		 {
				aa[0] = ' ';
		 }
		 if(aa[1] == '0')
		 {
				aa[1] = ' ';
		 }
		 lcd_wrstring_num(1,8,aa,3);			 				
		 _dis_open_close_door = 1;  //显示开关门标志
		 
		 if(input_door_switch ==0) // azalea_wait_for_modify 门开关 JP5
		 {
			 lcd_wrstring(0,12,"请关门  ");
		 }
		 else if((single_wash_complete_delay_time || single_wash_complete_skid
		    || middle_dehydration_stop_wait_timer || middle_dehydration_stop_skid_time
		    || high_dehydration_stop_wait_timer   || high_dehydration_stop_skid_time)
			                && _get_clothes_function == 0)
		 {//均布与中脱，高脱结束或刹车时间
				lcd_wrstring(0,12,"刹车延时");// azalea???
		 }
		 else if( single_wash_complete_delay_time == 0 && single_wash_complete_skid == 0
			&& middle_dehydration_stop_wait_timer == 0 && middle_dehydration_stop_skid_time == 0 
		    &&   high_dehydration_stop_wait_timer == 0 && high_dehydration_stop_skid_time == 0 
							&&   _get_clothes_function == 0)
		 {
				lcd_wrstring(0,12,"可以开门"); 
		 }
		 else if(_get_clothes_function)
		 {
				lcd_wrstring(0,12,"运行取衣"); 
		 }
		 else
		 {
				lcd_wrstring(0,12,"        ");
		 }
				 
	 }
	 else  
	 {		 
	 }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


			

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void disphex_asc_le(char *array,u32 idat,u8 num)//将16进制数转化为ASC码
{
 u8 i;
 u8 t;
 disphex_asc_be(array,idat,num);//把idat的ASC码存到aa数组里并倒序输出，不够4位末尾补0
 for(i=0;i<num/2;i++){          //倒序
	  t= *(array+i);
	  *(array+i) = *(array+num-1-i);
	  *(array+num-1-i) = t;
 }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void disphex_asc_be(char *array,u32 idat,u8 num)//转化出来倒序
{
 char t;
 u8 i;
 for(i=0;i<num;i++){//when i>num, num%2=1.
    t = idat % 10; 		//取个位，十位，百位          
    t = t+0x30;				//0的ASC码为0x30H
	  	*(array+i) = t; //
    idat = idat / 10;
 }
}


void kp_menucur(u8 sline,u8 ucol,u8 cur)
{
 u8 i;
 u8 t;
 t = sline+cur;
//______________clear cursor_______________光标
 for(i=sline;i<=2;i++){
	  if(i == t){
		  continue;
	  }
	  lcd_wrstring(i,ucol,"  ");
 }
//_______________setb cursor_______________
 lcd_wrstring(sline+cur,ucol,"》");      
 
}



void lcd_lastline(u8 g1, u8 g2)
{
 if(g1 == 1){
   lcd_wrstring(3,3,"返回");
   lcd_wrstring(3,10," ∧ ");
   lcd_wrstring(3,16," ∨ ");

   if(g2){
     lcd_wrstring(3,23,"设定");
   }
   else{
     lcd_wrstring(3,23,"查看");
   }
 }
 else if(g1 == 2){
   lcd_wrstring(3,3,"返回");
   lcd_wrstring(3,10," + ");
   lcd_wrstring(3,16," - ");
   lcd_wrstring(3,23,"确认"); 
 }
 else if(g1 == 3){
   lcd_wrstring(3,3,"返回");
   lcd_wrstring(3,10," + ");
   lcd_wrstring(3,16," - ");
   lcd_wrstring(3,23,"设定");
 }
}
//*************************洗涤正反转****************************************
void wash_pro(void)
{
 //洗涤、均布用
	 if(_enable_wash)//洗涤
	 {
		   if(wash_stop_time)
		   {
				 wash_stop_time--;
				 wash_1 = 0;
				 wash_2 = 0;
				 return;
		   }  
		   else if(wash_run_time)//运行时间
		   {
				 wash_run_time--;
				 if(_wash_direction)
				 {      
				   wash_1 = 0;
				   wash_2 = 1;      
				 } 
				 else
				 {      
				   wash_1 = 1;
				   wash_2 = 0;    
				 }
		   } 
		   else
		   {
				 wash_run_time =Code_I.P8[1];//洗涤转到时间
				 wash_stop_time = Code_I.P8[2];//洗涤停止时间
				 _wash_direction = ~_wash_direction;
		   }
	 }
	 else if(_enable_single_wash)//均布
	 {
		   if(wash_stop_time)
		   {
				 wash_stop_time--;
				 wash_1 = 0;
				 wash_2 = 0;
				 return;
		   }
		   else
		   { 
				 if(fact_liquid_height < Code_I.P6[6])
				 {
					   wash_1 = 0;
					   wash_2 = 0;
					   return;  
				 }
				 else
				 {             
					   wash_1 = 1;
					   wash_2 = 0;
				 }
		   }
	 }       
}
//*****************************脱水结束************************************

void dehydration_stop_pro(void)
{
	//中脱结束延时和刹车
	 if(middle_dehydration_stop_skid_time)
	 {//中脱停止刹车时间
		 middle_dehydration_stop_skid_time--; 
	 }
	 if(middle_dehydration_stop_wait_timer)
	 {//中脱停止等待时间
		   middle_dehydration_stop_wait_timer--;
		   if(middle_dehydration_stop_skid_time == 0)
		   {
				skid = 1;    
		   }
		   if(middle_dehydration_stop_wait_timer == 0)
		   {
				 skid = 0;
				 _dehydration_stop_pro = 0;
				 middle_dehydration_stop_skid_time = 0;
		   }
		   return; 
	 }
	 //高脱结束延时和刹车
	 if(high_dehydration_stop_skid_time)
	 {//高脱停止刹车时间
		  high_dehydration_stop_skid_time--;  
	 }
	 if(high_dehydration_stop_wait_timer)
	 {//高脱停止等待时间
		   high_dehydration_stop_wait_timer--;
		   if(high_dehydration_stop_skid_time == 0)
		   {
			   skid = 1;//刹车
		   }
		   if(high_dehydration_stop_wait_timer == 0)
		   {
				 skid = 0;
				 _dehydration_stop_pro = 0;
				 high_dehydration_stop_skid_time = 0; 
		   }       
	 }   
}
//*****************************************************************


void get_clothes_function(void)
{
	if(get_clothes_provide_timer)
	{
	   get_clothes_provide_timer--;
	   wash_1 = 0;
	   wash_2 = 0;
	 } 
	 else if(deasil_run_time)
	 {
	   deasil_run_time--;
	   wash_1 = 1;
	   wash_2 = 0;
	 }
	 else if(deasil_stop_time)
	 {
	   deasil_stop_time--;
	   wash_1 = 0;
	   wash_2 = 0;
	 }
	 else if(anticlockwise_run_time)
     {
	   anticlockwise_run_time--;
	   wash_1 = 0;
	   wash_2 = 1;
	 } 
	 else if(anticlockwise_stop_time)
	 {
	   anticlockwise_stop_time--;
	   wash_1 = 0;
	   wash_2 = 0;
	 } 
	 else
	 {
	   _get_clothes_function = 0;
	   LED_RUN = 1;
	   LED_STOP = 0;
//	   _glitter_stop_led = 1; //azalea_wait_for_modify
	 }
}

//*****************************************************************
void single_way_wash_stop_pro(void)
{
	 if(single_wash_complete_skid)
	 {//均布结束刹车时间
			single_wash_complete_skid--;  
	 }
	 if(single_wash_complete_delay_time)
	 {//均布结束延时时间
		   single_wash_complete_delay_time--;
		   if(single_wash_complete_skid == 0)
		   {
				skid = 1;//刹车
		   }
		   if(single_wash_complete_delay_time == 0)
		   {
				 skid = 0;
				 _single_way_wash_stop_pro = 0;
				 single_wash_complete_skid = 0; 
		   }   
	 }    
}
//*****************************************************************

void Auto_WashPro(void)
{
		//1秒执行区	 	  	  
	if(_1s_timer)
	{
		_1s_timer = 0;
			//时间正反显
		//_positive_reverse_dis = ~_positive_reverse_dis; //有高脱标志
		passdisp.pst_rev_dis = !passdisp.pst_rev_dis;
			//LED
		if(_have_fault == 1)
		{
			_glitter_heat_led = 0;
			_glitter_drain_led = 0;
			_glitter_spin_led = 0;
		    _glitter_wash_led = 0;
			_glitter_cold_led = 0;//
			_glitter_warm_led = 0;			
		}
		
		if(_glitter_warm_led)
		{
			LED_WARM = ~LED_WARM;
		}
		else
		{
			LED_WARM = 1;
		}
			
		if(_glitter_heat_led)
		{
			LED_HEAT = ~LED_HEAT;
		}
		else
		{
			LED_HEAT = 1;
		}
		if(_glitter_drain_led)
		{
			LED_DRAIN = ~LED_DRAIN;
		}
		else
		{
			LED_DRAIN = 1;
		}
		if(_glitter_spin_led)
		{
			LED_SPIN = ~LED_SPIN;
		}
		else
		{
			LED_SPIN = 1;
		}
		if(_glitter_wash_led)
		{
			LED_WASH = ~LED_WASH;
		}
		else
		{
			LED_WASH = 1;
		}
			
		if(run_state || _manual_mode)
		{
			back_light = 0; 
		} 
		else if(back_light_timer)
		{//背光
			back_light_timer--;
			back_light = 0;  
		} 
		else
		{
			back_light = 1;  
		}  
		
      //自动洗衣程序  //extern void Auto_WashPro(void);
        if(wait_trans_time)
		{//联系通讯3次
			wait_trans_time--;
			if(wait_trans_time == 0)
			{
				_wait_trans = 0;
				if(_have_wash)
				{
					_wash_txd = 0;        
				} 
				else if(dehydration_num == 'Z')
				{
					_middle_dehydration_txd = 0;            
				} 
				else if(dehydration_num == 'G')
				{
					_high_dehydration_txd = 0;            
				} 
				else if(_enable_single_wash)
				{//均步
					_single_wash_txd = 0;            
				}
			}
		}
        if(run_state)
		{//运行状态 
			main_program_pro();//洗衣程序处理
		}
		if(run_state || _manual_mode)//手动运行状态
		{
			course_auto_manual_mode();
			if((_enable_wash ||_enable_single_wash) && input_shake &&
			middle_dehydration_stop_wait_timer == 0 && middle_dehydration_stop_skid_time == 0 &&
			high_dehydration_stop_wait_timer == 0 && high_dehydration_stop_skid_time  == 0 &&
			single_wash_complete_delay_time == 0 && single_wash_complete_skid == 0)
			{                  
              wash_pro();// azalea_wait_for_modify  wash_pro  洗涤正反转 
	
			}
      }         
      else
	  {//停机
			if(_dehydration_stop_pro)
			{
				dehydration_stop_pro();   //脱水结束处理     
			}
			if(_single_way_wash_stop_pro)
			{
				single_way_wash_stop_pro();//停机后均布结束和刹车处理        
			}
			if(_dehydration_stop_pro == 0 && _single_way_wash_stop_pro == 0 && _get_clothes_function == 0)
			{
				door_lock = 0;//门
			}
      }
      if(_auto_mode_run_stop &&
        middle_dehydration_stop_wait_timer == 0 && middle_dehydration_stop_skid_time == 0 && 
        high_dehydration_stop_wait_timer == 0 && high_dehydration_stop_skid_time == 0 && 
        single_wash_complete_delay_time == 0 && single_wash_complete_skid == 0)
	  {  
        _auto_mode_run_stop = 0;
        //自动模式
         run_state = 0;
//         _clear_for_stop = 1;
        //打开门锁
        door_lock = 0; 
       //LED灯
		LED_RUN = 1;
		LED_STOP = 0; // AZALEA_LED
	    _bit_set = 0;
		set_liquid_height = Code_I.P6[1];
        //发停止音
        _tell_stop = 1;
        tell_stop_num = 5; //需修改成五分钟
		  
        //显示洗涤完成页
        Menu = MENUCUR7;
		DispMod = dismod7;
		passdisp.disp_flg = 1;//液晶刷新标志位  azalea_modify
		_new_page = 1;
		    //背光
		back_light_timer = 180;    
		    	
      }
      //取衣功能
      if(_get_clothes_function &&
        middle_dehydration_stop_wait_timer == 0 && middle_dehydration_stop_skid_time == 0 && 
        high_dehydration_stop_wait_timer == 0 && high_dehydration_stop_skid_time == 0 && 
        single_wash_complete_delay_time == 0 && single_wash_complete_skid == 0){
        get_clothes_function(); // azalea_wait_for_modify 
	    }
	}        			
}

void main_program_pro(void)
{
	 unsigned char a,b,i;
	 unsigned int d = 0,e = 0;
		unsigned char  *Z;
	  Z = mode_data;
	// 开/关机处理
	if(_enable_ini_program) //运行初始化
	{
		_enable_ini_program = 0;
	 	cool_water = 0;   //冷水  ini_relay()
		hot_water = 0;    //热水
	    calefaction = 0;  //加热
		drain = 0;        //排水
	    soap_liquid_1 = 0;//皂液1
	    soap_liquid_2 = 0;//皂液2
	    soap_liquid_3 = 0;//皂液3 
		_ini_program_ok = 1;
	    _new_step = 1;
	    program_step_num = 0;
	}
	if(_ini_program_ok)//主程序初始化完成
	{
		_ini_program_ok = 0;
		for(i = 0; i < 200;i++)  
		{
			Z[i] = AT24CXX_ReadOneByte(PROGRAM_ONE+(auto_run_mode-1)*256+i);
		}
			//return ;
		  
	}
	//d3代表排水, d2代表加热, d1代表热水,d0代表冷水
	if(_new_step)
	{
		_new_step = 0;
		_all_time = 0;
	    _drain_enable = 0;//允许排水
		drain = 0;//关排水端口  
		//运行时间ok
	    auto_mode_s = mode_data[program_step_num*8 + 5] + 1;//秒
		auto_mode_m = mode_data[program_step_num*8 + 4];//分
		 //总时间
	    a=b=25;  //25 - program_step_num;
		for(e=0;a>=program_step_num+1 && a!=0;a--)
		{
			 e += mode_data[(a - 1)*8 + 4];//分    
		}
		for(d=0;b>=program_step_num+1 && b!=0;b--)
		{
			 d += mode_data[(b - 1)*8 + 5];//秒    
		}
		a = d/60;//分
		e += a;//分
		all_time_h = e/60;
		all_time_m = e%60;
		all_time_s = d%60;  
		 
		//液位高度ok 
		set_liquid_height = mode_data[program_step_num*8 + 2]; 
		//设定温度ok
		set_temp = mode_data[program_step_num*8 + 6];   
		 if(input_door_switch ==0)
		{
			 lcd_wrstring(1,28,"憨");

		}
		else
		{
			 lcd_wrstring(1,28,"翰");

		}
        // azalea_wy
		 if(_enable_display && _manual_mode == 0 )
		 {//每1秒置1
			_enable_display = 0;
			if(fact_temp > 100)	
			{
				fact_temp = fact_temp - 100;
				if(fact_temp > 100)
				{
					fact_temp = fact_temp - 100;

				}
			}			
			disphex_asc_le(aa,fact_temp,3);//实际温度
			if(aa[0] == '0')
			{
				aa[0] = ' ';
			}
			if(aa[1] == '0')
			{
				aa[1] = ' ';
			}
			lcd_wrstring_num(2,8,aa,3);
			
			disphex_asc_le(aa,fact_liquid_height,4);//实际液位
			if(aa[0] == '0')
			{
				aa[0] = ' ';
			}
			lcd_wrstring_num(2,23,aa,4); 
		 }
			//azalea_wy_end
		
	   //洗涤ok
	    if(mode_data[program_step_num*8 + 3] == 'X')
	    {//洗涤     
			 if(_have_wash == 0)
			 {      
			   _have_wash = 1; //洗涤标志
			   _had_wash = 0;
			   _wash_txd = 0;
			   trans_num = 3;
			   _wait_trans = 0;
				   
			 }
	   }
	   else
	   {
			_have_wash = 0;
			_had_wash = 0;
	   }
		
      //均步 
       _enable_single_wash = (mode_data[program_step_num*8 + 3] == 'J')?1:0;//是否有均布
       if(_enable_single_wash == 0 && _had_single_way_wash)
	   {
			 _had_single_way_wash = 0;//已经有均布
			 _single_wash_dis = 0;    //显示均布
			 single_wash_complete_delay_time = Code_I.P10[1];//均步结束延时
			 single_wash_complete_skid = Code_I.P10[4];      //均步结束刹车
			 _single_way_wash_stop_pro = 1;
			 wash_1 = 0;
			 wash_2 = 0;               
       } 
       else if(_enable_single_wash && _had_single_way_wash == 0)
	   {
			 _single_wash_txd = 0;//均布发送标志 
			 _single_wash_dis = 1;//显示均布
			 _wait_trans = 0;
			 trans_num = 3;     
      } 
      else if(_enable_single_wash == 0)
	  {//是否有均布
			 _had_single_way_wash = 0;//已经有均布
			 _single_wash_dis = 0;//显示均布
			 _single_wash_txd = 0;//均布时通讯
			 wash_1 = 0;
			 wash_2 = 0;       
      }	 
	
	  //冷水ok 
      if(mode_data[program_step_num*8 + 1] & 0x01)
	  {
			_have_cool_water = 1; 
      } 
      else
	  {
			_have_cool_water = 0;
      }
      _add_cool_water_stop = 0;
	 
	 //热水ok
      if(mode_data[program_step_num*8 + 1] & 0x02)
	  {
			_have_hot_water = 1;
      }
      else
	  {
			_have_hot_water = 0;
      }
      _add_hot_water_stop = 0;
	 
	  //加热ok
      if(mode_data[program_step_num*8 + 1] & 0x04)
	  {
			_have_calefaction = (fact_temp >= set_temp && fact_liquid_height > Code_I.P6[5])?0:1;
      } 
      else
	  {
			_have_calefaction = 0;    
      }
	 
	 //皂液ok
      _soap_liquid_1 = (mode_data[program_step_num*8 + 7] == 1)?1:0;  //皂液1
      _soap_liquid_2 = (mode_data[program_step_num*8 + 7] == 2)?1:0;  //皂液2
      _soap_liquid_3 = (mode_data[program_step_num*8 + 7] == 3)?1:0;  //皂液3 
      if(_soap_liquid_1 || _soap_liquid_2 || _soap_liquid_3)
	  {
			soap_liquid_time = Code_I.P7[1] + 1;//加皂液时间
      }
	 
	 //排水ok
      if(mode_data[program_step_num*8 + 3] != 'Z' || mode_data[program_step_num*8 + 3] != 'G')
	  {    
			_drain = (mode_data[program_step_num*8 + 1] & 0x08)?1:0;     
      } 
      else
	  {
			_drain = 1;  
      }
		//中脱ok
      if(mode_data[program_step_num*8 + 3] == 'Z')
	  {
			//scan_dehydration_num = 'Z';
			dehydration_num = 'Z';  
      }
      else if(mode_data[program_step_num*8 + 3] == 'G')
	  {//高脱
			 //scan_dehydration_num = 'G';
			 dehydration_num = 'G';   
      }   
      else
      {
			if(dehydration_num == 'Z' && middle_dehydration_stop_wait_timer == 0 && 
				middle_dehydration_stop_skid_time == 0)
			{
				middle_dehydration_stop_wait_timer = Code_I.P10[2];//中脱结束延时
				middle_dehydration_stop_skid_time = Code_I.P10[5] + 5;//中脱刹车延时   
				_dehydration_stop_pro = 1;      
			}
			else if(dehydration_num == 'G' && high_dehydration_stop_wait_timer == 0 &&
				high_dehydration_stop_skid_time == 0)
			{
			   high_dehydration_stop_wait_timer = Code_I.P10[3];//高脱程序结束等待时间 
			   high_dehydration_stop_skid_time = Code_I.P10[6] + 5;//高脱结束刹车时间
			   _dehydration_stop_pro = 1;      
			}
			 _spin_enable = 0;//脱水 
			 dehydration_num = 0;
			 _middle_dehydration_txd = 0;
			 _high_dehydration_txd = 0;     
			 //_enable_dehydration = 0;//不允许脱水
		} 
	}
	
	//跳步和时间倒计时
	if(input_shake)
	{
			if(Code_I.P2[2] == 'y' && _have_wash == 0 && _enable_single_wash == 0 && _have_cool_water == 0 && 
			 _have_hot_water == 0 && _have_calefaction == 0 && _drain == 0 && _soap_liquid_1 == 0 && 
			 _spin_enable  == 0 && dehydration_num == 0 && input_shake )
			{//可以跳步
					auto_mode_m = 0;
					auto_mode_s = 0;
		  }	
			if(middle_dehydration_stop_wait_timer == 0 && middle_dehydration_stop_skid_time == 0 && 
      high_dehydration_stop_wait_timer == 0 && high_dehydration_stop_skid_time == 0 && 
      single_wash_complete_delay_time == 0 && single_wash_complete_skid == 0)
			{
					//总时间    
				 if(all_time_s)
				 {
					  all_time_s--;          
				 } 
				 else if(all_time_m)
				 {
					  all_time_m--;
					  all_time_s = 59;
				 } 
				 else if(all_time_h)
				 {
					  all_time_m = 59;
					  all_time_s = 59;            
				 }
				 //单步时间
				 if(auto_mode_s)
				 {
					  auto_mode_s--;
				 } 
				 else if(auto_mode_m)
				 {
					  auto_mode_m--;
					  auto_mode_s = 59;    
				 }     
				 else
				 {//换步或停止
					_new_step = 1;
					 _all_time = 0;
					program_step_num++;
					 if((mode_data[program_step_num*8 + 1] == 0 && mode_data[program_step_num*8 + 2] == 0 && 
						 mode_data[program_step_num*8 + 3] == 0 && mode_data[program_step_num*8 + 4] == 0 &&
						 mode_data[program_step_num*8 + 5] == 0 && mode_data[program_step_num*8 + 6] == 0 && 
						 mode_data[program_step_num*8 + 7] == 0) || program_step_num > 25)
							 {//停止         
								 //run_state = 0;
								 //_new_step = 0;
								 //_clear_for_stop = 1;
								 //led_run = 1;
								// LED_RUN = 0;
								 //led_stop = 0;
								// LED_STOP = 1;
				//手动模式    
								 _manual_mode = 0;
				//自动模式
								 //run_state = 0;
								 //_clear_for_stop = 1;
				//停取衣
								 _get_clothes_function = 0;
								 get_clothes_provide_timer = 0;
								 deasil_run_time = 0;
								 deasil_stop_time = 0;
								 anticlockwise_run_time = 0;
								 anticlockwise_stop_time = 0;
									
				//停洗涤
								 _have_wash = 0;
								 _had_wash = 0;
								 _wash_txd = 0;//洗涤发送    
								 _enable_wash = 0;    
							 //_enable_single_wash = 0;
								 wash_1 = 0;
								 wash_2 = 0;
								 wash_run_time = 0;
								 wash_stop_time = 0;    
				//停均步             
								 if(_had_single_way_wash)
								 {   
									 single_wash_complete_delay_time = Code_I.P10[1];//均步结束延时
									 single_wash_complete_skid = Code_I.P10[4];//均步结束刹车
									 _single_way_wash_stop_pro = 1;//停止均步的结束刹车处理       
								 }     
								 _had_single_way_wash = 0;//已经有均布
								 _single_wash_dis = 0;//显示均布
								 _enable_single_wash = 0;//是否有均布
								 _single_wash_txd = 0;//均布时通讯     
				//停冷水     
								 cool_water = 0;
								 _have_cool_water = 0;
				//停热水
								 hot_water = 0;
								 _have_hot_water = 0;
								 _add_cool_water_stop = 0;
				//停加热
								 calefaction = 0;
								 _have_calefaction = 0;
								 _add_hot_water_stop = 0;
				//停皂液
								 _soap_liquid_1 = 0;
								 _soap_liquid_2 = 0;
								 _soap_liquid_3 = 0;
								 //_soap_liquid_not_pro = 0;
								 soap_liquid_1 = 0;
								 soap_liquid_2 = 0;
								 soap_liquid_3 = 0;
								 _soap_enable = 0;
								 soap_liquid_num = 0;
				//停排水 
								 drain = 0;
								 _drain_enable = 0;
								 _drain = 0;
				//停脱水
								 _spin_enable = 0; 
								 _enable_dehydration = 0;
								 if(dehydration_num == 'Z' && middle_dehydration_stop_wait_timer == 0 && 
									 middle_dehydration_stop_skid_time == 0)
								 {
										 middle_dehydration_stop_wait_timer = Code_I.P10[2];//中脱结束延时
										 middle_dehydration_stop_skid_time = Code_I.P10[5] + 5;//中脱刹车延时   
										 _dehydration_stop_pro = 1;      
								 }
								 else if(dehydration_num == 'G' && high_dehydration_stop_wait_timer == 0 &&
									 high_dehydration_stop_skid_time == 0)
								 {
									 high_dehydration_stop_wait_timer = Code_I.P10[3];//高脱程序结束等待时间 
									 high_dehydration_stop_skid_time = Code_I.P10[6] + 5;//高脱结束刹车时间
									 _dehydration_stop_pro = 1;      
								 } 
								 if(dehydration_num == 0 && 
									 middle_dehydration_stop_wait_timer == 0 && middle_dehydration_stop_skid_time == 0 && 
									 high_dehydration_stop_wait_timer == 0 && high_dehydration_stop_skid_time == 0 && 
									 single_wash_complete_delay_time == 0 && single_wash_complete_skid == 0)
								 {
									 door_lock = 0;//门
								 }
								 dehydration_num = 0;
								 _middle_dehydration_txd = 0;
								 _high_dehydration_txd = 0; 
								 _auto_mode_run_stop = 1;//自动模式运行停止标志  	   		   		   	       
						 }		                  
						 else
						 {
				 //中脱
								 if(mode_data[program_step_num*8 + 3] == 'Z')
								 {//初始化中脱参数
										 if(dehydration_num == 0){//scan_dehydration_num       
											 dehydration_provide_timer = Code_I.P9[0];//脱水开始前的准备时间
											 middle_dehydration_max_run_timer = Code_I.P9[5] * 60;//中脱最长不超过的运行时间   
											 middle_dehydration_settle_timer = Code_I.P9[4] + 30;//中脱和高脱之间的停留时间             
											 //middle_dehydration_stop_wait_timer = 0;//中脱程序结束等待时间
											 //middle_dehydration_stop_skid_time = 0;//中脱结束刹车时间 
											 //high_dehydration_max_run_timer = 0;//高脱最长不超过的运行时间       
											 //high_dehydration_stop_wait_timer = 0;//高脱程序结束等待时间  
											 //high_dehydration_stop_skid_time = 0;//高脱结束刹车时间
											 _middle_dehydration_txd = 0;
											 trans_num = 3;
											 _wait_trans = 0;           
									 } 
									 else if(dehydration_num == 'G')
									 {//scan_dehydration_num
												 //dehydration_provide_timer = 0;//脱水开始前的准备时间
												 middle_dehydration_max_run_timer =Code_I.P9[5] * 60;//中脱最长不超过的运行时间   
												 middle_dehydration_settle_timer = Code_I.P9[4] + 30;//中脱和高脱之间的停留时间             
												 //middle_dehydration_stop_wait_timer = 0;//中脱程序结束等待时间
												 //middle_dehydration_stop_skid_time = 0;//中脱结束刹车时间 
												 //high_dehydration_max_run_timer = 0;//高脱最长不超过的运行时间       
												 //high_dehydration_stop_wait_timer = 0;//高脱程序结束等待时间  
												 //high_dehydration_stop_skid_time = 0;//高脱结束刹车时间
												 _middle_dehydration_txd = 0;
												 trans_num = 3;
												 _wait_trans = 0;             
									 }
								 }
								 else if(mode_data[program_step_num*8 + 3] == 'G')
								 {//初始化高脱参数//高脱
										 if(dehydration_num != 'G')
										 {//scan_dehydration_num    
											 if(dehydration_num == 0)
											 {
												 dehydration_provide_timer = Code_I.P9[0];//脱水开始前的准备时间          
											 }
											 //middle_dehydration_max_run_timer = 0;//中脱最长不超过的运行时间       
											 //middle_dehydration_settle_timer = 0;//中脱和高脱之间的停留时间         
											 //middle_dehydration_stop_wait_timer = 0;//中脱程序结束等待时间
											 //middle_dehydration_stop_skid_time = 0;//中脱结束刹车时间  
											 high_dehydration_max_run_timer = Code_I.P9[6] * 60;//高脱最长不超过的运行时间       
											 //high_dehydration_stop_wait_timer = 0;//高脱程序结束等待时间  
											 //high_dehydration_stop_skid_time = 0;//高脱结束刹车时间
											 _high_dehydration_txd = 0;
											 trans_num = 3;
											 _wait_trans = 0;     
										 }
								 } 
								 else if(mode_data[program_step_num*8 + 3] != 'X' && mode_data[program_step_num*8 + 3] != 'J')
								 {
									 wash_1 = 0;
									 wash_2 = 0;      
								 }	 	
					 } 
					 passdisp.disp_flg = 0;	 
				 }
			}			
	}
    
}

void course_auto_manual_mode(void)
{
	if(input_door_switch == 0)
	{
		_error_door_open = 1;  
	} 
	else
	{
		_error_door_open = 0;
	}
		
	//紧急停止
	 if(input_exigency_stop == 0)
	 {
		  delay(100);
		  if(input_exigency_stop == 0) // azalea_wait_for_modify 紧急停止  JP5
		  {   
			   run_state = 0;
			   _manual_mode = 0;
			   _new_step = 0;
//				   _clear_for_stop = 1;
			   LED_RUN = 1;
				   LED_STOP = 0; // AZALEA_LED
			//停洗涤
			  _wash_txd = 0;//洗涤发送    
			  _enable_wash = 0;
			  _had_wash = 0;
			 //_enable_single_wash = 0;
			  wash_1 = 0;
			  wash_2 = 0;
			  wash_run_time = 0;
			  wash_stop_time = 0;
					 
			//停均步
			  single_wash_complete_delay_time = 0;//均步结束延时
			  single_wash_complete_skid = 0;//均步结束刹车
			  _single_way_wash_stop_pro = 0;//停止均步的结束刹车处理                 
			  _had_single_way_wash = 0;//已经有均布
			  _single_wash_dis = 0;//显示均布
			  _enable_single_wash = 0;//是否有均布
			  _single_wash_txd = 0;//均布时通讯           
			//停冷水     
			  cool_water = 0;
			  _have_cool_water = 0;
			  _add_cool_water_stop = 0;
			//停热水
			  hot_water = 0;
			  _have_hot_water = 0;
			  _add_hot_water_stop = 0;
			//停加热
			  calefaction = 0;
			  _have_calefaction = 0;
			//停皂液
			  _soap_liquid_1 = 0;
			  _soap_liquid_2 = 0;
			  _soap_liquid_3 = 0;
			  //_soap_liquid_not_pro = 0;
			  soap_liquid_1 = 0;
			  soap_liquid_2 = 0;
			  soap_liquid_3 = 0;
			  _soap_enable = 0;
			  soap_liquid_num = 0;
			//停排水 
			  drain = 0;
			  _drain_enable = 0;
			  _drain = 0;
			//停脱水
			  _spin_enable = 0; 
			  _enable_dehydration = 0;
			  dehydration_num = 0;
			  _middle_dehydration_txd = 0;
			  _high_dehydration_txd = 0;
			  middle_dehydration_stop_wait_timer = 0;//中脱程序结束等待时间
			  middle_dehydration_stop_skid_time = 0;//中脱结束刹车时间       
			  high_dehydration_stop_wait_timer = 0;//高脱程序结束等待时间  
			  high_dehydration_stop_skid_time = 0;//高脱结束刹车时间   
			//发停止音
			 _tell_stop = 1;
			 tell_stop_num = 5;
			//显示紧急停止页
			 DispMod = dismod9; //azalea.du
			 passdisp.disp_flg = 0;//液晶刷新标志位  azalea_modify
			 _all_time = 0;
			 
			 _new_page = 1; 
			//门锁  
			door_lock = 0; 
			return;
		}
	 }
	 //超震停止  
	 if(input_shake == 0)
	 {
			 delay(100);
			 if(input_shake == 0)
			 {
					 if(_shake_bit == 0)
					 {    
						 _shake_bit = 1;
						 shake_num++;
						 if((shake_num && Code_I.P3[3] && shake_num >= Code_I.P3[3] + 1) || Code_I.P3[3] == 0){
							 _error_shake = 1;//震动故障
							 run_state = 0;
							 _manual_mode = 0;
							 _new_step = 0;
							 _clear_for_stop = 1;
//								 led_run = 0;
							 LED_RUN = 0;
							 LED_STOP =1; // AZALEA_LED
//								 led_stop = 0;
				 //停洗涤
							 _wash_txd = 0;//洗涤发送    
							 _enable_wash = 0;
							 _had_wash = 0;
							 //_enable_single_wash = 0;
							 wash_1 = 0;
							 wash_2 = 0;
							 wash_run_time = 0;
							 wash_stop_time = 0;     
				 //停均步   
							 single_wash_complete_delay_time = 0;//均步结束延时
							 single_wash_complete_skid = 0;//均步结束刹车
							 _single_way_wash_stop_pro = 0;//停止均步的结束刹车处理                 
							 _had_single_way_wash = 0;//已经有均布
							 _single_wash_dis = 0;//显示均布
							 _enable_single_wash = 0;//是否有均布
							 _single_wash_txd = 0;//均布时通讯            
				 //停冷水     
							 cool_water = 0;
							 _have_cool_water = 0;
							 _add_cool_water_stop = 0;
				 //停热水
							 hot_water = 0;
							 _have_hot_water = 0;
							 _add_hot_water_stop = 0;
				 //停加热
							 calefaction = 0;
							 _have_calefaction = 0;
				 //停皂液
							 _soap_liquid_1 = 0;
							 _soap_liquid_2 = 0;
							 _soap_liquid_3 = 0;
							 //_soap_liquid_not_pro = 0;
							 soap_liquid_1 = 0;
							 soap_liquid_2 = 0;
							 soap_liquid_3 = 0;
							 _soap_enable = 0;
							 soap_liquid_num = 0;
				 //停排水 
							 drain = 0;
							 _drain_enable = 0;
							_drain = 0;
				 //停脱水
							 _spin_enable = 0; 
							 _enable_dehydration = 0;
							 dehydration_num = 0;
							 _middle_dehydration_txd = 0;
							 _high_dehydration_txd = 0;
							 middle_dehydration_stop_wait_timer = 0;//中脱程序结束等待时间
							 middle_dehydration_stop_skid_time = 0;//中脱结束刹车时间       
							 high_dehydration_stop_wait_timer = 0;//高脱程序结束等待时间  
							 high_dehydration_stop_skid_time = 0;//高脱结束刹车时间   
				 //发停止音
							 //_tell_stop = 1;
							 //tell_stop_num = 5;
				 //显示紧急停止页
							 //lcd_menucur = 8;     //主菜单指针
							 //_lcd_refresh_flag = 1;//液晶刷新标志位
							 //_new_page = 1; 
				 //门锁  
							 door_lock = 0; 	 
						 }
					 }     
					 return;
			 }
	 } 
	 else
	 {
		_shake_bit = 0;
	 }
	 //门锁
	 if(door_lock == 0)
	 {
		door_lock = 1;//门锁  
	 } 
	 
	 if(_dehydration_stop_pro)
	 {  
		 //中脱结束延时和刹车
		 if(middle_dehydration_stop_skid_time)
		 {//中脱停止刹车时间
			middle_dehydration_stop_skid_time--; 
		 }
		 if(middle_dehydration_stop_wait_timer)
		 {//中脱停止等待时间
			 middle_dehydration_stop_wait_timer--;
			 if(middle_dehydration_stop_skid_time == 0)
			 {
					skid = 1;    // azalea_wait_for_modify  JP6刹车
			 }
			 if(middle_dehydration_stop_wait_timer == 0)
			 {
					  skid = 0;
				 _dehydration_stop_pro = 0;
				 middle_dehydration_stop_skid_time = 0;
			 }
			 return; 
		 }
		 //高脱结束延时和刹车
		 if(high_dehydration_stop_skid_time)
		 {//高脱停止刹车时间
			 high_dehydration_stop_skid_time--;  
		 }
		 if(high_dehydration_stop_wait_timer)
		 {//高脱停止等待时间
			 high_dehydration_stop_wait_timer--;
			 if(high_dehydration_stop_skid_time == 0)
			 {
				 skid = 1;//刹车 
			 }
			 if(high_dehydration_stop_wait_timer == 0)
			 {
				 skid = 0;
				 _dehydration_stop_pro = 0;
				 high_dehydration_stop_skid_time = 0; 
			 }   
			 return;    
		 }
	}
	//均步结束延时和刹车 
	if(_single_way_wash_stop_pro)
	 {
		 if(single_wash_complete_skid)
		 {//均步结束刹车时间
			 single_wash_complete_skid--;  
		 }
		 if(single_wash_complete_delay_time)
		 {//均步结束延间
			 single_wash_complete_delay_time--;
			 if(single_wash_complete_skid == 0)
			 {
				  skid = 1;//刹车 
			 }
			 if(single_wash_complete_delay_time == 0)
			 {
				 skid = 0;
				 _single_way_wash_stop_pro = 0;
				 single_wash_complete_skid = 0; 
			 }   
			 return;
		 }
	 } 
			//洗涤ok
	 if(_have_wash)
	 {//有洗涤
		 if(_had_wash == 0)
		 {//是否已经进入洗涤
			 _had_wash = 1;
			 wash_provide_timer = Code_I.P8[0];
		 } 
		 if(fact_liquid_height >= Code_I.P6[4])
		 {//洗涤停洗液差    
			 if(wash_provide_timer == 0)
			 {//洗涤开始前的准备时间              
				 if(_wash_txd == 0)
				 {       
					 _wash_txd = 1;
					 _enable_wash = 1; 
					 /*if(wash_run_time == 0 && wash_stop_time == 0){
						 _wash_direction = 0;//洗涤方向
						 wash_run_time = P08[1];
						 wash_stop_time = P08[2];        
					 }     
					 else if(wash_run_time == 0){
						 wash_run_time = P08[1];//洗涤转动时间    
					 } 
					 else if(wash_stop_time == 0){
						 wash_stop_time = P08[2];//洗涤停止时间
					 }  */
					 if(trans_num && _wait_trans == 0)
					 {
						 trans_num--;
						 _wait_trans = 1;
						  wait_trans_time = 16;//重复发送的时间间隔
						 _enable_commu_Inventer = 1;//允许发送频率azalea__
						 run_frequency = (unsigned int)Code_I.P14[0] * 100;//洗涤频率
						 acce_time = 80;//8s
						 dece_time = 20;//2s
					 }
				 }
			 }
			 else
			 {
				 wash_provide_timer--;//洗涤开始前的准备时间        
			 }
		 }
		 else
		 {//非洗涤
			 _wash_txd = 0;
			 _enable_wash = 0;
			 wash_run_time = 0;
			 //wash_stop_time = 0;
			 if(wash_provide_timer == 0)
			 {
				 wash_provide_timer == Code_I.P8[0];  
				 wash_1 = 0;
				 wash_2 = 0;
			 }
		 }
	 }
	 else
	 {//没有洗涤强行停止洗涤
		 _wash_txd = 0;
		 _enable_wash = 0;
		 wash_run_time = 0;
		 //wash_stop_time = 0;
	 }   
			
		 //均步
	 if(_enable_single_wash)
	{//有均步
	  if(fact_liquid_height >= Code_I.P6[6]){//低于此液位，均步排水失效                            
		  if(_single_wash_txd == 0){
				 _single_wash_txd = 1;
				 if(_had_single_way_wash == 0){//已经有均步        
					 _had_single_way_wash = 1;
					 wash_stop_time = Code_I.P9[0];
				 }  
				 if(trans_num && _wait_trans == 0){
					 trans_num--;
					 _wait_trans = 1;
					 wait_trans_time = 16;   
					 _enable_commu_Inventer = 1;//允许发送频率azalea__
					 run_frequency = (unsigned int)Code_I.P14[2] * 100;//洗涤频率
					 acce_time = 80;//8s
					 dece_time = (unsigned char)Code_I.P10[4] * 10;//均步结束刹车时间       
				 }
			 }                     
			 if(_manual_mode){//手动模式  
				 if(single_wash_have_water_time)
				 {//均步带水时间
					 single_wash_have_water_time--;
					 if(single_wash_have_water_time)
					 {
						 _drain_enable = 1;     
						 _drain = 0;
						 drain = 0;
					 } 
				 }
				 else if(single_wash_drain_time)
				 {  //均步排水时间
					 single_wash_drain_time--;
					 if(single_wash_drain_time)
					 {          
						 _drain_enable = 1; 
						 _drain = 1;
						 drain = 1;
					 } 
				 }  
				 else
				 {
					 //_enable_single_wash = 0;//是否有均布
					 _had_single_way_wash = 0;//已经有均布
					 _single_wash_txd = 0;
					 wash_1 = 0;
					 wash_2 = 0;         
				 }
			 }        
		 }      
	 }
		 //冷水ok  
	 if(_have_cool_water)
	 {
		 if(soap_liquid_time && Code_I.P7[5])//p7[5]加皂液关冷水
		 {
			 cool_water = 0;    
		 } 
		 else if(Code_I.P2[1] == 'y')
		 {//p2[1]自动补水
			 if(_add_cool_water_stop)
			 {
				 if(fact_liquid_height <= set_liquid_height - Code_I.P3[1])
				 {//p3[1]自动补水液差
					 cool_water = 1;
					 _add_cool_water_stop = 0;
				 } 
				 else{
					 cool_water = 0;        
				 }
			 } 
			 else if(fact_liquid_height < set_liquid_height){
				 cool_water = 1;
			 } 
			 else{
				 cool_water = 0;
				 _add_cool_water_stop = 1;      
			 }
			 //cool_water = (fact_liquid_height >= set_liquid_height)?0:1;   
		 } 
		 else{
			 if(fact_liquid_height < set_liquid_height){   
				 cool_water = 1;
			 } 
			 else{
				 _have_cool_water = 0;
				 cool_water = 0;      
			 }    
		 }        
	 } 
	 else{
		 cool_water = 0;
	 }
		 //热水ok
	 if(_have_hot_water && fact_liquid_height >= Code_I.P3[2])
	{
		 if(Code_I.P2[1] == 'y'){
			 if(_add_hot_water_stop){
				 if(fact_liquid_height <= set_liquid_height - Code_I.P3[1]){
					 hot_water = 1;
					 _add_hot_water_stop = 0;
				 } 
				 else{
					 hot_water = 0;        
				 }
			 } 
			 else if(fact_liquid_height < set_liquid_height){
				 hot_water = 1;
			 } 
			 else{
				 hot_water = 0;
				 _add_hot_water_stop = 1;      
			 }   
			 hot_water = (fact_liquid_height >= set_liquid_height)?0:1;   
		 } 
		 else{
			 if(fact_liquid_height < set_liquid_height){   
				 hot_water = 1;
			 } 
			 else{
				 _have_hot_water = 0;
				 hot_water = 0;      
			 }    
		 }  
	 }
	 else{
		 hot_water = 0;
	 } 
		 //加热ok
	 if(_have_calefaction)
	{
		 if(Code_I.P2[0] == 'y'){
			 calefaction = (fact_temp < set_temp && fact_liquid_height >= Code_I.P6[5])?1:0;  
		 } 
		 else{
			 if(fact_temp < set_temp && fact_liquid_height >= Code_I.P6[5]){
				 calefaction = 1;// azalea_wait_for_modify JP6 加热
			 }
			 else{
				 calefaction = 0;
				 _have_calefaction = 0;      
			 }    
		 }   
	 } 
	 else{
		 calefaction = 0;    
	 }
		 //皂液ok  
	 if(soap_liquid_time)
 {
		 if(_soap_liquid_pro){//允许皂液处理
			 soap_liquid_time--;
			 if(soap_liquid_time){
				 soap_liquid_1 = _soap_liquid_1;  //皂液1
				 soap_liquid_2 = _soap_liquid_2;  //皂液2
				 soap_liquid_3 = _soap_liquid_3;  //皂液3     
				 if(soap_liquid_1){//显示皂液1   
					 soap_liquid_num = 1;
					 _soap_enable = 1;
				 }
				 else if(soap_liquid_2){
					 soap_liquid_num = 2;
					 _soap_enable = 1;
				 }
				 else{
					 soap_liquid_num = 3;
					 _soap_enable = 1;        
				 }         
			 }  
		 } 
		 else{
			 soap_liquid_1 = 0;
			 soap_liquid_2 = 0;
			 soap_liquid_3 = 0;
			 _soap_enable = 0;
			 soap_liquid_num = 0;    
		 }  
	 }
	 else{
		 _soap_liquid_1 = 0;
		 _soap_liquid_2 = 0;
		 _soap_liquid_3 = 0;
		 soap_liquid_1 = 0;
		 soap_liquid_2 = 0;
		 soap_liquid_3 = 0;
		 _soap_enable = 0;
		 soap_liquid_num = 0;    
	 }
		 //排水
	 if(_manual_mode && (_enable_single_wash || dehydration_num))
	 {//手动模式   有均布||有脱水                      
			 ;
	 }
	 else
 {
		 drain = _drain;  
		 if(drain){
			 if(dehydration_num == 0){    
				 _drain_enable = 1;   
			 } 
			 else{
				 _drain_enable = 0;   
			 }
			 _soap_enable = 0;
			 soap_liquid_num = 0;
			 soap_liquid_1 = 0;//皂液1
			 soap_liquid_2 = 0;//皂液2
			 soap_liquid_3 = 0;//皂液3         
		 }
		 else{
			 _drain_enable = 0;    
		 }
	 }
		 //中脱ok
	 if(dehydration_num == 'Z')
	 {//中脱处理scan_dehydration_num
		 //if(fact_liquid_height > P06[7]){//排水最低液位
			 if(dehydration_provide_timer == 0)
			 {//脱水延迟时间            
				 if(middle_dehydration_settle_timer)
				 {//中脱停留时间
					 middle_dehydration_settle_timer--;      
				 }
				 if(middle_dehydration_max_run_timer == 0)
				 {//中脱最长不超过的时间  强行停止洗涤
					 if(_spin_enable){               
						 _drain = 0;     
						 _spin_enable = 0;//脱水
						 dehydration_num = 0;
						 wash_1 = 0;
						 wash_2 = 0;//停止脱水
						 middle_dehydration_stop_wait_timer = Code_I.P10[2];//中脱结束延时
						 middle_dehydration_stop_skid_time = Code_I.P10[5] + 5;//中脱刹车延时
						 //if(run_state){
							 _dehydration_stop_pro = 1;
						 //}
					 }          
				 } 
				 else
				 {
					 middle_dehydration_max_run_timer--;          
				 }    
				 if(_middle_dehydration_txd == 0)
				 {//中脱发送标志
					 _middle_dehydration_txd = 1;
					 _drain = 1;     
					 _spin_enable = 1;//脱水
					 //dehydration_num = 'Z';        
					 if(trans_num && _wait_trans == 0)
					 {
						 trans_num--;
						 _wait_trans = 1;
						 wait_trans_time = 16; 						 
						 _enable_commu_Inventer = 1;
						 run_frequency = (unsigned int)Code_I.P14[3]*100;//中脱频率
						 acce_time = 300;//30s
						 dece_time = (unsigned int)Code_I.P10[5]*10;//中脱结束刹车 
						 wash_1 = 1;
						 wash_2 = 0;//允许脱水
					 }
				 }     
			 }
			 else
			 {
				 dehydration_provide_timer--;//脱水延时时间
				 wash_1 = 0;
				 wash_2 = 0;//停止脱水
			 }
		 //}
	 }    	
	 else if(dehydration_num == 'G')
	{//scan_dehydration_num //高脱ok 
		 if(dehydration_provide_timer == 0){//脱水延迟时间
			 if(middle_dehydration_settle_timer == 0){//中脱停留时间 
				 //if(fact_liquid_height > P06[7]){//排水最低液位  
					 if(high_dehydration_max_run_timer == 0){//高脱最长不超过的时间  强行停止洗涤
						 if(_spin_enable)
						 {               
							 _drain = 0;     
							 _spin_enable = 0;//脱水
							 dehydration_num = 0;
							 wash_1 = 0;
							 wash_2 = 0;//停止脱水
							 high_dehydration_stop_wait_timer = Code_I.P10[3];//高脱程序结束等待时间 
							 high_dehydration_stop_skid_time = Code_I.P10[6] + 5;//高脱结束刹车时间
							 //if(run_state){
								 _dehydration_stop_pro = 1;
							 //}
						 }           
					 } 
					 else{
						 high_dehydration_max_run_timer--;          
					 } 
					 if(_high_dehydration_txd == 0){//高脱发送标志        
						 _high_dehydration_txd = 1;
						 _drain = 1;     
						 _spin_enable = 1;//脱水
						 //dehydration_num = 'G';           
						 if(trans_num && _wait_trans == 0)
						 {
							 trans_num--;
							 _wait_trans = 1;
							 wait_trans_time = 16;
							 _enable_commu_Inventer = 1;//azalea__
							 run_frequency = (unsigned int)Code_I.P14[4]*100;//高脱频率
							 acce_time = 300;//30s
							 dece_time = (unsigned int)Code_I.P10[6]*10;//高脱结束刹车                    
							 wash_1 = 1;
							 wash_2 = 0;//允许脱水
						 }
					 }
				 //}
			 }        
			 else{
				 middle_dehydration_settle_timer--;//中脱停留时间
			 } 
		 }
		 else{
			 dehydration_provide_timer--;
			 if(dehydration_provide_timer == 0){
				 middle_dehydration_settle_timer = 0;      
			 }          
		 }
	 }
}

void handshake_pro(void)
{
	 DispMod = dismod11;
	 Menu = MENUCUR11;
	 passdisp.disp_flg= 0;	
	 _new_page = 1;
	 _tell_alarm = 1;//发告警声
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void fault_dis_pro(void)
{
	
	unsigned char i,a,b,c[10],d=0;
	// unsigned char  *P;
	//if(new_fault || _error_commu || rxe_receive_timer >= 180 || _error_shake ||
	//		ad_fault_num >= 180 || _error_door_open)
	if(INVCurrent_fault)//变频器发生的故障
	{
		new_fault = INVCurrent_fault;
	}
	else
	{
		new_fault = 0;
	}
	if(new_fault || flag_error_usart/*_error_commu*/ || _error_shake /*|| ad_fault_num >= 180*/ || _error_door_open || flag_usart2_read_error )
	{
	   if(_fault_pro_complete == 0)
	   {  
		 _have_fault = 1;//有错误
		 _fault_pro_complete = 1;
		 if(_no_read_fault == 0)//把保存在e2prom的故障加载进来
		 {
			_no_read_fault = 1;  
			for( i =0; i <12; i++)
			{
				fault[i] = AT24CXX_ReadOneByte(ADD_FAULT_E2+i);
			}
		 }
		 if(new_fault)//新故障
		 { 
			if(fault[0] !=  new_fault)
			{
				 for(a=0;a<12;a++)
				 {
					c[a] = fault[a];
				 }  
				 for(a=0,b=1;b<11;a++,b++){
					fault[b] = c[a];              
				 }
				 fault[0] =  new_fault;
	//           write_e2_pro(10,7,0,fault);//数据个数 芯片地址 保存数据首地址 数据的首地址
				 for(i = 0; i < 12; i++)
				 {
					 AT24CXX_WriteOneByte(ADD_FAULT_E2+i,fault[i]);
				 }
			}
		 } 
		 else
		 {
		    if(/*_error_commu*/flag_error_usart )//通讯故障
		    {
				d = 0x1a;  
		    } 
		    else if(_error_shake)//超震故障
		    {
				d = 0x1b;
		    }
		    //else if(ad_fault_num >= 180)
//		    {
//				d = 0x1d;      
//		    } 
		    else if(_error_door_open)
		    {
				d= 0x1e;        
		    }
			else if(flag_usart2_read_error)//elc->inventer
			{
				d = 0x1f;
			}
		    if(fault[0] != d)
		    {
				 for(a=0;a<12;a++)
				 {
					c[a] = fault[a];
				 }  
				 for(a=0,b=1;b<11;a++,b++)
				 {
					fault[b] = c[a];              
				 }
			     fault[0] = d;
	//         write_e2_pro(10,7,0,fault);//数据个数 芯片地址 保存数据首地址 数据的首地址
				 for(i = 0; i < 12; i++)
				 {
					AT24CXX_WriteOneByte(ADD_FAULT_E2+i,fault[i]);
				 }
		    }     
		 }
	       //强制转换显示页
	  // dismenu.sub_menucur = 0x0a;   //故障查询菜单 8
	  // _lcd_refresh_flag = 1;//液晶刷新标志位
		 
		 //通过界面显示故障
		 DispMod = dismod10;
		 Menu = MENUCUR10;
		 passdisp.disp_flg= 0;	
		 _new_page = 1;
		

		// _tell_alarm = 1;//发告警声
		   //手动模式    
		 _manual_mode = 0;
	       //自动模式
		 run_state = 0;
	//   _clear_for_stop = 1;
	     _auto_mode_run_stop = 0;
	  //停取衣
		 _get_clothes_function = 0;
		 get_clothes_provide_timer = 0;
		 deasil_run_time = 0;
		 deasil_stop_time = 0;
		 anticlockwise_run_time = 0;
		 anticlockwise_stop_time = 0;      
	  //停洗涤
		 LED_WASH = 1;// azalea_170421
		 _glitter_wash_led	= 0;
		 
		_have_wash = 0;
		 _had_wash = 0;
		 _wash_txd = 0;//洗涤发送    
		 _enable_wash = 0;    
		 //_enable_single_wash = 0;
		 wash_1 = 0;
		 wash_2 = 0;
		 wash_run_time = 0;
		 wash_stop_time = 0; 
			 
	  //停均步
	  
	   
		 single_wash_complete_delay_time = 0;//均步结束延时
		 single_wash_complete_skid = 0;//均步结束刹车
		 _single_way_wash_stop_pro = 0;//停止均步的结束刹车处理                 
		 _had_single_way_wash = 0;//已经有均布
		 _single_wash_dis = 0;//显示均布
		 _enable_single_wash = 0;//是否有均布
		 _single_wash_txd = 0;//均布时通讯                
	  //停冷水 
		_glitter_cold_led = 0;//azalea_170421
		LED_COLD = 1;	  
		 cool_water = 0;
		 _have_cool_water = 0;
		 _add_cool_water_stop = 0;
	  //停热水
		  _glitter_warm_led = 0;
		  LED_WARM = 1;
 		  hot_water = 0;
		 _have_hot_water = 0;
	  //停加热
	     _glitter_heat_led = 0;
		 LED_HEAT = 1;
		 calefaction = 0;
		 _have_calefaction = 0;
		 _add_hot_water_stop = 0;
	  //停皂液
	   LED_SOAP = 1;
		 _soap_liquid_1 = 0;
		 _soap_liquid_2 = 0;
		 _soap_liquid_3 = 0;
	   //_soap_liquid_not_pro = 0;
		 soap_liquid_1 = 0;
		 soap_liquid_2 = 0;
		 soap_liquid_3 = 0;
		 _soap_enable = 0;
		 soap_liquid_num = 0;
	  //停排水 
	     LED_DRAIN = 1;
	     _glitter_drain_led = 0;
		 drain = 0;
		 _drain_enable = 0;
		 _drain = 0;
	  //停脱水
		 LED_SPIN= 1;
		 _glitter_spin_led = 0;
		 _spin_enable = 0; 
		 _enable_dehydration = 0;
		 dehydration_num = 0;
		 _middle_dehydration_txd = 0;
		 _high_dehydration_txd = 0; 
		 middle_dehydration_stop_wait_timer = 0;//中脱程序结束等待时间
		 middle_dehydration_stop_skid_time = 0;//中脱结束刹车时间       
		 high_dehydration_stop_wait_timer = 0;//高脱程序结束等待时间  
		 high_dehydration_stop_skid_time = 0;//高脱结束刹车时间         
		 LED_RUN = 1;
		 LED_STOP = 0;
		 
	   }
	} 
	else
	{
	   _have_fault = 0;
	   _fault_pro_complete = 0;
	   _no_read_fault = 0;
	   _tell_alarm = 0;  
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_mainmenu_handler(void)//dismod0
{
	 u8 i;
		 if(KeyVal==Return)
		 {
			 //_enable_commu_Inventer = 1;//azalea_test
				if(run_state == 0 && _manual_mode == 0 && _get_clothes_function == 0)// azalea???
				{
					Menu = MENUCUR5;
					DispMod = dismod5;
					dismenu.sub_menucur = 0x00;//主菜单光标指针  azalea???
					dismenu.sub_chosecur = 0x00;//选择指针 azalea???
					_new_page = 1;
					passdisp.disp_flg= 0;	
					_buzzer_run = 1;      //蜂鸣器运行标志	
					buzzer_num = 1;       //蜂鸣器响的次数
				}
				else
				{
					_buzzer_run = 1;      //蜂鸣器运行标志	
					buzzer_num = 3;       //蜂鸣器响的次数
				}
		 }
		 else if(KeyVal==UP)//auto_run_mode 1-标准 2-床单 3-台布 4-衬衣 5-毛巾 
		 {
			 if(_bit_set == 1)
			 {
				  fact_liquid_height++;
					// break;
			 }
            //auto_run_mode = 1;
			 if(run_state == 0 && _manual_mode == 0 && _get_clothes_function == 0 /*&& _num_4 == 0 */&& _have_fault == 0)
			 {
					if(auto_run_mode < 5)
					{
						auto_run_mode++; 
//						if(auto_run_mode == 2)//azalea_test
//						{
//							wash_1 = 0;
//							wash_2 = 1;
//						}
//						 if(auto_run_mode == 3)
//						{
//							wash_1 = 0;
//							wash_2 = 0;
//						}						
					}
					else
					{										
						 auto_run_mode = 1;
					}
					 _num_4 = 1;// azalea???
							 //读e2 
					for(i = 0; i <200; i++)
					{
						mode_data[i] = AT24CXX_ReadOneByte(PROGRAM_ONE+(auto_run_mode-1)*256+i);
					}
					_buzzer_run = 1;      //蜂鸣器运行标志	
					buzzer_num = 1;       //蜂鸣器响的次数 
			}
			else if(_num_4 == 0)
			{
				 _num_4 = 1;
				 _buzzer_run = 1;      //???????	
				 buzzer_num = 3;       //???????
			}
		 }
		 else if(KeyVal==Down)
		 {
			 if(_bit_set == 1)
			 {
				  fact_liquid_height--;
					// break;
			 }
			 if(run_state == 0 && _manual_mode == 0 && _get_clothes_function == 0 && _have_fault == 0 )
			 {
					Menu = MENUCUR6;
					DispMod = dismod6;
					dismenu.sub_menucur = 0x00;
					dismenu.sub_chosecur = 0;
					_read_data = 0;//???
					_read_function = 0;//???
			//		_new_page = 1;//????   azalea???	
					passdisp.disp_flg= 0;		
					_buzzer_run = 1;      //蜂鸣器运行标志	
					buzzer_num = 1;       //蜂鸣器响的次数					
			 }
			 else if(_num_4 == 0)
			 {
					 _num_4 = 1;
					 _buzzer_run = 1;      //蜂鸣器运行标志	
					 buzzer_num = 3;       //蜂鸣器响的次数
			 }
		 }
		 else if(KeyVal==Setup)
		 {
			 if(run_state == 0 && _manual_mode == 0 && _get_clothes_function == 0 && _have_fault == 0)
			 {
				   Menu = MENUCUR1;
				   DispMod = dismod1;	 
				   dismenu.sub_menucur = 0x01;
				   dismenu.sub_chosecur = 0;    //????
					//passdisp.disp_flg = 1;
				   _buzzer_run = 1;      //蜂鸣器运行标志	
				   buzzer_num = 1;        //蜂鸣器响的次数
			 }
		 }
		 else if(KeyVal==16) //RUN
		 {
			 if(run_state || _manual_mode || _get_clothes_function || _have_fault ||input_door_switch == 0) // azalea_wait_for_modify JP5 门开关
			 {
				 _buzzer_run = 1;      //蜂鸣器运行标志	
				 buzzer_num = 3;       //蜂鸣器响的次数
			 }
			 else
			 { 
				run_state = 1;//1-运行 0-停止
				door_lock = 1;//门锁   azalea_wait_for_modify JP6
				_enable_ini_program = 1;//运行初始化
				_clear_for_start = 1;
				LED_RUN = 0;
				LED_STOP = 1;
				 if(input_shake){ // azalea_wait_for_modify  JP5
					 _shake_bit = 0;
					 shake_num = 0;       
				 }
				 _soap_liquid_3 = 0;//皂液3
				 soap_liquid_time = 0;
				 _buzzer_run = 1;	
				 buzzer_num = 1;	
				_bit_set = 1;
			 }
				
		 }
		 else if(KeyVal==11) //停止 Stop STOP (azalea_wait)
		 {
				if(run_state == 0 && _manual_mode == 0 && _get_clothes_function == 0 
					&& input_door_switch && new_fault) // azalea_wait_for_modify 门开关 JP5
				{ 
//						usart3_flag_fault_clear = 1;
				//		break;
					 _buzzer_run = 1;	
					 buzzer_num = 1;
			 
				 }
				 if((run_state == 0 && _manual_mode == 0 && _get_clothes_function == 0) 
					 || input_door_switch == 0 || _have_fault)
				 {
							//蜂鸣器运行
					  _buzzer_run = 1;      //蜂鸣器运行标志	
					  buzzer_num = 3;       //蜂鸣器响的次数 
				 }
				 else
				 {
				//手动模式
					_manual_mode = 0;  
			 //自动模式
					run_state = 0;
//				    _clear_for_stop = 1;  //azalea_wait_for_modify
					_auto_mode_run_stop = 0;	 
					 //停取衣
					_get_clothes_function = 0;
					get_clothes_provide_timer = 0;
					deasil_run_time = 0;
					deasil_stop_time = 0;
					anticlockwise_run_time = 0;
					anticlockwise_stop_time = 0; 	 
					//停洗涤
					LED_WASH = 1;
					_have_wash = 0;
					_had_wash = 0;
					_wash_txd = 0;//洗涤发送    
					_enable_wash = 0;    
					 //_enable_single_wash = 0;
				    wash_1 = 0; // azalea_wait_for_modify JP7 x1
					wash_2 = 0;// JP7 x2
					wash_run_time = 0;
					wash_stop_time = 0; 
					_glitter_wash_led	= 0;							 
						 
					 //停均布
					 if(_had_single_way_wash)//
					 {   
						 single_wash_complete_delay_time = Code_I.P10[1];//均步结束延时
						 single_wash_complete_skid = Code_I.P10[4];//均步结束刹车
						 _single_way_wash_stop_pro = 1;//停止均步的结束刹车处理       
					 }     
					 _had_single_way_wash = 0;//已经有均布
					 _single_wash_dis = 0;//显示均布
					 _enable_single_wash = 0;//是否有均布
					 _single_wash_txd = 0;//均布时通讯   
					 _glitter_wash_led	= 0;							 

					 //停冷水
					 cool_water = 0; // azalea_wait_for_modify  下位机传过来的的冷水 JP6
					 _have_cool_water = 0;// 有冷水
					 _add_cool_water_stop = 0;//停止添加冷水
					 _glitter_cold_led = 0;
					 LED_COLD = 1;
					 //停热水
					 hot_water = 0;//azalea_wait_for_modify 热水 JP6
					 _have_hot_water = 0;
					 _glitter_warm_led = 0;
					 LED_WARM = 1;
					 //停加热
					 calefaction = 0;// azalea_wait_for_modify 加热 JP6
					 _have_calefaction = 0;    
					 _add_hot_water_stop = 0;
					 _glitter_heat_led = 0;
					 LED_HEAT = 1;
					 //停皂液
					 _soap_liquid_1 = 0;
					 _soap_liquid_2 = 0;
					 _soap_liquid_3 = 0;
					 LED_SOAP = 1;
					  //_soap_liquid_not_pro = 0;
					 soap_liquid_1 = 0;// azalea_wait_for_modify 皂液1 JP6
					 soap_liquid_2 = 0;// azalea_wait_for_modify 皂液2 JP6
					 soap_liquid_3 = 0;// azalea_wait_for_modify 皂液3 JP6
					 _soap_enable = 0;
					 soap_liquid_num = 0;
					 //停排水
					 drain = 0;//azalea_wait_for_modify 排水 JP6
					 LED_DRAIN = 1;
					 _drain_enable = 0;
					 _drain = 0;
					 _glitter_drain_led = 0;
					 //停脱水
					 _spin_enable = 0; 
					 _enable_dehydration = 0;
					 LED_SPIN= 1;
					 if(dehydration_num == 'Z' && middle_dehydration_stop_wait_timer == 0 && 
						 middle_dehydration_stop_skid_time == 0)
					 {
							 middle_dehydration_stop_wait_timer = Code_I.P10[2];//中脱结束延时
							 middle_dehydration_stop_skid_time = Code_I.P10[5] + 5;//中脱刹车延时   
							 _dehydration_stop_pro = 1;      
					 }
					 else if(dehydration_num == 'G' && high_dehydration_stop_wait_timer == 0 &&
						 high_dehydration_stop_skid_time == 0)
					 {
							 high_dehydration_stop_wait_timer = Code_I.P10[3];//高脱程序结束等待时间 
							 high_dehydration_stop_skid_time = Code_I.P10[6] + 5;//高脱结束刹车时间
							 _dehydration_stop_pro = 1;      
					 } 
					 if(dehydration_num == 0 && 
						 middle_dehydration_stop_wait_timer == 0 && middle_dehydration_stop_skid_time == 0 && 
						 high_dehydration_stop_wait_timer == 0 && high_dehydration_stop_skid_time == 0 && 
						 single_wash_complete_delay_time == 0 && single_wash_complete_skid == 0)
					 {
						  door_lock = 0;//门  azalea_wait_for_modify JP6
					 }
					 dehydration_num = 0;
					 _middle_dehydration_txd = 0;
					 _high_dehydration_txd = 0;  
					 //led灯
					 // led_run = 1;
					 LED_RUN = 1;
					 LED_STOP = 0; // AZALEA_LED
		     
					 _glitter_spin_led = 0;
					 _buzzer_run = 1;	
					 buzzer_num = 1; 
					 _bit_set = 0;	 
					 set_liquid_height = Code_I.P6[1];
					 passdisp.disp_flg = 0;
			 }
					 
					
		 }
		 else if(KeyVal==10)//洗涤 Wash
		 {
//			 _enable_display = 1;
			 if(run_state && _spin_enable == 0 && _drain_enable == 0 && 
             _get_clothes_function == 0 && _have_fault == 0 && input_door_switch 
				    	&& _auto_mode_run_stop == 0)
			{//脱水  排水
				if(_new_step == 0)
				{//运行时的新步
					 if(_have_wash == 0 && _enable_single_wash == 0)
					 {
						 _have_wash = 1;
						 _had_wash = 0;
						 _wash_txd = 0; 
						 _enable_wash = 1;
						 trans_num = 3;
						 _wait_trans = 0;
						 _enable_single_wash = 0;
						 _single_wash_dis = 0;//显示用
						 _single_wash_txd = 0;             
					 } 
					 else if(_have_wash && _enable_single_wash == 0)
					 {
						 _have_wash = 0;
						 _had_wash = 0;
						 _wash_txd = 0; 
						 _enable_wash = 0;  
						 //均布
						 _enable_single_wash = 1;//均步标志
						 _single_wash_dis = 1;//显示均步     
						 _single_wash_txd = 0; 
						 trans_num = 3;
						 _wait_trans = 0;             
					 }
					 else
					 {
						 _have_wash = 0;
						 _had_wash = 0;
						 _wash_txd = 0;
						 _enable_wash = 0;
						 wash_1 = 0;
						 wash_2 = 0;
						 wash_run_time = 0;
						 wash_stop_time = 0;           
						 //停均布
						 _enable_single_wash = 0;//是否有均布
						 _single_wash_dis = 0;//显示均布 
						 _single_wash_txd = 0;//均布时通讯
						 if(_had_single_way_wash){            
							 single_wash_complete_delay_time = Code_I.P10[1];//均步结束延时
							 single_wash_complete_skid = Code_I.P10[4];//均步结束刹车
							 _single_way_wash_stop_pro = 1;  
						 }
						 _had_single_way_wash = 0;//已经有均布
						 wash_1 = 0;
						 wash_2 = 0; 
					 }
				}    
				_buzzer_run = 1;	
				buzzer_num = 1;
							
			}
			else if(run_state == 0 && _spin_enable == 0 && _drain_enable == 0 && 
           _get_clothes_function == 0 && _have_fault == 0 && input_door_switch &&
           _enable_single_wash == 0)
			{//脱水  排水
					 if(_manual_mode == 0)
					 {//手动模式标志
						 _manual_mode = 1;
						 auto_mode_s = 0;//自动模式运行秒
						 auto_mode_m = 0;//自动模式运行时间分
						 //led_run = 0;
						 LED_RUN = 0;
						 LED_STOP = 1;
				
						 _clear_for_start = 1;//1--运行  0--停止  机器当前状态
						 set_liquid_height = Code_I.P6[1];
						 door_lock = 1;//门锁 
						 if(input_shake){
							 _shake_bit = 0;
							 shake_num = 0; 
						 }
						 _soap_liquid_3 = 0;//皂液3
						 soap_liquid_time = 0;
										
					 }       
					 if(_have_wash == 0)
					 { 
						 _have_wash = 1;
						 _had_wash = 0;
						 _wash_txd = 0;
						 _enable_wash = 1;
						 trans_num = 3;
						 _wait_trans = 0;						
					 }      
					 else
					 {
						 _have_wash = 0;
						 _had_wash = 0;
						 _wash_txd = 0; 
						 _enable_wash = 0;
						 _enable_single_wash = 0;
						 wash_1 = 0;
						 wash_2 = 0;
						 wash_run_time = 0;
						 wash_stop_time = 0;
					 }
				 _buzzer_run = 1;	
				 buzzer_num = 1; 
			}
			else
		    {    
				 _buzzer_run = 1;	
				 buzzer_num = 3;       
			}   
					passdisp.disp_flg = 0;
		 }
		 else if(KeyVal==4)//冷水 Cold
		 {
				 if(run_state && _spin_enable == 0 && _drain_enable == 0 && 
					 _get_clothes_function == 0 && _have_fault == 0 
				   && input_door_switch && _auto_mode_run_stop == 0)
				 {
						 if(_new_step == 0)
						 {
								 if(fact_liquid_height < set_liquid_height)
								 {  
										 _have_cool_water = ~_have_cool_water;
										 if(_have_cool_water)
										 {
												_add_cool_water_stop = 0;            
										 }
								 } 
								 else{
									 _have_cool_water = ~_have_cool_water;
										 if(_have_cool_water)
										 {
											 _add_cool_water_stop = 0;            
										 }
									 _buzzer_run = 1;	
									 buzzer_num = 3;
//									 break;          
								 }          
						 }      
						 _buzzer_run = 1;	
						 buzzer_num = 1;
				 }
				 else if(run_state == 0 && _spin_enable == 0 && _drain_enable == 0 && 
					 _get_clothes_function == 0 && _have_fault == 0 && input_door_switch
						&& _enable_single_wash == 0)
				 {
						 if(_manual_mode == 0)
						 {
								 _manual_mode = 1;
								 auto_mode_s = 0;
								 auto_mode_m = 0;
								 //led_run = 0;
							 LED_RUN = 0;
							 LED_STOP = 1;
							
								 _clear_for_start = 1;
								 set_liquid_height = Code_I.P6[1];
								 door_lock = 1;//门锁
								 if(input_shake){
									 _shake_bit = 0;
									 shake_num = 0; 
								 }
								 _soap_liquid_3 = 0;//皂液3
								 soap_liquid_time = 0;				 
						 }  
						 if(fact_liquid_height < set_liquid_height)
						 {
								 _have_cool_water = ~_have_cool_water;
								 if(_have_cool_water)
								 {
										_add_cool_water_stop = 0;            
								 }               
						 }  
						 else
						 {
								 _have_cool_water = ~_have_cool_water;
								 if(_have_cool_water)
								 {
										_add_cool_water_stop = 0;            
								 }
								 _buzzer_run = 1;	
								 buzzer_num = 3;
//								 break;          
						 }
						 _buzzer_run = 1;	
						 buzzer_num = 1; 
				 } 
				 else{
					 _buzzer_run = 1;	
					 buzzer_num = 3;      
				 }
				 passdisp.disp_flg = 0;
		 }
		 else if(KeyVal==8)//热水 Warm
		 {
//			 	 _enable_display = 1;

					if(run_state && _spin_enable == 0 && _drain_enable == 0 && 
						 _get_clothes_function == 0 && _have_fault == 0 
							&& input_door_switch && _auto_mode_run_stop == 0)
					{//1--运行 0--停止
							 if(_new_step == 0)
							 {
								 if(fact_liquid_height < set_liquid_height)
								 {
										 _have_hot_water = ~_have_hot_water;
										 if(_have_hot_water)
										 {    
												_add_hot_water_stop = 0;
										 }
								 }
								 else
								 {
										 _have_hot_water = ~_have_hot_water;
										 if(_have_hot_water)
										 {    
											 _add_hot_water_stop = 0;
										 }
										 _buzzer_run = 1;	
										 buzzer_num = 3;
//									 break;
								 }           
							 }      
								_buzzer_run = 1;	
								buzzer_num = 1;
					 }
					 else if(run_state == 0 && _spin_enable == 0 && _drain_enable == 0 && 
						 _get_clothes_function == 0 && _have_fault == 0
							&& input_door_switch && _enable_single_wash == 0)
					  {
							 if(_manual_mode == 0)
							 {
								 _manual_mode = 1;
								 auto_mode_s = 0;
								 auto_mode_m = 0;
								 //led_run = 0;
								 LED_RUN = 0;
								 LED_STOP = 1;
								
								 _clear_for_start = 1;
								 set_liquid_height = Code_I.P6[1];
								 door_lock = 1;//门锁
								 if(input_shake){
									 _shake_bit = 0;
									 shake_num = 0; 
								 } 
								 _soap_liquid_3 = 0;//皂液3
								 soap_liquid_time = 0;
												
							 }  
							 if(fact_liquid_height < set_liquid_height)
							 {
								 _have_hot_water = ~_have_hot_water;
								 if(_have_hot_water)
								 {    
									_add_hot_water_stop = 0;
								 }             
							 }
							 else
							 {
									 _have_hot_water = ~_have_hot_water;
									 if(_have_hot_water)
									 {    
										   _add_hot_water_stop = 0;
									 }
									 _buzzer_run = 1;	
									 buzzer_num = 3;
//									 break;       
							 }
							 _buzzer_run = 1;	
							 buzzer_num = 1; 
						 } 
						 else
						 {
							 _buzzer_run = 1;	
							 buzzer_num = 3;       
						 }
						 passdisp.disp_flg = 0;
		 }
		 else if(KeyVal==13)//加热 Heat
		 {
//			 _enable_display = 1;

				 if(run_state && _spin_enable == 0 && _drain_enable == 0 && 
					 _get_clothes_function == 0 && _have_fault == 0 
							&& input_door_switch && _auto_mode_run_stop == 0)
				 {
						 if(_new_step == 0){    
							 _have_calefaction = ~_have_calefaction;                
						 }      
						 _buzzer_run = 1;	
						 buzzer_num = 1;
				 }
				 else if(run_state == 0 && _spin_enable == 0 && _drain_enable == 0 && 
					 _get_clothes_function == 0 && _have_fault == 0 && input_door_switch 
						&& _enable_single_wash == 0)
				 {
						 if(_manual_mode == 0)
						 {
							 _manual_mode = 1;
							 auto_mode_s = 0;
							 auto_mode_m = 0;
							 //led_run = 0;
							 LED_RUN = 0;
							 LED_STOP = 1;
							
							 _clear_for_start = 1;
							door_lock = 1;//门锁
							 if(input_shake)
							 {
									_shake_bit = 0;
									shake_num = 0; 
							 } 
							 _soap_liquid_3 = 0;//皂液3
							 soap_liquid_time = 0;
											
						 }
						 _have_calefaction = ~_have_calefaction;
						 _buzzer_run = 1;	
						 buzzer_num = 1; 
				 } 
				 else
				 {
					 _buzzer_run = 1;	
					 buzzer_num = 3;       
				 }
				 passdisp.disp_flg = 0;
		 }
		 else if(KeyVal==1) //皂液 Soap
		 {

//			 L_Soap_ON;
//			 L_Soap_OFF;
				 if(_spin_enable == 0 && _drain_enable == 0 && _get_clothes_function == 0 && 
					 _have_fault == 0 && input_door_switch && _auto_mode_run_stop == 0 &&
					 _enable_single_wash == 0)
				 {
						 if(run_state){
							 if(_new_step == 0){
								 if(drain){//排水
									 _soap_liquid_1 = 0;
									 _soap_liquid_2 = 0;
									 _soap_liquid_3 = 0;
									 _soap_enable = 0;
									 soap_liquid_num = 0;
									 soap_liquid_time = 0;                          
									 _buzzer_run = 1;	
									 buzzer_num = 3;
//									 break;
								 }
							 }
						 }
						 else if(_manual_mode == 0){//手动模式
							 _manual_mode = 1;
							 auto_mode_s = 0;
							 auto_mode_m = 0;
							// led_run = 0;
							 LED_RUN = 0;
							 LED_STOP = 1;
							
							 _clear_for_start = 1;
							 door_lock = 1;//门锁
							 if(input_shake){
								 _shake_bit = 0;
								 shake_num = 0; 
							 }
							 if(_enable_single_wash){//排水
								 _soap_liquid_1 = 0;
								 _soap_liquid_2 = 0;
								 _soap_liquid_3 = 0;
								 _soap_enable = 0;
								 soap_liquid_num = 0;
								 soap_liquid_time = 0;  
								 _buzzer_run = 1;	
								 buzzer_num = 3;
//								 break;
							 } 
						 }
						 if(_soap_liquid_2 == 0 || _soap_liquid_3 == 0)
						 {
							 _soap_liquid_1 = ~_soap_liquid_1;//皂液1
							 if(_soap_liquid_1)
							 {
									soap_liquid_time = Code_I.P7[1] + 1;//加皂液时间
							 } 
							 else
							 {
									soap_liquid_time = 0;  
							 }
						 } 
						 else
						 {
							 _soap_liquid_1 = 0; 
							 _soap_liquid_2 = 0;
							 _soap_liquid_3 = 0;
							 soap_liquid_time = 0;//加皂液时间
						 }   
						 _buzzer_run = 1;	
						 buzzer_num = 1;
				 }     
				 else
				 {       
					 _buzzer_run = 1;	
					 buzzer_num = 3;       
				 } 
					passdisp.disp_flg = 0;				 
		 }
		 else if(KeyVal==5) //排水 Drain
		 {
//			 			 _enable_display = 1;

//			 L_Drain_ON;
//			 L_Drain_OFF;
					if(run_state && _spin_enable == 0 && _get_clothes_function == 0 && 
					 _have_fault == 0 && input_door_switch && _auto_mode_run_stop == 0){
					 if(_new_step == 0)
						 {
								_drain = ~_drain;        
					 }
					 if(_drain)
					 {
							 _drain_enable = 1;
							 _have_cool_water = 0; //冷水
							 _have_hot_water = 0;  //热水
							 _have_calefaction = 0;//加热
							 _soap_enable = 0;
							 _soap_liquid_1 = 0;//皂液1
							 _soap_liquid_2 = 0;//皂液2
							 _soap_liquid_3 = 0;//皂液3 
							 soap_liquid_time = 0;        
					 } 
					 else
					 {
							_drain_enable = 0;        
					 }
					 _buzzer_run = 1;	
					 buzzer_num = 1;
				 }
				 else if(run_state == 0 && _spin_enable == 0 && _get_clothes_function == 0 && 
					 _have_fault == 0 && input_door_switch)
				 {
					 if(_manual_mode == 0)
					 {
						 _manual_mode = 1;
						 auto_mode_s = 0;
						 auto_mode_m = 0;
						// led_run = 0;
						 LED_RUN = 0;
						 LED_STOP = 1;
					
						 _clear_for_start = 1;
						 door_lock = 1;//门锁
						 if(input_shake){
							 _shake_bit = 0;
							 shake_num = 0; 
						 }      
					 }
					 if(_drain_enable && wash_1 == 0 && wash_2 == 0 && 
						 single_wash_have_water_time == 0 && single_wash_drain_time == 0)
					 {
							 _drain_enable = 0;
							 drain = 0;
							 _drain = 0;
							 _enable_single_wash = 0;
							 _had_single_way_wash = 0;
							 _single_wash_dis = 0;
							 _single_wash_txd = 0;
					 } 
					 else
					 {
							 _enable_single_wash = ~_enable_single_wash; 
							 if(_enable_single_wash)
							 {
									 drain = 0;		
									 _have_cool_water = 0; //冷水
									 _have_hot_water = 0;  //热水
									 _have_calefaction = 0;//加热
									 _soap_enable = 0;
									 _soap_liquid_1 = 0;//皂液1
									 _soap_liquid_2 = 0;//皂液2
									 _soap_liquid_3 = 0;//皂液3
									 soap_liquid_time = 0;
									 //均布
									 _had_single_way_wash = 0;
									 _single_wash_dis = 0;
									 _single_wash_txd = 0;
									 _wait_trans = 0;
									 trans_num = 3;    
									 single_wash_have_water_time = Code_I.P9[2]; //均步带水时间
									 single_wash_drain_time = Code_I.P9[3];      //均步排水时间 
									 //洗涤
									 _have_wash = 0;
									 _had_wash = 0;
									 _wash_txd = 0;
									 _enable_wash = 0;
									 wash_1 = 0;
									 wash_2 = 0;
									 wash_run_time = 0;
									 wash_stop_time = 0;             
							 } 
							 else
							{
								 _drain_enable = 0;
								 _single_wash_dis = 0;
								 _single_wash_txd = 0;
								 if(_had_single_way_wash)
								 {//single_wash_have_water_time || single_wash_drain_time){
									 single_wash_complete_delay_time = Code_I.P10[1];//均步结束延时
									 single_wash_complete_skid = Code_I.P10[4];//均步结束刹车
									 _single_way_wash_stop_pro = 1;
								 }
								 _had_single_way_wash = 0;
								 wash_1 = 0;
								 wash_2 = 0;           
						}
					}
					 _buzzer_run = 1;	
					 buzzer_num = 1; 
				 }      
				 else{
					 _buzzer_run = 1;	
					 buzzer_num = 3;       
				 }
				 passdisp.disp_flg = 0;
		 }
		 else if(KeyVal==9) //脱水 Spin
		 {
//			 			 _enable_display = 1;

//			L_Spin_ON;
//			 L_Spin_OFF;
					if(run_state && _get_clothes_function == 0 && _have_fault == 0 
						&& input_door_switch && _auto_mode_run_stop == 0)
					{//自动模式
						 if(_new_step == 0)
						 {//新步
							 if(_had_single_way_wash)
							 {   
								 single_wash_complete_delay_time = Code_I.P10[1];//均步结束延时
								 single_wash_complete_skid = Code_I.P10[4];//均步结束刹车
								 _single_way_wash_stop_pro = 1;//停止均步的结束刹车处理 
								 wash_1 = 0;
								 wash_2 = 0;      
							 }
							 _had_single_way_wash = 0;//已经有均布
							 _single_wash_dis = 0;//显示均布
							 _enable_single_wash = 0;//是否有均布
							 _single_wash_txd = 0;//均布时通讯
							 if(dehydration_num == 0)
							 {
								 //_enable_dehydration = 1;//允许脱水
								 _drain = 1;
								 _spin_enable = 1;
								 dehydration_num = 'Z';       
								 _middle_dehydration_txd = 0;
								 dehydration_provide_timer = Code_I.P9[0];//脱水开始前的准备时间
								 middle_dehydration_max_run_timer = Code_I.P9[5] * 60;//中脱最长不超过的运行时间   
								 middle_dehydration_settle_timer = Code_I.P9[4] + 30;//中脱和高脱之间的停留时间             
								 //middle_dehydration_stop_wait_timer = 0;//中脱程序结束等待时间
								 //middle_dehydration_stop_skid_time = 0;//中脱结束刹车时间 
								 //high_dehydration_max_run_timer = 0;//高脱最长不超过的运行时间       
								 //high_dehydration_stop_wait_timer = 0;//高脱程序结束等待时间  
								 //high_dehydration_stop_skid_time = 0;//高脱结束刹车时间
								 trans_num = 3;
								 _wait_trans = 0;     
							 }
							 else if(dehydration_num == 'Z'){
								 //_enable_dehydration = 1;                    
								 _drain = 1;
								 _spin_enable = 1;
								 dehydration_num = 'G';           
								 _high_dehydration_txd = 0;
								 //middle_dehydration_stop_wait_timer = 0;//中脱程序结束等待时间
								 //middle_dehydration_stop_skid_time = 0;//中脱结束刹车时间  
								 high_dehydration_max_run_timer = Code_I.P9[6] * 60;//高脱最长不超过的运行时间       
								 //high_dehydration_stop_wait_timer = 0;//高脱程序结束等待时间  
								 //high_dehydration_stop_skid_time = 0;//高脱结束刹车时间
								 trans_num = 3;
								 _wait_trans = 0;               
							 } 
							 else
							{
								 //_enable_dehydration = 0;
								 wash_1 = 0;
								 wash_2 = 0;
								 run_frequency = 0;
								 acce_time = 0;
								 dece_time = 0;
								 high_dehydration_stop_wait_timer = Code_I.P10[3];//高脱程序结束等待时间 
								 high_dehydration_stop_skid_time = Code_I.P10[6] + 5;//高脱结束刹车时间
								 _dehydration_stop_pro = 1;//允许脱水处理 
								 _middle_dehydration_txd = 0;
								 _high_dehydration_txd = 0;  
								 _drain = 0;
								 _spin_enable = 0;
								 dehydration_num = 0;        
							 }
						 }
						 if(_spin_enable){//_enable_dehydration                                 
							 _have_wash = 0;
							 _had_wash = 0;
							 _wash_txd = 0;
							 _enable_wash = 0;
							 _have_cool_water = 0;   //冷水
							 _have_hot_water = 0;    //热水
							 _have_calefaction = 0;  //加热
							 _soap_liquid_1 = 0;//皂液1
							 _soap_liquid_2 = 0;//皂液2
							 _soap_liquid_3 = 0;//皂液3
							 soap_liquid_time = 0;
							 _soap_enable = 0;
							 soap_liquid_num = 0;
							 _drain_enable = 0;
							 _drain = 1;
						 }
						 _buzzer_run = 1;	
						 buzzer_num = 1;
					 }
					 else if(run_state == 0 && _get_clothes_function == 0 && _have_fault == 0 && input_door_switch)
					 {//手动模式
						 if(_manual_mode == 0){ 
							 _manual_mode = 1;
							 auto_mode_s = 0;
							 auto_mode_m = 0;
							// led_run = 0;
							 LED_RUN = 0;
							 LED_STOP = 1;
							
							 _clear_for_start = 1;
							 door_lock = 1;//门锁
							 if(input_shake){
								 _shake_bit = 0;
								 shake_num = 0; 
							 }        
						 }
						 if(dehydration_num == 0){
							 _drain = 1;
							 _spin_enable = 1;
							 dehydration_num = 'Z';         
							 _middle_dehydration_txd = 0;
							 dehydration_provide_timer = Code_I.P9[0];//脱水开始前的准备时间
							 middle_dehydration_max_run_timer = Code_I.P9[5] * 60;//中脱最长不超过的运行时间   
							 middle_dehydration_settle_timer = Code_I.P9[4] + 30;//中脱和高脱之间的停留时间                       
							 //middle_dehydration_stop_wait_timer = 0;//中脱程序结束等待时间
							 //middle_dehydration_stop_skid_time = 0;//中脱结束刹车时间 
							 //high_dehydration_max_run_timer = 0;//高脱最长不超过的运行时间       
							 //high_dehydration_stop_wait_timer = 0;//高脱程序结束等待时间  
							 //high_dehydration_stop_skid_time = 0;//高脱结束刹车时间
							 trans_num = 3;
							 _wait_trans = 0;
							 if(_had_single_way_wash){
								 single_wash_complete_delay_time = Code_I.P10[1];//均步结束延时
								 single_wash_complete_skid = Code_I.P10[4];//均步结束刹车
								 _single_way_wash_stop_pro = 1;
								 wash_1 = 0;
								 wash_2 = 0;
							 } 
						 }
						 else if(dehydration_num == 'Z'){   
							 _drain = 1;
							 _spin_enable = 1;
							 dehydration_num = 'G';       
							 _high_dehydration_txd = 0;
							 //middle_dehydration_stop_wait_timer = 0;//中脱程序结束等待时间
							 //middle_dehydration_stop_skid_time = 0;//中脱结束刹车时间  
							 high_dehydration_max_run_timer = Code_I.P9[6] * 60;//高脱最长不超过的运行时间                        
							 //high_dehydration_stop_wait_timer = 0;//高脱程序结束等待时间  
							 //high_dehydration_stop_skid_time = 0;//高脱结束刹车时间 
							 trans_num = 3;
							 _wait_trans = 0;
						 } 
						 else
						 {
							 wash_1 = 0;
							 wash_2 = 0;
							 run_frequency = 0;
							 acce_time = 0;
							 dece_time = 0;
							 high_dehydration_stop_wait_timer = Code_I.P10[3];//高脱程序结束等待时间 
							 high_dehydration_stop_skid_time = Code_I.P10[6] + 5;//高脱结束刹车时间
							 _dehydration_stop_pro = 1;//允许脱水处理
							 _middle_dehydration_txd = 0;
							 _high_dehydration_txd = 0;
							 _drain = 0;
							 _spin_enable = 0;
							 dehydration_num = 0;        
						 }
						 if(_spin_enable){//_enable_dehydration                            
							 _have_wash = 0;
							 _had_wash = 0;
							 _wash_txd = 0;
							 _enable_wash = 0;
							 _have_cool_water = 0;   //冷水
							 _have_hot_water = 0;    //热水
							 _have_calefaction = 0;  //加热
							 _soap_liquid_1 = 0;//皂液1
							 _soap_liquid_2 = 0;//皂液2
							 _soap_liquid_3 = 0;//皂液3
							 soap_liquid_time = 0;
							 _soap_enable = 0;
							 soap_liquid_num = 0;
							 _drain_enable = 0;
							 _drain = 1;
							 drain = 1;
							 
					
							 _enable_single_wash = 0;
							 _had_single_way_wash = 0;
							 _single_wash_dis = 0;
							 _single_wash_txd = 0;
							 //single_wash_complete_delay_time = 0;//均步结束延时
							 //single_wash_complete_skid = 0;//均步结束刹车
							 //_single_way_wash_stop_pro = 0;
						 }
						 _buzzer_run = 1;	
						 buzzer_num = 1; 
					 } 
					 else{
						 _buzzer_run = 1;	
						 buzzer_num = 3;       
					 }
				passdisp.disp_flg = 0;					 
		 }
		 else if(KeyVal==15) //取衣
		 {
				if(run_state == 0 && _manual_mode == 0 && _get_clothes_function == 0 && 
					_have_fault == 0 &&  Code_I.P2[3] == 'y' && input_door_switch)
				{              
					 door_lock = 1;        
					 deasil_run_time = Code_I.P11[1]+1;
					 deasil_stop_time = Code_I.P11[2];
					 anticlockwise_run_time = Code_I.P11[3];
					 anticlockwise_stop_time = Code_I.P11[4];
					 _get_clothes_function = 1;
					 
					 get_clothes_provide_timer = Code_I.P11[0];
					 _enable_commu_Inventer = 1;//azalea__
					 run_frequency = (unsigned int)Code_I.P14[7]*100;//5Hz
					 acce_time = 300;//30s
					 dece_time = 100;//10s 
					 _buzzer_run = 1;	
					 buzzer_num = 1;
					 LED_RUN = 0;
					 LED_STOP = 1;
     } 
     else
		{
			_buzzer_run = 1;	
		    buzzer_num = 3;      
     }     
		 }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_menu_first_page_handler(void)//dismod1
{
	int i;
			if(KeyVal==Return)
			{
				 Menu = MENUCUR0;   
				 DispMod = dismod0;
				 dismenu.sub_menucur = 0xFF;
		     dismenu.sub_chosecur = 0; 
					_buzzer_run = 1;      //蜂鸣器运行标志	
					buzzer_num = 1;       //蜂鸣器响的次数				
			}
			else if(KeyVal==UP)
			{
				 dismenu.sub_chosecur = dismenu.sub_chosecur - 1;
		     if(dismenu.sub_chosecur > lcd_maxcur)
				 {
						dismenu.sub_chosecur = lcd_maxcur;
		     }			 
					if(_num_4 == 0)
					{
						 _num_4 = 1;
						 _buzzer_run = 1;      //蜂鸣器运行标志	
						 buzzer_num = 1;       //蜂鸣器响的次数
					} 
			}
			else if(KeyVal==Down)
			{
				 dismenu.sub_chosecur = dismenu.sub_chosecur + 1;
		     if(dismenu.sub_chosecur > lcd_maxcur)
				 {
			      dismenu.sub_chosecur = 0;
					}	
					if(_num_4 == 0){
						 _num_4 = 1;
						 _buzzer_run = 1;      //蜂鸣器运行标志	
						 buzzer_num = 1;       //蜂鸣器响的次数
					 }				 
					 
			}
			else if(KeyVal==Setup)
			{
					if(dismenu.sub_chosecur == 1)//选择修改方式
					{
							dismenu.dis_mode = 1; 
							Menu = MENUCUR2;      //设置密码界面
							DispMod = dismod2;
						  for(i=0;i<6;i++)
							{
									passportdata[i] = 0;
							}
							
							 _buzzer_run = 1;      //蜂鸣器运行标志	
							 buzzer_num = 1;        //蜂鸣器响的次数
						//  passdisp.key_data = 0;		// azalea modify
							for(i = 0; i < 6; i++)
							{
								passdisp.dispdata[i] = 0;
							}
					}
					else//选择查看方式
					{
					   dismenu.dis_mode = 0;		
					   Menu = MENUCUR3;//功能列表界面
					   DispMod = dismod3;
					}	
					dismenu.sub_menucur = 0x01;
					dismenu.sub_chosecur = 0;    // 
			}
			else
			{
					 if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
						 _num_1 = 1;
						 _num_2 = 1;
						 _num_3 = 1;
						 _num_4 = 1;
						 _buzzer_run = 1;      //蜂鸣器运行标志	
						 buzzer_num = 3;       //蜂鸣器响的次数
					 } 
			}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_menu_second_page_handler(void)//dismod2
{
	u8 i;
	DispMod = dismod2;
	if(KeyVal==Return)
	{
		Menu = MENUCUR1; //
		DispMod = dismod1;
		dismenu.sub_menucur = 0x01;
		dismenu.sub_chosecur = 0;    //????	
	    _buzzer_run = 1;      //蜂鸣器运行标志	
		buzzer_num = 1;       //蜂鸣器响的次数
	}
	else if(KeyVal==UP)
	{
		passportdata[passdisp.dispnum] = passportdata[passdisp.dispnum] < 9 ? ++passportdata[passdisp.dispnum] : 0;
		passdisp.dispdata[passdisp.dispnum] = passportdata[passdisp.dispnum] ;
		if(_num_4 == 0)
		{
			 _num_4 = 1;
			 _buzzer_run = 1;      //蜂鸣器运行标志	
			 buzzer_num = 1;       //蜂鸣器响的次数
		}
	}
	else if(KeyVal==Down)
	{
		passportdata[passdisp.dispnum] = passportdata[passdisp.dispnum] > 0 ? --passportdata[passdisp.dispnum] : 9;
		passdisp.dispdata[passdisp.dispnum] = passportdata[passdisp.dispnum] ;
		if(_num_4 == 0)
		{
			_num_4 = 1;
			_buzzer_run = 1;      //蜂鸣器运行标志	
			buzzer_num = 1;       //蜂鸣器响的次数
		}
	
	}
	else if(KeyVal==Setup)
	{

		 if((passportdata[0]==passportdata_buffer[0])&&(passportdata[1]==passportdata_buffer[1])&&
				 (passportdata[2]==passportdata_buffer[2])&&(passportdata[3]==passportdata_buffer[3])&&
				 (passportdata[4]==passportdata_buffer[4])&&(passportdata[5]==passportdata_buffer[5]))
		 {
				 dismenu.dis_mode = 1;//1--修改方式；0--查询方式
				 Menu = MENUCUR3; //功能列表界面 
				 DispMod = dismod3;

				 dismenu.sub_menucur = 0x01;
				 dismenu.sub_chosecur = 0;    //????	 								 
		 }
		 else if((passportdata[0]==6)&&(passportdata[1]==0)&&(passportdata[2]==3)&&//密码为060309
			 (passportdata[3]==0)&&(passportdata[4]==9)&&(passportdata[5]==0))
		 {
				 for(i=0;i<6;i++)
				 {   
					 unsigned char *P=(unsigned char *)passportdata_buffer;
					 passportdata[i] = 2;//设置后的密码是222222
					 passportdata_buffer[i]=2;
					 passdisp.dispdata[i] = 2;
					 AT24CXX_WriteOneByte(ADD_PSWD_USER+i,*P++);//保存密码
				 }
				 dismenu.dis_mode = 1;//1--修改方式；0--查询方式
				 Menu = MENUCUR3;  //功能列表界面  
				 DispMod = dismod3;		 
				 dismenu.sub_menucur = 0x01;
				 dismenu.sub_chosecur = 0;    //????	 
				 _buzzer_run = 1;      //蜂鸣器运行标志	
				 buzzer_num = 1;        //蜂鸣器响的次数
		 }
		 else
		 {
				_buzzer_run = 1;      //蜂鸣器运行标志	
				buzzer_num = 3;       //蜂鸣器响的次数 
		 }
									
	}
	else if(KeyVal==Remove)
	{

		 passdisp.dispnum = passdisp.dispnum<5?++passdisp.dispnum:0;//密码移位
		 _buzzer_run = 1;	
		 buzzer_num = 1;
	}
	else
	{
		 if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0))
		 {
			   _num_1 = 1;
			   _num_2 = 1;
			   _num_3 = 1;
			   _num_4 = 1;
			   _buzzer_run = 1;      //蜂鸣器运行标志	
			   buzzer_num = 3;       //蜂鸣器响的次数
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_main_set_menu_handler(void)//dismod3
{
		  if(KeyVal==Return)
			{ 
				 if(dismenu.dis_mode)//修改方式  
				 {
						 Menu = MENUCUR2;      //返回密码设置界面
						 DispMod = dismod2;
					}
					else
					{
							Menu = MENUCUR1; //返回选择界面
							DispMod = dismod1;
					 }
					 dismenu.sub_menucur = 0x00;
					 dismenu.sub_chosecur = 0;    //????	
					  _buzzer_run = 1;      //蜂鸣器运行标志	
		 buzzer_num = 1;       //蜂鸣器响的次数
			
			}
			else if(KeyVal==UP)
			{
				 dismenu.sub_chosecur = dismenu.sub_chosecur - 1;
				 if(dismenu.sub_chosecur > lcd_maxcur || dismenu.sub_chosecur == 0)
				 {
						dismenu.sub_chosecur = 0;
				 }
					if(_num_4 == 0){
						 _num_4 = 1;
						 _buzzer_run = 1;      //蜂鸣器运行标志	
						 buzzer_num = 1;       //蜂鸣器响的次数
					 }				 
			}
			else if(KeyVal==Down)
			{
				 dismenu.sub_chosecur = dismenu.sub_chosecur + 1;
				 if(dismenu.sub_chosecur > lcd_maxcur)
				 {
						dismenu.sub_chosecur = lcd_maxcur;
				 }
				  if(_num_4 == 0){
					 _num_4 = 1;
					 _buzzer_run = 1;      //蜂鸣器运行标志	
					 buzzer_num = 1;       //蜂鸣器响的次数
				 }
			
			}
			else if(KeyVal==Setup)
			{
					Menu = MENUCUR4;      //各功能界面
					DispMod = dismod4;
					dismenu.sub_chosecurbuf = dismenu.sub_chosecur; 
					dismenu.sub_menucur = dismenu.sub_chosecur+1;
					dismenu.sub_chosecur = 0;    //????
					_new_page = 1;//????   azalea???
				 _buzzer_run = 1;      //蜂鸣器运行标志	
		 buzzer_num = 1;        //蜂鸣器响的次数
			}
			else
			{
				 if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
		   _num_1 = 1;
		   _num_2 = 1;
		   _num_3 = 1;
		   _num_4 = 1;
		   _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
	   }
			}
			 KeyVal = 0;  

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_lcd_menu_one_handler(void)//dismod4_1
{
		u8 i;
//		unsigned char *j,*z;
//	  unsigned char a[200];
	  
		 if(KeyVal==Return)
		 {
					if(dismenu.data_mode)//
					{
							dismenu.data_mode = 0;
						 for(i=0;i<6;i++) 
							{
									passportdata[i] = passportdata_buffer[i];
							}
								//	passdisp.dispnum = 0;	
					}     
					else
					{	                        
							Menu = MENUCUR3;   //功能列表界面
							DispMod = dismod3;
							dismenu.sub_menucur = 0x01;
							dismenu.sub_chosecur = 0;    //	
					}
					_buzzer_run = 1;      //蜂鸣器运行标志	
		 buzzer_num = 1;       //蜂鸣器响的次数
					passdisp.dispnum = 0;//光标停在密码设置界面的第一个位置
				 
			 }
				else if(KeyVal==UP)
				{
					 if(dismenu.data_mode && dismenu.sub_chosecur == 3)//下一级菜单中第4个
						 {
					 
									Code_I.P1[3] = (Code_I.P1[3] < 100)?(++Code_I.P1[3]):100;//机器容量
						 } 
					 else if(dismenu.data_mode && dismenu.sub_chosecur == 5)//密码设置
						 {
									passportdata[passdisp.dispnum] = passportdata[passdisp.dispnum]<9?++passportdata[passdisp.dispnum]:0;																 
									passdisp.dispdata[passdisp.dispnum] = passportdata[passdisp.dispnum];
						 }

					 else
					 {
								dismenu.sub_chosecur = dismenu.sub_chosecur - 1;
					 if(dismenu.sub_chosecur > lcd_maxcur || dismenu.sub_chosecur == 0)
						{
								dismenu.sub_chosecur = 0;
					 }
					 }

				}
				else if(KeyVal==Down)
				{
						 if(dismenu.data_mode && dismenu.sub_chosecur == 3)
						 {
						 
									Code_I.P1[3] = (Code_I.P1[3] >0)?(--Code_I.P1[3]):0;
						 } 
						 else if(dismenu.data_mode && dismenu.sub_chosecur == 5)
						{
									passportdata[passdisp.dispnum] = (passportdata[passdisp.dispnum]>0)?--passportdata[passdisp.dispnum]:9;
									passdisp.dispdata[passdisp.dispnum] = passportdata[passdisp.dispnum];
						}
						else if(dismenu.data_mode &&( dismenu.sub_chosecur == 0 || dismenu.sub_chosecur == 1 || dismenu.sub_chosecur == 2 || dismenu.sub_chosecur == 4))
						{
								  _buzzer_run = 1;	
									buzzer_num = 3;
						}
						else
					 {
			
						 dismenu.sub_chosecur = dismenu.sub_chosecur + 1;
						 if(dismenu.sub_chosecur > lcd_maxcur)
						 {
								dismenu.sub_chosecur = lcd_maxcur;
						 }
					 }
					 
					// if(keyval != '' && _num_4 == 0)
					{
						 _num_4 = 1;
						 _buzzer_run = 1;	
						 buzzer_num = 1;
					}

				}
				else if(KeyVal==Setup)
				{
					if(dismenu.dis_mode && dismenu.data_mode == 0)//修改模式下，不在设置数据
					{
						if(dismenu.sub_chosecur == 2 || dismenu.sub_chosecur == 4) //电机驱动选择与语言设置跳过
						{
							 _buzzer_run = 1;
								buzzer_num = 3;
								//break;
						}
				 
						dismenu.data_mode = 1;
						passdisp.dispnum = 0;
					  _buzzer_run = 1;	
		        buzzer_num = 1;

					}
					else if(dismenu.dis_mode && dismenu.data_mode)//修改模式，设置数据 P01_key
					{
							dismenu.data_mode = 0;
							if(dismenu.sub_chosecur == 0) // azalea_wait_for_modify  P01_初始化参数
							{
									lcd_wrstring(0,18,"运行中...");
   								RecoverFactoryPr();//恢复出厂参数	
								  delay(60000);
								  delay(60000);								
								  _new_page = 1;
								  lcd_wrstring(0,18,"          ");
							}
							else if(dismenu.sub_chosecur == 1)// azalea_wait_for_modify  P01_初始化程序
							{
									 //lcd_wrstring(1,18,"运行中....");
									//程序1
									 lcd_wrstring(1,18,"运行中....");
								  	Init_Procedure();
										delay(6000);
										//delay(60000);
									 lcd_wrstring(1,18,"          ");  	
							
							}
							else if(dismenu.sub_chosecur == 5)
							{
								 unsigned char *P=(unsigned char *)passportdata;
								 for(i=0;i<6;i++)
								 {
									 AT24CXX_WriteOneByte(ADD_PSWD_USER+i,*P++);
									 passportdata_buffer[i] = passportdata[i];
								 }
							
							}
							else
							{
									 AT24CXX_WriteOneByte(P01_PARAMETER_E2+3,Code_I.P1[3]);
							
							}
							 _buzzer_run = 1;	
								buzzer_num = 1;

					}
					
					else
					{
						_buzzer_run = 1;	
						buzzer_num = 3;
					}
				}
				else if(KeyVal==Remove)
				{
						if(dismenu.data_mode && dismenu.sub_chosecur == 5)
						{
								passdisp.dispnum = passdisp.dispnum<5?++passdisp.dispnum:0;
								_buzzer_run = 1;	
								buzzer_num = 1;
						}
						else
						{
						  _buzzer_run = 1;	
							buzzer_num = 3;
						}
				}
				else
				{
				 if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
					 _num_1 = 1;
					 _num_2 = 1;
					 _num_3 = 1;
					 _num_4 = 1;
					 _buzzer_run = 1;      //蜂鸣器运行标志	
					 buzzer_num = 3;       //蜂鸣器响的次数
				 }
				}
				 KeyVal = 0;
//				break;
				
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_lcd_menu_two_handler(void)//dismod4_2
{
		u8 i;
	//unsigned char *k,*j;
	//j = mode_data;
	// k = mode_data_buffer;
		    if(KeyVal==Return)
				{
					if(dismenu.data_mode)
					{
							dismenu.data_mode = 0;
					}
					else
					{														
							Menu = MENUCUR3;  //???????
							DispMod = dismod3;
							dismenu.sub_menucur = 0x01;
							dismenu.sub_chosecur = dismenu.sub_chosecurbuf;
					}
					 _buzzer_run = 1;      //蜂鸣器运行标志	
					buzzer_num = 1;       //蜂鸣器响的次数
	
				 }
				else if(KeyVal==UP)
				{
						if(dismenu.data_mode)
						{ 
								Code_I.P2[dismenu.sub_chosecur] = (Code_I.P2[dismenu.sub_chosecur] == 'n')?'y':'n';
						}
						else
						{
								dismenu.sub_chosecur = dismenu.sub_chosecur - 1;
								if(dismenu.sub_chosecur > lcd_maxcur || dismenu.sub_chosecur == 0)
								{
										dismenu.sub_chosecur = 0;
								}
						}
						if(_num_4 == 0){
								_num_4 = 1;
								_buzzer_run = 1;	
								buzzer_num = 1;
							}

				}
				else if(KeyVal==Down)
				{
					 if(dismenu.data_mode)
						{ 
								Code_I.P2[dismenu.sub_chosecur] = (Code_I.P2[dismenu.sub_chosecur] == 'n')?'y':'n';
						}
						else
						{
								 dismenu.sub_chosecur = dismenu.sub_chosecur + 1;
								 if(dismenu.sub_chosecur > lcd_maxcur)
								 {
										dismenu.sub_chosecur = lcd_maxcur;
								 }
					  }
						
						 if(_num_4 == 0){
								 _num_4 = 1;
								 _buzzer_run = 1;	
								 buzzer_num = 1;
							 }

				}
				else if(KeyVal==Setup)
				{
						if(dismenu.dis_mode && dismenu.data_mode == 0 && dismenu.sub_chosecur != 4)
						{   
								dismenu.data_mode = 1;
							 _buzzer_run = 1;
								buzzer_num = 1;
						}
						else if(dismenu.dis_mode && dismenu.data_mode)
						{
								unsigned char *P=(unsigned char *)Code_I.P2;
								dismenu.data_mode = 0;
								for(i=0;i<4;i++)
								{
										AT24CXX_WriteOneByte(P01_PARAMETER_E2+8+i,*P++);
								}
					_buzzer_run = 1;
		   buzzer_num = 1;
						} 
						else
						{
_buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
						}

				}			
				else
				{
					if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
						 _num_1 = 1;
						 _num_2 = 1;
						 _num_3 = 1;
						 _num_4 = 1;
						 _buzzer_run = 1;      //蜂鸣器运行标志	
						 buzzer_num = 3;       //蜂鸣器响的次数
					 }
				}
				KeyVal = 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_lcd_menu_three_handler(void)//dismod4_3
{
		u8 i;
	//unsigned char *k,*j;
	//j = mode_data;
	// k = mode_data_buffer;
		if(KeyVal==Return)
				{
					if(dismenu.data_mode)
					{
							dismenu.data_mode = 0;
					}
					else
					{
							Menu = MENUCUR3;   //???????
							DispMod = dismod3;
							dismenu.sub_menucur = 0x01;
							dismenu.sub_chosecur = dismenu.sub_chosecurbuf;    //????	
					}
					 _buzzer_run = 1;      //蜂鸣器运行标志	
						buzzer_num = 1;       //蜂鸣器响的次数
	
				 }
				else if(KeyVal==UP)//与说明书比较，少了自动补热温差 azalea P03
				{
					if(dismenu.data_mode){
						if(dismenu.sub_chosecur == 0)
						{
								//Code_I.P3[0] = (Code_I.P3[0] == 'n')?'y':'n';
						}
						else if(dismenu.sub_chosecur == 1)
						{
								Code_I.P3[1] = (Code_I.P3[1]<30)?(++Code_I.P3[1]):30;   
						}
						else if(dismenu.sub_chosecur == 2)
						{
								Code_I.P3[2] = (Code_I.P3[2]<99)?(++Code_I.P3[2]):99;
						}
						else
						{
								Code_I.P3[3] = (Code_I.P3[3]<3)?(++Code_I.P3[3]):3;   
						}
					}
					else
					{														
							dismenu.sub_chosecur = dismenu.sub_chosecur - 1;
							if(dismenu.sub_chosecur > lcd_maxcur || dismenu.sub_chosecur == 0)
							{
									dismenu.sub_chosecur = 0;
							}
					}
					if(_num_4 == 0){
						 _num_4 = 1;
						 _buzzer_run = 1;	
						 buzzer_num = 1;
					 }
				}
				else if(KeyVal==Down)
				{
						if(dismenu.data_mode)
						{
								if(dismenu.sub_chosecur == 0)
								{
										//Code_I.P3[0] = (Code_I.P3[0] == 'n')?'y':'n';
								}
								else if(dismenu.sub_chosecur == 1)
								{
										Code_I.P3[1] = (Code_I.P3[1]>1)?(--Code_I.P3[1]):1;   
								}
								else
								{
										Code_I.P3[dismenu.sub_chosecur] = (Code_I.P3[dismenu.sub_chosecur]>0)?(--Code_I.P3[dismenu.sub_chosecur]):0;
								} 
						}
						else
						{														
								dismenu.sub_chosecur = dismenu.sub_chosecur + 1;
								if(dismenu.sub_chosecur > lcd_maxcur)
								{
										dismenu.sub_chosecur = lcd_maxcur;
								}
						}
						if(_num_4 == 0){
							 _num_4 = 1;
							 _buzzer_run = 1;	
							 buzzer_num = 1;
						 }
				}
				else if(KeyVal==Setup)
				{
					 if(dismenu.dis_mode && dismenu.data_mode == 0)
						{   
								if(dismenu.sub_chosecur <= 3 && dismenu.sub_chosecur != 0)
								{		    
										dismenu.data_mode = 1;
										 _buzzer_run = 1;      	
										buzzer_num = 1;
								} 
								else
								{
							 _buzzer_run = 1;      	
		     buzzer_num = 3;
								//break;
								}
						}
						else if(dismenu.dis_mode && dismenu.data_mode)
						{
								unsigned char *P=(unsigned char *)Code_I.P3;
								dismenu.data_mode = 0;
								for(i=0;i<5;i++)
								{
										AT24CXX_WriteOneByte(P01_PARAMETER_E2+16+i,*P++);
								}
					_buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 1;       //蜂鸣器响的次数
						}
						else
						{ 
					 _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
						}

				}
				else
				{
					if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
		   _num_1 = 1;
		   _num_2 = 1;
		   _num_3 = 1;
		   _num_4 = 1;
		   _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
	   }
				}
				KeyVal = 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_lcd_menu_four_handler(void)//dismod4_4
{
		 if(KeyVal==Return)
				{
						Menu = MENUCUR3;   
						DispMod = dismod3;
						dismenu.sub_menucur = 0x01;
						dismenu.sub_chosecur = dismenu.sub_chosecurbuf;    //????	
					 _buzzer_run = 1;      //蜂鸣器运行标志	
						buzzer_num = 1;       //蜂鸣器响的次数
				}
				else if(KeyVal==UP)
				{
				
						dismenu.sub_chosecur = dismenu.sub_chosecur - 1;
						if(dismenu.sub_chosecur > lcd_maxcur || dismenu.sub_chosecur == 0)
						{
								dismenu.sub_chosecur = 0;
						}
						 if(_num_4 == 0){
							 _num_4 = 1;
							 _buzzer_run = 1;	
							 buzzer_num = 1;
						 }

				}
				else if(KeyVal==Down)
				{
						dismenu.sub_chosecur = dismenu.sub_chosecur + 1;
						if(dismenu.sub_chosecur > lcd_maxcur)
						{
								dismenu.sub_chosecur = lcd_maxcur;
						}
						if(_num_4 == 0){
							 _num_4 = 1;
							 _buzzer_run = 1;	
							 buzzer_num = 1;
						 }

				}
				else if(KeyVal==Setup)
				{
_buzzer_run = 1;      //蜂鸣器运行标志	
		 buzzer_num = 3;       //蜂鸣器响的次数

				}
				else
				{
					if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
		   _num_1 = 1;
		   _num_2 = 1;
		   _num_3 = 1;
		   _num_4 = 1;
		   _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
	   }
					
				}
				KeyVal = 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_lcd_menu_five_handler(void)//dismod4_5
{
		 if(KeyVal==Return)
				{
						Menu = MENUCUR3;    //???????
						DispMod = dismod3;
						// dismenu.sub_menucur = 0x01;//??????
						dismenu.sub_menucur = 0x01;
						dismenu.sub_chosecur = dismenu.sub_chosecurbuf;    //????	
	 _buzzer_run = 1;      //蜂鸣器运行标志	
		 buzzer_num = 1;       //蜂鸣器响的次数
				}
				else if(KeyVal==UP)
				{
						 dismenu.sub_chosecur = dismenu.sub_chosecur - 1;
						 if(dismenu.sub_chosecur > lcd_maxcur || dismenu.sub_chosecur == 0)
						 {
									dismenu.sub_chosecur = 0;
						 }
						 if(_num_4 == 0){
       _num_4 = 1;
       _buzzer_run = 1;	
		   buzzer_num = 1;
     }

					}
				else if(KeyVal==Down)
				{
						dismenu.sub_chosecur = dismenu.sub_chosecur + 1;
						if(dismenu.sub_chosecur > lcd_maxcur)
						{
								dismenu.sub_chosecur = lcd_maxcur;
						}
						if(_num_4 == 0){
       _num_4 = 1;
       _buzzer_run = 1;	
		   buzzer_num = 1;
     }

				}
				else if(KeyVal==Setup)
				{

 _buzzer_run = 1;	
		 buzzer_num = 3;
				}
				else
				{
					if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
		   _num_1 = 1;
		   _num_2 = 1;
		   _num_3 = 1;
		   _num_4 = 1;
		   _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
	   }
				}
				KeyVal = 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_lcd_menu_six_handler(void)//dismod4_6
{
		u8 i;
	//unsigned char *k,*j;
	//j = mode_data;
	// k = mode_data_buffer;
				if(KeyVal==Return)
				{
					if(dismenu.data_mode)
					{
							dismenu.data_mode = 0; 	    
					}
					else
					{
						Menu = MENUCUR3;   //???????
						DispMod = dismod3;
						dismenu.sub_menucur = 0x01;//??????
						dismenu.sub_chosecur = dismenu.sub_chosecurbuf;    //????
					}	
 _buzzer_run = 1;      //蜂鸣器运行标志	
		 buzzer_num = 1;       //蜂鸣器响的次数					
	
				 }
				else if(KeyVal==UP)
				{
						if(dismenu.data_mode)
						{
								if((dismenu.sub_chosecur != 3) && (dismenu.sub_chosecur != 4)&& (dismenu.sub_chosecur != 6))
								{
										Code_I.P6[dismenu.sub_chosecur] = (Code_I.P6[dismenu.sub_chosecur]<99)?(++Code_I.P6[dismenu.sub_chosecur]):99;
								}
								else if(dismenu.sub_chosecur == 3)
								{
										Code_I.P6[dismenu.sub_chosecur] = (Code_I.P6[dismenu.sub_chosecur]<50)?(++Code_I.P6[dismenu.sub_chosecur]):50;
								}
								else if(dismenu.sub_chosecur == 4)
								{
										Code_I.P6[dismenu.sub_chosecur] = (Code_I.P6[dismenu.sub_chosecur]<30)?(++Code_I.P6[dismenu.sub_chosecur]):30;
								}
								else
								{
								
								}
						}
						else
						{														
								dismenu.sub_chosecur = dismenu.sub_chosecur - 1;
								if(dismenu.sub_chosecur > lcd_maxcur || dismenu.sub_chosecur == 0)
								{
										dismenu.sub_chosecur = 0;
								}
						}
						 if(_num_4 == 0){
       _num_4 = 1;
       _buzzer_run = 1;	
		   buzzer_num = 1;
     }
				}
				else if(KeyVal==Down)
				{
					if(dismenu.data_mode)
					{
							if(dismenu.sub_chosecur != 6)
							{
									Code_I.P6[dismenu.sub_chosecur] = (Code_I.P6[dismenu.sub_chosecur]>0)?(--Code_I.P6[dismenu.sub_chosecur]):0;	
							}														
					}
					else
					{														
							dismenu.sub_chosecur = dismenu.sub_chosecur + 1;
							if(dismenu.sub_chosecur > lcd_maxcur)
							{
									dismenu.sub_chosecur = lcd_maxcur;
							}
					}
					 if(_num_4 == 0){
       _num_4 = 1;
       _buzzer_run = 1;	
		   buzzer_num = 1; 
     }
				}
				else if(KeyVal==Setup)
				{
					if(dismenu.dis_mode && dismenu.data_mode == 0 && dismenu.sub_chosecur <= 7)
					{   
							dismenu.data_mode = 1;
						  _buzzer_run = 1;      //蜂鸣器运行标志	
							buzzer_num = 1;       //蜂鸣器响的次数
					}
					else if(dismenu.dis_mode && dismenu.data_mode)
					{
						  unsigned char *P = (unsigned char *)Code_I.P6;
							dismenu.data_mode = 0;
							for(i=0;i<8;i++)
							{
									AT24CXX_WriteOneByte(P01_PARAMETER_E2+40+i,*P++);
							}
							 _buzzer_run = 1;      //蜂鸣器运行标志	
							buzzer_num = 1;       //蜂鸣器响的次数
							
					}
					else
					{
					 _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
					}

				}
				else
				{
					 if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
		   _num_1 = 1;
		   _num_2 = 1;
		   _num_3 = 1;
		   _num_4 = 1;
		   _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
	   }
				}
				KeyVal = 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_lcd_menu_seven_handler(void)//dismod4_7
{
	  	if(KeyVal==Return)
			 {
				 if(dismenu.data_mode)
				 {
						dismenu.data_mode = 0;
				 }
				 else
				 {
						 Menu = MENUCUR3;    //
						 DispMod = dismod3;
						 dismenu.sub_menucur = 0x01;//
						 dismenu.sub_chosecur = dismenu.sub_chosecurbuf;    //
				 } 
				  _buzzer_run = 1;      //蜂鸣器运行标志	
		 buzzer_num = 1;       //蜂鸣器响的次数
				 
				}
				else if(KeyVal==UP)
				{
					if(dismenu.data_mode)
					{
						if(dismenu.sub_chosecur == 1)
						{
							 Code_I.P7[1] = (Code_I.P7[1]<240)?(++Code_I.P7[1]):240;
						}
					}
					else
					{
							 dismenu.sub_chosecur = dismenu.sub_chosecur - 1;
							 if(dismenu.sub_chosecur > lcd_maxcur || dismenu.sub_chosecur == 0)
							 {
									dismenu.sub_chosecur = 0;
							 }
					}
					 if(_num_4 == 0){
       _num_4 = 1;
       _buzzer_run = 1;	
		   buzzer_num = 1;
     }

				}
				else if(KeyVal==Down)
				{
					
					 if(dismenu.data_mode)
					{
							if(dismenu.sub_chosecur == 1)
							{
									Code_I.P7[1] = (Code_I.P7[1]>1)?(--Code_I.P7[1]):1;	
							}														
					}
					else
					{														
							dismenu.sub_chosecur = dismenu.sub_chosecur + 1;
							if(dismenu.sub_chosecur > lcd_maxcur)
							{
									dismenu.sub_chosecur = lcd_maxcur;
							}
					}
					 if(_num_4 == 0){
       _num_4 = 1;
       _buzzer_run = 1;	
		   buzzer_num = 1; 
     }

				}
				else if(KeyVal==Setup)
				{
						if(dismenu.dis_mode && dismenu.data_mode == 0 )
						{
								if(dismenu.sub_chosecur == 1)
								{
										dismenu.data_mode = 1;
									  _buzzer_run = 1;      //蜂鸣器运行标志	
										buzzer_num = 1;       //蜂鸣器响的次数
								}
								else 
								{
											_buzzer_run = 1;	
											buzzer_num = 3;
								}
								
						}
						else if(dismenu.dis_mode && dismenu.data_mode)
						{
								dismenu.data_mode = 0;
							 AT24CXX_WriteOneByte(P01_PARAMETER_E2+49,Code_I.P7[1]);// azalea_wait
							 _buzzer_run = 1;      //蜂鸣器运行标志	
								buzzer_num = 1;       //蜂鸣器响的次数
						}
						else
						{
						   _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
						}

				}
				else
				{
							 if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
		   _num_1 = 1;
		   _num_2 = 1;
		   _num_3 = 1;
		   _num_4 = 1;
		   _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
	   }
				}
				KeyVal = 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_lcd_menu_eight_handler(void)//dismod4_8
{
		u8 i;
	//unsigned char *k,*j;
	//j = mode_data;
	// k = mode_data_buffer;
				if(KeyVal==Return)
				{
						if(dismenu.data_mode)
						{
								dismenu.data_mode = 0;
						}
						else
						{
								Menu = MENUCUR3;   //???????
								DispMod = dismod3;
								dismenu.sub_menucur = 0x01;//??????
								dismenu.sub_chosecur = dismenu.sub_chosecurbuf;    //????	
				
						}
						 _buzzer_run = 1;      //蜂鸣器运行标志	
		 buzzer_num = 1;       //蜂鸣器响的次数
					
						
				 }
				else if(KeyVal==UP)
					{
						
						if(dismenu.data_mode)
						{
								if(dismenu.sub_chosecur == 0)
								{
								     Code_I.P8[dismenu.sub_chosecur] = (Code_I.P8[dismenu.sub_chosecur]<99)?(++Code_I.P8[dismenu.sub_chosecur]):99;
								}
								else if(dismenu.sub_chosecur == 1 || dismenu.sub_chosecur == 2)
								{
										 Code_I.P8[dismenu.sub_chosecur] = (Code_I.P8[dismenu.sub_chosecur]<120)?(++Code_I.P8[dismenu.sub_chosecur]):120;
								}
						}
						else
						{
								dismenu.sub_chosecur = dismenu.sub_chosecur - 1;
								if(dismenu.sub_chosecur > lcd_maxcur || dismenu.sub_chosecur == 0)
								{
										dismenu.sub_chosecur = 0;
								}
						}
						if(_num_4 == 0){
       _num_4 = 1;
       _buzzer_run = 1;	
		   buzzer_num = 1;
     }
						
					}
				else if(KeyVal==Down)
				{
					if(dismenu.data_mode)
						{
								if(dismenu.sub_chosecur == 0 || dismenu.sub_chosecur == 1 || dismenu.sub_chosecur == 2)
								{
								     Code_I.P8[dismenu.sub_chosecur] = (Code_I.P8[dismenu.sub_chosecur]>1)?(--Code_I.P8[dismenu.sub_chosecur]):1;
								}
						}
						else
						{
								dismenu.sub_chosecur = dismenu.sub_chosecur + 1;
								if(dismenu.sub_chosecur > lcd_maxcur)
								{
										dismenu.sub_chosecur = lcd_maxcur;
								}
						}
						  if(_num_4 == 0){
       _num_4 = 1;
       _buzzer_run = 1;	
		   buzzer_num = 1;
     }

				}
				else if(KeyVal==Setup)
				{
						if(dismenu.dis_mode && dismenu.data_mode == 0 && dismenu.sub_chosecur <= 2)
							{   
									dismenu.data_mode = 1;
									   _buzzer_run = 1;      //蜂鸣器运行标志	
										buzzer_num = 1;       //蜂鸣器响的次数
							}
							else if(dismenu.dis_mode && dismenu.data_mode)
							{
									unsigned char *P = (unsigned char *)Code_I.P8;
									dismenu.data_mode = 0;
									for(i=0;i<3;i++)
									{
											AT24CXX_WriteOneByte(P01_PARAMETER_E2+56+i,*P++);
									}
									 _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 1;       //蜂鸣器响的次数
									
							}
							else
							{
							_buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
							}

				}
				else
				{
					  if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
		   _num_1 = 1;
		   _num_2 = 1;
		   _num_3 = 1;
		   _num_4 = 1;
		   _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
	   }
				}
				KeyVal = 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_lcd_menu_nine_handler(void)//dismod4_9
{
		u8 i;
	//unsigned char *k,*j;
	//j = mode_data;
	// k = mode_data_buffer;
		if(KeyVal==Return)
				{
					if(dismenu.data_mode)
						{
								dismenu.data_mode = 0;
						}
						else
						{
								Menu = MENUCUR3;  //???????
								DispMod = dismod3;
								dismenu.sub_menucur = 0x01;//??????
								dismenu.sub_chosecur = dismenu.sub_chosecurbuf;    //????	
						}
						_buzzer_run = 1;      //蜂鸣器运行标志	
		 buzzer_num = 1;       //蜂鸣器响的次数
				 }
				else if(KeyVal==UP)
					{
						
						if(dismenu.data_mode)
						{
								if(dismenu.sub_chosecur == 0)
								{
										Code_I.P9[dismenu.sub_chosecur] = (Code_I.P9[dismenu.sub_chosecur]<99)?(++Code_I.P9[dismenu.sub_chosecur]):99;								}
								else if(dismenu.sub_chosecur == 2 || dismenu.sub_chosecur == 3)
								{
										 Code_I.P9[dismenu.sub_chosecur] = (Code_I.P9[dismenu.sub_chosecur]<120)?(++Code_I.P9[dismenu.sub_chosecur]):120;
								}
								else if(dismenu.sub_chosecur == 4)
								{
								     Code_I.P9[dismenu.sub_chosecur] = (Code_I.P9[dismenu.sub_chosecur]<240)?(++Code_I.P9[dismenu.sub_chosecur]):240;
								}
								else if(dismenu.sub_chosecur == 5 || dismenu.sub_chosecur == 6)
								{
										 Code_I.P9[dismenu.sub_chosecur] = (Code_I.P9[dismenu.sub_chosecur]<30)?(++Code_I.P9[dismenu.sub_chosecur]):30;
								}
								else
								{
								
								}
								
						}
						else
						{
								dismenu.sub_chosecur = dismenu.sub_chosecur - 1;
								if(dismenu.sub_chosecur > lcd_maxcur || dismenu.sub_chosecur == 0)
								{
										dismenu.sub_chosecur = 0;
								}
						}
						 if(_num_4 == 0){
							 _num_4 = 1;
							 _buzzer_run = 1;	
							 buzzer_num = 1;
						 }
				}
				else if(KeyVal==Down)
				{/*
						 dismenu.sub_chosecur = dismenu.sub_chosecur + 1;
						 if(dismenu.sub_chosecur > lcd_maxcur)
						 {
								dismenu.sub_chosecur = lcd_maxcur;
						 }*/
					
					if(dismenu.data_mode)
						{
								if(dismenu.sub_chosecur == 0 || dismenu.sub_chosecur == 4 || dismenu.sub_chosecur == 5 || dismenu.sub_chosecur == 6)
								{
								     Code_I.P9[dismenu.sub_chosecur] = (Code_I.P9[dismenu.sub_chosecur]>1)?(--Code_I.P9[dismenu.sub_chosecur]):1;
								}
								else if(dismenu.sub_chosecur == 2 || dismenu.sub_chosecur == 3)
								{
								     Code_I.P9[dismenu.sub_chosecur] = (Code_I.P9[dismenu.sub_chosecur]>0)?(--Code_I.P9[dismenu.sub_chosecur]):0;
								}
								else
								{
								
								}
						}
						else
						{
								dismenu.sub_chosecur = dismenu.sub_chosecur + 1;
								if(dismenu.sub_chosecur > lcd_maxcur)
								{
										dismenu.sub_chosecur = lcd_maxcur;
								}
						}
						if(_num_4 == 0){
							 _num_4 = 1;
							 _buzzer_run = 1;	
							 buzzer_num = 1; 
						 }
				}
				else if(KeyVal==Setup)
				{
						if(dismenu.dis_mode && dismenu.data_mode == 0 && (dismenu.sub_chosecur <= 6 && dismenu.sub_chosecur != 1) )
							{   
									dismenu.data_mode = 1;
								_buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 1;       //蜂鸣器响的次数
							}
							else if(dismenu.dis_mode && dismenu.data_mode)
							{
									unsigned char *P = (unsigned char *)Code_I.P9;
									dismenu.data_mode = 0;
									for(i=0;i<8;i++)
									{
											AT24CXX_WriteOneByte(P01_PARAMETER_E2+64+i,*P++);
									}
									 _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 1;       //蜂鸣器响的次数
									
							}
							else
							{
  _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
							}	

				}
				else
				{
					if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
		   _num_1 = 1;
		   _num_2 = 1;
		   _num_3 = 1;
		   _num_4 = 1;
		   _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
	   }
				}
				KeyVal = 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_lcd_menu_ten_handler(void)//dismod4_10
{
		u8 i;
	//unsigned char *k,*j;
	//j = mode_data;
	// k = mode_data_buffer;
		 if(KeyVal==Return)
				{
						if(dismenu.data_mode)
						{
								dismenu.data_mode = 0;
						}
						else
						{
								Menu = MENUCUR3;   
								DispMod = dismod3;
								dismenu.sub_menucur = 0x01;
								dismenu.sub_chosecur = dismenu.sub_chosecurbuf;   
						}
							 _buzzer_run = 1;      //蜂鸣器运行标志	
		 buzzer_num = 1;       //蜂鸣器响的次数
				 }
				else if(KeyVal==UP)
					{
						if(dismenu.data_mode)
						{
								if(dismenu.sub_chosecur == 1 || dismenu.sub_chosecur == 4 || dismenu.sub_chosecur == 5 || dismenu.sub_chosecur == 6)
								{
										Code_I.P10[dismenu.sub_chosecur] = (Code_I.P10[dismenu.sub_chosecur]<120)?(++Code_I.P10[dismenu.sub_chosecur]):120;								}
								else if(dismenu.sub_chosecur == 2 )
								{
										 Code_I.P10[dismenu.sub_chosecur] = (Code_I.P10[dismenu.sub_chosecur]<150)?(++Code_I.P10[dismenu.sub_chosecur]):150;
								}
								else if(dismenu.sub_chosecur == 3)
								{
								     Code_I.P10[dismenu.sub_chosecur] = (Code_I.P10[dismenu.sub_chosecur]<240)?(++Code_I.P10[dismenu.sub_chosecur]):240;
								}
								else
								{
								
								}
						}
						else
						{
								dismenu.sub_chosecur = dismenu.sub_chosecur - 1;
								if(dismenu.sub_chosecur > lcd_maxcur || dismenu.sub_chosecur == 0)
								{
										dismenu.sub_chosecur = 0;
								}
						}
						if(_num_4 == 0){
							 _num_4 = 1;
							 _buzzer_run = 1;	
							 buzzer_num = 1;
						 }
					}
				else if(KeyVal==Down)
				{
					if(dismenu.data_mode)
						{
								if(dismenu.sub_chosecur != 0 )
								{
								     Code_I.P10[dismenu.sub_chosecur] = (Code_I.P10[dismenu.sub_chosecur]>1)?(--Code_I.P10[dismenu.sub_chosecur]):1;
								}
								else
								{
								
								}
						}
						else
						{
								 dismenu.sub_chosecur = dismenu.sub_chosecur + 1;
								 if(dismenu.sub_chosecur > lcd_maxcur)
								 {
										dismenu.sub_chosecur = lcd_maxcur;
								 }
					 }
					if(_num_4 == 0){
						 _num_4 = 1;
						 _buzzer_run = 1;	
						 buzzer_num = 1;
					 }
				}
				else if(KeyVal==Setup)
				{
						if(dismenu.dis_mode && dismenu.data_mode == 0 && (dismenu.sub_chosecur <= 6 && dismenu.sub_chosecur != 0) )
							{   
									dismenu.data_mode = 1;
								 _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 1;       //蜂鸣器响的次数
							}
							else if(dismenu.dis_mode && dismenu.data_mode)
							{
									unsigned char *P = (unsigned char *)Code_I.P10;
									dismenu.data_mode = 0;
									for(i=0;i<8;i++)
									{
											AT24CXX_WriteOneByte(P01_PARAMETER_E2+72+i,*P++);
									}
									 _buzzer_run = 1;      //蜂鸣器运行标志	
									buzzer_num = 1;       //蜂鸣器响的次数
							}
							else
							{
									  _buzzer_run = 1;      //蜂鸣器运行标志	
										buzzer_num = 3;       //蜂鸣器响的次数
							}	

				}
				else
				{
					 if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
						 _num_1 = 1;
						 _num_2 = 1;
						 _num_3 = 1;
						 _num_4 = 1;
						 _buzzer_run = 1;      //蜂鸣器运行标志	
						 buzzer_num = 3;       //蜂鸣器响的次数
					 }
				}
				KeyVal = 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_lcd_menu_eleven_handler(void)//dismod4_11
{
		u8 i;
	//unsigned char *k,*j;
	//j = mode_data;
	// k = mode_data_buffer;
			 if(KeyVal==Return)
				{
					if(dismenu.data_mode)
						{
								dismenu.data_mode = 0;
						}
						else
						{
								Menu = MENUCUR3;   
								DispMod = dismod3;
								dismenu.sub_menucur = 0x01;
								dismenu.sub_chosecur = dismenu.sub_chosecurbuf;  	
						}
						 _buzzer_run = 1;      //蜂鸣器运行标志	
		 buzzer_num = 1;       //蜂鸣器响的次数
				 }
				 else if(KeyVal==UP)
				{
					if(dismenu.data_mode)
						{
								if(dismenu.sub_chosecur == 0 || dismenu.sub_chosecur == 2 || dismenu.sub_chosecur == 4 )
								{
										Code_I.P11[dismenu.sub_chosecur] = (Code_I.P11[dismenu.sub_chosecur]<99)?(++Code_I.P11[dismenu.sub_chosecur]):99;								}
								else if(dismenu.sub_chosecur == 1 || dismenu.sub_chosecur == 3 )
								{
										 Code_I.P11[dismenu.sub_chosecur] = (Code_I.P11[dismenu.sub_chosecur]<120)?(++Code_I.P11[dismenu.sub_chosecur]):120;
								}
								else
								{
								
								}
						}
						else
						{
								dismenu.sub_chosecur = dismenu.sub_chosecur - 1;
								if(dismenu.sub_chosecur > lcd_maxcur || dismenu.sub_chosecur == 0)
								{
										dismenu.sub_chosecur = 0;
								}
						}
						if(_num_4 == 0){
							 _num_4 = 1;
							 _buzzer_run = 1;	
							 buzzer_num = 1;
						 }

				}
			 	else if(KeyVal==Down)
			 {
				 if(dismenu.data_mode)
						{
								     Code_I.P11[dismenu.sub_chosecur] = (Code_I.P11[dismenu.sub_chosecur]>1)?(--Code_I.P11[dismenu.sub_chosecur]):1;
						}
						else
						{
								 dismenu.sub_chosecur = dismenu.sub_chosecur + 1;
								 if(dismenu.sub_chosecur > lcd_maxcur)
								 {
										dismenu.sub_chosecur = lcd_maxcur;
								 }
					 }
						 if(_num_4 == 0){
						 _num_4 = 1;
						 _buzzer_run = 1;	
						 buzzer_num = 1; 
					 }

			 }
			 else if(KeyVal==Setup)
			{
				if(dismenu.dis_mode && dismenu.data_mode == 0 && dismenu.sub_chosecur <= 4  )
				{   
						dismenu.data_mode = 1;
					 _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 1;       //蜂鸣器响的次数
				}
				else if(dismenu.dis_mode && dismenu.data_mode)
				{
						unsigned char *P = (unsigned char *)Code_I.P11;
						dismenu.data_mode = 0;
						for(i=0;i<5;i++)
						{
								AT24CXX_WriteOneByte(P01_PARAMETER_E2+80+i,*P++);
						}
						  _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 1;       //蜂鸣器响的次数
				}
				else
				{
				_buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
				}

			}
			else
			{
				 if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
		   _num_1 = 1;
		   _num_2 = 1;
		   _num_3 = 1;
		   _num_4 = 1;
		   _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
	   }
			}
			KeyVal = 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_lcd_menu_twelve_handler(void)//dismod4_12
{
		u8 i;
	//unsigned char *k,*j;
	//j = mode_data;
	// k = mode_data_buffer;
				if(KeyVal==Return)
				{
					if(dismenu.data_mode)
						{
								dismenu.data_mode = 0;
						}
						else
						{
								Menu = MENUCUR3;   
								DispMod = dismod3;
								dismenu.sub_menucur = 0x01;
								dismenu.sub_chosecur = dismenu.sub_chosecurbuf; 
						}
						_buzzer_run = 1;      //蜂鸣器运行标志	
		 buzzer_num = 1;       //蜂鸣器响的次数
				 }
				else if(KeyVal==UP)
					{
							if(dismenu.data_mode)
							{
										Code_I.P12[dismenu.sub_chosecur] = (Code_I.P12[dismenu.sub_chosecur]<99)?(++Code_I.P12[dismenu.sub_chosecur]):99;		
									
							}
							else
							{
									dismenu.sub_chosecur = dismenu.sub_chosecur - 1;
									if(dismenu.sub_chosecur > lcd_maxcur || dismenu.sub_chosecur == 0)
									{
											dismenu.sub_chosecur = 0;
									}
							}
							if(_num_4 == 0){
       _num_4 = 1;
       _buzzer_run = 1;	
		   buzzer_num = 1;
     }

					}
				else if(KeyVal==Down)
				{
						if(dismenu.data_mode)
						{
								     Code_I.P12[dismenu.sub_chosecur] = (Code_I.P12[dismenu.sub_chosecur]>0)?(--Code_I.P12[dismenu.sub_chosecur]):0;
						}
						else
						{
								 dismenu.sub_chosecur = dismenu.sub_chosecur + 1;
								 if(dismenu.sub_chosecur > lcd_maxcur)
								 {
										dismenu.sub_chosecur = lcd_maxcur;
								 }
					 }
						 if(_num_4 == 0){
       _num_4 = 1;
       _buzzer_run = 1;	
		   buzzer_num = 1;
     }

				}
				else if(KeyVal==Setup)
				{
						if(dismenu.dis_mode && dismenu.data_mode == 0 && dismenu.sub_chosecur <= 3  )
						{   
								dismenu.data_mode = 1;
							 _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 1;       //蜂鸣器响的次数
						}
						else if(dismenu.dis_mode && dismenu.data_mode)
						{
								unsigned char *P = (unsigned char *)Code_I.P12;
								dismenu.data_mode = 0;
								for(i=0;i<4;i++)
								{
										AT24CXX_WriteOneByte(P01_PARAMETER_E2+88+i,*P++);
								}
								 _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 1;       //蜂鸣器响的次数
						}
						else
						{
						_buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
						}


				}
				else
				{
					if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
		   _num_1 = 1;
		   _num_2 = 1;
		   _num_3 = 1;
		   _num_4 = 1;
		   _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
	   }
				}
				KeyVal = 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_lcd_menu_thirteen_handler(void)//dismod4_13
{
	     if(KeyVal==Return)
				{
							Menu = MENUCUR3;   //???????
							DispMod = dismod3;
							dismenu.sub_menucur = 0x01;//??????
							dismenu.sub_chosecur = dismenu.sub_chosecurbuf;    //????	
								   _buzzer_run = 1;      //蜂鸣器运行标志	
		 buzzer_num = 1;       //蜂鸣器响的次数
				 }
				else if(KeyVal==UP)
				{
							dismenu.sub_chosecur = dismenu.sub_chosecur - 1;
							if(dismenu.sub_chosecur > lcd_maxcur || dismenu.sub_chosecur == 0)
							{
									dismenu.sub_chosecur = 0;
							}
							if(_num_4 == 0){
									 _num_4 = 1;
									 _buzzer_run = 1;	
									 buzzer_num = 1;
								 }
				}
				else if(KeyVal==Down)
				{
						 dismenu.sub_chosecur = dismenu.sub_chosecur + 1;
						 if(dismenu.sub_chosecur > lcd_maxcur)
						 {
									dismenu.sub_chosecur = lcd_maxcur;
						  }
						  if(_num_4 == 0){
								 _num_4 = 1;
								 _buzzer_run = 1;	
								 buzzer_num = 1;
							 }

				}
				else if(KeyVal==Setup)
				{
_buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数

				}
				else
				{
					 if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
		   _num_1 = 1;
		   _num_2 = 1;
		   _num_3 = 1;
		   _num_4 = 1;
		   _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
	   }
				}
				KeyVal = 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_lcd_menu_fourteen_handler(void)//dismod4_14
{
		u8 i;
	//unsigned char *k,*j;
	//j = mode_data;
	// k = mode_data_buffer;
		 if(KeyVal==Return)
		{
			if(dismenu.data_mode)
			{
				dismenu.data_mode = 0;
			}
			else
			{
				Menu = MENUCUR3;  
				DispMod = dismod3;
				dismenu.sub_menucur = 0x01;
				dismenu.sub_chosecur = dismenu.sub_chosecurbuf;   
			}
		    _buzzer_run = 1;      //蜂鸣器运行标志	
			buzzer_num = 1;       //蜂鸣器响的次数
		 }
		 else if(KeyVal==UP)
		 {
				if(dismenu.data_mode)
				{
						if(dismenu.sub_chosecur == 0 || dismenu.sub_chosecur == 1 )
						{
								Code_I.P14[dismenu.sub_chosecur] = (Code_I.P14[dismenu.sub_chosecur]<15)?(++Code_I.P14[dismenu.sub_chosecur]):15;	
						}
						else if(dismenu.sub_chosecur == 2)
						{
								Code_I.P14[dismenu.sub_chosecur] = (Code_I.P14[dismenu.sub_chosecur]<20)?(++Code_I.P14[dismenu.sub_chosecur]):20;	

						}
						else if(dismenu.sub_chosecur == 3)
						{
								Code_I.P14[dismenu.sub_chosecur] = (Code_I.P14[dismenu.sub_chosecur]<90)?(++Code_I.P14[dismenu.sub_chosecur]):90;	

						}
						else if(dismenu.sub_chosecur == 4)
						{
								Code_I.P14[dismenu.sub_chosecur] = (Code_I.P14[dismenu.sub_chosecur]<150)?(++Code_I.P14[dismenu.sub_chosecur]):150;	

						}
						else if(dismenu.sub_chosecur == 7)
						{
								Code_I.P14[dismenu.sub_chosecur] = (Code_I.P14[dismenu.sub_chosecur]<8)?(++Code_I.P14[dismenu.sub_chosecur]):8;	

						}
						else
						{
						
						}										
						
				}
				else
				{
						dismenu.sub_chosecur = dismenu.sub_chosecur - 1;
						if(dismenu.sub_chosecur > lcd_maxcur || dismenu.sub_chosecur == 0)
						{
								dismenu.sub_chosecur = 0;
						}
				}
				 if(_num_4 == 0)
				 {
					 _num_4 = 1;
					 _buzzer_run = 1;	
					 buzzer_num = 1;
				 }
			}
			else if(KeyVal==Down)
			{
					if(dismenu.data_mode)
					{
						if(dismenu.sub_chosecur == 0 || dismenu.sub_chosecur == 1 )
						{
						 Code_I.P14[dismenu.sub_chosecur] = (Code_I.P14[dismenu.sub_chosecur]>10)?(--Code_I.P14[dismenu.sub_chosecur]):10;
						}
						else if(dismenu.sub_chosecur == 2)
						{
						 Code_I.P14[dismenu.sub_chosecur] = (Code_I.P14[dismenu.sub_chosecur]>15)?(--Code_I.P14[dismenu.sub_chosecur]):15;

						}
						else if(dismenu.sub_chosecur == 3)
						{
						 Code_I.P14[dismenu.sub_chosecur] = (Code_I.P14[dismenu.sub_chosecur]>30)?(--Code_I.P14[dismenu.sub_chosecur]):30;

						}
						else if(dismenu.sub_chosecur == 4)
						{
						 Code_I.P14[dismenu.sub_chosecur] = (Code_I.P14[dismenu.sub_chosecur]>90)?(--Code_I.P14[dismenu.sub_chosecur]):90;

						}
						else if(dismenu.sub_chosecur == 7)
						{
						 Code_I.P14[dismenu.sub_chosecur] = (Code_I.P14[dismenu.sub_chosecur]>4)?(--Code_I.P14[dismenu.sub_chosecur]):4;

						}
						else
						{
						
						}		
					}
					else
					{
						 dismenu.sub_chosecur = dismenu.sub_chosecur + 1;
						 if(dismenu.sub_chosecur > lcd_maxcur)
						 {
								dismenu.sub_chosecur = lcd_maxcur;
						 }
				    }
					if(_num_4 == 0)
					{
						 _num_4 = 1;
						 _buzzer_run = 1;	
						 buzzer_num = 1;
					 }
			}
			else if(KeyVal==Setup)
			{
					if(dismenu.dis_mode && dismenu.data_mode == 0 &&( dismenu.sub_chosecur <= 7 && (dismenu.sub_chosecur != 5 || dismenu.sub_chosecur !=6)) )
					{   
						dismenu.data_mode = 1;
						_buzzer_run = 1;     
						buzzer_num = 1;  
					}
					else if(dismenu.dis_mode && dismenu.data_mode)
					{
							unsigned char *P = (unsigned char *)Code_I.P14;
							dismenu.data_mode = 0;
							for(i=0;i<8;i++)
							{
									AT24CXX_WriteOneByte(P01_PARAMETER_E2+104+i,*P++);
							}
							_buzzer_run = 1;      
	   buzzer_num = 1;   
					}
					else 
					{
					  _buzzer_run = 1;      
							buzzer_num = 3;  
					}

			}
			else
			{
				if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
	   _num_1 = 1;
	   _num_2 = 1;
	   _num_3 = 1;
	   _num_4 = 1;
	   _buzzer_run = 1;      //蜂鸣器运行标志	
	   buzzer_num = 3;       //蜂鸣器响的次数
   }
			}
			KeyVal = 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_lcd_menu_fifteen_handler(void)//dismod4_15
{
		if(KeyVal==Return)
				{
						Menu = MENUCUR3;   //???????
						DispMod = dismod3;
						dismenu.sub_menucur = 0x01;//??????
						dismenu.sub_chosecur = dismenu.sub_chosecurbuf;    //????	
						 _buzzer_run = 1;      //蜂鸣器运行标志	
							buzzer_num = 1;       //蜂鸣器响的次数
				 }
				else if(KeyVal==UP)
				{
						  _buzzer_run = 1;      	
							buzzer_num = 3;  

				}
				else if(KeyVal==Down)
				{
  _buzzer_run = 1;      	
		 buzzer_num = 3;  

				}
				else if(KeyVal==Setup)
				{

  _buzzer_run = 1;      	
		 buzzer_num = 3;  
				}
				else
				{
					 if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
		   _num_1 = 1;
		   _num_2 = 1;
		   _num_3 = 1;
		   _num_4 = 1;
		   _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
	   }
				}
				KeyVal = 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_lcd_menu_sixteen_handler(void) // dismod4_16
{
		 if(KeyVal==Return)
				{
						Menu = MENUCUR3;  //???????
						DispMod = dismod3;
						dismenu.sub_menucur = 0x01;//??????
						dismenu.sub_chosecur = dismenu.sub_chosecurbuf;    //????	
		 _buzzer_run = 1;      //蜂鸣器运行标志	
		 buzzer_num = 1;       //蜂鸣器响的次数
				 }
				else if(KeyVal==UP)
				{
						_buzzer_run = 1;      	
		 buzzer_num = 3;  

				}
				else if(KeyVal==Down)
				{
_buzzer_run = 1;      	
		 buzzer_num = 3;  

				}
				else if(KeyVal==Setup)
				{
_buzzer_run = 1;      	
		 buzzer_num = 3;  

				}
				else
				{
					 if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
		   _num_1 = 1;
		   _num_2 = 1;
		   _num_3 = 1;
		   _num_4 = 1;
		   _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
	   } 
				}
				KeyVal = 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_menu_fault_page_handler(void) //dismod5
{
	u8 i;
	i = fault_count_back;
	if(KeyVal==Return)
	{
		if(_have_fault == 0)
		{
			Menu = MENUCUR0;
			DispMod = dismod0;
		}
		else if(_have_fault == 1)
		{
			Menu = MENUCUR10;
			DispMod = dismod10;				
		}
		dismenu.sub_menucur = 0x00;
		dismenu.sub_chosecur = 0x00;
		passdisp.disp_flg= 1;
		lcd_maxcur = 0x0;
		lcd_cpage = 0x0;
		_buzzer_run = 1;            //蜂鸣器运行标志	
		buzzer_num = 1;             //蜂鸣器响的次数
	
	}
	else if(KeyVal==UP)
	{			
		if(i)
		{
			dismenu.sub_chosecur = dismenu.sub_chosecur -1;
		}
		//lcd_maxcur = i-1;//0--9最大指针
		if(dismenu.sub_chosecur > (i-1) ||dismenu.sub_chosecur == 0 )
		{
			dismenu.sub_chosecur = 0;
		}
		if(_num_4 == 0)
		{
		   _num_4 = 1;
		   _buzzer_run = 1;	
			buzzer_num = 1;
		}			
	}
	else if(KeyVal==Down)
	{
		if(i)
		{
				dismenu.sub_chosecur = dismenu.sub_chosecur + 1;
				if(dismenu.sub_chosecur > (i-1))
				{
							dismenu.sub_chosecur = i - 1;
				}
		}
		if(_num_4 == 0)
		{
			_num_4 = 1;
		   _buzzer_run = 1;	
			buzzer_num = 1;
		}
	
	}
	else
	{
		if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
	   _num_1 = 1;
	   _num_2 = 1;
	   _num_3 = 1;
	   _num_4 = 1;
	   _buzzer_run = 1;      //蜂鸣器运行标志	
	   buzzer_num = 3;       //蜂鸣器响的次数
   }
	}
		KeyVal = 0;
		
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_menu_set_program_handler(void)//dismod6_1
{
//		u8 i;
		unsigned char *k;
		k = mode_data_buffer;
		if(KeyVal==Return)
		{
				Menu = MENUCUR0;
				DispMod = dismod0;
				dismenu.sub_menucur = 0x00;
				dismenu.sub_chosecur = 0x00;
				passdisp.disp_flg= 1;
				_buzzer_run = 1;      //蜂鸣器运行标志	
				 buzzer_num = 1;       //蜂鸣器响的次数
				 _read_data = 0;//读数据
				 _read_function = 0;//读功能
				 step_num = 0;
				 lcd_chosen_sub_step = 0;//选择第几个参数(共5个)
				 _enable_single_wash = 0;//清均步
				
		}
		else if(KeyVal==UP)
		{
				switch(lcd_chosen_sub_step)
				{
					
					case 0://步
						 step_num = ++step_num;
						 if(step_num>24){
							 step_num = 0;       
						 }
						 _read_function = 0;
					break;
					case 1://分
							k[step_num * 8 + 4] = k[step_num * 8 + 4] + 1;
							if(*(k + step_num*8+4) > 99){
							 k[step_num * 8 + 4] = 99;
							 buzzer_num = 3;
							}
					break;
					case 2://秒
								k[step_num * 8 + 5] = k[step_num * 8 + 5] + 1;
								if(*(k + step_num*8+5) > 59){
								 k[step_num * 8 + 5] = 59;
								 buzzer_num = 3;
								} 
					break;
					case 3://温度
								if(k[step_num * 8 + 6] < Code_I.P12[3]){
									 k[step_num * 8 + 6] = Code_I.P12[1];	        
								 }
								 else{ 
									 k[step_num * 8 + 6] = k[step_num * 8 + 6] + 1;
									 if(*(k + step_num*8+6) > Code_I.P12[2]){
										 k[step_num * 8 + 6] = Code_I.P12[2];
										 buzzer_num = 3;
									 } 	         
								 }
					break;
					default:
						 if(k[step_num * 8 + 2] < Code_I.P6[2]){
							 k[step_num * 8 + 2] = Code_I.P6[2];
						 } 
						 else{
							 k[step_num * 8 + 2] = k[step_num * 8 + 2] + 1;
							 if(*(k + step_num*8+2) > Code_I.P6[0]){
								 k[step_num * 8 + 2] = Code_I.P6[0];
								 buzzer_num = 3;
							 }
						 }
						
					break;
				}
			
		
		}
		else if(KeyVal==Down)
		{
				switch(lcd_chosen_sub_step){
				 case 0://步
					 if(step_num>0){
						 step_num = --step_num;
					 }
					 else{
						 step_num = 24;
					 }
					 _read_function = 0;
					 break;
				 case 1://分
					 if(*(k + step_num*8+4) > 0){
						 k[step_num * 8 + 4] = --k[step_num * 8 + 4]; 
					 }
					 else{
						 k[step_num * 8 + 4] = 0;
						 buzzer_num = 3;
					 }
					 break;
				 case 2://秒
					 if(*(k + step_num*8+5) > 0){
						 k[step_num * 8 + 5] = --k[step_num * 8 + 5]; 
					 }
					 else{
						 k[step_num * 8 + 5] = 0;
						 buzzer_num = 3;
					 }
					 break;
				 case 3://温度
					 if(*(k + step_num*8+6) > Code_I.P12[3]){
						 k[step_num * 8 + 6] = --k[step_num * 8 + 6]; 
					 }
					 else{
						 k[step_num * 8 + 6] = Code_I.P12[1];
						 buzzer_num = 3;
					 }
					 break;
				 default://液位高度
					 if(*(k + step_num*8+2) > Code_I.P6[2]){
						 k[step_num * 8 + 2] = --k[step_num * 8 + 2]; 
					 }
					 else{
						 k[step_num * 8 + 2] = Code_I.P6[2];
						 buzzer_num = 3;
					 }
					 break;       
			 }
		
		}
		else if(KeyVal== Setup)//保存数据
		{
					Menu = MENUCUR6;  // azalea???
					DispMod = dismod6;
					dismenu.sub_menucur = 0x01;
					dismenu.sub_chosecur = 0x00;
					passdisp.disp_flg= 0;
					_new_page = 1;
					
					 _buzzer_run = 1;      //蜂鸣器运行标志	
						buzzer_num = 1;       //蜂鸣器响的次数
		
		}
		else if(KeyVal == Remove)
		{
				 _key_pro_ok_2 = 1;//用于设置键的联续按
//							_set_data = 1;
			dismenu.data_mode = 1;
				 lcd_chosen_sub_step = ++lcd_chosen_sub_step;
				 if(lcd_chosen_sub_step >= 5){   
						lcd_chosen_sub_step = 0;
				 } 
			 _buzzer_run = 1;	
				 buzzer_num = 1;
		}
		else if(KeyVal == Wash ) //洗涤
		{
			 if(_spin_enable == 0)
			{     
				 if(_wash_enable == 0 && _enable_single_wash == 0)
				 {
					 _wash_enable = 1;
					 *(k + step_num*8+3) = 'X';
					 } 
					 else if(_wash_enable && _enable_single_wash == 0)
					 {
						 _wash_enable = 0;
						 _enable_single_wash = 1;
						 *(k + step_num*8+3) = 'J';
					 } 
					 else
					 {
						 _wash_enable = 0;
						 _enable_single_wash = 0;
						 *(k + step_num*8+3) = 0;
					 }
					 _buzzer_run = 1;
					 buzzer_num = 1;
			 }
			 else
			 {
				 _buzzer_run = 1;
				 buzzer_num = 3;
			 }
		}
		else if(KeyVal == Cold ) //冷水
		{
				if(_spin_enable == 0 && _drain_enable == 0)
				{
					 _cold_enable = (_cold_enable)?0:1;
					 if(_cold_enable)
					 {
						 *(k + step_num*8+1) = *(k + step_num*8+1) | 0x01;		    
					 }
					 else
					 {
						 *(k + step_num*8+1) = *(k + step_num*8+1) & 0x0e;		    
					 }
					 _buzzer_run = 1;
					 buzzer_num = 1;
				 }
				 else
				 {
					 _buzzer_run = 1;
					 buzzer_num = 3;
				 }
		}
		else if(KeyVal == Warm )
		{
				if(_spin_enable == 0 && _drain_enable == 0)
			  {
					 _warm_enable = (_warm_enable)?0:1;
					 if(_warm_enable){
						 *(k + step_num*8+1) = *(k + step_num*8+1) | 0x02;
					 }
					 else{
						 *(k + step_num*8+1) = *(k + step_num*8+1) & 0x0d;
					 }
					 _buzzer_run = 1;
					 buzzer_num = 1;
				 }
				 else{
					 _buzzer_run = 1;
					 buzzer_num = 3;
				 }
		}
		else if(KeyVal == Heat) //加热
		{
			 if(_spin_enable == 0 && _drain_enable == 0){
					 _heat_enable = (_heat_enable)?0:1;
					 if(_heat_enable){
						 *(k + step_num*8+1) = *(k + step_num*8+1) | 0x04;
					 }
					 else{
						 *(k + step_num*8+1) = *(k + step_num*8+1) & 0x0b;		    
					 }
					 _buzzer_run = 1;
					 buzzer_num = 1; 
				 }
				 else{
					 _buzzer_run = 1;
					 buzzer_num = 3;
				 }
		}
		else if(KeyVal ==Soap) //zaoye
		{
				if(_spin_enable == 0 && _drain_enable == 0)
				{
					 if(soap_liquid_num == 0){
						 soap_liquid_num = 1;
						 _soap_enable = 1;
					 }
					 else if(soap_liquid_num == 1){
						 soap_liquid_num = 2;
						 _soap_enable = 1;
					 }
					 else if(soap_liquid_num == 2){
						 soap_liquid_num = 3;
						 _soap_enable = 1;
					 } 
					 else{
						 soap_liquid_num = 0;
						 _soap_enable = 0;
					 }
					 *(k + step_num*8+7) = soap_liquid_num;
					 _buzzer_run = 1;
					 buzzer_num = 1;
				 }
				 else{
					 _buzzer_run = 1;
					 buzzer_num = 3;
				 }
		}
		else if(KeyVal ==Drain)//排水
		{
				  if(_spin_enable == 0)
					{
							 _drain_enable = (_drain_enable)?0:1;
							 if(_drain_enable){
								 *(k + step_num*8+1) = *(k + step_num*8+1) | 0x08;	      
							 }
							 else{
								 *(k + step_num*8+1) = *(k + step_num*8+1) & 0x07;    
							 }
							 if(_drain_enable){
								 _cold_enable = 0;//冷水  
								 _warm_enable = 0;//热水
								 _heat_enable = 0;//加热
								 soap_liquid_num = 0;//皂液数量
								 _soap_enable = 0;//皂液
								 *(k + step_num*8+7) = 0;//皂液
								 *(k + step_num*8+1) = *(k + step_num*8+1) & 0x08;//冷水热水加热排水
							 }
							 _buzzer_run = 1;
							 buzzer_num = 1;
						 } 
						 else{	    
							 _buzzer_run = 1;
							 buzzer_num = 3;
						 }
		}
		else if(KeyVal == Spin) //脱水
		{
			if(dehydration_num == 0)
			{
				 dehydration_num = 1;
				 _spin_enable = 1;//脱水
				 _wash_enable = 0;//洗涤
				 _enable_single_wash = 0;//均布
				 _cold_enable = 0;//冷水
				 _warm_enable = 0;//热水
				 _heat_enable = 0;//加热
				 soap_liquid_num = 0;//皂液数量
				 _soap_enable = 0;//皂液
				 *(k + step_num*8+7) = 0;//皂液
				 _drain_enable = 0;
				 *(k + step_num*8+3) = 'Z';//脱水洗涤
				 *(k + step_num*8+1) = 0;//冷水热水加热排水
			 }
			 else if(dehydration_num == 1){
				 dehydration_num = 2;
				 _spin_enable = 1;//脱水
				 _wash_enable = 0;//洗涤
				 _enable_single_wash = 0;//均布
				 _cold_enable = 0;//冷水
				 _warm_enable = 0;//热水
				 _heat_enable = 0;//加热
				 soap_liquid_num = 0;//皂液数量
				 _soap_enable = 0;//皂液
				 *(k + step_num*8+7) = 0;//皂液
				 _drain_enable = 0;
				 *(k + step_num*8+3) = 'G';//脱水洗涤
				 *(k + step_num*8+1) = 0;//冷水热水加热排水
			 }
			 else{
				 dehydration_num = 0;
				 _spin_enable = 0;//脱水
				 _wash_enable = 0;//洗涤
				 _enable_single_wash = 0;//均布
				 _cold_enable = 0;//冷水
				 _warm_enable = 0;//热水
				 _heat_enable = 0;//加热
				 soap_liquid_num = 0;//皂液数量
				 _soap_enable = 0;//皂液
				 *(k + step_num*8+7) = 0;//皂液
				 _drain_enable = 0;
				 *(k + step_num*8+3) = 0;//脱水洗涤
				 *(k + step_num*8+1) = 0;//冷水热水加热排水
			 } 
			 _buzzer_run = 1;
			 buzzer_num = 1; 
		}
		else
		{
				if(KeyVal)
				{
						 _buzzer_run = 1;      //蜂鸣器运行标志	
							buzzer_num = 3;       //蜂鸣器响的次数
				}
		}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_menu_save_data_handler(void) // dismod6_2
{
		u8 i;
	unsigned char *k,*j;
	j = mode_data;
	 k = mode_data_buffer;
		if(KeyVal==RUN || KeyVal==STOP)
		{
			if(KeyVal==RUN)
			{					
				lcd_wrstring(1,3,"     数据正在保存,            ");
				lcd_wrstring(2,3,"           请稍候...          ");
				 j = mode_data;
				 k = mode_data_buffer;
				 for(i =0; i<200;i++)
				 {
						*(j + i) = *(k + i);
				 }
				 
				 for( i = 0; i < 200; i++)
				 {
						AT24CXX_WriteOneByte(PROGRAM_ONE+(auto_run_mode-1)*256+i,j[i]);

				 }
				 delay(6000);
			}
					 
			Menu = MENUCUR0;   //主菜单光标指针
			DispMod = dismod0;
			dismenu.sub_menucur = 0x00;//二级菜单指针
		    dismenu.sub_chosecur = 0x00;
			_new_page = 1;
			_buzzer_run = 1;      //蜂鸣器运行标志	
		    buzzer_num = 1;       //蜂鸣器响的次数
			 _read_data = 0;//读数据
			 _read_function = 0;//读功能
			 step_num = 0;
			 lcd_chosen_sub_step = 0;//选择第几个参数(共5个)  azalea123
				
			}
		  else
			{
			  if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
		   _num_1 = 1;
		   _num_2 = 1;
		   _num_3 = 1;
		   _num_4 = 1;
		   _buzzer_run = 1;      //蜂鸣器运行标志	
		   buzzer_num = 3;       //蜂鸣器响的次数
	   }
			}
				KeyVal = 0;	
						
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_wash_complete_menu_handler(void) //dismod7
{
		 if(KeyVal==Return)
		{
//				Menu = MENUCUR0;
//				DispMod = dismod0;
			Menu = MENUCUR0;
			DispMod = dismod0;
			dismenu.sub_menucur = 0x00;//二级菜单指针
			dismenu.sub_chosecur = 0x00;
			passdisp.disp_flg = 0;
			_new_page = 1;
			_buzzer_run = 1;      //蜂鸣器运行标志	
			buzzer_num = 1; 
		}
		else if(KeyVal==UP)
		{
		
		
		}
		else if(KeyVal==Down)
		{
		
		
		}
		else if(KeyVal==Setup)
		{
		
		
		}
		else
		{
		}
		
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_shake_stop_menu_handler(void)//dismod8
{
		if(KeyVal==Return)
		{
		
				Menu = MENUCUR0;
				DispMod = dismod0;
		
		
		}
		else if(KeyVal==UP)
		{
		
		
		}
		else if(KeyVal==Down)
		{
		
		
		}
		else if(KeyVal==Setup)
		{
		
		
		}
		else
		{
		}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_instancy_stop_menu_handler(void)//dismod9
{
		if(KeyVal==Return)
		{
		
				Menu = MENUCUR0;
				DispMod = dismod0;
		
		
		}
		else if(KeyVal==UP)
		{
		
		
		}
		else if(KeyVal==Down)
		{
		
		
		}
		else if(KeyVal==Setup)
		{
		
		
		}
		else
		{
		}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void key_menu_fault_dis_handler(void)//dismod10
{

	if(KeyVal == Return)
	{
		Menu = MENUCUR5;
		DispMod = dismod5;
		dismenu.sub_menucur = 0x00;//二级菜单指针
		dismenu.sub_chosecur = 0x00;
		passdisp.disp_flg = 0;
		_new_page = 1;
		_tell_alarm = 0;//去告警
		_buzzer_run = 1;      //蜂鸣器运行标志	
	   buzzer_num = 1; 
		
		
	}
	else if( KeyVal == 11 ) //stop 故障清除
	{
			Menu = MENUCUR0;
			DispMod = dismod0;
			dismenu.sub_menucur = 0x00;//二级菜单指针
			dismenu.sub_chosecur = 0x00;
			passdisp.disp_flg = 0;
			_new_page = 1;
			_buzzer_run = 1;      //蜂鸣器运行标志	
			buzzer_num = 1; 
			
			if(INVCurrent_fault)
			{
				usart3_flag_fault_clear = 1;
			}
			INVCurrent_fault = 0;
			flag_error_usart = 0;
			_error_shake = 0;
			_error_door_open = 0;
			flag_usart2_read_error = 0;
	}
	else 
	{
		if(KeyVal && (_num_4 == 0 && _num_3 == 0 && _num_2 == 0 && _num_1 == 0)){
	   _num_1 = 1;
	   _num_2 = 1;
	   _num_3 = 1;
	   _num_4 = 1;
	   _buzzer_run = 1;      //蜂鸣器运行标志	
	   buzzer_num = 3;       //蜂鸣器响的次数
   }
		
	}
	KeyVal = 0;
		
	
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



#endif



