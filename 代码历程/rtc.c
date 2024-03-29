#include "rtc.h"

/*
RTC_Config()函数代码框架：检测时钟是否设置，如果未设置则进行设置；

Time_Adjust()函数进行时间和闹钟的设置；

*/

//定义全局变量
char time_buf[12];		//2021/10/20  10:10:10
calendar_str calendar;	//时钟结构体

void RTC_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	
	/* Enables access to the RTC and backup registers. */
	PWR_BackupAccessCmd(ENABLE);
	
	if (BKP_ReadBackupRegister(BKP_DR1) != SET_FLAG)
	{
		/* Backup data register value is not correct or not yet programmed (when
		the first time the program is executed) */

		printf("\r\n\n RTC not yet configured....\n");

		/* RTC Configuration */
		RTC_Configuration();

		printf("\r\n RTC configured....\n");

		/* Adjust time by values entered by the user on the hyperterminal */
		Time_Adjust();

		BKP_WriteBackupRegister(BKP_DR1, SET_FLAG);
		
		/* Disables access to the RTC and backup registers. */
		PWR_BackupAccessCmd(DISABLE);
	}
	else
	{
		printf("\r\n No need to configure RTC....\n");
		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();
		/* Enable the specified RTC interrupts. */
		RTC_ITConfig(RTC_IT_SEC | RTC_IT_ALR , ENABLE);
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
	}

	/* NVIC configuration */
	RTC_NVIC_Configuration();
	
	/*Update time*/
	RTC_Get();
}

//该函数处理 RTC 全局中断请求
void RTC_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{
		/* Clear the RTC Second interrupt */
		//RTC_ClearITPendingBit(RTC_IT_SEC | RTC_IT_OW);
		
		/*Update time*/
		RTC_Get();
		
		printf("Real Time:%d-%d-%d %d:%d:%d\r\n",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);//输出秒数时间

	}
	if(RTC_GetITStatus(RTC_IT_ALR) != RESET)
	{	
		/* Clear the RTC Alarm interrupt */		
		RTC_ClearITPendingBit(RTC_IT_ALR);
		
		/*Update time*/
		RTC_Get();
		
		printf("----Alarm Time:%d-%d-%d %d:%d:%d\r\n",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);//输出闹铃时间	
		
		//配置下次闹钟为40s后
		PWR_BackupAccessCmd(ENABLE);
		RTC_EnterConfigMode();	
	    RTC_WaitForLastTask();
		RTC_SetAlarm(RTC_GetCounter()+86400);	  	
		RTC_WaitForLastTask();
		RTC_ExitConfigMode();   
		PWR_BackupAccessCmd(DISABLE);
		
	}
	
	/* Clear the RTC Second interrupt */
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}

void RTC_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure one bit for preemption priority */
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void RTC_Configuration(void)
{
	/* Check if the StandBy flag is set */
	if(PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)
	{/* System resumed from STANDBY mode */

		/* Clear StandBy flag */
		PWR_ClearFlag(PWR_FLAG_SB);

		/* Wait for RTC APB registers synchronisation */
		RTC_WaitForSynchro();
		/* No need to configure the RTC as the RTC configuration(clock source, enable,
		prescaler,...) is kept after wake-up from STANDBY */
	}
	else
	{	/* StandBy flag is not set */

		/* RTC clock source configuration ----------------------------------------*/
		/* Reset Backup Domain */
		/* Enable PWR and BKP clocks */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

		/* Allow access to BKP Domain */
		PWR_BackupAccessCmd(ENABLE);

		/* Reset Backup Domain */
		BKP_DeInit();

		/* Enable LSE */
		RCC_LSEConfig(RCC_LSE_ON);
		/* Wait till LSE is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{}

		/* Select LSE as RTC Clock Source */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

		/* Enable RTC Clock */
		RCC_RTCCLKCmd(ENABLE);

		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();

		/* Enable the specified RTC interrupts. */
		RTC_ITConfig(RTC_IT_SEC | RTC_IT_ALR , ENABLE);

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();

		/* Set RTC prescaler: set RTC period to 1sec */
		RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
	}
}

/**
* @brief  Adjusts time.
* @param  None
* @retval None
*/
void Time_Adjust(void)
{
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	/* Change the current time */
#if WIFI_AP
	//RTC_Set(2015,1,14,17,42,55);							//设置本地时间
	RTC_Set(YEAR,MONTH,DATE,HOUR,MIN,SEC);					//设置本地时间	
#else
	RTC_Set(calendar.w_year,calendar.w_month ,calendar.w_date, calendar.hour ,calendar.min,calendar.sec);	//根据网络时间进行设置
#endif
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	//RTC_Alarm_Set(N_YEAR,N_MONTH,N_DATE,N_HOUR,N_MIN,N_SEC);	//设置闹钟时间
	RTC_Alarm_Set(calendar.w_year,calendar.w_month ,calendar.w_date+1,N_HOUR,N_MIN,N_SEC);	//根据网络时间进行设置第二天6：00的闹钟
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	/*Exits from the RTC configuration mode.*/
	RTC_ExitConfigMode();
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}

//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
uint8_t Is_Leap_Year(uint16_t year)
{			  
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}

//设置时钟
//把输入的时钟转换为秒钟
//以1970年1月1日为基准
//1970~2099年为合法年份
//返回值:0,成功;其他:错误代码.
//月份数据表											 
uint8_t const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	  
//平年的月份日期表
const uint8_t mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
uint8_t RTC_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec)
{
	uint16_t t;
	uint32_t seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	//把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
		else seccount+=31536000;			  //平年的秒钟数
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //把前面月份的秒钟数相加
	{
		seccount+=(uint32_t)mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数	   
	}
	seccount+=(uint32_t)(sday-1)*86400;//把前面日期的秒钟数相加 
	seccount+=(uint32_t)hour*3600;//小时秒钟数
    seccount+=(uint32_t)min*60;	 //分钟秒钟数
	seccount+=sec;//最后的秒钟加上去

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟  
	PWR_BackupAccessCmd(ENABLE);	//使能RTC和后备寄存器访问 
	RTC_SetCounter(seccount);	//设置RTC计数器的值

	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成  	
	return 0;	    
}

//初始化闹钟		  
//以1970年1月1日为基准
//1970~2099年为合法年份
//syear,smon,sday,hour,min,sec：闹钟的年月日时分秒   
//返回值:0,成功;其他:错误代码.
uint8_t RTC_Alarm_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec)
{
	uint16_t t;
	uint32_t seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)						//把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))seccount+=31622400;	//闰年的秒钟数
		else seccount+=31536000;			 	//平年的秒钟数
	}
	smon-=1;
	for(t=0;t<smon;t++)	   						//把前面月份的秒钟数相加
	{
		seccount+=(uint32_t)mon_table[t]*86400;		//月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;	//闰年2月份增加一天的秒钟数	   
	}
	seccount+=(uint32_t)(sday-1)*86400;				//把前面日期的秒钟数相加 
	seccount+=(uint32_t)hour*3600;					//小时秒钟数
    seccount+=(uint32_t)min*60;	 					//分钟秒钟数
	seccount+=sec;								//最后的秒钟加上去 	
	
	//设置时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);						//使能后备寄存器访问
	RTC_SetAlarm(seccount);								//Sets the RTC alarm value.
	
	RTC_WaitForLastTask();								//等待最近一次对RTC寄存器的写操作完成  	
	
	return 0;	    
}

//获得现在是星期几
//功能描述:输入公历日期得到星期(只允许1901-2099年)
//输入参数：公历年月日 
//返回值：星期号																						 
uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day)
{	
	uint16_t temp2;
	uint8_t yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// 如果为21世纪,年份数加100  
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}	

//得到当前的时间
//返回值:0,成功;其他:错误代码.
uint8_t RTC_Get(void)
{
	static uint16_t daycnt=0;
	uint32_t timecount=0; 
	uint32_t temp=0;
	uint16_t temp1=0;	  
    timecount=RTC_GetCounter();	 
 	temp=timecount/86400;   //得到天数(秒钟数对应的)
	if(daycnt!=temp)//超过一天了
	{	  
		daycnt=temp;
		temp1=1970;	//从1970年开始
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//是闰年
			{
				if(temp>=366)temp-=366;//闰年的秒钟数
				else {temp1++;break;}  
			}
			else temp-=365;	  //平年 
			temp1++;  
		}   
		calendar.w_year=temp1;//得到年份
		temp1=0;
		while(temp>=28)//超过了一个月
		{
			if(Is_Leap_Year(calendar.w_year)&&temp1==1)//当年是不是闰年/2月份
			{
				if(temp>=29)temp-=29;//闰年的秒钟数
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//平年
				else break;
			}
			temp1++;  
		}
		calendar.w_month=temp1+1;	//得到月份
		calendar.w_date=temp+1;  	//得到日期 
	}
	temp=timecount%86400;     		//得到秒钟数   	   
	calendar.hour=temp/3600;     	//小时
	calendar.min=(temp%3600)/60; 	//分钟	
	calendar.sec=(temp%3600)%60; 	//秒钟
	calendar.week=RTC_Get_Week(calendar.w_year,calendar.w_month,calendar.w_date);//获取星期   
	return 0;
}
