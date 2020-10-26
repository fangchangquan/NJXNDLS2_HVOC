#ifndef __I2C_H
#define __I2C_H

#include "sys.h"
#include "delay.h"


#define AT24C01		              127
#define AT24C02		              255
#define AT24C04		              511
#define AT24C08		              1023
#define AT24C16		              2047
#define AT24C32		              4095
#define AT24C64	                8191
#define AT24C128	              16383
#define AT24C256	              32767  
#define AT24C512                65535

#define BMP280_WR_ADDR          0xEC
#define BMP280_RD_ADDR          0xED

#define SDA_IN()            {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	  //IO·½ÏòÉèÖÃ
#define SDA_OUT()           {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;}

#define  I2C_SDA_HIGH()     {PBout(9)=HIGH;}
#define  I2C_SDA_LOW()      {PBout(9)=LOW;}
#define  READ_SDA()         PBin(9)
#define  I2C_SCL_HIGH()     {PBout(8)=HIGH;}
#define  I2C_SCL_LOW()      {PBout(8)=LOW;}

#define N10                 10

#define ROUND_BUF_SIZE      4
#define VOL_UP_SIZE         1
#define VOL_DOWN_SIZE       1
#define ROUND_UP_SIZE       3
#define ROUND_DOWN_SIZE     3

typedef struct
{
  uchar  sensor_data_save      :1; 
  uchar  net_close_save        :1;		
  uchar  save                  :1;
	uchar  flash_ver_save        :1;
	uchar  flash_same_flag       :1;
	uchar  socket0_save          :1;
	uchar  socket1_save          :1;
	uchar  soft_socket_save      :1;
	
	
  uchar  save_len;
  uchar  save_mode;
	uchar  save_up_cnt;
	uchar  rd_buffer[8];
	
}EEPROM_t;

extern u8 sum_temp[N10];
extern u8 sum_a_temp[N10];
extern u8 sum_b_temp[N10];
extern u8 sum_c_temp[N10];
extern u8 rd_temp[N10];

extern EEPROM_t e2;

extern u16 net_rst_cnt;

void I2C_GPIO_Init(void);
void Clear_Sum_Temp(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Wait_Ack(void);
void I2C_Ack(void);
void I2C_Nack(void);
void I2C_Send_Byte(u8 data);
u8 I2C_Read_Byte(void);

void Bme280_Write_OneByte(u8 addr,u8 data);
u8 Bme280_Read_OneByte(u8 addr);
short Bmp280_Read_Para(u8 addr1,u8 addr2);


#endif

