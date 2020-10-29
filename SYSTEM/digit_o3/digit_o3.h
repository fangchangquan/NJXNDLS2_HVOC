#ifndef __DIGIT_O3_H
#define __DIGIT_O3_H

#include "sys.h"


uchar D_O3_SUM_CHECK(uchar buf[],uchar n);

void D_O3_Receive_Data(uchar buf[],uchar n);

void D_O3_Data_Process(void);

void D_O3_SEND_CMD_TO_SENSOR(uchar buf[],uchar n);


#endif

