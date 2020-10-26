/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include "delay.h"


uint sec_cnt=0;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */

u16 iwdg_cnt;


void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//===============================================================
void SysTick_Handler(void)
{

}
//===============================================================
//===============================================================
//===============================================================

/******************************************************************************/
/*                 STM32F40x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f40x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles ADC1_2 interrupt request.
  * @param  None
  * @retval None
  */
void ADC1_2_IRQHandler(void)
{
   ADC_ClearITPendingBit(ADC1,ADC_IT_JEOC);

}

/**
  * @brief  This function handles TIM3 interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
static uint  t1ms_cnt=0;
static uchar t1s_cnt=0;
//static uchar t3s_cnt=0;
//static uint  t3m_cnt=0;
//static uchar t1m_cnt=0;
	
if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
{
  TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

	t1ms_cnt++;
	t_flag.t1ms_flag=1;
	
	if(t1ms_cnt%10==0)
	{
		t_flag.t10ms_flag=1;
	}
	
	if(t1ms_cnt%100==0)
	{
		t_flag.t100ms_flag=1;
		  
	}
	if(t1ms_cnt%500==0)
	{
		t_flag.t500ms_flag=1;
			
	}
	if(t1ms_cnt==1000)
	{
		t1ms_cnt=0;
		t_flag.t1s_flag=1;
    
		if(flag.sick_tx_timer_flag==1)
		{
			 flag.sick_tx_timer_flag=0;
			 
			 t1s_cnt++;
			 
			 if(t1s_cnt>=3)
			 {
				  t1s_cnt=0;
				  flag.sick_rx_timer_flag=1;
			 }
		}
  }
}
}
//===============================================================
void TIM5_IRQHandler(void)								
{
}
//===============================================================
void EXTI0_IRQHandler(void) /* */
{
	  if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
    {    
       EXTI_ClearFlag(EXTI_Line0);
       EXTI_ClearITPendingBit(EXTI_Line0);
			
			 
    }  
}
//===============================================================
void WWDG_IRQHandler(void)
{				    	
//	WWDG_Feed_Dog();
// 	WWDG_ClearFlag();
	//WWDG->SR=0X00;//清除提前唤醒中断标志位
}

//===============================================================
void RTC_IRQHandler(void)                 //Alarm
{	
	
}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/
/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

