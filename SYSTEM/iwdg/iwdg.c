#include "iwdg.h"

#define SK_IWDG_PER             4  //625     4*8000 =6s

#define SK_IWDG_CNT             600 // 4*1000 = 1850ms  4*500=920ms  4*600 =1000+



void IWDG_Init(void)
{
	RCC_LSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);//确保LSI时钟开启
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler (SK_IWDG_PER);
	IWDG_SetReload (SK_IWDG_CNT);
	IWDG_ReloadCounter ();
	IWDG_Enable ();
}

void iwdg_feed(void)
{
	//IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_ReloadCounter ();
}
