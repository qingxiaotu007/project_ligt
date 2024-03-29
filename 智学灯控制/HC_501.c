#include "HC_501.h"


void HC_501_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);		//ʹ��GPIOB�˿�ʱ��
	
						//����һ������GPIO�Ľṹ�����	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;					//����PB4,PB5,PB6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    	//����PB4,PB5,PB6����Ϊ��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);            		 	//��ʼ��GPIOB
}

void HC_501_Check(void)
{
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4))
	{
		TIM_SetCompare2(TIM4,0);
	}
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5))
	{
		TIM_SetCompare3(TIM4,0);
	}
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6))
	{
		TIM_SetCompare4(TIM4,0);
	}
}
