#include "usart.h"
#include "structconfig.h"
/*****************************************************************************
*函  数：void Usart1_Init(uint32_t baudrate)
*功  能：Usart1初始化为双工模式( OpenMV 用此串口 )
*参  数：baudrate 波特率
*返回值：无
*备  注：对于连续的数据帧的接收 接收中断与空闲中断配合能解决丢包问题，
         具体接收方式见stm32f4xx_it.c 中的串口中断处理;
*****************************************************************************/
void Usart1_Init(int32_t baud)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//GPIOA时钟初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//USART1时钟初始化
	//连接USART1的通道到AF7
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	USART_DeInit(USART1);	//USART1复位
	
	//PA10(Tx)引脚（复用推挽输出模式）
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	//PA9(Rx)引脚（复用上拉输入）
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	//串口初始化
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=baud;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);	//USART1初始化
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//串口接收中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);	//串口空闲中断
	USART_Cmd(USART1, ENABLE);	//使能USART1
}
/*****************************************************************************
*函  数：void Usart2_Init(uint32_t baudrate)
*功  能：Usart2初始化为双工模式(ESP8266(wifi) 用此串口)
*参  数：baudrate 波特率
*返回值：无
*备  注：对于连续的数据帧的接收 接收中断与空闲中断配合能解决丢包问题
         具体接收方式见stm32f4xx_it.c 中的串口中断处理
*****************************************************************************/
void Usart2_Init(int32_t baud)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//GPIOA时钟初始化
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//USART2时钟初始化
	//连接USART2的通道到AF7
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
	USART_DeInit(USART2);	//USART2复位
	
	//PA2(Tx)引脚（复用推挽输出模式）
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	//PA3(Rx)引脚（复用上拉输入）
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	//串口2初始化
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=baud;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStructure);	//USART2初始化
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//串口接收中断
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);	//串口空闲中断
	USART_Cmd(USART2, ENABLE);	//使能USART2
}

//将printf();重定向到USART1
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t)ch);//USART1的发送函数
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){} //等待发送完成
	return ch;
}

int fgetc(FILE *f)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET)
	{}
	return (USART_ReceiveData(USART1));
}
/*****************************************************************************
*函  数：void Usart_Send(uint8_t *data, uint8_t length)
*功  能：Usart发送指定长度数据
*参  数：*data 要发送数据的地址
*        length 要发送数据的长度
*返回值：无
*备  注：宏定义WIFI_DEBUG，在structconfig.h 中定义和取消;
*        如果开启了WiFi调参功能，则数据从USART2传到ESP8266,然后经ESP8266再传到上位机;
*        如果未开启WiFi调参功能，则数据从USART1的Tx,Rx经线连接eLink32的Rx,Tx传到上位机;
*****************************************************************************/
void Usart_Send(uint8_t *data, uint8_t length)
{
	uint8_t  i;
	#if defined (WIFI_DEBUG) //开启WiFi（无线）调参
		for(i=0;i<length;i++)
		{
			USART_SendData(USART2, *(data+i));
			while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			{}
		}
	#else //有线调参
		for(i=0;i<length;i++) 
		{
			USART_SendData(USART1, *(data+i));
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			{}
		}
	#endif
}
