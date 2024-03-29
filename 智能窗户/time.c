#include "time.h"
#include "stm32f10x.h"                  // Device header
 
 
void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//开启时钟
	
	TIM_InternalClockConfig(TIM2);//采用内部时钟给 TIM2提供时钟源
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//结构体定义
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//设置时钟分割,这里是不分割
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period=36000-1;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler=50000-1;//预分频值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//定时0.001秒，进入一次中断
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM2
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//中断清除
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//开启定时器中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断优先级分组
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;//设置中断源
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//中断源使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//响应优先级
	NVIC_Init(&NVIC_InitStructure); //调用NVIC初始化函数
	
	TIM_Cmd(TIM2,ENABLE);//定时器2使能
}
