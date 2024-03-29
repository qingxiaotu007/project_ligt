#include "nettime.h"


volatile uint8_t  g_esp8266_rx_buf[4096];
volatile uint32_t g_esp8266_rx_cnt=0;
volatile uint32_t g_esp8266_rx_end=0;

//获取网络时间
void Get_Network_Time(void)
{
	int32_t rt;

	uint8_t buf[64];
	
	/* 连接服务器 */
	rt = net_connect_server();

	if(rt == 0)
	{
		printf("[OK] net connect time server success\r\n");	
	}
	else
	{
		printf("[error] net connect time server fail\r\n");
	}			
	
	//获取时间并设置
	memset(buf,0,sizeof buf);

	rt=net_get_time(buf);	

	if(rt == 0)
	{
		printf("[OK] net date:%s",buf);//Date: Sat, 27 Nov 2021 16:09:55 GMT
		delay_ms(100);
		rt = net_time_to_rtc((const char *)buf);
	}
	else
	{
		printf("[error] net get time fail\r\n");
	}
	
	//退出
	
		/* 退出透传模式 */
	esp8266_exit_transparent_transmission();
	
	/* 断开服务器连接 */
	esp8266_disconnect_server();

}


/* 连接服务器 */
int32_t net_connect_server(void)
{
	int32_t rt;
	
	/* 连接到网络服务器 */
	esp8266_send_str("AT+CIPSTART=\"TCP\",\"www.seniverse.com\",80\r\n");
	
	rt = esp8266_find_str_in_rx_packet("OK",5000);
	
	if(rt)
		return rt;
	
	/* 进入透传模式 */
	rt=esp8266_entry_transparent_transmission();
	if(rt)
		return rt;
	
	return 0;
}

/* 获取网络时间 */
int32_t net_get_time(uint8_t *result)
{
	int32_t rt;
	
	uint32_t timeout=5000;
	
	/* http的GET方法获取资源 */
	esp8266_send_str("GET /time15.asp HTTP/1.1\r\nHost:www.beijing-time.org\r\n");
	
	/* 发送任何数据则得到时间 */
	esp8266_send_str("...........\r\n");
	
	
	/* 等待接收完毕 */
	while((g_esp8266_rx_end==0) && (timeout--))
	{
		delay_ms(1);
	}
	
	g_esp8266_rx_end=0;
	
	/* 判断是否超时 */
	if(timeout == 0)
	{
		printf("[error] server response timeout\r\n");
		
		return -1;
	
	}
	
	/* 分析获取到的数据，提取时间 */
	rt=find_time_in_str((uint8_t *)g_esp8266_rx_buf,result);
	
	return rt;
}

int32_t find_time_in_str(uint8_t *str,uint8_t *result)	//获取http请求数据中的时间信息
{
	uint32_t i = 0;
	char *son = "Date";//关键字：Date
	char *p=NULL;
	
	/* 得到str字符串中"Date"的起始位置 */
	p = strstr((const char *)str,son);
	
	if(p==NULL)
	{
		return -1;
	}

	while(*p != '\r')
	{
		result[i++] = *p;
		p++;
	}
	
	result[i] = '\0';
	
	return 0;
}

/* 网络时间同步到RTC */
int32_t net_time_to_rtc(const char *net_time)	
{
	char buf[4]={0};
	
	/* 网络时间格式 "Date: Fri, 09 Jun 2023 04:04:14 GMT" */
	
	if(net_time == NULL)
		return -1;
	
	if(net_time[0] != 'D')
		return -2;
	

	/* 获取天 */
	memset(buf,0,sizeof(buf));
	strncpy(buf,&net_time[11],2);
	calendar.w_date =atoi(buf);

	
	/* 获取月份 */
	if(strncmp(&net_time[14],"Jan",3) == 0)
		calendar.w_month = 1;
	else if(strncmp(&net_time[14],"Feb",3) == 0)
		calendar.w_month = 2;
	else if(strncmp(&net_time[14],"Mar",3) == 0)
		calendar.w_month = 3;
	else if(strncmp(&net_time[14],"Apr",3) == 0)
		calendar.w_month = 4;
	else if(strncmp(&net_time[14],"May",3) == 0)
		calendar.w_month = 5;
	else if(strncmp(&net_time[14],"Jun",3) == 0)
		calendar.w_month = 6;
	else if(strncmp(&net_time[14],"Jul",3) == 0)
		calendar.w_month = 7;
	else if(strncmp(&net_time[14],"Aug",3) == 0)
		calendar.w_month = 8;
	else if(strncmp(&net_time[14],"Sep",3) == 0)
		calendar.w_month = 9;
	else if(strncmp(&net_time[14],"Oct",3) == 0)
		calendar.w_month = 10;
	else if(strncmp(&net_time[14],"Nov",3) == 0)
		calendar.w_month = 11;
	else if(strncmp(&net_time[14],"Dec",3) == 0)
		calendar.w_month = 12;
	
	
	/* 获取年 */
	memset(buf,0,sizeof(buf));
	strncpy(buf,&net_time[20],2);
	calendar.w_year=atoi(buf);
	
	/* 获取时 */
	memset(buf,0,sizeof(buf));
	strncpy(buf,&net_time[23],2);
	calendar.hour=atoi(buf);		
	
	/* 获取分 */
	memset(buf,0,sizeof(buf));
	strncpy(buf,&net_time[26],2);
	calendar.min=atoi(buf);	

	/* 获取秒 */
	memset(buf,0,sizeof(buf));
	strncpy(buf,&net_time[29],2);
	calendar.sec=atoi(buf);			


//	RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);
//	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure); 		

	
	return 0;
}
