#include "rtc.h"

/*
RTC_Config()���������ܣ����ʱ���Ƿ����ã����δ������������ã�

Time_Adjust()��������ʱ������ӵ����ã�

*/

//����ȫ�ֱ���
char time_buf[12];		//2021/10/20  10:10:10
calendar_str calendar;	//ʱ�ӽṹ��

void RTC_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	
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

//�ú������� RTC ȫ���ж�����
void RTC_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{
		/* Clear the RTC Second interrupt */
		//RTC_ClearITPendingBit(RTC_IT_SEC | RTC_IT_OW);
		
		/*Update time*/
		RTC_Get();
		
		printf("Real Time:%d-%d-%d %d:%d:%d\r\n",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);//�������ʱ��

	}
	if(RTC_GetITStatus(RTC_IT_ALR) != RESET)
	{	
		/* Clear the RTC Alarm interrupt */		
		RTC_ClearITPendingBit(RTC_IT_ALR);
		
		/*Update time*/
		RTC_Get();
		
		printf("----Alarm Time:%d-%d-%d %d:%d:%d\r\n",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);//�������ʱ��	
		
		//�����´�����Ϊ40s��
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
	//RTC_Set(2015,1,14,17,42,55);							//���ñ���ʱ��
	RTC_Set(YEAR,MONTH,DATE,HOUR,MIN,SEC);					//���ñ���ʱ��	
#else
	RTC_Set(calendar.w_year,calendar.w_month ,calendar.w_date, calendar.hour ,calendar.min,calendar.sec);	//��������ʱ���������
#endif
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	//RTC_Alarm_Set(N_YEAR,N_MONTH,N_DATE,N_HOUR,N_MIN,N_SEC);	//��������ʱ��
	RTC_Alarm_Set(calendar.w_year,calendar.w_month ,calendar.w_date+1,N_HOUR,N_MIN,N_SEC);	//��������ʱ��������õڶ���6��00������
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	/*Exits from the RTC configuration mode.*/
	RTC_ExitConfigMode();
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}

//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
uint8_t Is_Leap_Year(uint16_t year)
{			  
	if(year%4==0) //�����ܱ�4����
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}

//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
//�·����ݱ�											 
uint8_t const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�	  
//ƽ����·����ڱ�
const uint8_t mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
uint8_t RTC_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec)
{
	uint16_t t;
	uint32_t seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	//��������ݵ��������
	{
		if(Is_Leap_Year(t))seccount+=31622400;//�����������
		else seccount+=31536000;			  //ƽ���������
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //��ǰ���·ݵ����������
	{
		seccount+=(uint32_t)mon_table[t]*86400;//�·����������
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//����2�·�����һ���������	   
	}
	seccount+=(uint32_t)(sday-1)*86400;//��ǰ�����ڵ���������� 
	seccount+=(uint32_t)hour*3600;//Сʱ������
    seccount+=(uint32_t)min*60;	 //����������
	seccount+=sec;//�������Ӽ���ȥ

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��  
	PWR_BackupAccessCmd(ENABLE);	//ʹ��RTC�ͺ󱸼Ĵ������� 
	RTC_SetCounter(seccount);	//����RTC��������ֵ

	RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
	return 0;	    
}

//��ʼ������		  
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//syear,smon,sday,hour,min,sec�����ӵ�������ʱ����   
//����ֵ:0,�ɹ�;����:�������.
uint8_t RTC_Alarm_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec)
{
	uint16_t t;
	uint32_t seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)						//��������ݵ��������
	{
		if(Is_Leap_Year(t))seccount+=31622400;	//�����������
		else seccount+=31536000;			 	//ƽ���������
	}
	smon-=1;
	for(t=0;t<smon;t++)	   						//��ǰ���·ݵ����������
	{
		seccount+=(uint32_t)mon_table[t]*86400;		//�·����������
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;	//����2�·�����һ���������	   
	}
	seccount+=(uint32_t)(sday-1)*86400;				//��ǰ�����ڵ���������� 
	seccount+=(uint32_t)hour*3600;					//Сʱ������
    seccount+=(uint32_t)min*60;	 					//����������
	seccount+=sec;								//�������Ӽ���ȥ 	
	
	//����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);//ʹ��PWR��BKP����ʱ��   
	PWR_BackupAccessCmd(ENABLE);						//ʹ�ܺ󱸼Ĵ�������
	RTC_SetAlarm(seccount);								//Sets the RTC alarm value.
	
	RTC_WaitForLastTask();								//�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
	
	return 0;	    
}

//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//������������������� 
//����ֵ�����ں�																						 
uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day)
{	
	uint16_t temp2;
	uint8_t yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// ���Ϊ21����,�������100  
	if (yearH>19)yearL+=100;
	// ����������ֻ��1900��֮���  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}	

//�õ���ǰ��ʱ��
//����ֵ:0,�ɹ�;����:�������.
uint8_t RTC_Get(void)
{
	static uint16_t daycnt=0;
	uint32_t timecount=0; 
	uint32_t temp=0;
	uint16_t temp1=0;	  
    timecount=RTC_GetCounter();	 
 	temp=timecount/86400;   //�õ�����(��������Ӧ��)
	if(daycnt!=temp)//����һ����
	{	  
		daycnt=temp;
		temp1=1970;	//��1970�꿪ʼ
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//������
			{
				if(temp>=366)temp-=366;//�����������
				else {temp1++;break;}  
			}
			else temp-=365;	  //ƽ�� 
			temp1++;  
		}   
		calendar.w_year=temp1;//�õ����
		temp1=0;
		while(temp>=28)//������һ����
		{
			if(Is_Leap_Year(calendar.w_year)&&temp1==1)//�����ǲ�������/2�·�
			{
				if(temp>=29)temp-=29;//�����������
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//ƽ��
				else break;
			}
			temp1++;  
		}
		calendar.w_month=temp1+1;	//�õ��·�
		calendar.w_date=temp+1;  	//�õ����� 
	}
	temp=timecount%86400;     		//�õ�������   	   
	calendar.hour=temp/3600;     	//Сʱ
	calendar.min=(temp%3600)/60; 	//����	
	calendar.sec=(temp%3600)%60; 	//����
	calendar.week=RTC_Get_Week(calendar.w_year,calendar.w_month,calendar.w_date);//��ȡ����   
	return 0;
}
