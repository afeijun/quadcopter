#ifndef IIC__H
#define IIC__H

#include "stm32f4xx.h"
#include "math.h"
#include "stdio.h"
#include "delay.h"

/******************************注意！！！**************************************
用于MPU9250、FBM320与MCU通信
*******************************************************************************/

//PB6->SCL
#define RCC_IIC_SCL 	RCC_AHB1Periph_GPIOB	//端口时钟
#define IIC_SCL_PORT	GPIOB					//端口
#define IIC_SCL			GPIO_Pin_6				//引脚
//PB7->SDA
#define RCC_IIC_SDA		RCC_AHB1Periph_GPIOB	//端口时钟
#define IIC_SDA_PORT	GPIOB					//端口
#define IIC_SDA			GPIO_Pin_7				//引脚

//IO操作
#define IIC_SCL_H		GPIO_SetBits(IIC_SCL_PORT, IIC_SCL)
#define IIC_SCL_L		GPIO_ResetBits(IIC_SCL_PORT, IIC_SCL)
#define IIC_SDA_H		GPIO_SetBits(IIC_SDA_PORT, IIC_SDA)
#define IIC_SDA_L		GPIO_ResetBits(IIC_SDA_PORT, IIC_SDA)
#define READ_SDA		(IIC_SDA_PORT->IDR&IIC_SDA) ? 1 : 0	//输入SDA


//求偏移量
#define IIC_SCL_OFFSET	(uint8_t)(log(IIC_SCL)/log(2))
#define IIC_SDA_OFFSET	(uint8_t)(log(IIC_SDA)/log(2))

//SCL-->PB6
//SDA-->PB7
//改变SDA的输入输出模式
//输入模式
#define SDA_IN()	{IIC_SDA_PORT->MODER&=~(3<<(IIC_SDA_OFFSET*2));IIC_SDA_PORT->MODER|=0<<IIC_SDA_OFFSET*2;}
//输出模式
#define SDA_OUT()	{IIC_SDA_PORT->MODER&=~(3<<(IIC_SDA_OFFSET*2));IIC_SDA_PORT->MODER|=1<<IIC_SDA_OFFSET*2;}

/******************************注意！！！**************************************
移植此驱动，驱动DMP解算四元数时，注意从机地址要设置成0xD0
*******************************************************************************/
//IIC所有操作函数
void IIC_Init(void);
void IIC_Start(void);//发送IIC开始信号
void IIC_Stop(void);//发送IIC停止信号
void IIC_Ack(void);//IIC发送ACK信号
void IIC_NAck(void);//IIC不发送ACK信号
uint8_t IIC_WaitAck(void);//IIC等待ACK信号

void IIC_SendByte(uint8_t data);//IIC发送一个字节
uint8_t IIC_ReadByte(uint8_t ack);//IIC读取一个字节


//mpu9250
uint8_t IIC_ReadByteFromSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t *buf);
uint8_t IIC_WriteByteToSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t data);
uint8_t IIC_ReadMultByteFromSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t length,uint8_t *data);
uint8_t IIC_WriteMultByteToSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t length,uint8_t *data);

#endif
