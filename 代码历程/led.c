#include "led.h"
#include "key.h"
#include "adc.h"

//LED1-->PB9,LED2-->PB8,LED3-->PB7 
extern void LED_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ�ܶ�ʱ��4ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
   //���ø�����Ϊ�����������,���TIM4 CH2��PWM���岨��	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9; //TIM4_CH2��TIM4_CH3��TIM4_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM4
	TIM_TimeBaseStructure.TIM_Prescaler =720-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 100US����һ��
	TIM_TimeBaseStructure.TIM_Period = 100-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_OCInitStructure.TIM_Pulse = 0;							//����ռ�ձȴ�С����������compare��������һ��
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM4 Channe2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC2
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC2
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC2

	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ����װ�Ĵ���
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4
}

//�Ƶ��Զ�ģʽ
void LED_Auto(void)
{
	if(light1 == 0 && GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4))
	{
		led1=Get_Adc2_Average(ADC_Channel_8,5)*100/4096;
		TIM_SetCompare2(TIM4,led1);
	}
	if(light2 == 0 && GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5))
	{
		led2=Get_Adc2_Average(ADC_Channel_6,5)*100/4096;
		TIM_SetCompare3(TIM4,led2);
	}
	if(light3 == 0 && GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6))
	{
		led3=Get_Adc2_Average(ADC_Channel_5,5)*100/4096;
		TIM_SetCompare4(TIM4,led3);
	}
}
