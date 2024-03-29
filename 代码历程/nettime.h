#ifndef __NETTIME_H
#define __NETTIME_H

//����ͷ�ļ�
#include "stm32f10x.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include "esp8266.h"
#include "rtc.h"

//�궨��


//��������

//��������
int32_t net_connect_server(void);
int32_t net_get_time(uint8_t *result);
int32_t find_time_in_str(uint8_t *str,uint8_t *result);	//��ȡhttp���������е�ʱ����Ϣ
int32_t net_time_to_rtc(const char *net_time);


void Get_Network_Time(void);
void calculating_time(void);

#endif
