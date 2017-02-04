#include "include.h"	
#include "led.h"   
#include "timer.h" 
#include "stepper.h"
#include "stringdeal.h"
#include "moto_control.h"
//#include "LCD.h"
#include <string.h>
#include <stdlib.h>

#define bool int
#define false 0
#define true 1
	
#define X_AXIS 0 
#define I_AXIS 0
#define Y_AXIS 1
#define J_AXIS 1
#define Z_AXIS 2 
#define N_AXIS 3 
	
#define MAX_INT_DIGITS 12

#define steptime 2000
#define quickStepTime 2000

struct INTERP Itp;
struct SETTINGS Settings;
struct XY Current;
struct XY Target;
struct LINEAR Linear;
struct CIRCULA Circula;

u8 XYmove=0; //定义XY运动标志位
u8 timb=0;  //设置中断标志位
u8 line[250]={0};


//定时器3中断服务程序	 
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{
		//LED1=!LED1;
		if(XYmove == 1)
		{
			STEP1 = 1;
			delay_us(1);
			STEP1 = 0;				
		}
		if(XYmove==2)
		{
			STEP0 = 1;
			delay_us(1);
			STEP0 = 0;			
		}
		
		timb = 0;
	}				   
	TIM3->SR &= ~(1<<0);//清除中断标志位 	    
}
int main(void)
{		
	u8 t;
	u8 len;
	//系统时钟设置	
 	Stm32_Clock_Init(9); 
	//延时初始化
	delay_init(72);	     
	//串口初始化
	uart_init(72,9600); 
	
	Settings.Tnum=2499;
	Settings.Tfum=35;
	Settings.stepnum=106.666;
	Settings.mm_per_arc_segment=0.1;
	Settings.n_arc_correction=25;
	
	Itp.type=0;
	Itp.x=0;
	Itp.y=0;
	Itp.i=0;
	Itp.j=0;	
	
	stepperInit();
	//初始化与LED连接的硬件接口
	LED_Init();		  	 
	//10Khz的计数频率，计数到5000为500ms  
	TIM3_Int_Init(Settings.Tnum,Settings.Tfum);
	//LCD_Init();
	//POINT_COLOR=RED; 
	printf("赵日天一号机\r\n");
   	while(1)
	{
		if(USART_RX_STA & 0x8000)
		{	
			LED0 = 0;
			len = USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			for(t=0;t<len;t++)  //轮训的方式获得此次接收信息赋值给字符串line
			{
				line[t]=USART_RX_BUF[t];
				//LCD_ShowString(30,40,200,24,12,line);
			}
			USART_RX_STA = 0;
			if(line[0] == 0)
			{
				//printf("\r\n准备就绪\r\n\r\n");
				//delay_ms(500);
			}
			else if(line[0] == '$')
			{
				printf("\r\n“$”帮助信息\r\n\r\n"); //打印帮助信息
				printf("ok\r\n");
			}
			else if(line[0] == 'R')
			{
               	NVIC_SystemReset();
			}
			else 
			{
				gc_execute_line(line);
				Interpolation_Process();
				printf("ok\r\n");
			}
			memset(line,0,sizeof(line));
		}
	}
}



//非定时器中断版XY轴运动驱动函数
/*void Xzheng(void)
{
	XYmove=2;//X轴运动
	DIR0 = 1;
	
	STEP0 = 1;
	delay_us(steptime);
	STEP0 = 0;	
	
	Current.x++;
}
void Xni(void)
{
	XYmove=2;//X轴运动
	DIR0 = 0;
	
	STEP0 = 1;
	delay_us(steptime);
	STEP0 = 0;	
	
	Current.x--;
}
void Yzheng(void)
{
	XYmove=1;//Y轴运动
	DIR1 = 0;
	
	STEP1 = 1;
	delay_us(steptime);
	STEP1 = 0;	
	
	Current.y++;
}
void Yni(void)
{
	XYmove=1;//Y轴运动
	DIR1 = 1;
	
	STEP1 = 1;
	delay_us(steptime);
	STEP1 = 0;	
	
	Current.y--;
}*/





