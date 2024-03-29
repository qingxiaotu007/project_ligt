#include "key.h"
#include "delay.h"
#include <stdio.h>

int flag =0;
int count1 = 0;
int count2 = 0;
int count3 = 0;
int led1 = 0;
int led2 = 0;
int led3 = 0;
int light1 = 0;
int light2 = 0;
int light3 = 0;
extern void KEY1_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure; 	
	NVIC_InitTypeDef  NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);//选择 GPIO 管脚用作外部中断线路
	/*参数格式：参数1：GPIO_PortSourceGPIOX，X可以是A、B、C、D、E
	参数2：GPIO_PinSourceN，N可以是0~15   */

	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断请求模式
	EXTI_InitStructure.EXTI_Line = EXTI_Line1; //外部中断线
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //设置输入线路上降沿为中断请求
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; //使能
	EXTI_Init(&EXTI_InitStructure);//初始化

	NVIC_InitStructure.NVIC_IRQChannel =EXTI1_IRQn; //外部中断线1的中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;//抢占优先级最高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}



extern void KEY2_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure; 	
	NVIC_InitTypeDef  NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);//选择 GPIO 管脚用作外部中断线路
	/*参数格式：参数1：GPIO_PortSourceGPIOX，X可以是A、B、C、D、E
	参数2：GPIO_PinSourceN，N可以是0~15   */

	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断请求模式
	EXTI_InitStructure.EXTI_Line = EXTI_Line4; //外部中断线
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //设置输入线路上降沿为中断请求
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; //使能
	EXTI_Init(&EXTI_InitStructure);//初始化

	NVIC_InitStructure.NVIC_IRQChannel =EXTI4_IRQn; //外部中断线2的中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;//抢占优先级最高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}


extern void KEY3_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 	
	NVIC_InitTypeDef  NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);//选择 GPIO 管脚用作外部中断线路
	/*参数格式：参数1：GPIO_PortSourceGPIOX，X可以是A、B、C、D、E
	参数2：GPIO_PinSourceN，N可以是0~15   */

	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断请求模式
	EXTI_InitStructure.EXTI_Line = EXTI_Line3; //外部中断线
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //设置输入线路上降沿为中断请求
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; //使能
	EXTI_Init(&EXTI_InitStructure);//初始化

	NVIC_InitStructure.NVIC_IRQChannel =EXTI3_IRQn; //外部中断线1的中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;//抢占优先级最高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI1_IRQHandler(void)//中断函数的格式
{
	if (EXTI_GetITStatus(EXTI_Line1)==SET)//判断是否为EXTI1进来，返回值为 SET or RESET
	{
		light1 = 1;//灯一启动自动模式
		if(++count1 %2)
			flag = 1;
		else
			led1 = 0;
		TIM_SetCompare2(TIM4,led1);
		EXTI_ClearITPendingBit(EXTI_Line1);//清除中断函数的标志位，否则程序将一直卡在中断函数中
	}
}

void EXTI4_IRQHandler(void)//中断函数的格式
{
	if (EXTI_GetITStatus(EXTI_Line4)==SET)//判断是否为EXTI1进来，返回值为 SET or RESET
	{
		light2 = 1;//灯二启动自动模式
		if(++count2 %2)
			flag = 2;
		else
			led2 = 0;
		TIM_SetCompare3(TIM4,led2);
		EXTI_ClearITPendingBit(EXTI_Line4);//清除中断函数的标志位，否则程序将一直卡在中断函数中
	}
}

void EXTI3_IRQHandler(void)//中断函数的格式
{
	
	if (EXTI_GetITStatus(EXTI_Line3)==SET)//判断是否为EXTI1进来，返回值为 SET or RESET
	{
		light3 = 1;//灯三启动自动模式
		if(++count3 %2)
			flag = 3;
		else
			led3 = 0;
		TIM_SetCompare4(TIM4,led3);
		EXTI_ClearITPendingBit(EXTI_Line3);//清除中断函数的标志位，否则程序将一直卡在中断函数中
	}
}


void KEY_Led(void)
{		
	switch(flag)
	{
		case 1:
			flag = 0;led1 = 10;
			while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1))
			{ 
				TIM_SetCompare2(TIM4,led1++);
				delay_ms(100);
				if(led1>=100)
				{
					led1 = 1;
				}
			}
			break;
		case 2 :
			flag = 0;led2 = 10;
			while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4))
			{
				TIM_SetCompare3(TIM4,led2++);
				delay_ms(100);
				if(led2>=100)
				{
					led2 = 1;
				}
			}
			break;
		case 3 :
			flag = 0;led3 = 10;
			while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3))
			{
				TIM_SetCompare4(TIM4,led3++);
				delay_ms(100);
				if(led3>=100)
				{
					led3 = 1;
				}
			}
			break;
		default :
			flag = 0;
			break;
	}
}
