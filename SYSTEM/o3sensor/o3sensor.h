#ifndef  __O3SENSOR_H
#define  __O3SENSOR_H

#include "sys.h"
#include "delay.h"







#define MAX_DATA_QUEUE_DEPTH 50




void O3_Receive_Process(void);

void O3_Filter_Process(u16 *buf);

void O3_Data_Process(void);























#endif
