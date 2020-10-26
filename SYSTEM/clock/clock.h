/*
@file    clock.h
*/
#ifndef  _CLOCK_H_
#define  _CLOCK_H_

#include "sys.h"




#define HSE                           (0x01)
#define HSI                           (0x02)
#define HSE_OK                        (0x03)
#define HSE_error                     (0x04)
#define HSI_OK                        (0x05)
#define HSI_error                     (0x06)
#define Parameter_error               (0x07)

void TIM2_Init(void);
void TIM5_Initial(void);
void System_Clock_Config(void);
uint32_t SysClockGet(void);






#endif









