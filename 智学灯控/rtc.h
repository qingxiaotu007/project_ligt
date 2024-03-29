#ifndef __RTC_H
#define __RTC_H

//����ͷ�ļ�
#include "stm32f10x.h"
#include "uart.h"
#include "esp8266.h"
//�궨��
//�����Ƿ��ʼ����־��
#define SET_FLAG 0xA5A2

//����ʱ��
#define YEAR	2021
#define MONTH	12
#define DATE	23
#define WEEK 	1
#define HOUR	23
#define MIN		59
#define SEC		40

//��������ʱ��
#define N_YEAR	2021
#define N_MONTH	12
#define N_DATE	24
#define N_WEEK 	1
#define N_HOUR	0
#define N_MIN	1
#define N_SEC	0

//��������
//ʱ��ṹ��
typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;			
	//������������
	vu16 w_year;
	vu8  w_month;
	vu8  w_date;
	vu8  week;		 
}calendar_str;					 

extern calendar_str calendar;	//�����ṹ��

extern uint8_t const mon_table[12];	//�·��������ݱ�
void Disp_Time(uint8_t x,uint8_t y,uint8_t size);//���ƶ�λ�ÿ�ʼ��ʾʱ��
void Disp_Week(uint8_t x,uint8_t y,uint8_t size,uint8_t lang);//��ָ��λ����ʾ����

//��������
void RTC_Config(void);
void RTC_NVIC_Configuration(void);
void RTC_Configuration(void);
void Time_Adjust(void);

uint8_t Is_Leap_Year(uint16_t year);
uint8_t RTC_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec);
uint8_t RTC_Alarm_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec);
uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day);
uint8_t RTC_Get(void);

#endif
