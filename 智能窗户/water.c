#include "delay.h"
#include "water.h"
#include "stm32f10x.h"                  // Device header  
 
 
void Water_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);		//ʹ��GPIOC�˿�ʱ��
	
						//����һ������GPIO�Ľṹ�����	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;					//����PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    	//����PA0����Ϊ��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);            		 	//��ʼ��GPIOA
	
}
 
int Water_read(void)
{
	if(gpio_readC == 1)
	{ 
		return 0;
	}
	else
	{		
		return 1;
	}
}
