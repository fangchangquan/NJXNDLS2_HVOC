#include "no2sensor.h"
#include "adc.h"



u16 data_queue_no2[MAX_DATA_QUEUE_DEPTH + 1] = {0};



void enqueue_data_no2(u16 e)
{
//		static u16 data_queue[MAX_DATA_QUEUE_DEPTH + 1] = {0};
		static u16 queue_front = 0;//队首
		static u16 queue_rear = 0;//队尾
		//u16 temp = 0;
		
    if(queue_front == queue_rear)
    {
        data_queue_no2[queue_front] = e;
    }
    
    data_queue_no2[queue_rear] = e;
		//如果队列满了 
		if((queue_rear+1)%(MAX_DATA_QUEUE_DEPTH+1)==queue_front)
		{
			//queue_rear = (queue_rear + 1) % (MAX_DATA_QUEUE_DEPTH + 1);//队尾变成队首
			//temp = data_queue_o3[queue_front];//出队
			
			queue_front =  (queue_front + 1) % (MAX_DATA_QUEUE_DEPTH+1);
		}
	
    queue_rear = (queue_rear + 1) % (MAX_DATA_QUEUE_DEPTH + 1);//队尾后移1 
    
}

void NO2_Receive_Process(void)
{
		u32 no2_tmp=0;
		u32 temp = 0;
	
	 if((sensor.no2.no2_dat>0)&&(sensor.no2.no2_dat<=3300))
	 {
		  if(sensor.no2.no2_dat > 339)
			{
				no2_tmp = sensor.no2.no2_dat - 339;
			}
			else
			{
				no2_tmp = 0;
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
		 temp*=3;                       //分母*1000
		 temp=67*(temp+1000); //分母*1000，系数为0.0668
		 sensor.no2.no2_ppm=(no2_tmp*1000000/temp);//分子扩大100万倍，最终值扩大了1000倍
		 sensor.no2.no2_ug = (2054) * sensor.no2.no2_ppm / 1000; //1 ppm = 2054ug/m3; 最终值扩大了100倍
		 
		 enqueue_data_no2(sensor.no2.no2_ug);
			
			
		 NO2_Filter_Process(data_queue_no2);
			
	 }
	  
}		

void NO2_Filter_Process(u16 *buf)
 {
	 
	 
	  sensor.no2.no2_now=Filter_Temp_1(buf,MAX_DATA_QUEUE_DEPTH);
	 
 }

void NO2_Data_Process(void)
{
	 NO2_Receive_Process();
}


//u16 no2_group[50]={0};


//void NO2_Receive_Process(void)
//{
//	 static uchar no2_cnt=0;
//	
//	 if((sensor.no2.no2_dat>0)&&(sensor.no2.no2_dat<=3300))
//	 {
//		  if(no2_cnt<50)
//			{
//				 no2_group[no2_cnt]=sensor.no2.no2_dat;
//				 no2_cnt++;
//			}
//			else
//			{
//				 no2_cnt=0;
//				
//				 NO2_Filter_Process(no2_group);
//			}
//	 }
//	  
// }		

//void NO2_Filter_Process(u16 *buf)
// {
//	 
//	 
//	  sensor.no2.no2_now=Filter_Temp(buf,50);
//	 
// }

//void NO2_Data_Process(void)
//{
//	 u32 no2_tmp=0;
//	 u32 temp=0;
//	 
//	 NO2_Receive_Process();
//	
//	 if(sensor.no2.no2_now>=340)
//	 {
//		 
//	   no2_tmp=sensor.no2.no2_now-340;//分子*1000倍
//	 }
//	 else
//	 {
//		  no2_tmp=0;
//	 }
//	 
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
//	 temp*=3;                       //分母*1000
//	 temp=67*(temp+1000); //分母*1000，系数为0.0668
//	 sensor.no2.no2_ppm=(no2_tmp*1000000/temp);//分子扩大100万倍，最终值扩大了1000倍
//	 sensor.no2.no2_ug = (2054) * sensor.no2.no2_ppm / 1000; //1 ppm = 2054ug/m3; 最终值扩大了100倍
//	 //sensor.no2.no2_ug /= 1000;
//	
//}

