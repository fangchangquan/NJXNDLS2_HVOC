#ifndef  __NO2SENSOR_H
#define  __NO2SENSOR_H

#include "sys.h"
#include "delay.h"






#define MAX_DATA_QUEUE_DEPTH  50





void NO2_Receive_Process(void);

void NO2_Filter_Process(u16 *buf);

void NO2_Data_Process(void);























#endif
