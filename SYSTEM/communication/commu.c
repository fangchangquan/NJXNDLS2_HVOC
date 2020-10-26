#include "commu.h"
#include "usart.h"
#include "vocsensor.h"
#include "adc.h"
#include "key.h"

Buf_Def rec_buffer[BUF_SIZE]={0};
PC_To_Sensor_Def pc_to_sensor;
FAN_To_Sensor_Def fan_to_sensor;
Sensor_To_PC_Def sensor_to_pc; 

static volatile uchar kk=0;


//===============================================================
/*********************************************************************************
** 函数名称: Store_Message_To_Buffer
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
//void Store_Message_To_Buffer(uchar *buf,Buf_Def dest_buf[])
//{
//	  uint16_t i;
//	  uchar    h[BUF_SIZE]={0};
//    uchar k=0;
//    
//		
//		
//	  if(rs485.rx_ok_flag6==1)
//		{
//			
//	     for(i=0;i<RX_BUF_SIZE;i++)
//	     {
//			  		  
//			    if((buf[i]==0x55)&&(buf[i+1]==0x01))
//				  {
//					   h[k]=i;
//						
//					   if(k<BUF_SIZE)
//					   {
//						    k++;
//					   }
//					   else
//					   {
//						    k=4;
//					   }
//				  }
//				
//				  if((i==RX_BUF_SIZE-1)&&(buf[RX_BUF_SIZE6-1]==0xAA))
//				  {
//					   rs485.rx_ok_flag6=0;
//					 
//					   flag.usart1_success_flag=1;

//					   memcpy(rs485.rs485_rx_copy_buf6,buf,RX_BUF_SIZE);
//										 					 
//					   kk=k;
//					   k=0;
//					 
//					   break;
//				  }
//		   }
//     
//		   for(rs485.tail1=0;rs485.tail1<kk;rs485.tail1++)
//		   {
//		      for(i=0;i<REC_BUF_SIZE;i++)
//		      {
//				     dest_buf[rs485.tail1].buf[i]=rs485.rs485_rx_copy_buf6[h[rs485.tail1]+i];
//				
//		      }
//		 
//		      if(rs485.tail1==rs485.head1)
//		      {
//				     rs485.head1++;
//						
//				     if(rs485.head1>=kk)
//				     {
//					      rs485.head1=0;					 					
//					 
//				     }
//		      }	 
//	     }
//		 
//		   if(rs485.tail1>=kk)
//		   {
//			    rs485.tail1=kk;			 

//	     }
//				
//	  }
//		
//}
//=============================================================== 
/*********************************************************************************
** 函数名称Read_Message_From_Buffer
** 编写者  ：CZM
** 函数功能：接收数据
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
//void Read_Message_From_Buffer(Buf_Def *pdata)
//{  
//	  if(rs485.head1==rs485.tail1)
//		{
//			 return;
//		}
//		else
//		{
//		   memcpy(pdata[rs485.head1].copy_buf,rec_buffer[rs485.head1].buf,REC_BUF_SIZE);
//		   
//			 memcpy(&pc_to_sensor,&pdata[rs485.head1].copy_buf,REC_BUF_SIZE);

//			 rs485.head1++;
//			
//			 if(rs485.head1>=kk)
//			 {
//					 rs485.head1=kk;			   
//				   				 
//				   memset(rec_buffer[rs485.head1].buf,0,REC_BUF_SIZE);	             //清除空间
//				   
//				   rs485.rec_complete_flag=1;			   
//				 
//			 }
//		  
//    }
//}
//===============================================================
/*********************************************************************************
** 函数名称: Usart1_Receive_Data_Process
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
//void Usart1_Receive_Data_Process(void)
//{
//	 Store_Message_To_Buffer(rs485.rs485_rx_buf1,rec_buffer);	 
//	 Read_Message_From_Buffer(rec_buffer);
//	 
//}
//===============================================================
/*********************************************************************************
** 函数名称: Crc16_Modbus
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
uint16 Crc16_Modbus(u8 ptr[],uchar len)       
{ 
   uint16 i,j,tmp,CRC16;
	 uchar  h;
	 uchar  p[32]={0};
	 memcpy(p,ptr,len);
	 
   CRC16=0xffff;
   for (i=0;i<len;i++)
   {  
      CRC16=CRC16^ptr[i];
      for (j=0;j<8;j++)
      {

				if((CRC16&0x0001)!=0)
				{
					 CRC16=(CRC16>>1)^0xA001;
				}
				else
				{
					 CRC16>>=1;
				}
				
      }  
	 }
	 h=(uchar)CRC16;
	 CRC16>>=8;
	 tmp=CRC16;
	 CRC16=h;
	 CRC16<<=8;
	 CRC16|=tmp;
   return(CRC16);
}

//===============================================================
/*********************************************************************************
** 函数名称: Group_TO_Hex
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
u32 Group_TO_Hex(uchar group[],uchar n)
{
	uchar i; 
	u32 hex_dat=0;
	 
	hex_dat=group[0];
	for(i=1;i<n;i++)
  {    
     hex_dat<<=8;
     hex_dat|=group[i];
	}

  return hex_dat;
}	
//===============================================================
/*********************************************************************************
** 函数名称: Group_TO_Hex_Little_Endian3
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
uint32_t Group_TO_Hex_Little_Endian3(uchar str,uchar group[],uchar n)
{
	uchar i; 
	u32 hex_dat=0;
	 
	hex_dat=group[n-1];
	for(i=n-2;i>str;i--)
  {    
     hex_dat<<=8;
     hex_dat|=group[i];
	}
	if(i==0)
	{
	  hex_dat<<=8;
    hex_dat|=group[0];
	}
	
  return hex_dat;
	
}
//===============================================================
/*********************************************************************************
** 函数名称: Group_TO_Hex_Little_Endian
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
uint32_t Group_TO_Hex_Little_Endian(uchar str,uchar group[],uchar n)
{
	uchar i; 
	u32 hex_dat=0;
	 
	hex_dat=group[n-1];
	for(i=n-2;i>str;i--)
  {    
     hex_dat<<=8;
     hex_dat|=group[i];
	}
	if(i==4)
	{
	  hex_dat<<=8;
    hex_dat|=group[4];
	}
	
  return hex_dat;
}	

//===============================================================
/*********************************************************************************
** 函数名称: Usart1_Receive_PLC_Data_Process
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Usart1_Receive_PLC_Data_Process(uchar *buf,uchar length,uchar *dest_buf)
{
	 //uchar sum=0;
	
   if(rs485.rx_ok_flag_really==1)
	 {
		  memcpy(dest_buf,buf,length);
		  memset(buf,0,length);
		 
		  //sum=Sum_Nbyte_Check(dest_buf,length);
		  
		  
//		  if((sum==dest_buf[length-1])&&(length>6))
//			{
				 if(dest_buf[0] == key.code)
				 {
					 flag.usart1_success_flag=1;
					
					 rs485.rx_ok_flag_really=0;
				 
					 pc_to_sensor.dev_addr=dest_buf[0];
					 pc_to_sensor.code=dest_buf[1];
					 pc_to_sensor.reg_addr=(u16)dest_buf[2];
					 pc_to_sensor.reg_addr<<=8;
					 pc_to_sensor.reg_addr|=(u16)dest_buf[3];
					 
					 pc_to_sensor.num=(u16)dest_buf[length-4];
					 pc_to_sensor.num<<=8;
					 pc_to_sensor.num|=dest_buf[length-3];
				 }
				 
//			}
				 
		}	 
}

/*
函数名:Usart6_Receive_Fan_Data_Process
函数功能:用于接收风机传回的数据
*/
void Usart6_Receive_Fan_Data_Process(uchar *buf,uchar length,uchar *dest_buf)
{
	
   if(rs485.rx_ok_flag6==1)
	 {
		  memcpy(dest_buf,buf,length);
		  memset(buf,0,length);

		 if(dest_buf[0] == 0x07)
		 {
		
			 rs485.rx_ok_flag6=0;
			 
			 fan_to_sensor.value_7 = (u16)dest_buf[3];
			 fan_to_sensor.value_7 <<= 8;
			 fan_to_sensor.value_7 |= (u16)dest_buf[4];
		 }
		 if(dest_buf[0] == 0x08)
		 {
		
			 rs485.rx_ok_flag6=0;

			 fan_to_sensor.value_8 = (u16)dest_buf[3];
			 fan_to_sensor.value_8 <<= 8;
			 fan_to_sensor.value_8 |= (u16)dest_buf[4];
		 }
		 if(dest_buf[0] == 0x09)
		 {
		
			 rs485.rx_ok_flag6=0;
			 
			 fan_to_sensor.value_9 = (u16)dest_buf[3];
			 fan_to_sensor.value_9 <<= 8;
			 fan_to_sensor.value_9 |= (u16)dest_buf[4];
		 }	
		}	 
}

//===============================================================
/*********************************************************************************
** 函数名称: Hex_To_Group
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Hex_To_Group(u16 data,uchar group[],uchar start_n, uchar n)
{
	 if(n>=2)
	 {
		  group[start_n]=(uchar)(data>>8);
		  group[start_n+1]=(uchar)(data);
	 }
	 else
	 {
		  return;
	 }
	 
 }
		  
//===============================================================
/*********************************************************************************
** 函数名称: Usart1_Send_Data_Process
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Usart1_Send_Data_Process(PC_To_Sensor_Def *buf,Sensor_To_PC_Def *dest_buf)
{
	 u16 addr=0;
	 uchar group[32]={0};
	 uchar i;
	 
	 dest_buf->dev_addr=buf->dev_addr;
	 dest_buf->code=buf->code;
	 dest_buf->reg_addr=buf->reg_addr;
	
	 dest_buf->num=buf->num;
	 
	 addr=dest_buf->reg_addr;
	 
	 
	 if(dest_buf->num==1)
	 {
		  switch(addr)
      {				
				case TEMP_ADDR :Hex_To_Group(sensor.tem.temperature,dest_buf->data_buf,0,SENSOR_ONE_SIZE);
					              break;
				case HUMD_ADDR :Hex_To_Group(sensor.hum.hum_now,dest_buf->data_buf,0,SENSOR_ONE_SIZE);
					              break;
				case PRES_ADDR :Hex_To_Group(sensor.press.press_now,dest_buf->data_buf,0,SENSOR_ONE_SIZE);
					              break;
				case PM1_ADDR  :Hex_To_Group(sensor.pm.pm1_now,dest_buf->data_buf,0,SENSOR_ONE_SIZE);
					              break;
				case PM2_ADDR  :Hex_To_Group(sensor.pm.pm2_now,dest_buf->data_buf,0,SENSOR_ONE_SIZE);
					              break;
				case PM10_ADDR :Hex_To_Group(sensor.pm.pm10_now,dest_buf->data_buf,0,SENSOR_ONE_SIZE);
					              break;
				case TSP_ADDR  :Hex_To_Group(sensor.pm.tsp_now,dest_buf->data_buf,0,SENSOR_ONE_SIZE);
					              break;
				case O3_ADDR   ://Hex_To_Group(sensor.o3.o3_now,dest_buf->data_buf,0,SENSOR_ONE_SIZE);
												Hex_To_Group(sensor.o3.o3_ug,dest_buf->data_buf,0,SENSOR_ONE_SIZE);
					              break;
				case SO2_ADDR  ://Hex_To_Group(sensor.so2.so2_now,dest_buf->data_buf,0,SENSOR_ONE_SIZE);
												Hex_To_Group(sensor.so2.so2_ug,dest_buf->data_buf,0,SENSOR_ONE_SIZE);
					              break;
				case NO2_ADDR  ://Hex_To_Group(sensor.no2.no2_now,dest_buf->data_buf,0,SENSOR_ONE_SIZE);
												Hex_To_Group(sensor.no2.no2_ug,dest_buf->data_buf,0,SENSOR_ONE_SIZE);
					              break;
				case SPEED_ADDR:Hex_To_Group(sensor.motor_speed.speed,dest_buf->data_buf,0,SENSOR_ONE_SIZE);
					              break;
				case VOC_ADDR  :Hex_To_Group(sensor.voc.voc_ppm,dest_buf->data_buf,0,SENSOR_ONE_SIZE);
					              break;
				default        :break;
			}
			
			
	    group[0]=dest_buf->dev_addr;
			group[1]=dest_buf->code;
			group[2]=(u8)(dest_buf->reg_addr>>8);
			group[3]=(u8)dest_buf->reg_addr;
			group[4]=(u8)(dest_buf->num>>8);
			group[5]=(u8)dest_buf->num;
			group[6]=dest_buf->data_buf[0];
			group[7]=dest_buf->data_buf[1];
			
			dest_buf->crc=Crc16_Modbus(group,SENSOR_ONE_SIZE-2);
			Hex_To_Group(dest_buf->crc,dest_buf->data_buf,2,SENSOR_ONE_SIZE);
			sensor.sen_buf_length=SENSOR_ONE_SIZE;
			
	 }
	 else if(dest_buf->num==3)
	 {
		  
	 }
	 else if(dest_buf->num==4)
	 {
		  
	 }
	 else if((dest_buf->num==11)||(dest_buf->num==12))
	 {
		  Hex_To_Group(sensor.tem.tem_now,dest_buf->data_buf,0,SENSOR_SIZE);
		  Hex_To_Group(sensor.hum.hum_now,dest_buf->data_buf,2,SENSOR_SIZE);
		  Hex_To_Group(sensor.press.press_now,dest_buf->data_buf,4,SENSOR_SIZE);
		 
		  Hex_To_Group(sensor.pm.pm2_now,dest_buf->data_buf,6,SENSOR_SIZE);	//PM2.5 
		  Hex_To_Group(fan_to_sensor.value_7,dest_buf->data_buf,8,SENSOR_SIZE);//fan_7 speed
		  Hex_To_Group(fan_to_sensor.value_8,dest_buf->data_buf,10,SENSOR_SIZE);//fan_8 speed
		  Hex_To_Group(fan_to_sensor.value_9,dest_buf->data_buf,12,SENSOR_SIZE);//fan_9 speed
		 
		  Hex_To_Group(sensor.o3.o3_now,dest_buf->data_buf,14,SENSOR_SIZE);
		    //Hex_To_Group(sensor.o3.o3_ug,dest_buf->data_buf,14,SENSOR_SIZE);
		  Hex_To_Group(sensor.so2.so2_now,dest_buf->data_buf,16,SENSOR_SIZE);
		    //Hex_To_Group(sensor.so2.so2_ug,dest_buf->data_buf,16,SENSOR_SIZE);
		  Hex_To_Group(sensor.no2.no2_now,dest_buf->data_buf,18,SENSOR_SIZE);
		    //Hex_To_Group(sensor.no2.no2_ug,dest_buf->data_buf,18,SENSOR_SIZE);
		 
		  Hex_To_Group(sensor.motor_speed.speed,dest_buf->data_buf,20,SENSOR_SIZE);
		 
		  Hex_To_Group(sensor.voc.voc_ppm,dest_buf->data_buf,22,SENSOR_SIZE);
		 
		  group[0]=dest_buf->dev_addr;
			group[1]=dest_buf->code;
			group[2]=(u8)(dest_buf->reg_addr>>8);
			group[3]=(u8)dest_buf->reg_addr;
			group[4]=(u8)(dest_buf->num>>8);
			group[5]=(u8)dest_buf->num;
			for(i=0;i<24;i++)
			{
				
			  group[i+6]=dest_buf->data_buf[i];
			}

		  dest_buf->crc=Crc16_Modbus(group,SENSOR_SIZE+6);
		 
		  sensor.sen_buf_length=SENSOR_SIZE+8;
	 }
	 
}

//===============================================================
/*********************************************************************************
** 函数名称: Usart1_Send_To_PLC_Process
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Usart1_Send_To_PLC_Process(uchar *buf ,uchar length)
{
	 if(flag.usart1_success_flag==1)
	 { 
		 
		  flag.usart1_success_flag=0;
		  
		  Usart1_Send_Data_Process(&pc_to_sensor,&sensor_to_pc);
		 
//		  memcpy(buf,(u8*)(&sensor_to_pc),length);
		 
		  Data_Copy_Dest(&sensor_to_pc,buf,pc_to_sensor.num);
		 
	    Uart1_Put_Word(buf,length);
			rs485.iwdg_count_flag = 1;
	 }
	
}
//===============================================================
/*********************************************************************************
** 函数名称: Data_Copy_Dest
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Data_Copy_Dest(Sensor_To_PC_Def *s_buf,uchar buf[],uchar length)
{
	 uchar i;
	
	 buf[0]=s_buf->dev_addr;
	 buf[1]=s_buf->code;
	 buf[2]=(uchar)(s_buf->reg_addr>>8);
	 buf[3]=(uchar)s_buf->reg_addr;
	 buf[4]=(uchar)(s_buf->num>>8);
	 buf[5]=(uchar)s_buf->num;
	 if(length==1)
	 {	  
      buf[6]=s_buf->data_buf[0];
	    buf[7]=s_buf->data_buf[1];
	    buf[8]=(uchar)(s_buf->crc>>8);
	    buf[9]=(uchar)s_buf->crc;
	 }
	 else if((length==12)||(length==11))
	 {
		  for(i=0;i<24;i++)
		  {
			   buf[i+6]=s_buf->data_buf[i];
			}
			 
			buf[30]=(uchar)(s_buf->crc>>8);
	    buf[31]=(uchar)s_buf->crc;
		}
}
//===============================================================
/*********************************************************************************
** 函数名称: Group_TO_Hex_Little_Endian
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
