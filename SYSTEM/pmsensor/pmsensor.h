#ifndef  __PMSENSOR_H
#define  __PMSENSOR_H

#include "sys.h"
#include "delay.h"



#define   PM_TIME30                  30
#define   PM_TIME60                  60
#define   PM_DATA_SIZE               52

#define   PM_SWITCH_CMD              0x0C

#define   PM_RD_MEASURE_CMD          0x0B

#define   PM_MEASURE_TIME_CMD        0x0D

#define   PM_TIMING_MEA_CMD          0x05

#define   PM_DYNAMIC_MEA_CMD         0x06

#define   PM_CALIBRATION_CMD         0x07






typedef struct{
	uchar pm_start_byte;
	uchar pm_length;
	uchar pm_data_length;
	uchar pm_command;
	
	u32   pm1_data;
	u32   pm2_data;
	u32   pm10_data;
	u32   tsp_data;
	uchar start_dat;
	uchar pm_data_buf[52];
	uchar cal_para;
	
	uchar pm_check;
}PmDef;

extern PmDef pm;





u8 Sum_Check(uchar *buf,uchar n);

void PM_Receive_Process(uchar *buf,uchar n);

u32 Get_More_Data(uchar buf[],uchar s,uchar n);

void PM_Receive_Data_Process(void);

void Send_Start_Cmd(uchar buf[],uchar data,uchar n);

void Send_Measure_Cmd(uchar buf[],uchar data,uchar n);


void Send_Measure_Time_Cmd(uchar buf[],u16 data,uchar n);


void Send_Timing_Measure_Cmd(uchar buf[],u16 data,uchar n);

void Send_Dynamic_Measure_Cmd(uchar buf[],uchar data,uchar n);

void Send_Calibration_Cmd(uchar buf[],uchar data,uchar n);



void PM_Sensor_Init(void);













#endif
