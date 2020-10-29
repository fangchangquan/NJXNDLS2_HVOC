#include "digit_no2.h"
#include "usart.h"
#include "adc.h"

uchar no2_buffer[D_NO2_SIZE]={0};

uchar D_NO2_SUM_CHECK(uchar buf[],uchar n)
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
 
void D_NO2_Recieve_Data(uchar buf[],uchar n)
{
	u8 check_n=0;
	if(n<=D_NO2_SIZE)
	{
		memcpy(no2_buffer,buf,n);
		memset(buf,0,n);
		check_n = D_NO2_SUM_CHECK(no2_buffer,n);//校验
	}
	else
	{
		return ;
	}
	if(no2_buffer[0] == 0xff)
	{
		if(check_n == no2_buffer[n-1])
		{
			memcpy(sensor.no2.no2_buf,no2_buffer,n);
		}
	}
}

void D_NO2_Data_Process(void)
{
	sensor.no2.no2_ug = sensor.no2.no2_buf[2];
	sensor.no2.no2_ug <<= 8;
	sensor.no2.no2_ug |= sensor.no2.no2_buf[3];
}


/*
*函数功能：板子主动发送命令查询传感器数据
函数参数：无
*/
void D_NO2_SEND_CMD_TO_SENSOR(uchar buf[],uchar n)
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
	
	Uart3_Put_Word(buf,n);
}

void D_NO2_driving_mode(uchar buf[],uchar n)
{
	buf[0] = 0xff;
	buf[1] = 0x01;
	buf[2] = 0x78;
	buf[3] = 0x40;
	buf[4] = 0x00;
	buf[5] = 0x00;
	buf[6] = 0x00;
	buf[7] = 0x00;
	buf[8] = 0x47;
	
	Uart3_Put_Word(buf,n);
}




