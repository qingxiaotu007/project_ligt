#include "nettime.h"

//��ȡ����ʱ��
int Get_Network_Time(void)
{
	uint8_t ret;
	
	//���ӵ�Ŀ��TCP������
	ret = esp8266_connect_server("TCP",NETTIME_SERVERIP,NETTIME_PORTNUM);
	if(ret)
	{
		printf("esp8266_connection_target fail\r\n");
		return -7;
	}	
	printf("esp8266_connection_target success\r\n");
	delay_ms(300);
	
	//�������״̬
	ret = esp8266_check_connection_status();
	if(ret)
	{
		printf("esp8266_check_connection_status fail\r\n");
		
		//���������ȵ�
		while(Esp8266_Init());
	}	
	printf("esp8266_check_connection_status success\r\n");
	delay_ms(1000);delay_ms(1000);
	
	//����͸��ģʽ
	ret = esp8266_entry_transparent_transmission();
	if(ret)
	{
		printf("esp8266_entry_transparent_transmission fail\r\n");
		return -6;
	}	
	printf("esp8266_entry_transparent_transmission success\r\n");
	delay_ms(1000);

	//��Ŀ�������ȡʱ����Ϣ����
	esp8266_send_str("GET http://cgi.im.qq.com\n\n");
	delay_ms(20);//��ʱ20ms���ص���ָ��ͳɹ���״̬

	//delay_ms(1000);
	//�ȴ�ʱ����ַ����ʱ����Ϣ����
	while(esp8266_find_str_in_rx_packet("Connection: close",5000));
	
	//��������ʱ��
	calculating_time();
	
	//�˳�͸��ģʽ����������ATָ��
	ret=esp8266_exit_transparent_transmission();
	if(ret)
	{
		printf("esp8266_exit_transparent_transmission fail\r\n");
		return -1;
	}	
	printf("esp8266_exit_transparent_transmission success\r\n");
	delay_ms(1000);
	
	//�Ͽ�������
	ret=esp8266_disconnect_server();
	if(ret)
	{
		printf("esp8266_disconnect_server fail\r\n");
		return -1;
	}	
	printf("esp8266_disconnect_server success\r\n");
	delay_ms(1000);
	return 0;
}

//��������ʱ��
void calculating_time(void)
{
	u8 *p_start = NULL;
	u8 *p_end   = NULL;
	char resp[12]={0};

	if(Rx3Counter != 0)
	{
		strncpy(resp,"Date",5);
		Rx3Buffer[Rx3Counter] = 0;
		//printf("get_tim_srt��%s\r\n",Rx3Buffer);

		if(strstr((char*)Rx3Buffer,resp)) 
		{
			strncpy((char *)resp,"GMT",4);
			p_end = (u8*)strstr((char*)Rx3Buffer,(char*)resp);
			p_start = p_end - 9; 
			//printf("get_net_str %s\r\n",p_start);

			calendar.hour = ((*p_start - 0x30)*10 + (*(p_start+1) - 0x30) + 8) % 24;  //GMT0-->GMT8��ʱ��+8
			calendar.min = ((*(p_start+3) - 0x30)*10 + (*(p_start+4) - 0x30)) % 60;
			calendar.sec = ((*(p_start+6) - 0x30)*10 + (*(p_start+7) - 0x30)) % 60;
			calendar.w_year = ((*(p_start-5) - 0x30)*1000 + (*(p_start-4) - 0x30)*100+ (*(p_start-3) - 0x30)*10+ (*(p_start-2) - 0x30)); 
			calendar.w_date = ((*(p_start-12) - 0x30)*10 + (*(p_start-11) - 0x30)); 

			if        ((u8*)strstr((char*)Rx3Buffer,(char*) "Jan")) calendar.w_month=1; 
			else if   ((u8*)strstr((char*)Rx3Buffer,(char*) "Feb")) calendar.w_month=2; 
			else if   ((u8*)strstr((char*)Rx3Buffer,(char*) "Mar")) calendar.w_month=3; 
			else if   ((u8*)strstr((char*)Rx3Buffer,(char*) "Apr")) calendar.w_month=4; 
			else if   ((u8*)strstr((char*)Rx3Buffer,(char*) "May")) calendar.w_month=5; 
			else if   ((u8*)strstr((char*)Rx3Buffer,(char*) "Jun")) calendar.w_month=6; 
			else if   ((u8*)strstr((char*)Rx3Buffer,(char*) "Jul")) calendar.w_month=7; 
			else if   ((u8*)strstr((char*)Rx3Buffer,(char*) "Aug")) calendar.w_month=8; 
			else if   ((u8*)strstr((char*)Rx3Buffer,(char*) "Sep")) calendar.w_month=9; 
			else if   ((u8*)strstr((char*)Rx3Buffer,(char*) "Oct")) calendar.w_month=10; 
			else if   ((u8*)strstr((char*)Rx3Buffer,(char*) "Nov")) calendar.w_month=11; 
			else if   ((u8*)strstr((char*)Rx3Buffer,(char*) "Dec")) calendar.w_month=12;

			//printf("Update to network time\n");
			//RTC_Set(calendar.w_year,calendar.w_month ,calendar.w_date ,calendar.hour ,calendar.min,calendar.sec);																					
		}
		//���ڽ�������λ����0,��������飻
		Rx3Counter = 0;
	}       
}
