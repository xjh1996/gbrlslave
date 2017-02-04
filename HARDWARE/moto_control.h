#ifndef __MOTO_CONTROL_H
#define __MOTO_CONTROL_H

#include "include.h"
#include "stepper.h"

u8 Interpolation_Process(void);//main中调用的线程
/////////////////////////////////////////////////////////////////
u8 Linear_SetTarget(double x,double y);
//void Linear_QuadrantJudge(s32 x,s32 y);
u8 Linear_Interpolation(double x,double y);
u8 Circula_SetSNR(u8 type);
u8 Circula_Interpolation(double x,double y,double i,double j);

#endif
