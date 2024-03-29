#include "wifi.h"

//初始化WiFi模块
void wifi_init(void)
{
	USART3_sendstr(USART3,"AT+CWMODE=3\r\n");//设置为 softAP+station 共存模式
	delay_ms(500);
	
	USART3_sendstr(USART3,"AT+RST\r\n");//重启
	delay_ms(1500);
	
	USART3_sendstr(USART3,"AT+CIPAP=\"192.168.1.1\"\r\n");//设置IP:192.168.1.1
	delay_ms(500);

	USART3_sendstr(USART3,"AT+CWSAP=\"CZJ\",\"12345678\",5,0\r\n");//设置wifi名称是CZJ，密码12345678，最多5个人同时连接，连接时无需密码；
	delay_ms(500);
	
	USART3_sendstr(USART3,"AT+CIPMUX=1\r\n");//启动多连接
	delay_ms(500);
	
	USART3_sendstr(USART3,"AT+CIPSERVER=1,8080\r\n");//设置端口8080
	delay_ms(500);
	
	printf("wifi_init end\n");//串口1输出提示；
}
//wifi模块发送语句---每次固定发送num个字节
void wifisend(char *buf,int num)
{
	//每次wifi模块发送数据的时候，都事先发送一个固定前缀
	char sendfront[32];//定义前缀的缓冲区
	sprintf(sendfront,"AT+CIPSEND=0,%d\r\n",num);//组合字符串
	
	USART3_sendstr(USART3,sendfront);
	delay_ms(5);
	USART3_sendstr(USART3,buf);
}
//发送len长度的字符串
void USART3_sendlenth(USART_TypeDef* USARTx, uint8_t *Data,uint8_t Len)
{ 
	while(Len--){				                          //判断是否到达字符串结束符
	    USART_SendData(USARTx, *Data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET); //等待发送完
	}
}
//发送一个完整的字符串
void USART3_sendstr(USART_TypeDef* USARTx, char *Data)
{ 
	//循环发送1个字节，直到准备发送的字节是'\0',也就是字符串末尾，停止发送
	while(*Data!=0){				                        
		USART_SendData(USARTx, *Data);
		Data++;
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
}