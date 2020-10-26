#include "pmsensor.h"
#include "adc.h"


PmDef pm;


uchar pm_group[PM_TIME60]={0};
uchar pm_buffer[PM_SIZE]={0};
uchar pm_check_sum=0;




//===============================================================
/*********************************************************************************
** 函数名称: Sum_Check
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
u8 Sum_Check(uchar *buf,uchar n)
{
	 uchar i;
	 u8   sum=0;
	
	 for(i=0;i<n-1;i++)
	 {
		  sum+=buf[i];
	 }
	 
	 sum=256-sum;
	 
	 return sum;
	 
 }
//===============================================================
/*********************************************************************************
** 函数名称: PM_Receive_Process
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void PM_Receive_Process(uchar *buf,uchar n)//UART4
{
	 uchar pm_cmd=0;
 
	 if(n>=PM_SIZE)
	 {
		 
	    memcpy(pm_buffer,buf,n);
		  memset(buf,0,n);
		  pm_cmd=pm_buffer[2];
		  pm_check_sum=pm_buffer[n-1];
	 }
	 else
	 {
		  return;
	 }
	 
	if(pm_buffer[0]==0x16)
	{
		 switch(pm_cmd)
		 {
			 case PM_SWITCH_CMD      :pm.pm_length=pm_buffer[1]+3;
			                          pm.start_dat=pm_buffer[3];
			                          pm.pm_data_length=pm_buffer[1]-1;
			                          
			                          pm_check_sum=Sum_Check(pm_buffer,pm.pm_length);
			                          pm.pm_check=pm_buffer[pm.pm_length-1];
			 
				                        break;
			 case PM_RD_MEASURE_CMD  :pm.pm_length=pm_buffer[1]+3;
			                          pm.pm_data_length=pm_buffer[1]-1;
			                          
			                          
			                          pm_check_sum=Sum_Check(pm_buffer,pm.pm_length);
			                          pm.pm_check=pm_buffer[pm.pm_length-1];
				                        break;
			 case PM_MEASURE_TIME_CMD:
				                        break;
			 case PM_TIMING_MEA_CMD  :
				                        break;
			 case PM_DYNAMIC_MEA_CMD :
				                        break;
			 case PM_CALIBRATION_CMD :pm.pm_length=pm_buffer[1]+3;
			                          pm.cal_para=pm_buffer[3];
			                          pm.pm_data_length=pm_buffer[1]-1;
			                          pm_check_sum=Sum_Check(pm_buffer,pm.pm_length);
			                          pm.pm_check=pm_buffer[pm.pm_length-1];
				                        break;
			 
			 default                 :break;
		 }
		 
		 pm.pm_command=pm_cmd;
		 
	 }
		 
}
//===============================================================
/*********************************************************************************
** 函数名称: Get_More_Data
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
u32 Get_More_Data(uchar buf[],uchar s,uchar n)
{
	 u32 temp=0;
	
	 if((s+3<=n)&&(n<100))
	 {
		  temp=buf[s];
		  temp<<=8;
		  temp|=buf[s+1];
		  temp<<=8;
		  temp|=buf[s+2];
		  temp<<=8;
		  temp|=buf[s+3];
		  
		  return temp;
	 }
	 else
	 {
		  return 0;
	 }
	 
	 
	
}
//===============================================================
/*********************************************************************************
** 函数名称: PM_Receive_Data_Process
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void PM_Receive_Data_Process(void)
{
	 u32 rec_temp=0;
	 uchar i;
	
	 if(pm_check_sum==pm.pm_check)
	 {
		  switch(pm.pm_command)
			{
				case PM_RD_MEASURE_CMD       :for(i=0;i<pm.pm_data_length;i++)
			                                {
																	        pm.pm_data_buf[i]=pm_buffer[i+3];
																      }
					                            rec_temp=Get_More_Data(pm.pm_data_buf,0,PM_DATA_SIZE);
				                              if(rec_temp!=0)
																			{
																				 pm.pm1_data=rec_temp;
																				 sensor.pm.pm1_now=pm.pm1_data;
																				 
																			}
																			
																			rec_temp=Get_More_Data(pm.pm_data_buf,4,PM_DATA_SIZE);
				                              if(rec_temp!=0)
																			{
																				 pm.pm2_data=rec_temp;
																				 sensor.pm.pm2_now=pm.pm2_data;
																			}
																			
																			rec_temp=Get_More_Data(pm.pm_data_buf,8,PM_DATA_SIZE);
				                              if(rec_temp!=0)
																			{
																				 pm.pm10_data=rec_temp;
																				 sensor.pm.pm10_now=pm.pm10_data;
																			}
					                            
																			rec_temp=Get_More_Data(pm.pm_data_buf,12,PM_DATA_SIZE);
				                              if(rec_temp!=0)
																			{
																				 pm.tsp_data=rec_temp;
																				 sensor.pm.tsp_now=pm.tsp_data;
																			}
				                              break;
				case PM_CALIBRATION_CMD      :
					                            break;
				default                      :break;
			}
			
		}
	 
}
		  
//===============================================================
/*********************************************************************************
** 函数名称: Send_Start_Cmd
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Send_Start_Cmd(uchar buf[],uchar data,uchar n)
{
	 buf[0]=0x11;
	 buf[1]=n-3;
	 buf[2]=0x0c;
	 buf[3]=data;
	 buf[4]=0x1e;
	 buf[5]=Sum_Check(buf,n);
	 
	 Uart5_Put_Word(buf,n);
}
//===============================================================
/*********************************************************************************
** 函数名称: Send_Measure_Cmd
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Send_Measure_Cmd(uchar buf[],uchar data,uchar n)
{
	 buf[0]=0x11;
	 buf[1]=n-3;
	 buf[2]=0x0b;
	 buf[3]=data;
	 buf[4]=Sum_Check(buf,n);
	 
	 Uart5_Put_Word(buf,n);
}
//===============================================================
/*********************************************************************************
** 函数名称: Send_Measure_Time_Cmd
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Send_Measure_Time_Cmd(uchar buf[],u16 data,uchar n)
{
	 buf[0]=0x11;
	 buf[1]=n-3;
	 buf[2]=0x0d;
	 buf[3]=(u8)(data>>8);
	 buf[4]=(u8)data;
	 buf[5]=Sum_Check(buf,n);
	 
	 Uart5_Put_Word(buf,n);
}

//===============================================================
/*********************************************************************************
** 函数名称: Send_Timing_Measure_Cmd
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Send_Timing_Measure_Cmd(uchar buf[],u16 data,uchar n)
{
	 buf[0]=0x11;
	 buf[1]=n-3;
	 buf[2]=0x05;
	 buf[3]=(u8)(data>>8);
	 buf[4]=(u8)data;
	 buf[5]=Sum_Check(buf,n);
	 
	 Uart5_Put_Word(buf,n);
}


//===============================================================
/*********************************************************************************
** 函数名称: Send_Dynamic_Measure_Cmd
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Send_Dynamic_Measure_Cmd(uchar buf[],uchar data,uchar n)
{
	 buf[0]=0x11;
	 buf[1]=n-3;
	 buf[2]=0x06;
	 buf[3]=data;
	 buf[4]=Sum_Check(buf,n);
	 
	 Uart5_Put_Word(buf,n);
}

//===============================================================
/*********************************************************************************
** 函数名称: Send_Calibration_Cmd
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Send_Calibration_Cmd(uchar buf[],uchar data,uchar n)
{
	 buf[0]=0x11;
	 buf[1]=n-3;
	 buf[2]=0x07;
	 buf[3]=data;
	 buf[4]=Sum_Check(buf,n);
	 
	 Uart5_Put_Word(buf,n);
}


//===============================================================
/*********************************************************************************
** 函数名称: PM_Sensor_Init
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void PM_Sensor_Init(void)
{
	 Send_Start_Cmd(rs485.rs485_tx_buf43,0x02,6);
	
	 Delay_Ms(500);
	 
	 Delay_Ms(500);
	
}



