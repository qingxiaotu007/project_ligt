#ifndef __MQ_H
#define __MQ_H 
 
 
#define gpio2_readC  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)   //读取GPIOB的P0口电平（0/1）
 
void Mq_Init(void);                                         //延时初始化函数
int Mq_read(void); 											//读取A0口
 
 
#endif
