#ifndef __MQ_H
#define __MQ_H 
 
 
#define gpio2_readC  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)   //��ȡGPIOB��P0�ڵ�ƽ��0/1��
 
void Mq_Init(void);                                         //��ʱ��ʼ������
int Mq_read(void); 											//��ȡA0��
 
 
#endif
