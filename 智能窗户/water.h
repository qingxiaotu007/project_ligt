#ifndef __WATER_H
#define __WATER_H 
 
 
#define gpio_readC  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)   //��ȡGPIOA��P0�ڵ�ƽ��0/1��
 
void Water_Init(void);                                         //��ʱ��ʼ������
int Water_read(void); 											//��ȡA0��
 
 
#endif
