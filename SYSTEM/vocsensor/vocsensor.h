#ifndef  __VOCSENSOR_H
#define  __VOCSENSOR_H

#include "sys.h"
#include "delay.h"




uchar SUM_CHECK_VOC(uchar buf[],uchar n);

void VOC_Receive_Process(uchar buf[],uchar n);

void VOC_Receive_Data_Process(void);

void VOC_Send_Cmd(uchar buf[],uchar n);

void VOC_Send_Initiative_To_Passive(uchar buf[],uchar n);

void VOC_Send_Passive_To_Initiative(uchar buf[],uchar n);





#endif
