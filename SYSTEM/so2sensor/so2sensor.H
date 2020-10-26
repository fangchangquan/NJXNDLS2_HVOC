#ifndef  __SO2SENSOR_H
#define  __SO2SENSOR_H

#include "sys.h"
#include "delay.h"


#define MAX_DATA_QUEUE_DEPTH 50











void SO2_Receive_Process(void);

void SO2_Filter_Process(u16 *buf);
 
void SO2_Data_Process(void);






#endif
