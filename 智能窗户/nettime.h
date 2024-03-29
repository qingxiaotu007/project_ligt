#ifndef __NETTIME_H
#define __NETTIME_H

//包含头文件
#include "stm32f10x.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include "esp8266.h"
#include "rtc.h"

//宏定义
#define NETTIME_PORTNUM 	80						//网络时间 80	
#define NETTIME_SERVERIP 	"cgi.im.qq.com"			//网络时间IP


//变量声明

//函数声明
int Get_Network_Time(void);
void calculating_time(void);

#endif
