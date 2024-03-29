#include "time.h"
#include "stm32f10x.h"                  // Device header
 
 
void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//����ʱ��
	
	TIM_InternalClockConfig(TIM2);//�����ڲ�ʱ�Ӹ� TIM2�ṩʱ��Դ
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//�ṹ�嶨��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//����ʱ�ӷָ�,�����ǲ��ָ�
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period=36000-1;//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=50000-1;//Ԥ��Ƶֵ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//��ʱ0.001�룬����һ���ж�
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��TIM2
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//�ж����
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//������ʱ���ж�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�ж����ȼ�����
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;//�����ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//�ж�Դʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//��Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStructure); //����NVIC��ʼ������
	
	TIM_Cmd(TIM2,ENABLE);//��ʱ��2ʹ��
}
