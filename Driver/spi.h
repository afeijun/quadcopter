#ifndef SPI__H
#define SPI__H

#include "stm32f4xx.h"

#define NSS_L	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
#define NSS_H	GPIO_SetBits(GPIOB,GPIO_Pin_12);

void SPI2_Init(void);
uint16_t SPI_WriteReadByte(uint16_t data);
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);
#endif
