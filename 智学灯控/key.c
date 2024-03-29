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

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);//ѡ�� GPIO �ܽ������ⲿ�ж���·
	/*������ʽ������1��GPIO_PortSourceGPIOX��X������A��B��C��D��E
	����2��GPIO_PinSourceN��N������0~15   */

	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //�ж�����ģʽ
	EXTI_InitStructure.EXTI_Line = EXTI_Line1; //�ⲿ�ж���
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //����������·�Ͻ���Ϊ�ж�����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; //ʹ��
	EXTI_Init(&EXTI_InitStructure);//��ʼ��

	NVIC_InitStructure.NVIC_IRQChannel =EXTI1_IRQn; //�ⲿ�ж���1���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;//��ռ���ȼ����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //��Ӧ���ȼ�
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

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);//ѡ�� GPIO �ܽ������ⲿ�ж���·
	/*������ʽ������1��GPIO_PortSourceGPIOX��X������A��B��C��D��E
	����2��GPIO_PinSourceN��N������0~15   */

	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //�ж�����ģʽ
	EXTI_InitStructure.EXTI_Line = EXTI_Line4; //�ⲿ�ж���
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //����������·�Ͻ���Ϊ�ж�����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; //ʹ��
	EXTI_Init(&EXTI_InitStructure);//��ʼ��

	NVIC_InitStructure.NVIC_IRQChannel =EXTI4_IRQn; //�ⲿ�ж���2���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;//��ռ���ȼ����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //��Ӧ���ȼ�
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

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);//ѡ�� GPIO �ܽ������ⲿ�ж���·
	/*������ʽ������1��GPIO_PortSourceGPIOX��X������A��B��C��D��E
	����2��GPIO_PinSourceN��N������0~15   */

	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //�ж�����ģʽ
	EXTI_InitStructure.EXTI_Line = EXTI_Line3; //�ⲿ�ж���
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //����������·�Ͻ���Ϊ�ж�����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; //ʹ��
	EXTI_Init(&EXTI_InitStructure);//��ʼ��

	NVIC_InitStructure.NVIC_IRQChannel =EXTI3_IRQn; //�ⲿ�ж���1���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;//��ռ���ȼ����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI1_IRQHandler(void)//�жϺ����ĸ�ʽ
{
	if (EXTI_GetITStatus(EXTI_Line1)==SET)//�ж��Ƿ�ΪEXTI1����������ֵΪ SET or RESET
	{
		light1 = 1;//��һ�����Զ�ģʽ
		if(++count1 %2)
			flag = 1;
		else
			led1 = 0;
		TIM_SetCompare2(TIM4,led1);
		EXTI_ClearITPendingBit(EXTI_Line1);//����жϺ����ı�־λ���������һֱ�����жϺ�����
	}
}

void EXTI4_IRQHandler(void)//�жϺ����ĸ�ʽ
{
	if (EXTI_GetITStatus(EXTI_Line4)==SET)//�ж��Ƿ�ΪEXTI1����������ֵΪ SET or RESET
	{
		light2 = 1;//�ƶ������Զ�ģʽ
		if(++count2 %2)
			flag = 2;
		else
			led2 = 0;
		TIM_SetCompare3(TIM4,led2);
		EXTI_ClearITPendingBit(EXTI_Line4);//����жϺ����ı�־λ���������һֱ�����жϺ�����
	}
}

void EXTI3_IRQHandler(void)//�жϺ����ĸ�ʽ
{
	
	if (EXTI_GetITStatus(EXTI_Line3)==SET)//�ж��Ƿ�ΪEXTI1����������ֵΪ SET or RESET
	{
		light3 = 1;//���������Զ�ģʽ
		if(++count3 %2)
			flag = 3;
		else
			led3 = 0;
		TIM_SetCompare4(TIM4,led3);
		EXTI_ClearITPendingBit(EXTI_Line3);//����жϺ����ı�־λ���������һֱ�����жϺ�����
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
