#ifndef __DELAY_H
#define __DELAY_H

//����ͷ�ļ�
#include "stm32f10x.h"

//�궨��

//��������

//��������
void Delay_Init(void);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);

#endif
