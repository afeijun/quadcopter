#ifndef DELAY__H
#define DELAY__H

#include "stm32f4xx.h"

/**********延时方式选择*************/
#define NOINT_DELAY //不开中断延时
//#define INT_DELAY //开中断延时
/**********************************/

void Delay_Init(void);
void delay_ms(uint32_t nms);
void delay_us(uint32_t nus);
void delay(int cnt);

uint32_t micros(void);	//微妙计数器
uint32_t millis(void); //毫秒计数器

#endif
