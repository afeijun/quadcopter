#ifndef LED__H
#define LED__H

#include "stm32f4xx.h"
#include "delay.h"
#include "stdlib.h"
#include "structconfig.h"

#define LED_OFF GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define LED_ON	GPIO_ResetBits(GPIOB, GPIO_Pin_8)
#define SET1 	GPIO_SetBits(GPIOB, GPIO_Pin_9)
#define SET0	GPIO_ResetBits(GPIOB, GPIO_Pin_9)

void LED_Init(void);
void LED_Run(void);
void RGB_LED_Rand(void);
void RGB_LED_Runing(void);
void RGB_LED_Off(void);
void RGB_LED_Red(void);
void RGB_LED_Orange(void);
void RGB_LED_Yellow(void);
void RGB_LED_Green(void);
void RGB_LED_Cyan(void);
void RGB_LED_Blue(void);
void RGB_LED_Violet(void);
void RGB_LED_FLY(void);
void RGB_LED_White(void);
void GYRO_Offset_LED(void);
void ACC_Offset_LED(void);
void BAR_Offset_LED(void);
void WiFi_OFFON_LED(void);
void BATT_Alarm_LED(void);
void OneNET_LED(uint8_t color[],uint8_t num);

void RGB_Unlock(uint8_t N,uint8_t flag);
void RunTimer_Test(void);
void RunTimer_Test2(void);

#endif
