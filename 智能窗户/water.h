#ifndef __WATER_H
#define __WATER_H 
 
 
#define gpio_readC  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)   //读取GPIOA的P0口电平（0/1）
 
void Water_Init(void);                                         //延时初始化函数
int Water_read(void); 											//读取A0口
 
 
#endif
