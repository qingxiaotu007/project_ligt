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
#define NETTIME_PORTNUM 	80						//����ʱ�� 80	
#define NETTIME_SERVERIP 	"cgi.im.qq.com"			//����ʱ��IP


//��������

//��������
int Get_Network_Time(void);
void calculating_time(void);

#endif
