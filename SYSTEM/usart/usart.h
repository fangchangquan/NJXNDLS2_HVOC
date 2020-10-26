#ifndef __USART_H
#define __USART_H
//#include "sys.h"

#include "sys.h"
#include "stdio.h"
#include "stdarg.h"
#include "stm32F4xx_gpio.h"

#define RX_BUF_SIZE          50

#define TX_BUF_SIZE1         32
#define RX_BUF_SIZE1         8

#define TX_BUF_SIZE2         4//VOC
#define RX_BUF_SIZE2         4

#define TX_BUF_SIZE3         9//D_NO2
#define RX_BUF_SIZE3         9

#define TX_BUF_SIZE4         5
#define RX_BUF_SIZE4         56

#define TX_BUF_SIZE5         2
#define RX_BUF_SIZE5         6

#define TX_BUF_SIZE6         8
#define RX_BUF_SIZE6         7

#define TX_BUF_SIZE_TO_FAN         8//
#define RX_BUF_SIZE_TO_FAN         7

#define RS485_DE1             PAout(11)
#define RS485_DE2							PDout(15)


typedef struct {
	uchar rs485_tx_buf1[TX_BUF_SIZE1];
	uchar rs485_rx_buf1[RX_BUF_SIZE1];
	uchar rs485_rx_copy_buf1[RX_BUF_SIZE1];
	
	//与风机通信的buf
	uchar rs485_tx_buf11[TX_BUF_SIZE_TO_FAN];
	uchar rs485_rx_buf11[RX_BUF_SIZE_TO_FAN];
	uchar rs485_rx_copy_buf11[RX_BUF_SIZE_TO_FAN];
	
	uchar rs485_tx_buf2[TX_BUF_SIZE2];
	uchar rs485_rx_buf2[RX_BUF_SIZE2];
	uchar rs485_tx_buf3[TX_BUF_SIZE3];
	uchar rs485_rx_buf3[RX_BUF_SIZE3];
	uchar rs485_tx_buf4[TX_BUF_SIZE4];
	uchar rs485_tx_buf43[6];
	uchar rs485_rx_buf4[RX_BUF_SIZE4];
	uchar rs485_tx_buf5[TX_BUF_SIZE5];
	uchar rs485_rx_buf5[RX_BUF_SIZE5];
	uchar rs485_tx_buf6[TX_BUF_SIZE6];
	uchar rs485_rx_buf6[RX_BUF_SIZE6];
	
	
	uchar rs485_rx_copy_buf6[RX_BUF_SIZE6];
	
	uchar tx_ptr1;
	uchar rx_ptr1;
	uchar tx_ptr2;
	uchar rx_ptr2;
	uchar tx_ptr3;
	uchar rx_ptr3;
	uchar tx_ptr4;
	uchar rx_ptr4;
	uchar tx_ptr5;
	uchar rx_ptr5;
	uchar tx_ptr6;
	uchar rx_ptr6;
	
  uint16  tx_data;
	uint16  rx_data;
	uchar head1;
	uchar tail1;
	
  uchar rx_ok_flag1           :1;
	uchar rx_ok_flag_really     :1;
	uchar rx_ok_flag2           :1;
	uchar rx_ok_flag3           :1;
	uchar rx_ok_flag4           :1;
	uchar rx_ok_flag5           :1;
	uchar rx_ok_flag6           :1;

	
	uchar rx_fan_ok_flag1       :1;
	
	uchar rx_count_flag         :1;
	
	uchar iwdg_count_flag       :1;
	
	
	uchar rec_complete_flag     :1;
	
}RS485_Def;




extern RS485_Def rs485;
	  	



void My_Usart1_Init(u32 bound);
void My_Usart2_Init(u32 bound);
void My_Usart3_Init(u32 bound);
void My_Uart4_Init(u32 bound);
void My_Usart5_Init(u32 bound);
void My_Usart6_Init(u32 bound);


void USART_Initial(void);
int fputc(int ch, FILE *f);


void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
void UART4_IRQHandler(void);
void UART5_IRQHandler(void);
void USART6_IRQHandler(void);


void Uart1_Putchar(u8 ch);
void Uart1_Put_Word(u8 word[],u8 num);

void Uart2_Putchar(u8 ch);
void Uart2_Put_Word(u8 word[],u8 num);

void Uart3_Putchar(u8 ch);
void Uart3_Put_Word(uchar word[],u8 num);
void USART3_Receive_Message_Process(void);

void Uart4_Putchar(u8 ch);
void Uart4_Put_Word(uchar word[],u8 num);

void Uart5_Putchar(u8 ch);
void Uart5_Put_Word(uchar word[],u8 num);

void Uart6_Putchar(u8 ch);
void Uart6_Put_Word(uchar word[],u8 num);


#endif
