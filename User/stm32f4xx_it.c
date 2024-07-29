/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "stdio.h"
#include "led.h"
#include "ANO_DT.h"
#include "remotedata.h"
#include "structconfig.h"
#include "delay.h"

static uint8_t RxBuffer[30],MVRxBuffer[10];
static uint8_t RxCounter = 0,MVRxCounter = 0;
uint8_t LED_Scan = 0;
uint8_t IMU_Scan = 0;
uint8_t MPU_Scan = 0;
uint8_t IRQ_Scan = 0;
uint8_t Batt_Scan = 0;
uint8_t ANO_Scan = 0;

#ifdef INT_DELAY
extern uint32_t sysTickUptime;
#endif

/****************************************************************************************************
*函  数: void SysTick_Handler(void)
*功  能: SYSTick中断函数
*参  数: 无
*返回值：无
*备  注: 只有宏定义了 INT_DELAY 才会编译这个函数
****************************************************************************************************/
#ifdef INT_DELAY
void SysTick_Handler(void)
{
	 sysTickUptime++;
}
#endif



/****************************************************************************************************
*函  数: void USART2_IRQHandler(void)
*功  能: USART2中断函数，上位机和WiFi遥控数据共用
*参  数: 无
*返回值: 无
*备  注: 上位机与WiFi遥控最好不要同时用，当遥控数据一帧数据接收完成才触发空闲中断;
*      : 当用WiFi无线调参的时候用USART2接收上位机数据;
*      : 对于连续的数据帧的接收 接收中断与空闲中断配合能解决对报问题;
****************************************************************************************************/
void USART2_IRQHandler(void)
{
	uint8_t clear = clear; //防止编译时报错
	uint8_t res;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收中断
	{
		res = USART2->DR; //读DR清除中断标志
		ANO_DT_Data_Receive_Prepare(res); //上位机数据接收与解析
		RxBuffer[RxCounter++] = res; 
	}else if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) //空闲中断
	{
		clear = USART2->SR; //读SR寄存器
		clear = USART2->DR; //读DR寄存器（先读SR,再度DR,就是为了清除IDIE中断）
		WiFi_Data_ReceiveAnalysis(RxBuffer,RxCounter-1); //WiFi遥控器数据的解析
		RxCounter = 0;
	}
	USART_ClearITPendingBit(USART2,USART_IT_RXNE);
}
/****************************************************************************************************
*函  数: void USART1_IRQHandler(void)
*功  能: USART1中断函数，上位机与OpenMV共用
*参  数: 无
*返回值: 无
*备  注: 当插线调试参数时，用USART1
*      : 对于连续的数据帧的接收 接收中断与空闲中断配合能解决对报问题
****************************************************************************************************/
void USART1_IRQHandler(void)
{
	uint8_t clear = clear; //防止编译时报错
	uint8_t res;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收中断
	{
		res = USART1->DR; //读DR清除中断标志
		ANO_DT_Data_Receive_Prepare(res); //上位机数据接收与解析
		MVRxBuffer[MVRxCounter++] = res;
	}else if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) //空闲中断
	{
		clear = USART1->SR; //读SR寄存器
		clear = USART1->DR; //读DR寄存器（先读SR,再度DR,就是为了清除IDIE中断
//		Openmv_Data_ReceiveAnalysis(MVRxBuffer,MVRxCounter-1); //Openmv数据接收与解析函数
		MVRxCounter = 0;
	}
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
}
/****************************************************************************************************
*函  数: void TIM4_IRQHandler(void) 
*功  能: TIM4定时器中断，1ms进一次中断也就是1000Hz
*参  数: 无
*返回值: 无
*备  注: 此函数是整个程序的运行时基，不同的中断时间对应不同频率，
*        对于一些计算对调用时间要求比较严格时可用此方法；
*        扫描频率 = 1000Hz/分频系数；
****************************************************************************************************/
void TIM4_IRQHandler(void)
{
	static uint16_t ms2 = 0,ms5 = 0,ms10 = 0,ms100 = 0,ms200 = 0,ms400 = 0; //分频系数
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET)
	{
		ms2++;
		ms5++;
		ms10++;		
		ms100++;
		ms200++;
		ms400++;
		if(ms2 >= 2)//500Hz
		{
			ms2 = 0;
			ANO_Scan = 1;
		}
		if(ms5 >= 5)//200Hz
		{
			ms5 = 0;
			MPU_Scan = 1;
		}
		if(ms10 >= 10)//100Hz
		{
			ms10 = 0;
			IMU_Scan = 1;
		}
		if(ms100 >= 100)//10Hz
		{
			ms100 = 0;
			LED_Scan = 1;
		}
		if(ms200 >= 200)//5Hz
		{
			ms200 = 0;
			IRQ_Scan = 1;
		}
		if(ms400 >= 400)//2.5Hz
		{
			ms400 = 0;
			Batt_Scan = 1;
		}
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	RunTimer_Test2();
}


/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */

void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}


//外部中断
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line1)==SET)	//ȷ���ж���1���ж�
	{
		printf("Exit Enter!!!\r\n");	//�жϷ������
	}
	EXTI_ClearITPendingBit(EXTI_Line1); //����жϱ�־λ
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
