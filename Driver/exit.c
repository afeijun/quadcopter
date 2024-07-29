#include "exit.h"

/****************************************************************************************************
*函  数: void EXTI_GPIOConfig(void)
*功  能: 配置与NRF的IRQ相连的IO
*参  数: 无
*返回值：无
*备  注: 无
****************************************************************************************************/
void EXTI_GPIOConfig(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	//GPIOC时钟初始化
	
	//PB2引脚配置输入模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
}
/****************************************************************************************************
*函  数: void Exti_Init(void)
*功  能: 外部中断
*参  数: 无
*返回值：无
*备  注: NRF的IRQ引脚触发的外部中断
****************************************************************************************************/
void Exti_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_EXTIT,ENABLE); //使能外部中断时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能系统配置时钟
	
	EXTI_GPIOConfig();	//EXIT相关的GPIO
	EXTI_DeInit();	//复位外部中断
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);	//将中断线EXTI Line2与PB2连接
	
	//初始化中断线2
	EXTI_InitStructure.EXTI_Line=EXTI_Line2;//外部中断线2
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;//使能外部中断线2
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//中断模式
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿触发
	EXTI_Init(&EXTI_InitStructure);
}

