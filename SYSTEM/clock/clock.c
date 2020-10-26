//--------------------------- Include ---------------------------//
#include "clock.h"
#include "main.h"
#include "delay.h"


//===============================================================
void TIM2_Init(void)                                  //1ms
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
NVIC_InitTypeDef NVIC_InitStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

TIM_DeInit(TIM2);
TIM_TimeBaseStructure.TIM_Period=999;			               //arr
TIM_TimeBaseStructure.TIM_Prescaler=83;		               //psc,
TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_Trigger,ENABLE);

NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;            //TIM3中断
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;  //先占优先级2级
NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;         //从优先级1级
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;            //IRQ通道被使能
NVIC_Init(&NVIC_InitStructure);                          //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
TIM_Cmd(TIM2,ENABLE);
}
//===============================================================
void TIM5_Initial(void)                                  //1ms
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
NVIC_InitTypeDef NVIC_InitStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);

TIM_DeInit(TIM5);
TIM_TimeBaseStructure.TIM_Period=49;			               //arr
TIM_TimeBaseStructure.TIM_Prescaler=83;		               //psc,
TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);

TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);

NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;            //TIM3中断
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;  //先占优先级0级
NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;         //从优先级3级
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;            //IRQ通道被使能
NVIC_Init(&NVIC_InitStructure);                          //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
TIM_Cmd(TIM5,ENABLE);
	
}
//===============================================================
void System_Clock_Config(void)
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
    
        
RCC_PLLConfig(RCC_PLLSource_HSE,8,336,2,4);            //  PLLCLK =(( 8MHz /8)* 336)/2 = 168 MHz 
    
RCC_PLLCmd(ENABLE);				                             // Enable PLL 
   
while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)     // Wait till PLL is ready
{
;
}
    
RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	           //Select PLL as system clock source 
   
while(RCC_GetSYSCLKSource()!= 0x08);		               // Wait till PLL is used as system clock source 

}
}
//===============================================================
unsigned char SysClockSet(unsigned char OSC, unsigned char Clock)
{
  unsigned int  temp = 0, PLLM = 0, PLLN = 0, PLLP = 0, PLLQ = 0;
        unsigned int        OSC_Sta;
        unsigned int        OSC_VALUE         = HSI_VALUE;
        unsigned int        OSC_RDY           = RCC_CR_HSIRDY; 
        unsigned int        OSC_ON            = RCC_CR_HSION; 
        unsigned char       OSC_ERROR         = HSI_error; 
        unsigned int        OSC_OK            = HSI_OK;
        unsigned int        OSC_SW            = RCC_CFGR_SW_HSI;
        unsigned int        OSC_SWS           = RCC_CFGR_SWS_HSI;
        unsigned int        OSC_SRC           = RCC_PLLCFGR_PLLSRC_HSI;        
        if(OSC == HSE)  
                        {
                        OSC_VALUE         = HSE_VALUE;
                        OSC_RDY           = RCC_CR_HSERDY; 
                        OSC_ON            = RCC_CR_HSEON; 
                        OSC_ERROR         = HSE_error; 
                        OSC_OK            = HSE_OK;
                        OSC_SW            = RCC_CFGR_SW_HSE;
                        OSC_SWS           = RCC_CFGR_SWS_HSE;
                        OSC_SRC           = RCC_PLLCFGR_PLLSRC_HSE;        
                        }
        else if(OSC != HSI) return(Parameter_error);
  
        switch (Clock)
                        {
                        case         0 :  PLLM = (OSC_VALUE/1000000);         PLLN = 96;          PLLP = 8;                PLLQ = 2;         break;                //12MHz
                        case         1 :  PLLM = (OSC_VALUE/1000000);         PLLN = 96;          PLLP = 6;                PLLQ = 2;         break;                //16MHz
                        case         2 :  PLLM = (OSC_VALUE/2000000);         PLLN = 72;          PLLP = 8;                PLLQ = 3;         break;                //18MHz
                        case         3 :  PLLM = (OSC_VALUE/2000000);         PLLN = 96;          PLLP = 8;                PLLQ = 4;         break;                //24MHz
                        case         4 :  PLLM = (OSC_VALUE/2000000);         PLLN = 120;         PLLP = 8;                PLLQ = 5;         break;                //30MHz
                        case         5 :  PLLM = (OSC_VALUE/2000000);         PLLN = 96;          PLLP = 6;                PLLQ = 4;         break;                //32MHz
                        case         6 :  PLLM = (OSC_VALUE/2000000);         PLLN = 144;         PLLP = 8;                PLLQ = 6;         break;                //36MHz
                        case         7 :  PLLM = (OSC_VALUE/2000000);         PLLN = 120;         PLLP = 6;                PLLQ = 5;         break;                //40MHz
                        case         8 :  PLLM = (OSC_VALUE/2000000);         PLLN = 168;         PLLP = 8;                PLLQ = 7;         break;                //42MHz
                        case         9 :  PLLM = (OSC_VALUE/2000000);         PLLN = 192;         PLLP = 8;                PLLQ = 8;         break;                //48MHz
                        case         10:  PLLM = (OSC_VALUE/2000000);         PLLN = 216;         PLLP = 8;                PLLQ = 9;         break;                //54MHz
                        case         11:  PLLM = (OSC_VALUE/2000000);         PLLN = 168;         PLLP = 6;                PLLQ = 7;         break;                //56MHz
                        case         12:  PLLM = (OSC_VALUE/2000000);         PLLN = 120;         PLLP = 4;                PLLQ = 5;         break;                //60MHz
                        case         13:  PLLM = (OSC_VALUE/2000000);         PLLN = 192;         PLLP = 6;                PLLQ = 8;         break;                //64MHz
                        case         14:  PLLM = (OSC_VALUE/2000000);         PLLN = 144;         PLLP = 4;                PLLQ = 6;         break;                //72MHz
                        case         15:  PLLM = (OSC_VALUE/2000000);         PLLN = 240;         PLLP = 6;                PLLQ = 10;        break;                //80MHz
                        case         16:  PLLM = (OSC_VALUE/2000000);         PLLN = 168;         PLLP = 4;                PLLQ = 7;         break;                //84MHz
                        case         17:  PLLM = (OSC_VALUE/2000000);         PLLN = 192;         PLLP = 4;                PLLQ = 8;         break;                //96MHz
                        case         18:  PLLM = (OSC_VALUE/2000000);         PLLN = 216;         PLLP = 4;                PLLQ = 9;         break;                //108MHz
                        case         19:  PLLM = (OSC_VALUE/2000000);         PLLN = 120;         PLLP = 2;                PLLQ = 5;         break;                //120MHz
                        case         20:  PLLM = (OSC_VALUE/2000000);         PLLN = 144;         PLLP = 2;                PLLQ = 6;         break;                //144MHz
                        case         21:  PLLM = (OSC_VALUE/2000000);         PLLN = 168;         PLLP = 2;                PLLQ = 7;         break;                //168MHz
                        case         22:  PLLM = (OSC_VALUE/2000000);         PLLN = 192;         PLLP = 2;                PLLQ = 8;         break;                //192MHz
                        case         23:  PLLM = (OSC_VALUE/2000000);         PLLN = 216;         PLLP = 2;                PLLQ = 9;         break;                //216MHz
                        case         24:  PLLM = (OSC_VALUE/2000000);         PLLN = 240;         PLLP = 2;                PLLQ = 10;        break;                //240MHz
//                        case        25:  PLLM = (OSC_VALUE/2000000);         PLLN = 260;         PLLP = 2;                PLLQ = 11;         break;                //260MHz
                        default:          PLLM = (OSC_VALUE/2000000);         PLLN = 240;         PLLP = 2;                PLLQ = 10;        break;                //240MHz
                        }
        //????????,????????????
        OSC_Sta = RCC->CR & OSC_RDY;
        if(OSC_Sta == 0)
                        {
                        RCC->CR |= OSC_ON;
                        do
												{
												 OSC_Sta = RCC->CR & OSC_RDY;
												 temp++;
												}while((OSC_Sta == 0) && (temp < 0x0600));
                        if(OSC_Sta == 0)return(OSC_ERROR); //????
                        }
        //????????????????
        RCC->CFGR &= (~(RCC_CFGR_SW));
        RCC->CFGR |= OSC_SW;
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != OSC_SWS);

        //??PLL
        RCC->CR &= (~(RCC_CR_PLLON));                                //???PLL
        RCC->CR &= (~(RCC_CR_PLLI2SON));                //??PLLI2S
        RCC->PLLCFGR = PLLM | (PLLN << 6) | (((PLLP >> 1) -1) << 16) | (PLLQ << 24) | (OSC_SRC);

        //??PLL,?????
        RCC->CR |= RCC_CR_PLLON;
        while((RCC->CR & RCC_CR_PLLRDY) == 0); 
                        
        //??PLLI2S,?????                
        RCC->CR |= RCC_CR_PLLI2SON;        
        while((RCC->CR & RCC_CR_PLLI2SRDY) == 0); 
                        
        //???????PLL?????
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
        RCC->CFGR |= RCC_CFGR_SW_PLL;
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
        return(OSC_OK);
                        
}
//===============================================================
uint32_t SysClockGet(void)
{
  uint32_t    PLLM = 0, PLLN = 0, PLLP = 0, PLLSRC = 0;
        
        if                        ((RCC->CFGR & RCC_CFGR_SWS ) == RCC_CFGR_SWS_HSE) return  HSE_VALUE;
        else if        ((RCC->CFGR & RCC_CFGR_SWS ) == RCC_CFGR_SWS_HSI) return  HSI_VALUE;
        else                 
                        {
                                PLLM                 = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
                                PLLN                 = ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN)>>6);
                                PLLP                 = ((((RCC->PLLCFGR & RCC_PLLCFGR_PLLP)>>16)+1)<<1);
                                PLLSRC               = RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC;
                                if(PLLSRC == 0)      return (((HSI_VALUE * PLLN) / PLLM )/ PLLP);
                                else                 return (((HSE_VALUE * PLLN) / PLLM )/ PLLP);
                        }
}
//===============================================================

//===============================================================

//===============================================================



