#ifndef __DIGIT_NO2__
#define __DIGIT_NO2__

#include "sys.h"


uchar SUM_CHECK(uchar buf[],uchar n);
void D_NO2_Recieve_Data(uchar buf[],uchar n);
void D_NO2_Data_Process(void);
void D_NO2_SEND_CMD_TO_SENSOR(uchar buf[],uchar n);

#endif

