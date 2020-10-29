#include "fan_cmd.h"
#include "sys.h"
#include "usart.h"




//查询风机转速指令
void Usart6_Send_Cmd_To_Fan_7_Speed(uchar buf[] ,uchar length)
{
	  buf[0]=0x07;
	  buf[1]=0x04;
	  buf[2]=0x00;
	  buf[3]=0x00;
		buf[4]=0x00;
		buf[5]=0x01;
		buf[6]=0x31;
		buf[7]=0xAC;
	
	  Uart6_Put_Word(buf,length);
}
void Usart6_Send_Cmd_To_Fan_8_Speed(uchar buf[] ,uchar length)
{
	  buf[0]=0x08;
	  buf[1]=0x04;
	  buf[2]=0x00;
	  buf[3]=0x00;
		buf[4]=0x00;
		buf[5]=0x01;
		buf[6]=0x31;
		buf[7]=0x53;
	
	  Uart6_Put_Word(buf,length);
}
void Usart6_Send_Cmd_To_Fan_9_Speed(uchar buf[] ,uchar length)
{
	  buf[0]=0x09;
	  buf[1]=0x04;
	  buf[2]=0x00;
	  buf[3]=0x00;
		buf[4]=0x00;
		buf[5]=0x01;
		buf[6]=0x30;
		buf[7]=0x82;
	
	  Uart6_Put_Word(buf,length);
}
/*
//查询风机状态指令
void Usart6_Send_Cmd_To_Fan_7_Status(uchar buf[],uchar length)
{
		buf[0]=0x07;
	  buf[1]=0x04;
	  buf[2]=0xD0;
	  buf[3]=0x11;
		buf[4]=0x00;
		buf[5]=0x01;
		buf[6]=0x59;
		buf[7]=0x69;
	
	  Uart6_Put_Word(buf,length);
}
void Usart6_Send_Cmd_To_Fan_8_Status(uchar buf[],uchar length)
{
		buf[0]=0x08;
	  buf[1]=0x04;
	  buf[2]=0xD0;
	  buf[3]=0x11;
		buf[4]=0x00;
		buf[5]=0x01;
		buf[6]=0x59;
		buf[7]=0x96;
	
	  Uart6_Put_Word(buf,length);
}
void Usart6_Send_Cmd_To_Fan_9_Status(uchar buf[],uchar length)
{
		buf[0]=0x09;
	  buf[1]=0x04;
	  buf[2]=0xD0;
	  buf[3]=0x11;
		buf[4]=0x00;
		buf[5]=0x01;
		buf[6]=0x58;
		buf[7]=0x47;
	
	  Uart6_Put_Word(buf,length);
}

//查询风机警告指令
void Usart6_Send_Cmd_To_Fan_7_Warning(uchar buf[],uchar length)
{
		buf[0]=0x07;
	  buf[1]=0x04;
	  buf[2]=0xD0;
	  buf[3]=0x12;
		buf[4]=0x00; 
		buf[5]=0x01;
		buf[6]=0xA9;
		buf[7]=0x69;
	
	  Uart6_Put_Word(buf,length);
}
void Usart6_Send_Cmd_To_Fan_8_Warning(uchar buf[],uchar length)
{
		buf[0]=0x08;
	  buf[1]=0x04;
	  buf[2]=0xD0;
	  buf[3]=0x12;
		buf[4]=0x00;
		buf[5]=0x01;
		buf[6]=0xA9;
		buf[7]=0x96;
	
	  Uart6_Put_Word(buf,length);
}
void Usart6_Send_Cmd_To_Fan_9_Warning(uchar buf[],uchar length)
{
		buf[0]=0x09;
	  buf[1]=0x04;
	  buf[2]=0xD0;
	  buf[3]=0x12;
		buf[4]=0x00;
		buf[5]=0x01;
		buf[6]=0xA8;
		buf[7]=0x47;
	
	  Uart6_Put_Word(buf,length);
}
*/