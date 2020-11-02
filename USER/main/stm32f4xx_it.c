/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include "motor.h"
#include "main.h"
#include "i2c.h"
#include "clock.h"
#include "adc.h"
#include "humidity.h"
#include "vocsensor.h"
#include "pmsensor.h"
#include "digit_no2.h"



uint32_t systemtick = 0;
//uchar   test_cnt=0;

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
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
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
// 
//}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

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


void TIM2_IRQHandler(void)
{
static uint16  t1ms_cnt=0;
static uchar   t1s_cnt=0;
static uchar   t1m_cnt=0;	
	
if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
{
  TIM_ClearITPendingBit(TIM2,TIM_IT_Update);

	t1ms_cnt++;
	t_flag.t1ms_flag=1;

//	if(flag.test_flag==1)
//	{
//		 test_cnt++;
//		 if(flag.test_flag==0)
//		 {
//			  motor.v_count=test_cnt;
//			  test_cnt=0;
//		 }
//	 }
	
	if(t1ms_cnt%5==0)
	{
     t_flag.t5ms_flag=1;
		
		 //Get_ADC();	    
	}

	if(t1ms_cnt%10==0)
	{
		t_flag.t10ms_flag=1;
		
		
		
	}
	
	
	if(t1ms_cnt%20==0)
	{
		t_flag.t20ms_flag=1;

//    Get_Temperature_Value();
		
	}
	if(t1ms_cnt%50==0)
	{
		t_flag.t50ms_flag=1;


		
	}
	
	if(t1ms_cnt%100==0)
	{
		t_flag.t100ms_flag=1;
		
	}
		
	if(t1ms_cnt%200==0)
	{
		t_flag.t200ms_flag=1;
    
	  
		
	}
	
	if(t1ms_cnt%500==0)
	{
		 t_flag.t500ms_flag=1;
		

	}
	
	if(t1ms_cnt==1000)
	{
		
		t_flag.t1s_flag=1;
    		
		t1s_cnt++;
		if(rs485.rx_ok_flag2==1)
		{
			VOC_Receive_Process(rs485.rs485_rx_buf2,RX_BUF_SIZE2);//voc接受数据
		}
		
		if(rs485.rx_ok_flag3==1)
		{
			rs485.rx_ok_flag3=0;
			D_NO2_Recieve_Data(rs485.rs485_rx_buf3,RX_BUF_SIZE3);//D_NO2接受数据
		}
		
//		PM_Receive_Process(rs485.rs485_rx_buf5,RX_BUF_SIZE5);
		
		
		if(t1s_cnt % 5 ==0)
		{
			 t1s_cnt=0;
			 t_flag.t5s_flag=1;
       //			 IWDG_Feed();
		}
		
   	if(t_flag.t1m_flag==1)
    {
       t1m_cnt++;
       
       if(t1m_cnt>=3)
       {
           t1m_cnt=0;
            
           flag.pulse_count_flag=1;
       }

		}	
		
		
  }
	if(t1ms_cnt == 1200)
	{
		t1ms_cnt=0;
		t_flag.t1_5s_flag = 1;
	}

}

}

//===============================================================
//void TIM5_IRQHandler(void)
//{
//   if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)
//   { 
//		  TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);
//      TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
//		 
//		  
//     
//			
//	 }
//	
//}
//===============================================================

void EXTI1_IRQHandler(void)
{
//	 if(EXTI_GetITStatus(EXTI_Line1) != RESET)
//	 {
//		  EXTI_ClearITPendingBit(EXTI_Line1);
//		 
//	 }
	 
	 
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
