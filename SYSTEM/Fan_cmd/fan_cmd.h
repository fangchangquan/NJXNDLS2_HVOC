#ifndef __FAN_CMD__
#define __FAN_CMD__

#include "sys.h"

void Usart6_Send_Cmd_To_Fan_7_Speed(uchar buf[] ,uchar length);
void Usart6_Send_Cmd_To_Fan_8_Speed(uchar buf[] ,uchar length);
void Usart6_Send_Cmd_To_Fan_9_Speed(uchar buf[] ,uchar length);

void Usart6_Send_Cmd_To_Fan_7_status(uchar buf[],uchar length);
void Usart6_Send_Cmd_To_Fan_8_status(uchar buf[],uchar length);
void Usart6_Send_Cmd_To_Fan_9_status(uchar buf[],uchar length);

void Usart6_Send_Cmd_To_Fan_7_Warning(uchar buf[],uchar length);
void Usart6_Send_Cmd_To_Fan_8_Warning(uchar buf[],uchar length);
void Usart6_Send_Cmd_To_Fan_9_Warning(uchar buf[],uchar length);


#endif

