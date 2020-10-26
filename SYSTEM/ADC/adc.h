#ifndef  __ADC_H
#define  __ADC_H

#include "sys.h"
#include "delay.h"
#include "usart.h"


#define ADC1_DR_Address   ((u32)0x40012400+0x4c)
#define ADC_DATA            40 
#define ADC_COUNT           (ADC_DATA/10)               
#define N0                  12                              
#define NUM3			          5
#define N3				          25

#define PM_SIZE             56

#define VOC_SIZE            9

#define D_NO2_SIZE          9//数字式NO2传感器

#define MAX_VOL				      3300    //MV
#define MIN_VOL				      100    //13.5-13.8V
#define MAX_TEMP            850

#define PMV                 1
#define TEMP                2
#define VOL                 3
#define O3V                 4
#define NO2V                5
#define SO2V                6
   
#define ADC1_CHANNEL0_EN    1        //PM1.0,2.5,10
#define ADC1_CHANNEL1_EN    1        //温度
#define ADC1_CHANNEL2_EN    2        //3.3V
#define ADC1_CHANNEL5_EN    5        //O3
#define ADC1_CHANNEL6_EN    6        //NO2        
#define ADC1_CHANNEL15_EN   15       //SO2


#define COUNT 				      ADC1_CHANNEL1_EN

#define MAX                 COUNT*32

#define R_VALUE             67
#define VOL_DIF_VALUE       50

typedef struct
{
	u16   vol_dat;
	u16   vol_now;
  
}volTypeDef;	

typedef struct
{
	 s16  temp_now;
	 struct temdef
	 {
		  long temperature;
		  int32_t ut;
		  int32_t tem_now;
		 
	 }tem;
	 
	 struct humdef
	 {
		  int32_t  hum_uh;
	    int32_t  hum_now;	  
		 
	 }hum;
	
	 struct pressdef
	 {
		  
		  int32_t press_up;
		  int32_t press_now;
		 
	 }press;
	 
	 
	 struct o3def
	 {
	    u16  o3_dat;
	    u16  o3_now;
		  u16  o3_ppm;
		  u32  o3_ug;
		 
	 }o3;
	 
	 struct no2def
	 {
		  u8   no2_buf[D_NO2_SIZE];
	    u16  no2_dat;
	    u16  no2_now;
	    u16  no2_ppm;
		  u32  no2_ug;
		 
	 }no2;
	 
	 struct so2def
	 {
	    u16  so2_dat;
	    u16  so2_now;
		  u16  so2_ppm;
	    u32  so2_ug;
	 }so2;
	 
	 struct vocdef
	 {
	    u16  voc_dat;
	    u16  voc_now;
		  u16  voc_ppm;
	    u32  voc_ug;
		  u8   voc_buf[VOC_SIZE];
		 
	 }voc;
	 
	 struct pmdef
	 {
	    u16  pm_dat;
	    u16  pm1_now;//ug/m3
      u16  pm2_now;
		  u16  pm10_now;
		  u16  tsp_now;
		  u8   pm_buf[PM_SIZE];
		 
	 }pm;
	 
	 struct motordef
	 {
		  u16  speed;
		 
	 }motor_speed;
	 
	 uchar sen_buf_length;
	 
}DefSensor;



extern volTypeDef vol;
extern DefSensor  sensor;

extern s16 t_temp;






void ADC_GPIO_Init(void);
void ADC_DMA_Init(void);
void ADC_Start(FunctionalState stat);
void ADC_Initial(void);
void Get_ADC(void);
u16 Filter_Temp(u16 queue[],uchar n);
u16 Filter_Temp_1(u16 queue[],uchar n);
u16 Amplitiude_Limterfilter_Pulse(void);
s16 Amplitiude_Limterfilter_Tmp(s16 newvalue,s16 value);
s16 Linear_onepart(u16 xn,u16 x1,u16 x2,s16 y1,s16 y2);
s16 linear_allpart(u16 xn,u16 queue_x[],s16 queue_y[],uchar n);
u16 Get_AD(uchar value_temp);
u16 Glide_average_value_filter(uchar glide_temp);	

void Get_PM_Data(void);

void Get_Temp_Data(void);

void Get_Vol_Data(void);

void Get_O3_Data(void);

void Get_No2_Data(void);

void Get_So2_Data(void);

void Get_Tmp_Vol_O3_NO2_SO2_Data(void);




#endif

