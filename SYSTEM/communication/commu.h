#ifndef  __COMMU_H
#define  __COMMU_H

#include "sys.h"
#include "delay.h"



#define  REC_BUF_SIZE            52
#define  BUF_SIZE                4   //接收4组
#define  SENSOR_SIZE             24
#define  SENSOR_ONE_SIZE         10

#define  TEMP_ADDR               0x0000
#define  HUMD_ADDR               0x0001
#define  PRES_ADDR               0x0002
#define  PM1_ADDR                0x0003
#define  PM2_ADDR                0x0004
#define  PM10_ADDR               0x0005
#define  TSP_ADDR                0x0006
#define  O3_ADDR                 0x0007
#define  SO2_ADDR                0x0008
#define  NO2_ADDR                0x0009
#define  SPEED_ADDR              0x000A
#define  VOC_ADDR                0x000B

typedef struct
{
	  uchar buf[REC_BUF_SIZE];
	  uchar copy_buf[REC_BUF_SIZE];
	  uchar length;
	  
	
}Buf_Def;


typedef struct                         //pc发到传感器板 例如：01 03 00 00 00 0C 10
{
    u8   dev_addr;                     //设备地址(1-6)      01
	  u8   code;                         //功能码             03

    u16  reg_addr;            		     /* 寄存器首地址      00 00   	  2B */  
    u16  num; 	                       //寄存器个数         00 0C
	  u8   crc;               	         /* CRC       		    10          1B */
}PC_To_Sensor_Def ;  

typedef struct                         //风机发到传感器板 例如：
{
    u8   dev_addr;                     //设备地址(7,8,9)
	  u8   code;                         //功能码             04,06

    u8  length;            		         /* 个数      */
    u16  value_7; 	
		u16  value_8; 
		u16  value_9; 	//寄存器的值         
	  u8   crc;               	         /* CRC       		    10          1B */
}FAN_To_Sensor_Def ;  

typedef struct                         //pc发到传感器板
{
    u8   dev_addr;                     //设备地址(1-6)
	  u8   code;                         //功能码

    u16  reg_addr;            		     /* 寄存器首地址         	  2B */  
    u16  num; 	
	  u8   data_buf[SENSOR_SIZE];
	  u16   crc;               	         /* CRC       		    1B */
}Sensor_To_PC_Def ;  





extern Buf_Def rec_buffer[BUF_SIZE];
extern PC_To_Sensor_Def pc_to_sensor;
extern FAN_To_Sensor_Def fan_to_sensor;
extern Sensor_To_PC_Def sensor_to_pc; 





void Store_Message_To_Buffer(uchar *buf,Buf_Def dest_buf[]);

void Read_Message_From_Buffer(Buf_Def *pdata);


void Usart1_Receive_Data_Process(void);

uint16 Crc16_Modbus(u8 ptr[],uchar len);



u32 Group_TO_Hex(uchar group[],uchar n);

uint32_t Group_TO_Hex_Little_Endian3(uchar str,uchar group[],uchar n);

uint32_t Group_TO_Hex_Little_Endian(uchar str,uchar group[],uchar n);

void Usart1_Receive_PLC_Data_Process(uchar *buf,uchar length,uchar *dest_buf);
void Usart6_Receive_Fan_Data_Process(uchar *buf,uchar length,uchar *dest_buf);

void Hex_To_Group(u16 data,uchar group[],uchar start_n, uchar n);

void Usart1_Send_Data_Process(PC_To_Sensor_Def *buf,Sensor_To_PC_Def *dest_buf);

void Usart1_Send_To_PLC_Process(uchar *buf ,uchar length);

void Data_Copy_Dest(Sensor_To_PC_Def *s_buf,uchar buf[],uchar length);






#endif
