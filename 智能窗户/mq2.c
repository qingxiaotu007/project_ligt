#include "delay.h"
#include "mq2.h"
#include "stm32f10x.h"                  // Device header  
 
 
void Mq_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);		//使能GPIOB端口时钟
	
						//定义一个设置GPIO的结构体变量	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;					//设置PB1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    	//设置PA0引脚为浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);            		 	//初始化GPIOA
	
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
