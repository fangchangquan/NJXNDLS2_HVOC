#include "o3sensor.h"
#include "adc.h"



u16 data_queue_o3[MAX_DATA_QUEUE_DEPTH + 1] = {0};
//static u16 queue_front = 0;//队首
//static u16 queue_rear = 0;//队尾
//static uchar so2_cnt = 0;


void enqueue_data_o3(u16 e)
{
//		static u16 data_queue[MAX_DATA_QUEUE_DEPTH + 1] = {0};
		static u16 queue_front = 0;//队首
		static u16 queue_rear = 0;//队尾
		//u16 temp = 0;
		
    if(queue_front == queue_rear)
    {
        data_queue_o3[queue_front] = e;
    }
    
    data_queue_o3[queue_rear] = e;
		//如果队列满了 
		if((queue_rear+1)%(MAX_DATA_QUEUE_DEPTH+1)==queue_front)
		{
			//queue_rear = (queue_rear + 1) % (MAX_DATA_QUEUE_DEPTH + 1);//队尾变成队首
			//temp = data_queue_o3[queue_front];//出队
			
			queue_front =  (queue_front + 1) % (MAX_DATA_QUEUE_DEPTH+1);
		}
	
    queue_rear = (queue_rear + 1) % (MAX_DATA_QUEUE_DEPTH + 1);//队尾后移1 
    
}


//===============================================================
/*********************************************************************************
** 函数名称: O3_Receive_Process
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void O3_Receive_Process(void)
{
		u32 o3_tmp=0;
		u32 temp=0;
	
	 if((sensor.o3.o3_dat>0)&&(sensor.o3.o3_dat<=3300))
	 {
		 if(sensor.o3.o3_dat > 228)
		 {
			 o3_tmp=sensor.o3.o3_dat - 228;
		 }
		 else
		 {
			  o3_tmp = 0;
		 }
		 if(sensor.tem.tem_now>=200)
		 {
				temp=sensor.tem.tem_now-200;      //*10
				temp /= 10;                    // /10
		 }
		 else
		 {
				temp=200-sensor.tem.tem_now;
				temp /= 10; 
		 }
		 temp*=3;                       //*1000
		 temp=93*(temp+1000);
		 sensor.o3.o3_ppm=(o3_tmp*1000000/temp); //扩大100万倍
		 sensor.o3.o3_ug = sensor.o3.o3_ppm * 2143 / 1000; // 1 ppm = 2143ug/m3;
		 
		 enqueue_data_o3(sensor.o3.o3_ug);
		  	
		 O3_Filter_Process(data_queue_o3);
			
	 }
	  
 }		
//===============================================================
/*********************************************************************************
** 函数名称: O3_Filter_Process
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void O3_Filter_Process(u16 *buf)
 {
	 
	 
	  sensor.o3.o3_now=Filter_Temp_1(buf,MAX_DATA_QUEUE_DEPTH);
	 
	 
 }
 
void O3_Data_Process(void)
{
	O3_Receive_Process();
}
//===============================================================
/*********************************************************************************
** 函数名称: O3_Data_Process
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
//void O3_Data_Process(void)
//{
//	 u32 o3_tmp=0;
//	 u32 temp=0;
//	 
//	 O3_Receive_Process();
//	 if(sensor.o3.o3_now>=221)
//	 {
//		 
//	    o3_tmp=sensor.o3.o3_now-221;   //*1000
//	 }
//	 else
//	 {
//		  o3_tmp=0;
//	 }
//	 if(sensor.tem.tem_now>=200)
//		 {
//				temp=sensor.tem.tem_now-200;      //*10
//				temp /= 10;                    // /10
//		 }
//	 else
//	 {
//			temp=200-sensor.tem.tem_now;
//			temp /= 10; 
//	 }
//	 temp*=3;                       //*1000
//	 temp=93*(temp+1000);
//	 sensor.o3.o3_ppm=(o3_tmp*1000000/temp); //扩大100万倍
//	 sensor.o3.o3_ug = sensor.o3.o3_ppm * 2143 / 1000; // 1 ppm = 2143ug/m3;
//	
//	
//}
////===============================================================













































