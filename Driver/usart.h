#ifndef USART__H
#define USART__H

#include "stm32f4xx.h"
#include "stdio.h"

void Usart1_Init(int32_t baud);
void Usart2_Init(int32_t baud);
void Usart_Send(uint8_t *data, uint8_t length);

#endif
