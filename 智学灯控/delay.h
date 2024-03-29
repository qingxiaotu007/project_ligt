#ifndef __DELAY_H
#define __DELAY_H

//包含头文件
#include "stm32f10x.h"

//宏定义

//变量声明

//函数声明
void Delay_Init(void);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);

#endif
