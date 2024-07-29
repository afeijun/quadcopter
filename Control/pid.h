#ifndef PID__H
#define PID__H

#include "stm32f4xx.h"
#include "structconfig.h"
#include "paramsave.h"
#include "led.h"

void PidParameter_init(void);
void PID_Postion_Cal(PID_TYPE*PID,float target,float measure);
#endif
