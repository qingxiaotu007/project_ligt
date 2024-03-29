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


//变量声明

//函数声明
int32_t net_connect_server(void);
int32_t net_get_time(uint8_t *result);
int32_t find_time_in_str(uint8_t *str,uint8_t *result);	//获取http请求数据中的时间信息
int32_t net_time_to_rtc(const char *net_time);


void Get_Network_Time(void);
void calculating_time(void);

#endif
