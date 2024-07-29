#ifndef POWER__H
#define POWER__H

#include "stm32f4xx.h"
#include "structconfig.h"
#include "filter.h"
#include "nrf24l01.h"
#include "stdio.h"

void BATT_Init(void);
void LowVoltage_Alarm(void);
void WiFi_Switch(uint8_t flag);
void OpenMV_Switch(uint8_t flag);
void NRF2401_Switch(uint8_t flag);

#endif
