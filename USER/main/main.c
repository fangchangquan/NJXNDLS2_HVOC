#include "sys.h"
#include "main.h"
#include "clock.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"
#include "motor.h"
#include "temp.h"
#include "humidity.h"
#include "o3sensor.h"
#include "no2sensor.h"
#include "so2sensor.h"
#include "vocsensor.h"
#include "pmsensor.h"
#include "commu.h"

#include "i2c.h"
#include "light.h"

#include "key.h"
#include "iwdg.h"
#include "digit_no2.h"
#include "digit_o3.h"
#include "fan_cmd.h"


//static int plc_cmd_count = 0;
static int iwdg_count_reload = 0; 
static int sensor_send_cmd_fan_count = 0;
//===============================================================
/*********************************************************************************
** 函数名称: T1ms_Control
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void T1ms_Control(void)
{

	 if(t_flag.t1ms_flag==1)
	 {
		  t_flag.t1ms_flag=0;
      
		  
	 }	 
}
//===============================================================
/*********************************************************************************
** 函数名称: T10ms_Control
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void T10ms_Control(void)
{
	 if(t_flag.t10ms_flag==1)
	 {
		  t_flag.t10ms_flag=0;

		  Motor_Speed_Process();		 

      Keynum_Process();//不持续执行
	 }	
}

//===============================================================
/*********************************************************************************
** 函数名称: T1ms_Control
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void T50ms_Control(void)
{

	 if(t_flag.t50ms_flag==1)
	 {
		  t_flag.t50ms_flag=0;
      
		  //Get_Tmp_Vol_O3_NO2_SO2_Data();	
		  Usart6_Receive_Fan_Data_Process(rs485.rs485_rx_buf6,RX_BUF_SIZE_TO_FAN, rs485.rs485_rx_copy_buf6);
	 }	 
}
//===============================================================
/*********************************************************************************
** 函数名称: T100ms_Control
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void T100ms_Control(void)
{

	 if(t_flag.t100ms_flag==1)
	 {
		 t_flag.t100ms_flag=0;
		 if(rs485.rx_ok_flag1==1)
		 {
			 sensor_send_cmd_fan_count++;
			 if(sensor_send_cmd_fan_count % 4 == 1){Usart6_Send_Cmd_To_Fan_7_Speed(rs485.rs485_tx_buf6,TX_BUF_SIZE6);}
			 if(sensor_send_cmd_fan_count % 4 == 2){Usart6_Send_Cmd_To_Fan_8_Speed(rs485.rs485_tx_buf6,TX_BUF_SIZE6);}
			 if(sensor_send_cmd_fan_count % 4 == 3){Usart6_Send_Cmd_To_Fan_9_Speed(rs485.rs485_tx_buf6,TX_BUF_SIZE6);}
//			 if(sensor_send_cmd_fan_count % 7 == 4){Usart6_Send_Cmd_To_Fan_7_ERR_CODE(rs485.rs485_tx_buf6,TX_BUF_SIZE6);}
//			 if(sensor_send_cmd_fan_count % 7 == 5){Usart6_Send_Cmd_To_Fan_8_ERR_CODE(rs485.rs485_tx_buf6,TX_BUF_SIZE6);}
//			 if(sensor_send_cmd_fan_count % 7 == 6){Usart6_Send_Cmd_To_Fan_9_ERR_CODE(rs485.rs485_tx_buf6,TX_BUF_SIZE6);}
			 if(sensor_send_cmd_fan_count % 4 == 0)
			 {
				 	sensor_send_cmd_fan_count = 0;
				  rs485.rx_ok_flag1=0;
			 }
		 }
		 
	 } 	 
}	 
//===============================================================
/*********************************************************************************
** 函数名称: T200ms_Control
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void T200ms_Control(void)
{

	 if(t_flag.t200ms_flag==1)
	 {
		  t_flag.t200ms_flag=0;
		 	
		 	Get_Temperature_Value();
		 
		  Get_Press_Value();
		 
		  Get_Humidity_Value();	
		 
			Usart1_Send_To_PLC_Process(rs485.rs485_tx_buf1,29);
		}			
}
//===============================================================
/*********************************************************************************
** 函数名称: T500ms_Control
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void T500ms_Control(void)
{
	 if(t_flag.t500ms_flag==1)
	 {
		  t_flag.t500ms_flag=0;
	    
		  Light_Control();
		 
		
		 if(rs485.iwdg_count_flag == 1)
			{
				rs485.iwdg_count_flag = 0;
				iwdg_count_reload = 0;
			}
		 
	 }	 
}

//===============================================================
/*********************************************************************************
** 函数名称: T1s_Control
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void T1s_Control(void)
{

	 if(t_flag.t1s_flag==1)
	 {
		  t_flag.t1s_flag=0;

		  
		  
		  Send_Measure_Cmd(rs485.rs485_tx_buf5,0x07,TX_BUF_SIZE5);//pm

			PM_Receive_Process(rs485.rs485_rx_buf5,RX_BUF_SIZE5);
		  PM_Receive_Data_Process();
		  Usart1_Receive_PLC_Data_Process(rs485.rs485_rx_buf1,RX_BUF_SIZE1, rs485.rs485_rx_copy_buf1);

			
		 //Get_Tmp_Vol_O3_NO2_SO2_Data();	
	
	 }	 
}

void T1_5s_Control(void)//1.2S
{
	if(t_flag.t1_5s_flag == 1)
	{
		t_flag.t1_5s_flag = 0;
		VOC_Receive_Data_Process();
		D_NO2_Data_Process();
	}
}
//===============================================================
/*********************************************************************************
** 函数名称: T5s_Control
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void T5s_Control(void)
{
	 if(t_flag.t5s_flag==1)
	 {
		  t_flag.t5s_flag=0;
		 D_NO2_SEND_CMD_TO_SENSOR(rs485.rs485_tx_buf3,TX_BUF_SIZE3);//发送指令 
		 VOC_Send_Cmd(rs485.rs485_tx_buf2,TX_BUF_SIZE2);//查询VOC数据 
		 if(iwdg_count_reload<20)//5s钟喂狗一次，超过70s，标志位没清零，则重启（每发送一次数据，标志位清零一次）
			{
				iwdg_count_reload ++;
				iwdg_feed();
			}
	 }	 
}

//===============================================================
/*********************************************************************************
** 函数名称: IWDG_Init
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
/*
void IWDG_Init(u8 prer,u16 rlr)
{
	 IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  
	 while (IWDG->SR&0x01);
	 IWDG_SetPrescaler(prer);  
	 while (IWDG->SR&0x02);
	 IWDG_SetReload(rlr); 

	 IWDG_ReloadCounter();  

	 IWDG_Enable();  
}*/

//===============================================================
/*********************************************************************************
** 函数名称: IWDG_Feed
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
//void IWDG_Feed(void)
//{
//   IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  
//   IWDG_ReloadCounter();
//}
//===============================================================
/*********************************************************************************
** 函数名称: Double_Led_Charge_Control
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void System_Init(void)
{
	NVIC_Configuration();
  SystemInit();
	delay_init(168);
	System_Clock_Config();

  TIM2_Init();
	
  ADC_Initial();
	Delay_Ms(500);
	
	USART_Initial();
 	Key_Init();
	
	I2C_GPIO_Init();
	Delay_Ms(500);
	
  Light_GPIO_Init();
	
	Delay_Ms(500);
	Bmp280_Init();
	
	PM_Sensor_Init();
	
	IWDG_Init();
	
  Motor_Init();
	
//	D_NO2_driving_mode(rs485.rs485_tx_buf3,TX_BUF_SIZE3);
	
  
  //IWDG_Init(IWDG_Prescaler_128,3125);//5S
}
//===============================================================
/*********************************************************************************
** 函数名称: Double_Led_Charge_Control
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/

//===============================================================
/*********************************************************************************
** 函数名称: main
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
//主函数

int main(void)
{
	 
	System_Init();
	
	Keynum_Process();
	while(1)
	{
		
		 T1ms_Control();
	   T10ms_Control();
		 T50ms_Control();
     T100ms_Control();
		 T200ms_Control();
	   T500ms_Control();
	   T1s_Control();
		 T1_5s_Control();
		 T5s_Control();
		 
	}
	
	
}











	


