#include "delay.h"
#include "mq2.h"
#include "stm32f10x.h"                  // Device header  
 
 
void Mq_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);		//ʹ��GPIOB�˿�ʱ��
	
						//����һ������GPIO�Ľṹ�����	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;					//����PB1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    	//����PA0����Ϊ��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);            		 	//��ʼ��GPIOA
	
}
 
int Mq_read(void)
{
	if(gpio2_readC == 1)
	{ 
		return 0;
	}
	else
	{		
		return 1;
	}
}
