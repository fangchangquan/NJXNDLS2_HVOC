#ifndef __KEY_H
#define __KEY_H

#include "sys.h"


#define KEY1             (0x01)
#define KEY2             (0x02)
#define KEY3             (0x03)
#define KEY4             (0x04)
#define KEY5             (0x05)
#define KEY6             (0x06)

#define AN_XD_DL          1		  //消抖时间
#define AN_CJ_DL          200
#define AN_JG_DL          15		//连击键加速初值
#define AN_LA_DL          100		//连击键加速值
#define AN_CJC_DL         200



typedef struct 
{
  
  u8       code;
	  
}KEYBOARD_t;



extern KEYBOARD_t key;

extern void Key_Init(void);

void Key_Timer_Inc(void);

void Scan(void);

void Key_Scan(void);

void Keynum_Process(void);




#endif




