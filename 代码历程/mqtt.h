#ifndef __MQTT_H
#define __MQTT_H

//����ͷ�ļ�
#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include "uart.h"
#include "esp8266.h"
#include "cjson.h"
#include "key.h"

//�궨��
//�˴��ǰ����Ʒ������Ĺ���ʵ����½����------------ע���޸�Ϊ�Լ����Ʒ����豸��Ϣ��������
#define MQTT_BROKERADDRESS		"iot-06z00ce7lx7wruw.mqtt.iothub.aliyuncs.com"
#define MQTT_CLIENTID			"k0zh8ZASROn.Light|securemode=2,signmethod=hmacsha256,timestamp=1711291102967|"
#define MQTT_USARNAME			"Light&k0zh8ZASROn"
#define MQTT_PASSWD				"b981f49d7da8a98e2ed253afc9747f1a577b606d5522bd6e6dadf81b69d30dd4"
#define	MQTT_PUBLISH_TOPIC		"/sys/k0zh8ZASROn/Light/thing/event/property/post"
#define MQTT_SUBSCRIBE_TOPIC	"/sys/k0zh8ZASROn/Light/thing/service/property/set"


#define BYTE0(dwTemp)       (*( char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
	
#define CONNECT_MQTT_LED(x)	PCout(13)=(x)?0:1

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
