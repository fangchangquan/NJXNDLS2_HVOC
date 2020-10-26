//------------------------- Include -----------------------//

//#include "..\include\main.h"


#include "main.h"
#include "motor.h"
#include "net.h"
#include "sick.h"
#include "led.h"
#include "w5500.h"
#include "pulse.h"
#include "ult.h"
#include "tftp.h"
#include "flash_if.h"
#include "i2c.h"
#include "stmflash.h"

//#define IAP_ENABLE

//------------------------- Define ------------------------//

//------------------------ Variable -----------------------//

//------------------- Function Prototype ------------------//

//------------------------ Function -----------------------//

Flag_Def flag;

int receive_length;;//receive_length1,receive_length2;
unsigned char receive_buffer[1024];//receive_buffer1[1024],receive_buffer2[1024];
unsigned char send_buffer[1024];
float sick_value;
#define USART_RS232_3
 
uint8_t g_socket_rcv_buf[MAX_MTU_SIZE];
uint32_t tftp_server;
uint8 filename[20];
uint32_t t1, t2;




//------------------------ Function -----------------------//
/*
 * Name                : Motor_control_conmand
 * Description         : ---
 * --------------------
 */
 
u8  Motor_Txbuf1[12] = {'s',' ','r','0','x','3','6',' ','2','0','0','0'};                //s r0x36 2000          73, 20, 72, 30, 78, 33, 36, 20, 32, 30, 30, 30
u8  Motor_Txbuf2[15] = {'s',' ','r','0','x','2','f',' ','2','0','0','0','0','0','0'};    //s r0x2f 2000000       73, 20, 72, 30, 78, 32, 66, 20, 32, 30, 30, 30, 30, 30, 30 
u8  Motor_Txbuf3[10] = {'s',' ','r','0','x','2','4',' ','1','1'};                        //s r0x24 11            73, 20, 72, 30, 78, 32, 34, 20, 31, 31 
u8  Motor_Txbuf4[ 9] = {'s',' ','r','0','x','2','4',' ','1'};                            //s r0x24 1             73, 20, 72, 30, 78, 32, 34, 20, 31 
u8  Motor_Txbuf5[ 9] = {'s',' ','r','0','x','2','4',' ','0'};                            //s r0x24 0             73, 20, 72, 30, 78, 32, 34, 20, 30 
u8  Motor_Txbuf6[12] = {'s',' ','r','0','x','3','7',' ','2','0','0','0'};                //s r0x37 2000          73, 20, 72, 30, 78, 33, 37, 20, 32, 30, 30, 30
u8  Motor_Txbuf7[12] = {'s',' ','r','0','x','2','f',' ','5','0','0','0'};                //s r0x2f 5000          73, 20, 72, 30, 78, 32, 66, 20, 35, 30, 30, 30 
u8  Motor_Txbuf8[ 7] = {'g',' ','r','0','x','1','8'};                                    //g r0x18               67, 20, 72, 30, 78, 31, 38 

/*
 * Name                : get_send_tcp_message
 * Description         : ---
 */
 
 

//===============================================================
void TIM3_Initial(void)                                  //1ms
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
NVIC_InitTypeDef NVIC_InitStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

TIM_DeInit(TIM3);
TIM_TimeBaseStructure.TIM_Period=999;			               //arr
TIM_TimeBaseStructure.TIM_Prescaler=83;		               //psc,
TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);

TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_Trigger,ENABLE);

NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;            //TIM3中断
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;  //先占优先级0级
NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;         //从优先级3级
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;            //IRQ通道被使能
NVIC_Init(&NVIC_InitStructure);                          //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
TIM_Cmd(TIM3,ENABLE);
	
flag.w5500_init_flag=0;
flag.zj_startup_flag=0;
}
//===============================================================
void System_Clock_Config(void)
{
ErrorStatus HSEStartUpStatus;
RCC_DeInit();											  
RCC_HSEConfig(RCC_HSE_ON);
HSEStartUpStatus=RCC_WaitForHSEStartUp();
while(HSEStartUpStatus!=SUCCESS);
if(HSEStartUpStatus== SUCCESS)
{
	
RCC_PLLCmd(DISABLE);
	
PWR->CR |= PWR_CR_VOS;
	
RCC_HCLKConfig(RCC_SYSCLK_Div1);                       // HCLK = SYSCLK 
    
RCC_PCLK2Config(RCC_HCLK_Div2);	                       //PCLK2 = HCLK 
    
RCC_PCLK1Config(RCC_HCLK_Div4);	                       //PCLK1 = HCLK/2
    
        
RCC_PLLConfig(RCC_PLLSource_HSE,8,336,2,4);   //  PLLCLK =(( 8MHz /8)* 336)/2 = 168 MHz 
    
RCC_PLLCmd(ENABLE);				                       // Enable PLL 
   
while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)     // Wait till PLL is ready
{
;
}
    
RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	           //Select PLL as system clock source 
   
while(RCC_GetSYSCLKSource()!= 0x08);		           // Wait till PLL is used as system clock source 

}
}
//===============================================================
unsigned char SysClockSet(unsigned char OSC, unsigned char Clock)
{
  unsigned int  temp = 0, PLLM = 0, PLLN = 0, PLLP = 0, PLLQ = 0;
        unsigned int        OSC_Sta;
        unsigned int        OSC_VALUE         = HSI_VALUE;
        unsigned int        OSC_RDY           = RCC_CR_HSIRDY; 
        unsigned int        OSC_ON            = RCC_CR_HSION; 
        unsigned char       OSC_ERROR         = HSI_error; 
        unsigned int        OSC_OK            = HSI_OK;
        unsigned int        OSC_SW            = RCC_CFGR_SW_HSI;
        unsigned int        OSC_SWS           = RCC_CFGR_SWS_HSI;
        unsigned int        OSC_SRC           = RCC_PLLCFGR_PLLSRC_HSI;        
        if(OSC == HSE)  
                        {
                        OSC_VALUE         = HSE_VALUE;
                        OSC_RDY           = RCC_CR_HSERDY; 
                        OSC_ON            = RCC_CR_HSEON; 
                        OSC_ERROR         = HSE_error; 
                        OSC_OK            = HSE_OK;
                        OSC_SW            = RCC_CFGR_SW_HSE;
                        OSC_SWS           = RCC_CFGR_SWS_HSE;
                        OSC_SRC           = RCC_PLLCFGR_PLLSRC_HSE;        
                        }
        else if(OSC != HSI) return(Parameter_error);
  
        switch (Clock)
                        {
                        case         0 :  PLLM = (OSC_VALUE/1000000);         PLLN = 96;          PLLP = 8;                PLLQ = 2;         break;                //12MHz
                        case         1 :  PLLM = (OSC_VALUE/1000000);         PLLN = 96;          PLLP = 6;                PLLQ = 2;         break;                //16MHz
                        case         2 :  PLLM = (OSC_VALUE/2000000);         PLLN = 72;          PLLP = 8;                PLLQ = 3;         break;                //18MHz
                        case         3 :  PLLM = (OSC_VALUE/2000000);         PLLN = 96;          PLLP = 8;                PLLQ = 4;         break;                //24MHz
                        case         4 :  PLLM = (OSC_VALUE/2000000);         PLLN = 120;         PLLP = 8;                PLLQ = 5;         break;                //30MHz
                        case         5 :  PLLM = (OSC_VALUE/2000000);         PLLN = 96;          PLLP = 6;                PLLQ = 4;         break;                //32MHz
                        case         6 :  PLLM = (OSC_VALUE/2000000);         PLLN = 144;         PLLP = 8;                PLLQ = 6;         break;                //36MHz
                        case         7 :  PLLM = (OSC_VALUE/2000000);         PLLN = 120;         PLLP = 6;                PLLQ = 5;         break;                //40MHz
                        case         8 :  PLLM = (OSC_VALUE/2000000);         PLLN = 168;         PLLP = 8;                PLLQ = 7;         break;                //42MHz
                        case         9 :  PLLM = (OSC_VALUE/2000000);         PLLN = 192;         PLLP = 8;                PLLQ = 8;         break;                //48MHz
                        case         10:  PLLM = (OSC_VALUE/2000000);         PLLN = 216;         PLLP = 8;                PLLQ = 9;         break;                //54MHz
                        case         11:  PLLM = (OSC_VALUE/2000000);         PLLN = 168;         PLLP = 6;                PLLQ = 7;         break;                //56MHz
                        case         12:  PLLM = (OSC_VALUE/2000000);         PLLN = 120;         PLLP = 4;                PLLQ = 5;         break;                //60MHz
                        case         13:  PLLM = (OSC_VALUE/2000000);         PLLN = 192;         PLLP = 6;                PLLQ = 8;         break;                //64MHz
                        case         14:  PLLM = (OSC_VALUE/2000000);         PLLN = 144;         PLLP = 4;                PLLQ = 6;         break;                //72MHz
                        case         15:  PLLM = (OSC_VALUE/2000000);         PLLN = 240;         PLLP = 6;                PLLQ = 10;        break;                //80MHz
                        case         16:  PLLM = (OSC_VALUE/2000000);         PLLN = 168;         PLLP = 4;                PLLQ = 7;         break;                //84MHz
                        case         17:  PLLM = (OSC_VALUE/2000000);         PLLN = 192;         PLLP = 4;                PLLQ = 8;         break;                //96MHz
                        case         18:  PLLM = (OSC_VALUE/2000000);         PLLN = 216;         PLLP = 4;                PLLQ = 9;         break;                //108MHz
                        case         19:  PLLM = (OSC_VALUE/2000000);         PLLN = 120;         PLLP = 2;                PLLQ = 5;         break;                //120MHz
                        case         20:  PLLM = (OSC_VALUE/2000000);         PLLN = 144;         PLLP = 2;                PLLQ = 6;         break;                //144MHz
                        case         21:  PLLM = (OSC_VALUE/2000000);         PLLN = 168;         PLLP = 2;                PLLQ = 7;         break;                //168MHz
                        case         22:  PLLM = (OSC_VALUE/2000000);         PLLN = 192;         PLLP = 2;                PLLQ = 8;         break;                //192MHz
                        case         23:  PLLM = (OSC_VALUE/2000000);         PLLN = 216;         PLLP = 2;                PLLQ = 9;         break;                //216MHz
                        case         24:  PLLM = (OSC_VALUE/2000000);         PLLN = 240;         PLLP = 2;                PLLQ = 10;        break;                //240MHz
//                        case        25:  PLLM = (OSC_VALUE/2000000);         PLLN = 260;         PLLP = 2;                PLLQ = 11;         break;                //260MHz
                        default:          PLLM = (OSC_VALUE/2000000);         PLLN = 240;         PLLP = 2;                PLLQ = 10;        break;                //240MHz
                        }
        //????????,????????????
        OSC_Sta = RCC->CR & OSC_RDY;
        if(OSC_Sta == 0)
                        {
                        RCC->CR |= OSC_ON;
                        do
												{
												 OSC_Sta = RCC->CR & OSC_RDY;
												 temp++;
												}while((OSC_Sta == 0) && (temp < 0x0600));
                        if(OSC_Sta == 0)return(OSC_ERROR); //????
                        }
        //????????????????
        RCC->CFGR &= (~(RCC_CFGR_SW));
        RCC->CFGR |= OSC_SW;
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != OSC_SWS);

        //??PLL
        RCC->CR &= (~(RCC_CR_PLLON));                                //???PLL
        RCC->CR &= (~(RCC_CR_PLLI2SON));                //??PLLI2S
        RCC->PLLCFGR = PLLM | (PLLN << 6) | (((PLLP >> 1) -1) << 16) | (PLLQ << 24) | (OSC_SRC);

        //??PLL,?????
        RCC->CR |= RCC_CR_PLLON;
        while((RCC->CR & RCC_CR_PLLRDY) == 0); 
                        
        //??PLLI2S,?????                
        RCC->CR |= RCC_CR_PLLI2SON;        
        while((RCC->CR & RCC_CR_PLLI2SRDY) == 0); 
                        
        //???????PLL?????
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
        RCC->CFGR |= RCC_CFGR_SW_PLL;
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
        return(OSC_OK);
                        
}
//===============================================================
uint32_t SysClockGet(void)
{
  uint32_t    PLLM = 0, PLLN = 0, PLLP = 0, PLLSRC = 0;
        
        if                        ((RCC->CFGR & RCC_CFGR_SWS ) == RCC_CFGR_SWS_HSE) return  HSE_VALUE;
        else if        ((RCC->CFGR & RCC_CFGR_SWS ) == RCC_CFGR_SWS_HSI) return  HSI_VALUE;
        else                 
                        {
                                PLLM                 = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
                                PLLN                 = ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN)>>6);
                                PLLP                 = ((((RCC->PLLCFGR & RCC_PLLCFGR_PLLP)>>16)+1)<<1);
                                PLLSRC               = RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC;
                                if(PLLSRC == 0)      return (((HSI_VALUE * PLLN) / PLLM )/ PLLP);
                                else                 return (((HSE_VALUE * PLLN) / PLLM )/ PLLP);
                        }
}


//===============================================================
void T200ms_Control(void)
{

	 if(t_flag.t200ms_flag==1)
	 {
		  t_flag.t200ms_flag=0;
		 		  
		  RS485_Send_Data();			  			

	 }	 
}
//===============================================================
void T30ms_Control(void)
{
	 if(t_flag.t30ms_flag==1)
	 {
		  t_flag.t30ms_flag=1;
	    
	
		  Motor_Lift_Abs_Control();

		  Motor_Lift_Control();

	 }	 
}


//===============================================================
void T50ms_Control(void)
{
   if(t_flag.t50ms_flag==1)
	 {
		  t_flag.t50ms_flag=0;
		 		  
		  Motor_Arrive_To_Point_Control(); 
		  Motor_Move_Abs_Control();
		  Motor_Move_Control();
	 }
 }
//===============================================================
//===============================================================
//===============================================================
//===============================================================
void T1s_Control(void)
{
	 if(t_flag.t1s_flag==1)
	 {
		  t_flag.t1s_flag=1;
		 
		  Write_Socket0_Data();
		  Write_Socket1_Data();
		  Write_Save_Data();
		 
		  Load_Socket0_Data();
		  Load_Socket1_Data();
		  Load_Para();
	 }	 
}
//===============================================================
void Net_Reconnect_Clear_Timer_Socket0(void)
{
	  if( flag.kpa_tick_flag0==0)
		{
			  flag.kpa_tick_flag0=1;
		}
		message.kpa_no_data_tick0=0;
		message.kpa_send_tick0=0;
}	
//===============================================================
void Net_Reconnect_Clear_Timer_Socket1(void)
{
	  if( flag.kpa_tick_flag1==0)
		{
			  flag.kpa_tick_flag1=1;
		}
		message.kpa_no_data_tick1=0;
		message.kpa_send_tick1=0;
		
}
//===============================================================
void Net_Reconnect_Send_Socket0(void)
{
	  if( flag.kpa_send_flag0==1)
		{
			  flag.kpa_send_flag0=0;
			  
			  message.kpa_send_tick0=0;
		    setkeepalive(0);
			
			  flag.socket0_heart_start_flag=1;		
        
			  
		}
}	

//===============================================================
void Heart_Socket0_Timer(void)
{	 
	 static uchar socket0_cnt=0;
	
	 Kpa_Timer_Count_Socket0();
	 if(flag.socket0_heart_start_flag==1)
	 {
		  socket0_cnt++;
		  if(socket0_cnt>=5)
			{
				 socket0_cnt=0;
				 flag.socket0_heart_start_flag=0;
				 flag.socket0_heart_stop_flag=1;
			}
		}
}

//===============================================================
void CLose_Socket0(void)
{
	 if(flag.socket0_heart_stop_flag==1)
	 { 
	    if((getSn_SR(0)==SOCK_CLOSED)||(getSn_SR(0)==SOCK_CLOSE_WAIT))
			{
				 message.socket0_cnt++;
			   e2.socket0_save=1;
				 flag.socket0_heart_stop_flag=0;
			}
	 }
}
//===============================================================
void Net_Reconnect_Send_Socket1(void)
{
	  if( flag.kpa_send_flag1==1)
		{
			  flag.kpa_send_flag1=0;
			  
			  message.kpa_send_tick1=0;
		    setkeepalive(1);	

			  flag.socket1_heart_start_flag=1;
        		
		}
}	

//===============================================================
void Heart_Socket1_Timer(void)
{
	 static uchar socket1_cnt=0;
	 
	 Kpa_Timer_Count_Socket1();
	 if(flag.socket1_heart_start_flag==1)
	 {
		  socket1_cnt++;
		  if(socket1_cnt>=5)
			{
				 socket1_cnt=0;
				 flag.socket1_heart_start_flag=0;
				 flag.socket1_heart_stop_flag=1;
			}
		}
}
//===============================================================
void CLose_Socket1(void)
{
	 if(flag.socket1_heart_stop_flag==1)
	 {
		  
	    if((getSn_SR(1)==SOCK_CLOSED)||(getSn_SR(1)==SOCK_CLOSE_WAIT))
			{
				 message.socket1_cnt++;
			   e2.socket1_save=1;
				 flag.socket1_heart_stop_flag=0;
			}
	 }
}
//===============================================================
void Kpa_Timer_Count_Socket0(void)
{
	 if(flag.kpa_tick_flag0==1)
	 {
		  message.kpa_no_data_tick0++;
		  if(message.kpa_no_data_tick0>=NO_DATA_PERIOD)
			{
				 message.kpa_no_data_tick0=0;
				 flag.kpa_tick_flag0=0;
				 message.kpa_send_tick0++;
				 if(message.kpa_send_tick0>=KPA_SEND_PERIOD)
				 {
					  message.kpa_send_tick0=0;
					  flag.kpa_send_flag0=1;
				 }
			}
	 }
	 
}
//===============================================================
void Kpa_Timer_Count_Socket1(void)
{
	 if(flag.kpa_tick_flag1==1)
	 {
		  message.kpa_no_data_tick1++;
		  if(message.kpa_no_data_tick1>=NO_DATA_PERIOD)
			{
				 message.kpa_no_data_tick0=1;
				 flag.kpa_tick_flag0=1;
				 message.kpa_send_tick1++;
				 if(message.kpa_send_tick1>=KPA_SEND_PERIOD)
				 {
					  message.kpa_send_tick1=0;
					  flag.kpa_send_flag1=1;
				 }
			}
	 }
	 
}
//===============================================================
void Net_Reconect0(void)
{
	 if(flag.socket0_close == 1)
		{
			
			close(0);

			flag.socket0_close = 0;
			re_cnt++;
		}
}
//===============================================================
void Net_Reconect1(void)
{
	 if(flag.socket1_close == 1)
		{
			
			close(1);

			flag.socket1_close = 0;
		}
}
//===============================================================
//===============================================================
void Sick_Net_Client(void)
{
	 
	 switch(getSn_SR(2))								  				         /*获取socket的状态*/
	  {
		   
		   case SOCK_INIT  :													        	         /*socket处于初始化状态*/
			                  if(connect(2,remote_ip,2112)!=SOCK_OK)               /*socket连接服务器*/ 
												{
													 disconnect(2);
												}
		                    break;
		
		   case SOCK_ESTABLISHED: 												             /*socket处于连接建立状态*/
			                       if(getSn_IR(2) & Sn_IR_CON)
			                       {
				                         setSn_IR(2, Sn_IR_CON); 							         /*清除接收中断标志位*/
			                       }
														  
														 message.socket_mode=2;
														 t_flag.sick_net_cnt=0;
														 
		                          Sick_Position_Init();

														  flag.sick_send_flag=1;
														  Sick_Line_Sensor_Get_Position();
														 
			                        receive_length=getSn_RX_RSR(2); 								  	         /*定义len为已接收数据的长度*/
			                        if(receive_length>0)
			                        {
																 flag.sick_receive_flag=1;
				                         recv(2,receive_buffer,receive_length); 							   		         /*接收来自Server的数据*/
				                    														 
																 Sick_Receive_Data();

			                        }	
				                      								
		                          break;
			
		    case SOCK_CLOSE_WAIT: 											    	         /*socket处于等待关闭状态*/
			                       close(2);//disconnect
		                         break;
				
        case SOCK_CLOSED:											        		         /*socket处于关闭状态*/
			                   socket(2,Sn_MR_TCP,6000,Sn_MR_ND);
				                 if(local_port3>6000)
												 {
													  local_port3=2000;
												 }
												 flag.sick_send_flag=0;
												 
		                     break;
				default:         break;

	}
}
//===============================================================
void get_send_tcp_message(void)
{
	
		//处理TCP server信息
	  /*SOCKET 0 */
		switch(getSn_SR(0))                          
		{
			 case SOCK_INIT:
				              listen(0);		 
				              break;
			 case SOCK_ESTABLISHED:

				 if(getSn_IR(0) & Sn_IR_CON)
				 {
					 setSn_IR(0, Sn_IR_CON); 
           
           flag.kpa_tick_flag0=0;
					 flag.kpa_send_flag0=0;
					 message.kpa_no_data_tick0=0;
					 message.kpa_send_tick0=0;					 
					}
				 message.socket_mode=0;
					
				 flag.w5500_init_flag=1;			
				 receive_length = getSn_RX_RSR(0);       
				 if(receive_length>=0)
				 {
					 recv(0,receive_buffer,receive_length); 
           flag.build_con_flag=1;
					 
					 Check_Message();
					 Send_Message_Auto();
					 delay_xms(10);//10
					 				 
//					 Net_Reconnect_Clear_Timer_Socket0();
					 
					 socket0_cnt=0;
					}
//					 Net_Reconnect_Send_Socket0();

					socket0_cnt++;
					////////////////
					// 此处延时1秒钟
					////////////////
				// delay_ms(1000);
				 break;
			 case SOCK_SYNSENT:
				                 break;
			 case SOCK_CLOSE_WAIT:
				 close(0);//disconnect                            /*断开连接*/
				 break;
			 case SOCK_CLOSED:
				 
				 socket(0,Sn_MR_TCP,60000,Sn_MR_ND);       /*打开socket0的60000端口*/		

			 
				 break;
			 

		}		
    

		delay_xms(1);
		
		/*SOCKET 1*/
	  switch(getSn_SR(1))                          
		{
			 case SOCK_INIT:
				              listen(1);		 
				              break;
			 case SOCK_ESTABLISHED:
				 if(getSn_IR(1) & Sn_IR_CON)
				 {
					 setSn_IR(1, Sn_IR_CON); 
           
           flag.kpa_tick_flag0=0;
					 flag.kpa_send_flag0=0;
					 message.kpa_no_data_tick0=0;
					 message.kpa_send_tick0=0;					 
					}
				 message.socket_mode=1;
					
				 flag.w5500_init_flag=1;	
					
				 receive_length = getSn_RX_RSR(1);       
				 if(receive_length>=0)
				 {
					 recv(1,receive_buffer,receive_length); 
					 Check_Control_Message();
					 Receive_Message_Data();
					 Receive_Message_Process();					 
					 
					 Receive_Lift_Clear_Zero_Cmd();
           Receive_Emergency_Stop_Cmd();
					 
					 Receive_Cmd_Data_Process();			 
					 Send_Control_Cmd_Ack();
					 delay_xms(10);
					 
//					 Net_Reconnect_Clear_Timer_Socket1();
					 socket1_cnt=0;
					}
				 
//					 Net_Reconnect_Send_Socket1();

					socket1_cnt++;
					////////////////
					// 此处延时1秒钟
					////////////////
				// delay_ms(1000);
				 break;
			 case SOCK_SYNSENT:
				                 break;
			 case SOCK_CLOSE_WAIT:
				 close(1);//disconnect                            /*断开连接*/
				 break;
			 case SOCK_CLOSED:
				 
				 socket(1,Sn_MR_TCP,60001,Sn_MR_ND);       /*打开socket0的60000端口*/
				 
			 		 
				 break;
		}		
		
}

//===============================================================
void setkeepalive(SOCKET s)//自动发送

{

   IINCHIP_WRITE(Sn_KPALVTR(s),0x01);//1*5秒

}
//===============================================================
void Send_Keepalive(SOCKET s)//手动发送

{
IINCHIP_WRITE(Sn_KPALVTR(s),0x00);//可以放在初始化
IINCHIP_WRITE(Sn_CR(s),Sn_CR_SEND_KEEP);

return;

}
//===============================================================
void Soft_Heart_Socket0_Timer(void)
{
	 
	 static uchar soft_socket0_cnt=0;
	
	 if(flag.soft_st0_heart_start_flag==1)
	 {
		  soft_socket0_cnt++;
		  if(soft_socket0_cnt>=5)
			{
				 soft_socket0_cnt=0;
				 flag.soft_st0_heart_start_flag=0;
         flag.soft_st0_heart_stop_flag=1;				
			}
		}
}
//===============================================================
void Soft_Heart_Socket1_Timer(void)
{
	 
	 static uchar soft_socket1_cnt=0;
	
	 if(flag.soft_st1_heart_start_flag==1)
	 {
		  soft_socket1_cnt++;
		  if(soft_socket1_cnt>=5)
			{
				 soft_socket1_cnt=0;
				 flag.soft_st1_heart_start_flag=0;
         flag.soft_st1_heart_stop_flag=1;			
			}
		}
}
//===============================================================
void Soft_PHY_Check(void)
{
	 if((flag.soft_st0_heart_stop_flag==1)||(flag.soft_st1_heart_stop_flag==1))
		{
			   PHY_check();
			   
				 flag.kpa_tick_flag0=0;
				 flag.kpa_send_flag0=0;
				 message.kpa_no_data_tick0=0;
			   message.kpa_send_tick0=0;
         flag.soft_st0_heart_stop_flag=0;
				 flag.kpa_tick_flag1=0;
				 flag.kpa_send_flag1=0;
				 message.kpa_no_data_tick1=0;
			   message.kpa_send_tick1=0;
         flag.soft_st1_heart_stop_flag=0;
			   message.socket_cnt++;
			   e2.soft_socket_save=1;
			
		}
}
//===============================================================
//===============================================================

//===============================================================
void IWDG_Init(u8 prer,u16 rlr)
{
IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  
while (IWDG->SR&0x01);
IWDG_SetPrescaler(prer);  
while (IWDG->SR&0x02);
IWDG_SetReload(rlr); 

IWDG_ReloadCounter();  

IWDG_Enable();  
}

//===============================================================
void IWDG_Feed(void)
{
IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  
IWDG_ReloadCounter();
}
//===============================================================
void W5500_Para_Init(void)
{
	 w5500.initialize();
	 delay_xms(100);
	
}
//===============================================================
void tftp_iap()
{
	uint8_t ret;
	
	FLASH_Unlock();
	t1 = (*(__IO uint32_t*)USER_FLAG_ADDR);
	t2 = (*(__IO uint32_t*)(USER_FLAG_ADDR + 4));
	
	if (t1 != 0xAAAABBBB && (t2 == 0xCCCCDDDD))	
		reboot_app();
	
	FLASH_ProgramWord(USER_FLAG_ADDR + 4, 0xCCCCDDDD);		
	FLASH_Lock();
	
	delay_ms(3000);
	memcpy(filename, "robot.bin", sizeof("robot.bin"));
			
	tftp_server = (remote_ip3[0] << 24) | (remote_ip3[1] << 16) | (remote_ip3[2] << 8) | (remote_ip3[3]);
	TFTP_read_request(tftp_server,filename);
	
	while(1)
	{	
		ret = TFTP_run();							
		
		if(ret != TFTP_PROGRESS)
			break;
	}
	if(ret == TFTP_SUCCESS)
	{
		FLASH_Unlock();
		FLASH_ProgramWord(USER_FLAG_ADDR, 0);		
		
		FLASH_Lock();
		
		reboot_app();	
	}			
}
//===============================================================
void reboot_app(void)
{
	uint32_t ApplicationAddress =  APPLICATION_ADDRESS;
	
	/* Test if user code is programmed starting from address "APPLICATION_ADDRESS" */
  if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) != 0x20000000) return;
	
  pFunction Jump_To_Application;
  uint32 JumpAddress;
	JumpAddress = *(vu32*) (ApplicationAddress + 0x00000004);
	Jump_To_Application = (pFunction) JumpAddress;
	//initialize user application's stack pointer
	 __set_MSP(*(__IO uint32_t *)ApplicationAddress);	
	Jump_To_Application();
}

//===============================================================	
void System_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  SystemInit();
	delay_init(168);
	System_Clock_Config();
		
  TIM3_Initial();
	spi1.initialize();
	W5500_Para_Init();
	USART_Initial();
  Net_Init();
 
	socket(2,Sn_MR_TCP,5000,0x00);
	
//	Pulse_GPIO_Init();
	Motor_Para_Init();
  
	Open_Pin_Init();
	
	I2C_GPIO_Init();
	
	delay_xms(500);
	delay_xms(500);

	POWER12_24V_CTL=LOW; 

	delay_xms(10);
	
  #ifdef IAP_ENABLE	
	TFTP_init(SOCK_TFTP,g_socket_rcv_buf);
	#endif
	
	EEPROM_Init();
	delay_xms(10);
	Load_Para();
}
	
	
/*
 * Name                : main
 * Description         : ---
 * Author              : xzh
 *
 * History
 * --------------------
 * Rev                 : 0.00
 * Date                : 2017.09.20
 * 
 * create.
 * --------------------
 */
//===============================================================

void V_Motor_Init(void)
{
    V_Motor_Init_Send();	
	
}		
//===============================================================
void System_Zj_Startup_Process(void)
{
	 if(flag.zj_startup_flag==1)
	 {
		  flag.zj_startup_flag=0;
		  
		  System_Init();
	 }

	 
}
//===============================================================
int main(void)
{	
	System_Init();
	FLASH_Init();
  delay_xms(10);
	e2.flash_ver_save=1;
	if(e2.flash_same_flag==0)
	{
	   Write_Flash_Ver(FLASH_VER_ADDR);
		 delay_xms(10);
	}
	
	Flash_Read_Ver(FLASH_VER_ADDR,1);
	
//	IWDG_Init(5,3000);//10秒钟
	
	while(1)
	{	
		#ifdef IAP_ENABLE
	  tftp_iap();
    #else
		V_Motor_Init();
		
	  get_send_tcp_message();
	  Sick_Net_Client();	
		Net_Reconect0();
		Net_Reconect1();
		
    T200ms_Control();
	
//	  T50ms_Control();
//    T30ms_Control();
		
    Motor_Arrive_To_Point_Control();
		delay_xms(30);//30		
		Motor_Move_Abs_Control();
		delay_xms(30);//30		
		Motor_Move_Control();
		delay_xms(30);//30		
			
		Motor_Lift_Abs_Control();
		delay_xms(30);//30
		Motor_Lift_Control();		
		delay_xms(15);

    Motor_Error_Get(5,rs485.rs485_tx_buf5);
//		delay_xms(5);
		Motor_Error_Get(4,rs485.rs485_tx_buf4);

		
    #endif
	}
	 
}






	
	
