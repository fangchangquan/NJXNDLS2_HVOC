#ifndef  __HUMIDITY_H
#define  __HUMIDITY_H

#include "sys.h"
#include "delay.h"



#define TEM_ADDR             0xFA

#define PRESS_ADDR           0xF7

#define HUM_ADDR             0xFD

typedef struct{
	u32 ut;
	u32 up;
	u32 uh;
	
	s32 ct;
	u32 cp;
	u32 ch;
	u32 id;
	
	u16 dig_t1;
	s16 dig_t2;
	s16 dig_t3;
	
	u16 dig_p1;
	s16 dig_p2;
	s16 dig_p3;
	s16 dig_p4;
	s16 dig_p5;
	s16 dig_p6;
	s16 dig_p7;
	s16 dig_p8;
	s16 dig_p9;
	
	u8  dig_h1;
	s16 dig_h2;
  u8  dig_h3;
	s16 dig_h4;
  s16 dig_h5;
	u8  dig_h6;	
	
}TphDef;



extern TphDef tph;
	
extern uchar bmpcnt_t,bmpcnt_p;






u8 Read_One_Register(uchar addr);

short Read_Two_Register(uchar addr);

void Read_TPH_Register(void);


void Bmp280_Init(void);

long  Bmp180_Get_UT(void);

long  Bmp180_Get_UP(void);

long  Bmp180_Get_UH(void);

int32 Bme280_Compensate_Temperature(int32 adc_t);

int32_t Bme280_Compensate_Temperature_Float(int32_t adc_t);

int32 Bme280_Compensate_Pressure(int32 adc_p);

int32_t Bme280_Compensate_Pressure_Float(int32_t adc_p);

unsigned long Bme280_Compensate_Hum(long adc_h);

double Bme280_Compensate_Humidity(int32_t adc_h);

void Get_Temperature_Value(void);

void Get_Press_Value(void);

void Get_Humidity_Value(void);


















#endif
