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
	u16 Tnum;  //���ö�ʱ������ֵ //800hz
	s16 Tfum; //36��Ƶ
	double stepnum;  // ������	
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
	s32 L;//�岹�ܲ���
	s32 Xe;//X����Ӧ��������
	s32 Ye;//Y����Ӧ��������
	s32 F; //��ƫ�����
	u8  Quadrant;//���ͱ�־λ��������ֵ L1 2 3 4��
};
struct CIRCULA
{
	s32 L;//�岹�ܲ���
	s32 Xe;//X����Ӧ��������
	s32 Ye;//Y����Ӧ��������
	s32 F; //��ƫ�����
	u8  Quadrant;//���ޱ�־λ��������ֵ SRx NRx��
	struct XY Centre;
	u8 type;	//˳ ��Բ ��־λ
//	float Radius;	//Բ���뾶ֵ
};
#define bool int
#define false 0
#define true 1
	
#define POS 0		//���ٶ�λ G00
#define LINE 1		//ֱ�߲岹 G01
#define CW_ARC 2	//˳Բ�岹 G02
#define CCW_ARC 3	//��Բ�岹 G03

#define CR 1	//˳Բ ��־
#define NR 2	//��Բ ��־

#define SR1	1		//��һ���� ˳Բ     
#define NR1 5	//��һ���� ��Բ
#define SR2 2		//  ��
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

extern u8 XYmove; //����XY�˶���־λ
extern u8 timb;  //�����жϱ�־λ



#endif

