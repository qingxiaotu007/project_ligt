#include "hc_501.h"

void HC_501_Config(void)
{
  	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/*使能GPIO时钟 PB4 使能AFIO时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);

	/*配置PB4上拉输入模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode 用以设置选中管脚的工作状态。 //上拉输入模式  低电平有效
	GPIO_Init(GPIOB,&GPIO_InitStructure);//根据 GPIO_InitStruct 中指定的参数初始化外设 GPIOx 寄存器
	
	/*连接EXT4 到 PB4 引脚*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);//选择 GPIO 管脚用作外部中断线路
	
	/*配置EXTI4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;//EXTI_Line 选择了待使能或者失能的外部线路。 //EXTI_Line4外部中断线 4
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//设置 EXTI 线路为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//EXTI_Trigger 设置了被使能线路的触发边沿。//EXTI_Mode_Interrupt设置输入线路下降沿为中断请求
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//EXTI_LineCmd 用来定义选中线路的新状态
	EXTI_Init(&EXTI_InitStructure);//根据 EXTI_InitStruct 中指定的参数初始化外设 EXTI 寄存器
	
	/*配置EXTI4优先级及中断向量入口地址 */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//NVIC_IRQChanne4 该参数用以使能或者失能指定的 IRQ 通道。
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//该参数设置了成员 NVIC_IRQChannel 中的先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//该参数设置了成员 NVIC_IRQChannel 中的从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//该参数指定了在成员 NVIC_IRQChannel 中定义的 IRQ 通道被使能还是失能。
	NVIC_Init(&NVIC_InitStructure);//根据 NVIC_InitStruct 中指定的参数初始化外设 NVIC 寄存器
}


 
