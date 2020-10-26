#include "sys.h"
#include "delay.h"



Flag_Def flag;



//===============================================================
/*********************************************************************************
** ��������: RCC_Initial
** ��д��  ��CZM
** �������ܣ����ڵ�������ǿ��
** �����������
** �����������
** ��    ע����
*********************************************************************************/
void RCC_Initial(void)
{
	  RCC_DeInit();	
}

//===============================================================
/*********************************************************************************
** ��������: Error_Handler
** ��д��  ��CZM
** �������ܣ����ڵ�������ǿ��
** �����������
** �����������
** ��    ע����
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
** ��������: System_Clock_HSI_Config
** ��д��  ��CZM
** �������ܣ����ڵ�������ǿ��
** �����������
** �����������
** ��    ע����
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
** ��������: NVIC_Configuration
** ��д��  ��CZM
** �������ܣ����ڵ�������ǿ��
** �����������
** �����������
** ��    ע����
*********************************************************************************/
void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

}





