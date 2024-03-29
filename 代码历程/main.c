#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "uart.h"
#include "rtc.h"
#include "key.h"
#include "esp8266.h"
#include "nettime.h"
#include "mqtt.h"
#include "adc.h"
#include "HC_501.h"

/**
******************************************************************************
* @��Ŀ   	����STM32F1�����ܵƹ�
* @����  		
* @�汾 		V1.1.0
* @��������	2024/03/18
* @���   	ͨ��WiFi�����ȵ㣬��ȡ����ʱ�䣬����RTC������MQTT���Ӱ����ƣ��ѿ������״̬�ϱ��������հ����Ƶ��·����ݣ����ƵƺͿ���ͨ�ϣ�
******************************************************************************
* @˵��
*	1��֪ʶ�㣺����ͨ�š�esp8266 wifiģ���ATָ���ʹ�á�RTC��MQTT���Ӱ����ơ�cJSON���ݽ�����
* 
* 2��ע��㣺
* 		����WiFi����ģʽ������ĺ꣺WIFI_AP
* 
* �޸�ʱ����Ϣ��
* 		2121-8-19-15��00
******************************************************************************
*/ 




int main(void)
{
SystemInit();       							//��ʼ��ϵͳʱ��,�Զ����ó�72Mhz  
Delay_Init();						    		//��ʱ��ʼ��
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//������������Ϊ2λ

LED_PWM_Init();							//�Ƴ�ʼ��

KEY1_Init();                            //���س�ʼ��
KEY2_Init();
KEY3_Init();

Adc2_Init();
Adc_Init();

HC_501_Config();                        		//����������ʼ��

USART1_Config(9600); 							//���ڳ�ʼ��
USART3_Config(115200); 							//ESP8266���ڳ�ʼ��

while( Esp8266_Init());							//����WiFi��STA����ģʽ����
Get_Network_Time();								//��ȡ����ʱ��
RTC_Config();	  								//RTC��ʼ��

while( Mqtt_Connect_Aliyun() );					//����MQTT���Ӱ�����
CONNECT_MQTT_LED(1);							//���ӷ�����״ָ̬ʾ�ƣ�����-���ӳɹ�

printf("init end.\r\n");						//��ʾ��ɹ��ܻ�������
GPIO_ResetBits(GPIOC, GPIO_Pin_13);				//��������͵�ƽ������

while(1)
{
	//���Դ���
//		printf("light == %d",(Get_Adc2_Average(ADC_Channel_8,5)*100/4096));
//		printf("light == %d",(Get_Adc2_Average(ADC_Channel_6,5)*100/4096));
//		printf("light == %d",(Get_Adc2_Average(ADC_Channel_5,5)*100/4096));
//		
//		printf("4hc_501 == %d",GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4));
//		printf("5hc_501 == %d",GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5));
//		printf("6hc_501 == %d",GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6));
	
	LED_Auto();  //�Զ�ģʽ
	
	if(flag > 0)    //���������������
		KEY_Led();
	
	//���������·�����
	mqtt_msg_handle();
	
	//ÿ��30���ϴ��豸״̬
	if(!(calendar.sec%30))
	{
		//�ϱ��豸״̬
		mqtt_report_devices_status();	
	}
	//ÿ��50�뷢��������
	if(!(calendar.sec%50))
	{
		/*	�豸���ڱ���ʱ������(����ʱ����mqtt_connect����Ϊ60s)��������Ҫ����һ�α��ģ�����ping����
			���ӱ���ʱ���ȡֵ��ΧΪ30��~1200�롣����ȡֵ300�����ϡ�
			��������ƽ̨����CONNACK��ӦCONNECT��Ϣʱ����ʼ������ʱ���յ�PUBLISH��SUBSCRIBE��PING�� PUBACK��Ϣʱ�������ü�ʱ����
		*/
		//����������������Ƶ���������������������������һ��ʱ�䲻������Ӧ��Ϣ[��ѡ]
		if( ! mqtt_send_heart() )
		{
			CONNECT_MQTT_LED(1);
		}
		else 
		{
			D	//���Լ�
			CONNECT_MQTT_LED(0);
			//����Ͽ������ƣ�����������MQTT���Ӱ�����
			while( Mqtt_Connect_Aliyun() );
			CONNECT_MQTT_LED(1);	
		}	

		//�����������Ƿ����ˣ�������رյ�
		HC_501_Check();
		
	}
}
}
