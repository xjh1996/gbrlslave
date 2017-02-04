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

u8 XYmove=0; //����XY�˶���־λ
u8 timb=0;  //�����жϱ�־λ
u8 line[250]={0};


//��ʱ��3�жϷ������	 
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//����ж�
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
	TIM3->SR &= ~(1<<0);//����жϱ�־λ 	    
}
int main(void)
{		
	u8 t;
	u8 len;
	//ϵͳʱ������	
 	Stm32_Clock_Init(9); 
	//��ʱ��ʼ��
	delay_init(72);	     
	//���ڳ�ʼ��
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
	//��ʼ����LED���ӵ�Ӳ���ӿ�
	LED_Init();		  	 
	//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  
	TIM3_Int_Init(Settings.Tnum,Settings.Tfum);
	//LCD_Init();
	//POINT_COLOR=RED; 
	printf("������һ�Ż�\r\n");
   	while(1)
	{
		if(USART_RX_STA & 0x8000)
		{	
			LED0 = 0;
			len = USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			for(t=0;t<len;t++)  //��ѵ�ķ�ʽ��ô˴ν�����Ϣ��ֵ���ַ���line
			{
				line[t]=USART_RX_BUF[t];
				//LCD_ShowString(30,40,200,24,12,line);
			}
			USART_RX_STA = 0;
			if(line[0] == 0)
			{
				//printf("\r\n׼������\r\n\r\n");
				//delay_ms(500);
			}
			else if(line[0] == '$')
			{
				printf("\r\n��$��������Ϣ\r\n\r\n"); //��ӡ������Ϣ
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



//�Ƕ�ʱ���жϰ�XY���˶���������
/*void Xzheng(void)
{
	XYmove=2;//X���˶�
	DIR0 = 1;
	
	STEP0 = 1;
	delay_us(steptime);
	STEP0 = 0;	
	
	Current.x++;
}
void Xni(void)
{
	XYmove=2;//X���˶�
	DIR0 = 0;
	
	STEP0 = 1;
	delay_us(steptime);
	STEP0 = 0;	
	
	Current.x--;
}
void Yzheng(void)
{
	XYmove=1;//Y���˶�
	DIR1 = 0;
	
	STEP1 = 1;
	delay_us(steptime);
	STEP1 = 0;	
	
	Current.y++;
}
void Yni(void)
{
	XYmove=1;//Y���˶�
	DIR1 = 1;
	
	STEP1 = 1;
	delay_us(steptime);
	STEP1 = 0;	
	
	Current.y--;
}*/





