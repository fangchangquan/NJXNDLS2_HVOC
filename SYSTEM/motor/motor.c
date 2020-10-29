#include "motor.h"
#include "adc.h"





MotorDef motor;

//===============================================================
/*********************************************************************************
** 函数名称: Motro_GPIO_Init
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Motro_GPIO_Init(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//NOPULL; 
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	
	 GPIO_Init(GPIOC,&GPIO_InitStructure); 
	
}
//===============================================================
/*********************************************************************************
** 函数名称: Motor_Tim8_Init
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Motor_Tim3_Init(void)
{
	 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	 TIM_ICInitTypeDef  TIM_ICInitStructure;
	 NVIC_InitTypeDef NVIC_InitStructure;
	 
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); 
	 
	 TIM_DeInit(TIM3);
	
   TIM_TimeBaseStructure.TIM_Period=999;			               //arr
   TIM_TimeBaseStructure.TIM_Prescaler=83;		               //psc,
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
   TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);

	 TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
   TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
   TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
   TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
   TIM_ICInitStructure.TIM_ICFilter = 0x00;

  
   TIM_ICInit(TIM3,&TIM_ICInitStructure);
	 
   GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3);

   NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;            //TIM3中断
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;  //先占优先级2级
   NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;         //从优先级1级
   NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;            //IRQ通道被使能
   NVIC_Init(&NVIC_InitStructure);                          //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	 
	 
	 TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);
   TIM_Cmd(TIM3,ENABLE);
	 
	 t_flag.t1m_flag=1;
 }
//===============================================================
/*********************************************************************************
** 函数名称: EXTI_GPIO_Config
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
 void EXTI_GPIO_Config(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;  
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource8); 
	EXTI_ClearITPendingBit(EXTI_Line8);  
	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;   
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;   
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;  
	EXTI_Init(&EXTI_InitStructure);     /* Configures the nested vectored interrupt controller. */ 
	
	NVIC_InitTypeDef NVIC_InitStructure;    /* Enable the USARTx Interrupt */   
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;   
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;   
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);

  t_flag.t1m_flag=1;
}
//===============================================================
/*********************************************************************************
** 函数名称: Motor_Init
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Motor_Init(void)
{
	 Motro_GPIO_Init();
	

	 Motor_Tim3_Init();
	
//	 EXTI_GPIO_Config();
}	 
//===============================================================
/*********************************************************************************
** 函数名称: Double_Led_Charge_Control
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void TIM3_IRQHandler(void)
{
	 static u16 pulse_count=0;
	
	 if(TIM_GetITStatus(TIM3,TIM_IT_CC3)!=RESET)
   {
      TIM_ClearITPendingBit(TIM3,TIM_IT_CC3);
		 
		  if(t_flag.t1m_flag==1)
			{
		     pulse_count++;
			}
		 
		  if(flag.pulse_count_flag==1)
			{
				 motor.motor_count=pulse_count;
				 pulse_count=0;
				 t_flag.t1m_flag=0;
				
				 flag.count_finish_flag=1;
				
			}
						
		}
	 
}			 
//===============================================================
/*********************************************************************************
** 函数名称: Motor_Speed_Process
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Motor_Speed_Process(void)
{
	static u32 plus_count_flag = 0;
	 if(flag.count_finish_flag==1)
	 {
		 
		  flag.pulse_count_flag=0;
		  motor.motor_speed=(motor.motor_count*20);           //5 PULSE/ROUND
		  sensor.motor_speed.speed=motor.motor_speed;
		  flag.count_finish_flag=0;
		  t_flag.t1m_flag=1;
		  plus_count_flag=0;
	 }
	 else
	 {
		 plus_count_flag++;
		 if(plus_count_flag > 50)
		 {
				sensor.motor_speed.speed = 0;
			 plus_count_flag=0;
		 }
	 }
	 
}
		 


//===============================================================
/*********************************************************************************
** 函数名称: Double_Led_Charge_Control
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void EXTI9_5_IRQHandler(void) /*  */
{
	  static u16 pulse_count=0;
	
	  if(EXTI_GetITStatus(EXTI_Line8)!=RESET)
    {    
       EXTI_ClearFlag(EXTI_Line8);
       EXTI_ClearITPendingBit(EXTI_Line8);
			
			 if(t_flag.t1m_flag==1)
			{
		     pulse_count++;
			}
		 
		  if(flag.pulse_count_flag==1)
			{
				 motor.motor_count=pulse_count;
				 t_flag.t1m_flag=0;
				
				 flag.count_finish_flag=1;
				
			}
    }  
}
//===============================================================
/*********************************************************************************
** 函数名称: Double_Led_Charge_Control
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/














