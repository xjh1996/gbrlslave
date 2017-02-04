#include "stepper.h"

void stepperInit(void)
{
	RCC->APB2ENR|=1<<2;
	GPIOA->CRL&=0X000000FF;
	GPIOA->CRL|=0X33333300;
	STEP0 = 0;
	STEP1 = 0;
}
void Xzheng(void)
{
	XYmove=2;//X轴运动
	DIR0 = 1;
	timb=1;
	
	TIM3->CR1|=0x01;    //使能定时器3
	while(timb ==1);
	TIM3->CR1&=0Xfe;
	Current.x++;
}
void Xni(void)
{
	XYmove=2;//X轴运动
	DIR0 = 0;
	timb=1;
	
	TIM3->CR1|=0x01;    //使能定时器3
	while(timb==1);
	TIM3->CR1&=0Xfe;
	Current.x--;
}
void Yzheng(void)
{
	XYmove=1;//Y轴运动
	DIR1 = 0;
	timb=1;
	
	TIM3->CR1|=0x01;    //使能定时器3
	while(timb==1);
	TIM3->CR1&=0Xfe;
	Current.y++;
}
void Yni(void)
{
	XYmove=1;//Y轴运动
	DIR1 = 1;
	timb=1;
	
	TIM3->CR1|=0x01;    //使能定时器3
	while(timb==1);
	TIM3->CR1&=0Xfe;
	Current.y--;
}

