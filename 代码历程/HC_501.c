#include "HC_501.h"


void HC_501_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);		//使能GPIOB端口时钟
	
						//定义一个设置GPIO的结构体变量	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;					//设置PB4,PB5,PB6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    	//设置PB4,PB5,PB6引脚为浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);            		 	//初始化GPIOB
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
