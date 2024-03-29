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
  * @项目   	基于STM32F1的智能窗户
  * @作者  		
  * @版本 		V1.1.0
  * @创建日期	2024/03/18
  * @简介   	通过WiFi连接热点，获取网络时间，配置RTC、利用MQTT连接阿里云，把开发板的状态上报，并接收阿里云的下发数据，控制灯和开关通断；
  ******************************************************************************
  * @说明
  *	1、知识点：串口通信、esp8266 wifi模块的AT指令的使用、RTC、MQTT连接阿里云、cJSON数据解析；
  * 
  * 2、注意点：
  * 		更给WiFi工作模式，需更改宏：WIFI_AP
  * 
  * 修改时间信息：
  * 		2121-8-19-15：00
  ******************************************************************************
  */ 
int count = 0;
int count2 = 0;

int main(void)
{
	SystemInit();       							//初始化系统时钟,自动配置成72Mhz  
	Delay_Init();						    		//延时初始化
	TIM4_PWM_Init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//将优先组配置为2位
	
	Led_Config();  									//灯初始化
	USART1_Config(9600); 							//串口初始化
	USART3_Config(115200); 							//串口初始化

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

	

	while( Esp8266_Init());							//配置WiFi以STA联网模式工作

	
	while( Mqtt_Connect_Aliyun() );					//配置MQTT链接阿里云
	CONNECT_MQTT_LED(1);							//连接服务器状态指示灯，点亮-连接成功
	printf("init end.\r\n");						//提示完成功能基本配置
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);				//引脚输出低电平，灯亮




   	while(1)
	{	
		//处理阿里云下发数据
		mqtt_msg_handle();
		
		//每隔30秒上传设备状态
		if(count2>0)
		{
			count2 = 0 ;
			mqtt_report_devices_status();	
		}
		
		//每隔50秒发送心跳包
		if(count >= 2)
		{
			/*	设备端在保活时间间隔内(保护时间在mqtt_connect设置为60s)，至少需要发送一次报文，包括ping请求。
				连接保活时间的取值范围为30秒~1200秒。建议取值300秒以上。
				从物联网平台发送CONNACK响应CONNECT消息时，开始心跳计时。收到PUBLISH、SUBSCRIBE、PING或 PUBACK消息时，会重置计时器。
			*/
			//发送心跳包，过于频繁发送心跳包，服务器将会持续一段时间不发送响应信息[可选]
			count = 0;	
			if( ! mqtt_send_heart() )
			{
				CONNECT_MQTT_LED(1);
			}
			else 
			{
				D	//调试键
				CONNECT_MQTT_LED(0);
				//如果断开阿里云，则重新配置MQTT链接阿里云
				while( Mqtt_Connect_Aliyun() );
				CONNECT_MQTT_LED(1);	
			}				
		}

	}
}



void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)//检查指定的TIM中断发生与否:TIM 中断源
	{
 
		count++;
		count2++;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除TIMx的中断待处理位:TIM 中断源 
		
	}
 
}

void EXTI4_IRQHandler(void)
{ 
	//人体红外检测模块
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)   // EXTI_GetITStatus检查指定的 EXTI 线路触发请求发生与否//EXTI_Line0 外部中断线 0
	{
		window = 15;
		//控制继电器
		TIM_SetCompare2(TIM3,window);
		
		EXTI_ClearITPendingBit(EXTI_Line4);//清除中断标志位
	}
}	

