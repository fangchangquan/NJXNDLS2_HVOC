#ifndef __MOTOR_H
#define __MOTOR_H


#include "sys.h"



#define  LIFT_LENGRH              4
#define  MAX_H_SPEED              300*36//3000000//600000
#define  MIN_H_SPEED              10//10000


typedef struct {
	
	
	
	u16 motor_count;
  u16 motor_speed;
	

	
}MotorDef; 







extern MotorDef motor;




void Motro_GPIO_Init(void);

void Motor_Tim3_Init(void);

void Motor_Init(void);

void Motor_Speed_Process(void);












	
#endif


