#ifndef _WIFI_H
#define _WIFI_H

//����ͷ�ļ�
#include "stm32f10x.h"
#include <stdio.h>
#include "delay.h"

//�궨��

//��������
void USART3_sendstr(USART_TypeDef* USARTx, char *Data);
void USART3_sendlenth(USART_TypeDef* USARTx, uint8_t *Data,uint8_t Len);
void wifisend(char *buf,int num);
void wifi_init(void);

#endif
