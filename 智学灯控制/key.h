#ifndef __KEY_H
#define __KEY_H

//包含头文件
#include "stm32f10x.h"

//宏定义

//变量声明
extern int flag;
extern int led1;
extern int led2;
extern int led3;
extern int light1;
extern int light2;
extern int light3;
//函数声明
extern void KEY1_Init(void);
extern void KEY2_Init(void);
extern void KEY3_Init(void);
extern void KEY_Led(void);

#endif
