#include "motor.h"

#define Moto_PwmMax 1000
int16_t MOTO1_PWM = 0;
int16_t MOTO2_PWM = 0;
int16_t MOTO3_PWM = 0;
int16_t MOTO4_PWM = 0;

/******************************************************************************************
*函  数：void MOTOR_Init(void)
*功  能：输出PWM初始化
*参  数：无
*返回值：无
*备  注：TIMx_ARR  决定方波的周期
*        TIMx_CCRx 决定方波的占空比
*******************************************************************************************/
void MOTOR_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE); //GPIOA GPIOB 时钟使能 
	
	//GPIO配置：TIM3 CH1（PA6）,TIM3 CH2（PA7）,TIM3 CH3(PB0), TIM3 CH4(PB1)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	//复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100M
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//上拉输入
	//PA6,PA7初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//PB0,PB1初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//连接TIM3的通道到AF2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);
	//配置时基单元
	TIM_TimeBaseStructure.TIM_Period = 1000-1;	//自动重装载值
	TIM_TimeBaseStructure.TIM_Prescaler = 100-1;	//预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//时钟分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上自增
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	//配置输出比较器（OC）
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	//PWM1 模式配置 通道1
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	//PWM2 模式配置 通道2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	//PWM1 模式配置 通道3
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	//PWM1 模式配置 通道4
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	//ARR 寄存器预装载
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	
	TIM_Cmd(TIM3,ENABLE); //使能TIM3
}
/*****************************************************************************
*函  数：void Moto_Pwm(int16_t MOTO1_PWM,int16_t MOTO2_PWM,int16_t MOTO3_PWM,int16_t MOTO4_PWM)
*功  能：电机要输出数值转换成PWM波形输出
*参  数：MOTO1_PWM 电机1
*        MOTO2_PWM 电机2
*        MOTO3_PWM 电机3
*        MOTO3_PWM 电机4
*返回值：无 
*备  注：无
*****************************************************************************/
void Moto_Pwm(int16_t MOTO1_PWM,int16_t MOTO2_PWM,int16_t MOTO3_PWM,int16_t MOTO4_PWM)
{
	if(MOTO1_PWM>Moto_PwmMax)	MOTO1_PWM = Moto_PwmMax;
	if(MOTO2_PWM>Moto_PwmMax)	MOTO2_PWM = Moto_PwmMax;
	if(MOTO3_PWM>Moto_PwmMax)	MOTO3_PWM = Moto_PwmMax;
	if(MOTO4_PWM>Moto_PwmMax)	MOTO4_PWM = Moto_PwmMax;
	if(MOTO1_PWM<0)	MOTO1_PWM = 0;
	if(MOTO2_PWM<0)	MOTO2_PWM = 0;
	if(MOTO3_PWM<0)	MOTO3_PWM = 0;
	if(MOTO4_PWM<0)	MOTO4_PWM = 0;
	
	TIM3->CCR1 = MOTO1_PWM;
	TIM3->CCR2 = MOTO2_PWM;
	TIM3->CCR3 = MOTO3_PWM;
	TIM3->CCR4 = MOTO4_PWM;
}