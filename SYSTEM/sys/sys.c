#include "sys.h"
#include "delay.h"



Flag_Def flag;



//===============================================================
/*********************************************************************************
** 函数名称: RCC_Initial
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void RCC_Initial(void)
{
	  RCC_DeInit();	
}

//===============================================================
/*********************************************************************************
** 函数名称: Error_Handler
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Error_Handler(void)
{
  Delay_Ms(33);
  
  while(1)
  {
		return;
  }
}
//===============================================================
/*********************************************************************************
** 函数名称: System_Clock_HSI_Config
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void System_Clock_HSI_Config(void)
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
/*********************************************************************************
** 函数名称: NVIC_Configuration
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级

}





