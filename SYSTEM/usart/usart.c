#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "motor.h"
#include "key.h"
#include "iwdg.h"
#include "light.h"
#include "commu.h"

//static uint32 plc_count=0;
/*
* 注：
* 串口1没引出，不使用
* 串口2,3,4,5是232接口
* 串口6是485接口 
*/



/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 

void _ttywrch(int ch) 
{ 
ch = ch; 
} 

//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	UART5->DR = (u8) ch;      
	return ch;
}
#endif

RS485_Def rs485;



//===============================================================
/*********************************************************************************
** 函数名称: My_Usart1_Init
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void My_Usart1_Init(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                          //设置端口为输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
	
  USART_Cmd(USART1, ENABLE);
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(USART1,USART_IT_PE,ENABLE);
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	USART_ITConfig(USART1,USART_IT_ERR,ENABLE);
	

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

}


//===============================================================
/*********************************************************************************
** 函数名称: My_Usart2_Init
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void My_Usart2_Init(u32 bound)//VOC
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
 

	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStructure); 

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
	
  USART_Cmd(USART2, ENABLE);
	
	//USART_ClearFlag(USART2, USART_FLAG_TC);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

}
//===============================================================
/*********************************************************************************
** 函数名称: My_Usart3_Init
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void My_Usart3_Init(u32 bound)//数字式NO2气体传感器
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStructure); 

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
  USART_Init(USART3, &USART_InitStructure);
	
  USART_Cmd(USART3, ENABLE); 
	
	//USART_ClearFlag(USART3, USART_FLAG_TC);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//===============================================================
/*********************************************************************************
** 函数名称: My_Usart4_Init
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void My_Usart4_Init(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_UART4);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure); 

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode  = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(UART4, &USART_InitStructure);
	
  USART_Cmd(UART4, ENABLE);
	
	//USART_ClearFlag(UART4, USART_FLAG_TC);
	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

}

//===============================================================
/*********************************************************************************
** 函数名称: My_Usart5_Init
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void My_Usart5_Init(u32 bound)//颗粒物传感器
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode  = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(UART5, &USART_InitStructure);
	
  USART_Cmd(UART5, ENABLE);
	
	//USART_ClearFlag(UART5, USART_FLAG_TC);
	
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);

}
//===============================================================
/*********************************************************************************
** 函数名称: My_Usart6_Init
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void My_Usart6_Init(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
 
	//串口6对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6);
	
	//USART6端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure); 	
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                          //设置端口为输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStructure); 
	
   //USART6 初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
  USART_Init(USART6, &USART_InitStructure);
	
  USART_Cmd(USART6, ENABLE);
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}
//===============================================================
/*********************************************************************************
** 函数名称: USART_Initial
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void USART_Initial(void)
{
	 My_Usart1_Init(9600);//485_1
   My_Usart2_Init(9600);//VOC
	 My_Usart3_Init(9600);//数字式NO2气体传感器
   My_Usart4_Init(9600);//数字式O3气体传感器
   My_Usart5_Init(9600);//颗粒物传感器
   My_Usart6_Init(19200);//485_2
	
	 RS485_DE1=IN;//OUT;
	 RS485_DE2=IN;//out
	
	 rs485.rec_complete_flag=1;
	
}
//===============================================================
/*********************************************************************************
** 函数名称: Uart1_Putchar
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/

void Uart1_Putchar(u8 ch)
{
	RS485_DE1=OUT;
	USART_ClearFlag(USART1,USART_FLAG_TXE);
  USART_ClearFlag(USART1,USART_FLAG_TC);
	
	USART_SendData(USART1, (uint8_t) ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}
	RS485_DE1=IN;	
}	

//===============================================================
/*********************************************************************************
** 函数名称: Uart1_Put_Word
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Uart1_Put_Word(u8 word[],u8 num)
{
	u8 i=0;
	
	for(i=0;i<num;i++)
	{
		Uart1_Putchar(word[i]);
		delay_us(500);
	}
}
//===============================================================
/*********************************************************************************
** 函数名称: USART1_IRQHandler
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无//(rs485.rs485_rx_buf1[0]==0x07) || (rs485.rs485_rx_buf1[0]==0x08) || (rs485.rs485_rx_buf1[0]==0x09)
*********************************************************************************/
void USART1_IRQHandler(void)
{    
	
		if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
    {
				USART_ReceiveData(USART1);
				USART_ClearFlag(USART1, USART_FLAG_ORE);
    }
		if (USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET)
    {
        USART_ReceiveData(USART1);
        USART_ClearFlag(USART1, USART_FLAG_FE);
    }
		if (USART_GetFlagStatus(USART1, USART_FLAG_NE) != RESET)
    {
        USART_ReceiveData(USART1);
        USART_ClearFlag(USART1, USART_FLAG_NE);
    }
		if (USART_GetFlagStatus(USART1, USART_FLAG_IDLE) != RESET)
    {
        USART_ReceiveData(USART1);
        USART_ClearFlag(USART1, USART_FLAG_IDLE);
    }

		
	  if(USART_GetITStatus(USART1,USART_IT_RXNE)!= RESET)//接收到数据
	  {
       USART_ClearFlag(USART1,USART_FLAG_RXNE);  
       USART_ClearITPendingBit(USART1,USART_IT_RXNE); //清除中断标志.
			 
			 enqueue_data_usart1(USART_ReceiveData(USART1));
			rs485.rx_ptr1++;
			if(rs485.rx_ptr1>=8)
			{
				
				
			}
		  // rs485.rs485_rx_buf1[rs485.rx_ptr1]=USART_ReceiveData(USART1);//USART1->DR;
			 //iwdg_feed();
//			 if((rs485.rs485_rx_buf1[0]==key.code)&&(key.code!=0))
//			 {				
//					rs485.rx_ptr1++;
//          if(rs485.rx_ptr1>=RX_BUF_SIZE1)
//			     {
//				      rs485.rx_ptr1=0;
//						 rs485.rx_plc_ptr_1 = 0;
//							rs485.rx_ok_flag1=1;//rx_fan_ok_flag1   
//						  rs485.rx_ok_flag_really=1;
//						 rs485.rx_ok_iwdg_flag = 1;
//						 
//					 }	
//					 if(rs485.rx_ptr1>=2)//添加多次判断，防止读命令“03”，对拨码开关为03的板子造成干扰。
//					 {
//							if(rs485.rs485_rx_buf1[1] == 0x00)
//						  {
//						   rs485.rx_ptr1=0;
//						  }
//				   }
//		   }
//			 rs485.rx_plc_ptr_1 ++;
//			 if(rs485.rx_plc_ptr_1 >= RX_BUF_SIZE1)
//			 {
//				 rs485.rx_plc_ptr_1 = 0;
//				 rs485.iwdg_count_flag_2 = 1;
//			 }
	 }  											 
} 



//===============================================================
/*********************************************************************************
** 函数名称: Uart2_Putchar
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Uart2_Putchar(u8 ch)
{
	USART_ClearFlag(USART2,USART_FLAG_TXE);
  USART_ClearFlag(USART2,USART_FLAG_TC);
	
	USART_SendData(USART2, (uint8_t) ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
}	
//===============================================================
/*********************************************************************************
** 函数名称: Uart2_Put_Word
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Uart2_Put_Word(u8 word[],u8 num)
{
	u8 i;
	
	for(i=0;i<num;i++)
	{
		Uart2_Putchar(word[i]);
		Delay_Ms(10);
	}
}
//===============================================================
/*********************************************************************************
** 函数名称: USART2_IRQHandler
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void USART2_IRQHandler(void)
{
	
	  if(USART_GetITStatus(USART2,USART_IT_RXNE)!= RESET)//接收到数据
	  {
       USART_ClearFlag(USART2,USART_FLAG_RXNE);  
       USART_ClearITPendingBit(USART2,USART_IT_RXNE); //清除中断标志.
		
		   rs485.rs485_rx_buf2[rs485.rx_ptr2]=USART_ReceiveData(USART2);//USART2->DR;
	
			 
			    rs485.rx_ptr2++;	
       
          if(rs485.rx_ptr2>=RX_BUF_SIZE2)
			    {
				     rs485.rx_ptr2=0;
				   
            rs485.rx_ok_flag2=1;
			    }		   
    }

}

//===============================================================
/*********************************************************************************
** 函数名称: Uart3_Putchar
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Uart3_Putchar(u8 ch)
{
	USART_ClearFlag(USART3,USART_FLAG_TXE);
  USART_ClearFlag(USART3,USART_FLAG_TC);
	
	USART_SendData(USART3, (uint8_t) ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
  {}
}	
//===============================================================
/*********************************************************************************
** 函数名称: Uart3_Put_Word
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Uart3_Put_Word(uchar word[],u8 num)
{
	u8 i;
	
	for(i=0;i<num;i++)
	{
		Uart3_Putchar(word[i]);
		delay_us(100);
	}
}
//===============================================================
/*********************************************************************************
** 函数名称: USART3_IRQHandler
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void USART3_IRQHandler(void)
{
    if(USART_GetITStatus(USART3,USART_IT_RXNE)!= RESET)//接收到数据
	  {
       USART_ClearFlag(USART3,USART_FLAG_RXNE);  
       USART_ClearITPendingBit(USART3,USART_IT_RXNE); //清除中断标志.
		
		   rs485.rs485_rx_buf3[rs485.rx_ptr3]=USART_ReceiveData(USART3);//USART3->DR;

			 rs485.rx_ptr3++;
			
				if(rs485.rx_ptr3>=RX_BUF_SIZE3)//RX_BUF_SIZE3
				{
					 rs485.rx_ptr3=0;
					rs485.rx_ok_flag3=1;
				}
//		   }
		}			   

}

//===============================================================
/*********************************************************************************
** 函数名称: Uart4_Putchar
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/

void Uart4_Putchar(u8 ch)
{
	USART_ClearFlag(UART4,USART_FLAG_TXE);
  USART_ClearFlag(UART4,USART_FLAG_TC);
	
	USART_SendData(UART4, (uint8_t) ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET)
  {}
}	
//===============================================================
/*********************************************************************************
** 函数名称: Uart4_Put_Word
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Uart4_Put_Word(uchar word[],u8 num)//升降
{
	u8 i;
	
	for(i=0;i<num;i++)
	{
		Uart4_Putchar(word[i]);
		Delay_Ms(10);//10ms
	}
}

//===============================================================
/*********************************************************************************
** 函数名称: UART4_IRQHandler
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void UART4_IRQHandler(void)
{
    if(USART_GetITStatus(UART4,USART_IT_RXNE)!= RESET)//接收到数据
	  {
       USART_ClearFlag(UART4,USART_FLAG_RXNE);  
       USART_ClearITPendingBit(UART4,USART_IT_RXNE); //清除中断标志.
		
		   rs485.rs485_rx_buf4[rs485.rx_ptr4]=USART_ReceiveData(UART4);//USART6->DR;
	
			 if(rs485.rs485_rx_buf4[0]==0xff)
			 {
			    rs485.rx_ptr4++;			
          if(rs485.rx_ptr4>=RX_BUF_SIZE4)
			    {
				     rs485.rx_ptr4=0;
             rs485.rx_ok_flag4=1;
			    }
			 }
			 
		}			   

}

//===============================================================
/*********************************************************************************
** 函数名称: Uart5_Putchar
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Uart5_Putchar(u8 ch)
{
	USART_ClearFlag(UART5,USART_FLAG_TXE);
  USART_ClearFlag(UART5,USART_FLAG_TC);
	
	USART_SendData(UART5, (uint8_t) ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET)
  {}
}	
//===============================================================
/*********************************************************************************
** 函数名称: Uart5_Put_Word
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Uart5_Put_Word(uchar word[],u8 num)//水平
{
	u8 i;
	
	for(i=0;i<num;i++)
	{
		Uart5_Putchar(word[i]);
		delay_us(500);
	}
}
//===============================================================
/*********************************************************************************
** 函数名称: UART5_IRQHandler
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void UART5_IRQHandler(void)
{
   if(USART_GetITStatus(UART5,USART_IT_RXNE)!= RESET)//接收到数据
	  {
       USART_ClearFlag(UART5,USART_FLAG_RXNE);  
       USART_ClearITPendingBit(UART5,USART_IT_RXNE); //清除中断标志.
		
		   rs485.rs485_rx_buf5[rs485.rx_ptr5]=USART_ReceiveData(UART5);//USART6->DR;
	
			
			 rs485.rx_ptr5++;	
			 if(rs485.rs485_rx_buf5[4] == 0xDA)
			 {
				 rs485.rx_ptr5=0;
			 }				 
       if(rs485.rx_ptr5>=RX_BUF_SIZE5)
			 {
				   rs485.rx_ptr5=0;
				 
           rs485.rx_ok_flag5=1;
			 }
		}			   
}
//===============================================================
/*********************************************************************************
** 函数名称: Usart6_Putchar
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
//===============================================================
void Usart6_Putchar(u8 ch)
{
	RS485_DE2=OUT;
	USART_ClearFlag(USART6,USART_FLAG_TXE);
  USART_ClearFlag(USART6,USART_FLAG_TC);

	USART_SendData(USART6, (uint8_t) ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET)
  {}		
	RS485_DE2=IN;
}	

//===============================================================
/*********************************************************************************
** 函数名称: Uart6_Put_Word
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Uart6_Put_Word(uchar word[],u8 num)//水平
{
	u8 i;
	
	for(i=0;i<num;i++)
	{
		Usart6_Putchar(word[i]);
		delay_us(500);
	}
}

//===============================================================
/*********************************************************************************
** 函数名称: USART6_IRQHandler
** 编写者  ：CZM
** 函数功能：串口6中断程序，与上位机通讯
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void USART6_IRQHandler(void)
{
   if(USART_GetITStatus(USART6,USART_IT_RXNE)!= RESET)//接收到数据
	 {
       USART_ClearFlag(USART6,USART_FLAG_RXNE);  
       USART_ClearITPendingBit(USART6,USART_IT_RXNE); //清除中断标志.
		
		   //rs485.rs485_rx_buf6[rs485.rx_ptr6]=USART_ReceiveData(USART6);//USART6->DR;//
			 enqueue_data_usart6(USART_ReceiveData(USART6));
//		 if((rs485.rs485_rx_buf6[0]==0x07) || (rs485.rs485_rx_buf6[0]==0x08) || (rs485.rs485_rx_buf6[0]==0x09))
//			 {
//				 rs485.rx_ptr6++;
//				 if(rs485.rx_ptr6>=RX_BUF_SIZE6)
//				 {
//						rs485.rx_ptr6=0;
//					 
//						rs485.rx_ok_flag6=1;              
//				 }	
//			 }  
   }

}	 

void enqueue_data_usart1(uchar e)
{
//		static u16 data_queue[MAX_DATA_QUEUE_DEPTH + 1] = {0};
		//static u8 queue_front_1 = 0;//队首
		//static u8 queue_rear_1 = 0;//队尾
		//u16 temp = 0;
		
    if(rs485.queue_front_1 == rs485.queue_rear_1)
    {
        rs485.data_queue_usart1[rs485.queue_front_1] = e;
    }
    
    rs485.data_queue_usart1[rs485.queue_rear_1] = e;
		
		rs485.queue_rear_1 = (rs485.queue_rear_1 + 1) % (MAX_DATA_QUEUE_DEPTH + 1);//队尾后移1 
		//如果队列满了 
		if((rs485.queue_rear_1+1)%(MAX_DATA_QUEUE_DEPTH+1)==rs485.queue_front_1)
		{
			//queue_rear_1 = (queue_rear_1 + 1) % (MAX_DATA_QUEUE_DEPTH + 1);//队尾变成队首
			//temp = data_queue_o3[queue_front_1];//出队
			
			//rs485.queue_front_1 =  (rs485.queue_front_1 + 1) % (MAX_DATA_QUEUE_DEPTH+1);
			rs485.queue_front_1 = 0;
			rs485.queue_rear_1 = 0;
			memset(rs485.data_queue_usart1,0,MAX_DATA_QUEUE_DEPTH+1);
		}
	
}

void enqueue_data_usart6(uchar e)
{
//		static u16 data_queue[MAX_DATA_QUEUE_DEPTH2 + 1] = {0};
		//static u8 queue_front_6 = 0;//队首
		//static u8 queue_rear_6 = 0;//队尾
		//u16 temp = 0;
		
    if(rs485.queue_front_6 == rs485.queue_rear_6)
    {
        rs485.data_queue_usart6[rs485.queue_front_6] = e;
    }
    
    rs485.data_queue_usart6[rs485.queue_rear_6] = e;
		
		rs485.queue_rear_6 = (rs485.queue_rear_6 + 1) % (MAX_DATA_QUEUE_DEPTH2 + 1);//队尾后移1 
		//如果队列满了 
		if((rs485.queue_rear_6+1)%(MAX_DATA_QUEUE_DEPTH2+1)==rs485.queue_front_6)
		{
			//queue_rear_6 = (queue_rear_6 + 1) % (MAX_DATA_QUEUE_DEPTH2 + 1);//队尾变成队首
			
			//rs485.queue_front_6 =  (rs485.queue_front_6 + 1) % (MAX_DATA_QUEUE_DEPTH2+1);
			rs485.queue_front_6 = 0;
			rs485.queue_rear_6 = 0;
			memset(rs485.data_queue_usart6,0,MAX_DATA_QUEUE_DEPTH2+1);
		}
	
}






