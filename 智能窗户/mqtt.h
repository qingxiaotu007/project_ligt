#ifndef __MQTT_H
#define __MQTT_H

//包含头文件
#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include "uart.h"
#include "esp8266.h"
#include "cjson.h"
#include "water.h"
#include "dht11.h"
#include "servo.h"
#include "adc.h"
#include "mq2.h"

//宏定义
//此处是阿里云服务器的公共实例登陆配置------------注意修改为自己的云服务设备信息！！！！
#define MQTT_BROKERADDRESS		"iot-06z00jorgebw4hj.mqtt.iothub.aliyuncs.com"
#define MQTT_CLIENTID			"k0c5pSUT4fD.window|securemode=2,signmethod=hmacsha256,timestamp=1710334218656|"
#define MQTT_USARNAME			"window&k0c5pSUT4fD"
#define MQTT_PASSWD				"b987594f3e61241ff8dc2b9abace2afdcfef4a21582bb12282a8c59a60d9a33d"
#define	MQTT_PUBLISH_TOPIC		"/sys/k0c5pSUT4fD/window/thing/event/property/post"
#define MQTT_SUBSCRIBE_TOPIC	"/sys/k0c5pSUT4fD/window/thing/service/property/set"


#define BYTE0(dwTemp)       (*( char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
	
#define CONNECT_MQTT_LED(x)	PCout(13)=(x)?0:1

volatile extern uint8_t  window; //窗户参数

//变量声明

//函数声明
int Mqtt_Connect_Aliyun(void);											//配置MQTT链接阿里云
int32_t mqtt_connect(char *client_id,char *user_name,char *password);	//MQTT连接服务器
int32_t mqtt_subscribe_topic(char *topic,uint8_t qos,uint8_t whether);	//MQTT消息订阅
uint32_t mqtt_publish_data(char *topic, char *message, uint8_t qos);	//MQTT消息发布
int32_t mqtt_send_heart(void);											//MQTT发送心跳包
void mqtt_report_devices_status(void);									//设备状态上报
void mqtt_disconnect(void);												//MQTT无条件断开
void mqtt_send_bytes(uint8_t *buf,uint32_t len);						//MQTT发送数据
void mqtt_msg_handle(void);												//处理MQTT下发数据
uint32_t  mqtt_cjson_parse(char *pbuf);									//解析MQTT下发数据
#endif
