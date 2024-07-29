#include "timer.h"

/*****************************************************************************
*函  数：void TIM4_Init(void)
*功  能：TIM4初始化为1ms计数一次
*参  数：无
*返回值：无
*备  注：更新中断时间 Tout = (ARR-1)*(PSC-1)/CK_INT
*****************************************************************************/
void TIM4_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	//TIM2~TIM5时钟来源于APB1*2
	TIM_TimeBaseStructure.TIM_Period = 1000-1;	//自动重装载值
	TIM_TimeBaseStructure.TIM_Prescaler = 100-1;	//预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//时钟不分配 100MHz
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM4, ENABLE);
}