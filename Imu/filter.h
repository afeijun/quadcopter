#ifndef FILTER__H
#define FILTER__H

#include "stm32f4xx.h"
#include "structconfig.h"
#include "math.h"

void Aver_Filter(float data,float *filt_data,uint8_t n);
void  SortAver_FilterXYZ(INT16_XYZ *acc,FLOAT_XYZ *Acc_filt,uint8_t n);
#endif
