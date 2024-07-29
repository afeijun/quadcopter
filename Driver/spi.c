#include "spi.h"

static void SPI_GPIOConfig(void)
{
	
	//复用引脚配置,将PB13 14 15复用为SPI2的对应引脚
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15, GPIO_AF_SPI2);
	
	//GPIO模式配置
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
	
	//SCK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//MOSI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//MISO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}
/*****************************************************************************
*函  数：void SPI2_Init(void)
*功  能：SPI2初始化
*参  数：无
*返回值：无
*备  注：调试SPI通信时一定要分清主机从机模式；
*        主机从机模式的空闲状态电平（）
*****************************************************************************/
void SPI2_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);	//spi2时钟初始化
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);
	
	SPI_GPIOConfig();	//SPI2外设引脚配置
	
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;	//4分频 25MHz/4=6.25MHz
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;	//第一个时钟边沿采样
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;	//SCK空闲电平为低
	SPI_InitStructure.SPI_CRCPolynomial=7;	//CRC校验
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;	//单位数据长度8
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;	//全双工通信
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;	//高位先行
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;	//主机模式
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;		//NSS引脚软件控制
	SPI_Init(SPI2, &SPI_InitStructure);	//SPI2初始化
	
	SPI_Cmd(SPI2, ENABLE);	//SPI2使能
}
/*****************************************************************************
*函  数：void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
*功  能：SPI2通信速度设置
*参  数：无
*返回值：无
*备  注：无
*****************************************************************************/
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI_Cmd(SPI2, DISABLE);   //失能SPI2
	SPI2->CR1 &= ~(0x07 << 3);//清除原来的设置
	SPI2->CR1 |= SPI_BaudRatePrescaler;
	SPI_Cmd(SPI2, ENABLE);   //使能SPI2
}
/*****************************************************************************
*函  数：uint8_t SPI2_WriteReadByte(uint8_t data)
*功  能：SPI2读写一个字节
*参  数：无
*返回值：无
*备  注：无
*****************************************************************************/
uint16_t SPI_WriteReadByte(uint16_t data)
{

	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE));	//等待发送缓冲区为空
	SPI_I2S_SendData(SPI2, data);	//发送数据
	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE));	//等待接收缓冲区非空
	return SPI_I2S_ReceiveData(SPI2);
}

