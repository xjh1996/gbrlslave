#ifndef __INCLUDE_H
#define	__INCLUDE_H

#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include <string.h>
#include <stdlib.h>
#include <math.h>
struct INTERP
{
	u8 type;
	double x;
	double y;
	double i;
	double j;
	
};

struct SETTINGS
{
	u16 Tnum;  //设置定时器处初值 //800hz
	s16 Tfum; //36分频
	double stepnum;  // 脉冲数	
	double mm_per_arc_segment;
	u8 n_arc_correction;
};
struct XY
{
	s32 x;
	s32 y;
};
//struct POSITION
//{
//	struct XY Previous;
//	struct XY Current;	
//};
struct LINEAR
{
	s32 L;//插补总步数
	s32 Xe;//X坐标应进给步数
	s32 Ye;//Y坐标应进给步数
	s32 F; //新偏差计算
	u8  Quadrant;//线型标志位（用来赋值 L1 2 3 4）
};
struct CIRCULA
{
	s32 L;//插补总步数
	s32 Xe;//X坐标应进给步数
	s32 Ye;//Y坐标应进给步数
	s32 F; //新偏差计算
	u8  Quadrant;//象限标志位（用来赋值 SRx NRx）
	struct XY Centre;
	u8 type;	//顺 逆圆 标志位
//	float Radius;	//圆弧半径值
};
#define bool int
#define false 0
#define true 1
	
#define POS 0		//快速定位 G00
#define LINE 1		//直线插补 G01
#define CW_ARC 2	//顺圆插补 G02
#define CCW_ARC 3	//逆圆插补 G03

#define CR 1	//顺圆 标志
#define NR 2	//逆圆 标志

#define SR1	1		//第一象限 顺圆     
#define NR1 5	//第一象限 逆圆
#define SR2 2		//  二
#define NR2 6
#define SR3 3
#define NR3 7
#define SR4 4
#define NR4 8

#define X_AXIS 0 
#define I_AXIS 0
#define Y_AXIS 1
#define J_AXIS 1
#define Z_AXIS 2 
#define N_AXIS 3 
	
#define MAX_INT_DIGITS 12

//extern struct POSITION Position;
extern struct XY Current;
extern struct XY Target;
extern struct INTERP Itp;
extern struct SETTINGS Settings;
extern struct LINEAR Linear;
extern struct CIRCULA Circula;

extern u8 XYmove; //定义XY运动标志位
extern u8 timb;  //设置中断标志位



#endif

