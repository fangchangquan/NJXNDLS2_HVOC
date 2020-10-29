#include "digit_o3.h"
#include "usart.h"
#include "adc.h"


uchar o3_buffer[D_O3_SIZE] = {0};

uchar D_O3_SUM_CHECK(uchar buf[],uchar n)
{
	uchar i=0;
	u8 sum=0;
	for(i=1;i<n-2;i++)
	{
		sum += buf[i];
	}
	sum = ~sum + 1;
	
	return sum;
	
}

void D_O3_Receive_Data(uchar buf[],uchar n)
{
	u8 check_n=0;
	if(n<=D_O3_SIZE)
	{
		memcpy(o3_buffer,buf,n);
		memset(buf,0,n);
		check_n = D_O3_SUM_CHECK(o3_buffer,n);
	}
	else
	{
		return ;
	}
	if(o3_buffer[0] == 0xff)
	{
		if(check_n == o3_buffer[n-1])
		{
			memcpy(sensor.o3.o3_buf,o3_buffer,n);
		}
	}
	
}

void D_O3_Data_Process(void)
{
	sensor.o3.o3_ug = sensor.o3.o3_buf[2];
	sensor.o3.o3_ug <<= 8;
	sensor.o3.o3_ug |= sensor.o3.o3_buf[3];
}

/*
*函数功能：板子主动发送命令查询传感器数据
函数参数：无
*/
void D_O3_SEND_CMD_TO_SENSOR(uchar buf[],uchar n)
{
	buf[0] = 0xff;
	buf[1] = 0x01;
	buf[2] = 0x86;
	buf[3] = 0x00;
	buf[4] = 0x00;
	buf[5] = 0x00;
	buf[6] = 0x00;
	buf[7] = 0x00;
	buf[8] = 0x79;
	
	Uart4_Put_Word(buf,n);
}
