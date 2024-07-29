#ifndef CONTROL__H
#define CONTROL__H

#include "stm32f4xx.h"
#include "pid.h"
#include "structconfig.h"
#include "led.h"
#include "math.h"
#include "motor.h"
#include "stdio.h"
#include "imu.h"

void Yaw_Carefree(FLOAT_ANGLE *Target_Angle, const FLOAT_ANGLE *Measure_Angle);
void Control(FLOAT_ANGLE *att_in,FLOAT_XYZ *gyr_in, RC_TYPE *rc_in, uint8_t armed);
#endif
