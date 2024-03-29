#include "wifi.h"

//��ʼ��WiFiģ��
void wifi_init(void)
{
	USART3_sendstr(USART3,"AT+CWMODE=3\r\n");//����Ϊ softAP+station ����ģʽ
	delay_ms(500);
	
	USART3_sendstr(USART3,"AT+RST\r\n");//����
	delay_ms(1500);
	
	USART3_sendstr(USART3,"AT+CIPAP=\"192.168.1.1\"\r\n");//����IP:192.168.1.1
	delay_ms(500);

	USART3_sendstr(USART3,"AT+CWSAP=\"CZJ\",\"12345678\",5,0\r\n");//����wifi������CZJ������12345678�����5����ͬʱ���ӣ�����ʱ�������룻
	delay_ms(500);
	
	USART3_sendstr(USART3,"AT+CIPMUX=1\r\n");//����������
	delay_ms(500);
	
	USART3_sendstr(USART3,"AT+CIPSERVER=1,8080\r\n");//���ö˿�8080
	delay_ms(500);
	
	printf("wifi_init end\n");//����1�����ʾ��
}
//wifiģ�鷢�����---ÿ�ι̶�����num���ֽ�
void wifisend(char *buf,int num)
{
	//ÿ��wifiģ�鷢�����ݵ�ʱ�򣬶����ȷ���һ���̶�ǰ׺
	char sendfront[32];//����ǰ׺�Ļ�����
	sprintf(sendfront,"AT+CIPSEND=0,%d\r\n",num);//����ַ���
	
	USART3_sendstr(USART3,sendfront);
	delay_ms(5);
	USART3_sendstr(USART3,buf);
}
//����len���ȵ��ַ���
void USART3_sendlenth(USART_TypeDef* USARTx, uint8_t *Data,uint8_t Len)
{ 
	while(Len--){				                          //�ж��Ƿ񵽴��ַ���������
	    USART_SendData(USARTx, *Data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET); //�ȴ�������
	}
}
//����һ���������ַ���
void USART3_sendstr(USART_TypeDef* USARTx, char *Data)
{ 
	//ѭ������1���ֽڣ�ֱ��׼�����͵��ֽ���'\0',Ҳ�����ַ���ĩβ��ֹͣ����
	while(*Data!=0){				                        
		USART_SendData(USARTx, *Data);
		Data++;
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
}