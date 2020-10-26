#include "so2sensor.h"
#include "adc.h"


u16 data_queue[MAX_DATA_QUEUE_DEPTH + 1] = {0};
//static u16 queue_front = 0;//����
//static u16 queue_rear = 0;//��β
//static uchar so2_cnt = 0;


void enqueue_data(u16 e)
{
//		static u16 data_queue[MAX_DATA_QUEUE_DEPTH + 1] = {0};
		static u16 queue_front = 0;//����
		static u16 queue_rear = 0;//��β
		//u16 temp = 0;
		
    if(queue_front == queue_rear)
    {
        data_queue[queue_front] = e;
    }
    
    data_queue[queue_rear] = e;
		//����������� 
		if((queue_rear+1)%(MAX_DATA_QUEUE_DEPTH+1)==queue_front)
		{
			//queue_rear = (queue_rear + 1) % (MAX_DATA_QUEUE_DEPTH + 1);//��β��ɶ���
			//temp = data_queue[queue_front];//����
			
			queue_front =  (queue_front + 1) % (MAX_DATA_QUEUE_DEPTH+1);
		}
	
    queue_rear = (queue_rear + 1) % (MAX_DATA_QUEUE_DEPTH + 1);//��β����1 
    
}


void SO2_Receive_Process(void)
{
	 u32 so2_tmp=0;
	 u32 temp=0;
	 
	 if((sensor.so2.so2_dat>0)&&(sensor.so2.so2_dat<=3300))
	 {
		 if(sensor.so2.so2_dat > 689)
		 {
				so2_tmp=sensor.so2.so2_dat-689;//*1000
		 }
		 else
		 {
			  so2_tmp = 0;
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
		 temp=162*(temp+1000); //ϵ��Ϊ2.67���ң���·�Ŵ�6����ԼΪ16.2��������10��Ϊ162
		 sensor.so2.so2_ppm=(so2_tmp*1000000/temp); //����100�򱶣�ʵ������100��
		 sensor.so2.so2_ug = sensor.so2.so2_ppm * (2857)/100; // 1 ppm = 2857ug/m3;
		 
		 //500msһ�����ݣ����볤��ΪMAX_DATA_QUEUE_DEPTH�Ķ����У���ƽ��ֵ����ȥһ������ȥһ�������������ȶ�
		 enqueue_data(sensor.so2.so2_ug);
		 
		 SO2_Filter_Process(data_queue);
		 
 }
}	 

 void SO2_Filter_Process(u16 *buf)
 {
	  sensor.so2.so2_now=Filter_Temp_1(buf,MAX_DATA_QUEUE_DEPTH);
 }
 
void SO2_Data_Process(void)
{
	SO2_Receive_Process();
}
