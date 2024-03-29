#ifndef __MQTT_H
#define __MQTT_H

//����ͷ�ļ�
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

//�궨��
//�˴��ǰ����Ʒ������Ĺ���ʵ����½����------------ע���޸�Ϊ�Լ����Ʒ����豸��Ϣ��������
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

volatile extern uint8_t  window; //��������

//��������

//��������
int Mqtt_Connect_Aliyun(void);											//����MQTT���Ӱ�����
int32_t mqtt_connect(char *client_id,char *user_name,char *password);	//MQTT���ӷ�����
int32_t mqtt_subscribe_topic(char *topic,uint8_t qos,uint8_t whether);	//MQTT��Ϣ����
uint32_t mqtt_publish_data(char *topic, char *message, uint8_t qos);	//MQTT��Ϣ����
int32_t mqtt_send_heart(void);											//MQTT����������
void mqtt_report_devices_status(void);									//�豸״̬�ϱ�
void mqtt_disconnect(void);												//MQTT�������Ͽ�
void mqtt_send_bytes(uint8_t *buf,uint32_t len);						//MQTT��������
void mqtt_msg_handle(void);												//����MQTT�·�����
uint32_t  mqtt_cjson_parse(char *pbuf);									//����MQTT�·�����
#endif
