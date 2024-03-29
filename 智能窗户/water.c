#include "delay.h"
#include "water.h"
#include "stm32f10x.h"                  // Device header  
 
 
void Water_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);		//使能GPIOC端口时钟
	
						//定义一个设置GPIO的结构体变量	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;					//设置PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    	//设置PA0引脚为浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);            		 	//初始化GPIOA
	
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
