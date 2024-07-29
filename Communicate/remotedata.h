#ifndef REMOTEDATA__H
#define REMOTEDATA__H

#include "stm32f4xx.h"
#include "structconfig.h"
#include "nrf24l01.h"
#include "led.h"
#include "power.h"

void WiFi_Data_ReceiveAnalysis(uint8_t *buff, uint8_t cnt);
void Remote_Data_ReceiveAnalysis(void);
void Button_command(uint8_t Button);
void NRF_SingalCheck(void);
void SendToRemote(void);
#endif
