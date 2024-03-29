#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "uart.h"
#include "rtc.h"
#include <stdio.h>
#include "esp8266.h"
#include "mqtt.h"
#include "water.h"
#include "dht11.h"
#include "servo.h"
#include "adc.h"
#include "mq2.h"
#include "time.h"
#include "hc_501.h"
/**
  ******************************************************************************
  * @��Ŀ   	����STM32F1�����ܴ���
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
int count = 0;
int count2 = 0;

int main(void)
{
	SystemInit();       							//��ʼ��ϵͳʱ��,�Զ����ó�72Mhz  
	Delay_Init();						    		//��ʱ��ʼ��
	TIM4_PWM_Init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//������������Ϊ2λ
	
	Led_Config();  									//�Ƴ�ʼ��
	USART1_Config(9600); 							//���ڳ�ʼ��
	USART3_Config(115200); 							//���ڳ�ʼ��

	while(!DHT11_Init()){
	 printf(" Error! T DHT11 HAS NO RESPOND...\n");
	 
	}
	 printf("\r\n THE DHT11 HAS RESPOND");
	 delay_ms(100);		
	 
	Water_Init(); 
	Mq_Init();
 
	Adc_Init();			
	Adc2_Init();		
	
	Timer_Init();
	HC_501_Config();

	

	while( Esp8266_Init());							//����WiFi��STA����ģʽ����

	
	while( Mqtt_Connect_Aliyun() );					//����MQTT���Ӱ�����
	CONNECT_MQTT_LED(1);							//���ӷ�����״ָ̬ʾ�ƣ�����-���ӳɹ�
	printf("init end.\r\n");						//��ʾ��ɹ��ܻ�������
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);				//��������͵�ƽ������




   	while(1)
	{	
		//���������·�����
		mqtt_msg_handle();
		
		//ÿ��30���ϴ��豸״̬
		if(count2>0)
		{
			count2 = 0 ;
			mqtt_report_devices_status();	
		}
		
		//ÿ��50�뷢��������
		if(count >= 2)
		{
			/*	�豸���ڱ���ʱ������(����ʱ����mqtt_connect����Ϊ60s)��������Ҫ����һ�α��ģ�����ping����
				���ӱ���ʱ���ȡֵ��ΧΪ30��~1200�롣����ȡֵ300�����ϡ�
				��������ƽ̨����CONNACK��ӦCONNECT��Ϣʱ����ʼ������ʱ���յ�PUBLISH��SUBSCRIBE��PING�� PUBACK��Ϣʱ�������ü�ʱ����
			*/
			//����������������Ƶ���������������������������һ��ʱ�䲻������Ӧ��Ϣ[��ѡ]
			count = 0;	
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
		}

	}
}



void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)//���ָ����TIM�жϷ������:TIM �ж�Դ
	{
 
		count++;
		count2++;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//���TIMx���жϴ�����λ:TIM �ж�Դ 
		
	}
 
}

void EXTI4_IRQHandler(void)
{ 
	//���������ģ��
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)   // EXTI_GetITStatus���ָ���� EXTI ��·�������������//EXTI_Line0 �ⲿ�ж��� 0
	{
		window = 15;
		//���Ƽ̵���
		TIM_SetCompare2(TIM3,window);
		
		EXTI_ClearITPendingBit(EXTI_Line4);//����жϱ�־λ
	}
}	

