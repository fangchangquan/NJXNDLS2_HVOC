#include "vocsensor.h"
#include "adc.h"


uchar voc_buffer[VOC_SIZE]={0};

//校验函数
uchar SUM_CHECK_VOC(uchar buf[],uchar n)
{
	uchar i=0;
	u8 sum=0;
	for(i=1;i<n-1;i++)
	{
		sum += buf[i];
	}
	sum = ~sum + 1;
	
	return sum;
	
}
/*********************************************************************************
** 函数名称: VOC_Receive_Process
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
 void VOC_Receive_Process(uchar buf[],uchar n)
 {
	  uchar temp_check=0;
	 
	  if(n>=VOC_SIZE)
	  {
	     memcpy(voc_buffer,buf,n);
		   memset(buf,0,n);
		 
		   temp_check=SUM_CHECK_VOC(voc_buffer,n);
	  }
	  else
	  {
		   return;
	  }
		
		if(voc_buffer[0]==0xff)
		{
			 if(temp_check==voc_buffer[n-1])
			 {
				  memcpy(sensor.voc.voc_buf,voc_buffer,n);
			 }
			 
		}
		
 }
 
//===============================================================
/*********************************************************************************
** 函数名称: VOC_Receive_Data_Process
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
 void VOC_Receive_Data_Process(void)//*10＃，即扩大10倍
 {
		sensor.voc.voc_ppm=sensor.voc.voc_buf[6];
		//sensor.voc.voc_now=sensor.voc.voc_buf[1];
		sensor.voc.voc_ppm<<=8;
		//sensor.voc.voc_now<<=8;
		sensor.voc.voc_ppm|=sensor.voc.voc_buf[7];
		//sensor.voc.voc_now|=sensor.voc.voc_buf[2];
	 
 }
//===============================================================
/*********************************************************************************
** 函数名称: VOC_Send_Initiative_To_Passive
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
 void VOC_Send_Cmd(uchar buf[],uchar n)//发送指令读数据
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
	
	Uart2_Put_Word(buf,n);
}	
//收←◆21 00 05 02 00 00 00 31 C8 //获取传感器类型，最大量程，单位，单位小数位数
//收←◆FF D7 21 00 05 02 31 00 D0 //获取传感器类型，最大量程，单位，单位小数位数
//===============================================================
/*********************************************************************************
** 函数名称: VOC_Send_Passive_To_Initiative
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void VOC_Send_Passive_To_Initiative(uchar buf[],uchar n)//改为主动模式
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
	
	  Uart2_Put_Word(buf,n);
	
}	

 void VOC_Send_Initiative_To_Passive(uchar buf[],uchar n)//改为被动模式
{
	buf[0] = 0xff;
	buf[1] = 0x01;
	buf[2] = 0x78;
	buf[3] = 0x41;
	buf[4] = 0x00;
	buf[5] = 0x00;
	buf[6] = 0x00;
	buf[7] = 0x00;
	buf[8] = 0x46;
	
	Uart2_Put_Word(buf,n);
}
//===============================================================
/*********************************************************************************
** 函数名称: Double_Led_Charge_Control
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
