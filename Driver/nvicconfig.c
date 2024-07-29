#include "nvicconfig.h"
#include "stm32f4xx.h"
/*****************************************************************************
*函  数：void NVIV_Config(void)
*功  能：配置工程中所有中断的优先级
*参  数：无
*返回值：无
*备  注：此优先级中断不要随便更改哦
*****************************************************************************/
void NVIC_config(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//配置NVIC为分组2
	
	
	NVIC_InitTypeDef NVIC_InitStructure;
	//程序时基TIM4定时器中断优先级
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;//外部中断1通道
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//外部中断1通道使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//响应优先级
	NVIC_Init(&NVIC_InitStructure);
	//USART2中断（用于连接ESP8266与手机或上位机通信）
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;//外部中断1通道
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//外部中断1通道使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//响应优先级
	NVIC_Init(&NVIC_InitStructure);
	//USART1中断（用于调式）
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;//外部中断1通道
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//外部中断1通道使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;//响应优先级
	NVIC_Init(&NVIC_InitStructure);
	//NRF2401的IRQ中断
	NVIC_InitStructure.NVIC_IRQChannel=EXTI2_IRQn;//外部中断1通道
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//外部中断1通道使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;//响应优先级
	NVIC_Init(&NVIC_InitStructure);
}