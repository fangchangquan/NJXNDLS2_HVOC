#include "key.h"
#include "delay.h"


KEYBOARD_t key;


volatile uchar key_code=0;
volatile uchar key_value=0;

u16 key_jsq=0;

//===============================================================
/*********************************************************************************
** 函数名  :Key_Init
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Key_Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;

RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
GPIO_Init(GPIOE,&GPIO_InitStructure);
	
GPIOE->IDR&=0x0ffff;

	
}
//===============================================================
/*********************************************************************************
** 函数名称: Key_Timer_Inc
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/

void Key_Timer_Inc(void)
{
if(key_jsq<65530)
{
key_jsq++;
}
}
//===============================================================
/*********************************************************************************
** 函数名称: Scan
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Scan(void)
{
u16 temp1=0;
	
temp1=(GPIO_ReadInputData(GPIOE)&0x003C)>>2;	
Delay_Ms(10);

key_value=(uchar)temp1;

}
//===============================================================
/*********************************************************************************
** 函数名称: Key_Scan
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Key_Scan(void)
{
Scan();
key_code=(uchar)((~key_value)&0xf);
}

//===============================================================
/*********************************************************************************
** 函数名称: Keynum_Process
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Keynum_Process(void)
{
   Key_Scan();
	
	 switch(key_code)
	 {
		  case KEY1 :key.code=KEY1;																				 						 					
						  
						     break;

		  case KEY2 :key.code=KEY2;	
							
						     break;
       
      case KEY3 :key.code=KEY3;																				 						 					
						  
						     break;

		  case KEY4 :key.code=KEY4;	
							
						     break;
      case KEY5 :key.code=KEY5;																				 						 					
						  
						     break;

		  case KEY6 :key.code=KEY6;	
							
						     break;    		 
		  default:   break;

		}

}
