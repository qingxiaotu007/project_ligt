#include "led.h"
#include "key.h"
#include "adc.h"

//LED1-->PB9,LED2-->PB8,LED3-->PB7 
extern void LED_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器4时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
   //设置该引脚为复用输出功能,输出TIM4 CH2的PWM脉冲波形	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9; //TIM4_CH2，TIM4_CH3，TIM4_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM4
	TIM_TimeBaseStructure.TIM_Prescaler =720-1; //设置用来作为TIMx时钟频率除数的预分频值 100US计数一次
	TIM_TimeBaseStructure.TIM_Period = 100-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_OCInitStructure.TIM_Pulse = 0;							//设置占空比大小，在主函数compare中又设置一遍
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM4 Channe2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC2
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC2
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC2

	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //使能重装寄存器
	TIM_Cmd(TIM4, ENABLE);  //使能TIM4
}

//灯的自动模式
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
