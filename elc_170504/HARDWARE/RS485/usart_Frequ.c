#ifndef __USART_FREQU_C
#define __USART_FREQU_C

#include "usart_Frequ.h"
#include "delay.h"
#include "crc.h"

u8 counter_error_usart3 = 0;
u16 crcData3 = 0;
extern u16 acce_time;
extern u16 dece_time;
extern u16 frequency;
extern u8 fault_clear;
extern u16 systick_flag_200ms_rd ;
extern u8 flag_usart3_wr_500ms;
extern u8 flag_rd;
extern u8 flag_wr;
extern void Delay(__IO uint32_t nTime);
extern u16 MBS_HoldRegs[100];
static __IO uint32_t ReadINV_Timeout_Count;

void ReadINV_Timeout_Count_Decrement(void)
{
	if (ReadINV_Timeout_Count != 0x00)
	{ 
		ReadINV_Timeout_Count--;
	}
}

void USART3_IRQHandler(void)
{
	u8 res;	    
	counter_error_usart3 ++;
	if(counter_error_usart3 >= 100)
	{
		counter_error_usart3 = 0;
		flag_error_usart3 = 1;  
	}
 	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收到数据
	{	 
	 	counter_error_usart3 = 0;
		res =USART_ReceiveData(USART3); 	//读取接收到的数据
		if(RS485_RX_CNT3 < 100)
		{
			RS485_RX_BUF3[RS485_RX_CNT3]=res;		//记录接收到的值
			RS485_RX_CNT3++;						//接收数据增加1 
		}
		else
		{
			RS485_RX_CNT3 = 0;
		}			
	}

  	if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)//发送中断
		{
			counter_error_usart3 = 0;
			if(sendPosi3 <= RS485_TX_CNT3)
			{
				USART_SendData(USART3,RS485_TX_BUF3[sendPosi3 ++]);
			}
			else
			{
				sendPosi3 = 0;
				RS485_TX_EN3 = 0;
				delay_ms(1);//1ms
				USART_ITConfig(USART3,USART_IT_TXE,DISABLE);//发完后关掉发送中断			
				USART_ITConfig(USART3,USART_IT_RXNE,ENABLE); //打开接收中断	
			}
		}
} 
  
void RS485_Init3(u32 bound)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);//使能GPIOA,D时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART1时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //PD7端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,ENABLE);//复位串口2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,DISABLE);//停止复位
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;///奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式
	USART_Init(USART3, &USART_InitStructure); ; //初始化串口
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; //使能串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART3, ENABLE);                    //使能串口 
	RS485_TX_EN3=0;			//默认为接收模式 
}

void usart_ReadRegister3(u16 add,u16 num)//0x03 读1个或多个数据
{
	u16 crcData = 0;
	RS485_TX_BUF3[0] = localAddr3;
	RS485_TX_BUF3[1] = 0x03;	
	RS485_TX_BUF3[2] = (add&0xff00)>>8; 
	RS485_TX_BUF3[3] = add&0x00ff;
	RS485_TX_BUF3[4] = (num&0xff00)>>8; 
	RS485_TX_BUF3[5] = num&0x00ff;
	RS485_TX_CNT3 = 6;
	crcData = crc16(RS485_TX_BUF3,RS485_TX_CNT3); 
	RS485_TX_BUF3[6] = (crcData >> 8) & 0xff;	//高位在前  
	RS485_TX_BUF3[7] = crcData & 0xff; 		//低位在后
	RS485_TX_CNT3 = 8;
	sendPosi3 = 0;
	RS485_TX_EN3 = 1;
	delay_ms(1);//1ms
	USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART3,USART_IT_TXE,ENABLE);//打开发送中断	
}
void usart_WriteRegister3(u16 add,u16 data)//0x06 写一个数据
{	
	u16 crcData = 0;
	RS485_TX_BUF3[0] = localAddr3;
	RS485_TX_BUF3[1] = 0x06; 
	RS485_TX_BUF3[2] = (add&0xff00)>>8; 
	RS485_TX_BUF3[3] = add&0x00ff;
	RS485_TX_BUF3[4] = (data&0xff00)>>8; 
	RS485_TX_BUF3[5] = data&0x00ff;
	RS485_TX_CNT3 = 6;
	crcData = crc16(RS485_TX_BUF3,RS485_TX_CNT3); 
	RS485_TX_BUF3[6] = (crcData >> 8) & 0xff;  
	RS485_TX_BUF3[7] = crcData & 0xff;
	RS485_TX_CNT3 = 8;
	sendPosi3 = 0;
	RS485_TX_EN3 = 1;
	delay_ms(1);//1ms
	USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);//关掉接收中断
	USART_ITConfig(USART3,USART_IT_TXE,ENABLE);//打开发送中断 
}

u8 modbus_dataUnpack_2(void)//做变频器的主机，与变频器通讯
{	
	u8 byteCount;
	u8 tempAddr;
	u8 errcode = 0x00;
	int i;
	if(RS485_RX_CNT3 > 4 )
	{
		if(RS485_RX_BUF3[0] != 1 && RS485_RX_BUF3[1] == 1)
		{
			RS485_RX_BUF3[0] = RS485_RX_BUF3[1];
			for(i = 0; i < RS485_RX_CNT3; i++)
			{
				RS485_RX_BUF3[i] = RS485_RX_BUF3[i+1];
			}
			RS485_RX_BUF3[i] = 0;
		}
		switch(RS485_RX_BUF3[1])
		{
			case 0x03:  //读变频器的值
			{ 
				byteCount = RS485_RX_BUF3[2];//只读低8位
				tempAddr = RS485_RX_BUF3[2] >> 1;
				if(RS485_RX_CNT3 >= byteCount+5)
				{
					if(RS485_RX_BUF3[0] == localAddr3)
					{
 						crcData3 = crc16(RS485_RX_BUF3,byteCount+3);//读10个数据 10.6us
						if(crcData3 == (RS485_RX_BUF3[byteCount+3]<<8) + RS485_RX_BUF3[byteCount+4])
						{	
							if(trans_data_num_old <= 1)
							{
								MBS_HoldRegs3[trans_data_num_old] = ((u16)RS485_RX_BUF3[3]<<8) + RS485_RX_BUF3[4];//3:数据高8位 4:低8位	
								MBS_HoldRegs[68 + trans_data_num_old] = MBS_HoldRegs3[trans_data_num_old];
							}
							if(trans_data_num_old == 4)
							{
								MBS_HoldRegs3[trans_data_num_old] = ((u16)RS485_RX_BUF3[3]<<8) + RS485_RX_BUF3[4];
								MBS_HoldRegs[63] = MBS_HoldRegs3[trans_data_num_old];

							}
						    if(trans_data_num_old == 2)
							{
								for(i = 0; i < tempAddr; i++)
								{
									MBS_HoldRegs3[5+i] = ((u16)RS485_RX_BUF3[3+i*2]<<8) + RS485_RX_BUF3[4+i*2];//3:数据高8位 4:低8位	
									MBS_HoldRegs[68 + trans_data_num_old+i] = MBS_HoldRegs3[3+i];
								}
							}
							
							flag_reading3 = 0;//解析成功的
							flag_rw_err = 0;
						}
						else
						{
							errcode = ERR_CHECKSUM;
						  //flag_writing3 = 1;							
				//			counter_rd_error3 ++;
						}
					}
					RS485_RX_CNT3 = 0;
					flag_rd3_timeout=0;
				}
			}break;
			case 0x06://向变频器写数据，后变频器返回的值
			{
				if(RS485_RX_CNT3 >= 8)
				{
					if(RS485_RX_BUF3[0] == localAddr3)
					{						
 						crcData3 = crc16(RS485_RX_BUF3,6);//写一个数据 3.476us						
						if(crcData3 == ((RS485_RX_BUF3[6]<<8) + RS485_RX_BUF3[7]))
						{	
							if(trans_data_num_old == 5)
							{
								modbus_check3[0] = (RS485_RX_BUF3[4]<<8) + RS485_RX_BUF3[5];
								if(modbus_check3[0] == acce_time)
								{
									//flag_writing3 = 0;
									flag_wr_accetime = 0;									
								}
							}
							else if(trans_data_num_old == 6)
							{
								modbus_check3[1] = (RS485_RX_BUF3[4]<<8) + RS485_RX_BUF3[5];
								if(modbus_check3[1] == dece_time)
								{
									//flag_writing3 = 0;
									flag_wr_decetime = 0;
								}
							}
							else if(trans_data_num_old == 7)
							{
								modbus_check3[2] = (RS485_RX_BUF3[4]<<8) + RS485_RX_BUF3[5];
								if(modbus_check3[2] == frequency)
								{
									//flag_writing3 = 0;
									flag_wr_freqency = 0;
									
								}
							}
							else if(trans_data_num_old == 10)
							{
								modbus_check3[3] = (RS485_RX_BUF3[4]<<8) + RS485_RX_BUF3[5];
								if(modbus_check3[3] == fault_clear)
								{
									flag_writing3 = 0;	
								}
							}
							
							if((flag_wr_accetime == 0) && (flag_wr_decetime == 0) && (flag_wr_freqency == 0))
							{
								flag_writing3 = 0;
							}
						}	
						else
						{					
							errcode = ERR_CHECKSUM;
						}
					}
					RS485_RX_CNT3 = 0;
					flag_wr3_timeout = 0;
				}
			}
			break;			
			default:
			{
				RS485_RX_CNT3 = 0;
			}
			break;
		}
		counter_rw_err = 0;
	}
	else
	{
		flag_rw_err = 1;
		counter_rw_err++;
		
		
	}
	RS485_RX_CNT3 = 0;
	return errcode;
}

void ELC2INV_Read_write(void)
{
//	static u8 by_reset_count = 0;
	trans_data_num_old = trans_data_num;
	switch(trans_data_num)
	{
		case 0:
			usart_ReadRegister3(0x2000,1); //变频器当前命令
			trans_data_num = 1;
		break;
		case 1:
			usart_ReadRegister3(0x2001,1); //变频器当前状态
			trans_data_num = 2;		
		break;
		case 2:
			usart_ReadRegister3(0x4000,5);//变频器运行频率
			trans_data_num = 0;
		break;
		case 4:
			usart_ReadRegister3(0x6000,1);//变频器当前故障
		break;
		case 5:
			usart_WriteRegister3(0x000D,acce_time);//写加速时间
			flag_wr_accetime = 1;
			trans_data_num = 6;
		break;
		case 6:
			usart_WriteRegister3(0x000E,dece_time);//写减速时间
			flag_wr_decetime = 1;
			trans_data_num = 7;
		break;
		case 7:
			usart_WriteRegister3(0x3000,frequency);//写频率
			flag_wr_freqency = 1;
			trans_data_num = 15;
		break;
		case 10:
			usart_WriteRegister3(0x2000,7);//写故障清除
		break;	
		default:
		break;
	}
}

u8 presetReadINV_Adress(void)
{
	u8 errcode = 0x00;
	while(1)
	{
		if(flag_rd == 1)
		{
			flag_rd = 0;
			if(flag_reading3 == 0)
			{
				ELC2INV_Read_write();
				//OUT_COOL_WATER = ~OUT_COOL_WATER;
				flag_reading3 = 1;
				//counter_rw_err = 0;
			}
			else
			{
				if(flag_rw_err == 1 /*&& counter_rw_err == 2*/)
				{
					//OUT_HOT_WATER = ~OUT_HOT_WATER;
					ELC2INV_Read_write();
					flag_rd3_timeout++ ;
					counter_rw_err = 0;

				}
			}
		
		}
		if(flag_rd3_timeout >= 5)
		{
			errcode =  ERR_TIMEOUT;
			flag_rd3_timeout = 0;
			break;
		}
		
		delay_ms(10);
		errcode = modbus_dataUnpack_2();//变频器的主机，收到数据后解包
		if(flag_reading3 == 0 && (trans_data_num_old == 2 || trans_data_num_old == 4))
		{
			//WriteINV_M_TimerOut = 10;
			errcode = ERR_NONE;
			break;	
		}
		
		
	}
	return errcode;
}

u8 presetWriteINV_Adress(void)
{
	u8 errcode = 0x00;
	while(1)
	{
		if(flag_wr == 1)
		{
			flag_wr = 0;
			if(flag_writing3 == 0)
			{
				ELC2INV_Read_write();
				flag_writing3 = 1;
			}
			else
			{
				if(flag_rw_err == 1)
				{
					ELC2INV_Read_write();
					flag_wr3_timeout++;

				}
			}
		}
		
		if(flag_wr3_timeout >= 5)
		{
			errcode =  ERR_TIMEOUT;
			flag_wr3_timeout = 0;
			break;
		}
		delay_ms(10);//10ms
		errcode = modbus_dataUnpack_2();//变频器的主机，收到数据后解包	
		if(flag_writing3 == 0 )
		{
			if(trans_data_num_old == 7 || trans_data_num_old == 10 )
			{
				errcode = ERR_NONE;
				break;
			}
		}
	}
	return errcode;
}


u8 presetWrite10INV_Adress(void)
{
	u8 errcode = 0x00;
	while(1)
	{
		if(flag_wr == 1)
		{
			flag_wr = 0;
			if(flag_writing3 == 0)
			{
				ELC2INV_Read_write();
				flag_writing3 = 1;
			}
			else
			{
				if(flag_rw_err == 1 && (flag_wr_accetime == 1 || flag_wr_decetime == 1 ||flag_wr_freqency == 1))
				{
					if(flag_wr_accetime == 1)
					{
						usart_WriteRegister3(0x000D,acce_time);//写加速时间

					}
					else if(flag_wr_decetime == 1)
					{
						usart_WriteRegister3(0x000E,dece_time);//写减速时间
	
					}
					else if(flag_wr_freqency == 1)
					{
						usart_WriteRegister3(0x3000,frequency);//写频率

					}
					//ELC2INV_Read_write();
					flag_wr3_timeout++;

				}
			}
		}
		
		if(flag_wr3_timeout >= 5)
		{
			errcode =  ERR_TIMEOUT;
			flag_wr3_timeout = 0;
			break;
		}
		delay_ms(10);//10ms
		errcode = modbus_dataUnpack_2();//变频器的主机，收到数据后解包	
		if(flag_writing3 == 0 )
		{
			if(trans_data_num_old == 7 || trans_data_num_old == 10 )
			{
				errcode = ERR_NONE;
				break;
			}
		}
	}
	return errcode;
}


#endif




