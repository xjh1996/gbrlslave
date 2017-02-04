#include "moto_control.h"

#define M_PI 3.14159265358979323846

u8 Interpolation_Process(void)//main�е��õ��߳�
{
	switch(Itp.type)
	{
		case POS: 
			Linear_Interpolation(Itp.x,Itp.y);
			break;
		case LINE:
			Linear_Interpolation(Itp.x,Itp.y);			
			break;
		case CW_ARC:
			Circula_SetSNR(CR);
			Circula_Interpolation(Itp.x,Itp.y,Itp.i,Itp.j);
			break;
		case CCW_ARC:
			Circula_SetSNR(NR);
			Circula_Interpolation(Itp.x,Itp.y,Itp.i,Itp.j);
			//mc_arc(0);		
			break;
		
		default:;
	}
	//sys.motion_eCurrent.xute_ok=true;
	return 0;
}

static void Linear_StepCalculate(double x,double y)
{
	Linear.Xe=(s32)(x*Settings.stepnum)-Current.x;
	Linear.Ye=(s32)(y*Settings.stepnum)-Current.y;
}
static void Linear_QuadrantJudge(void)
{
	if((Linear.Xe)>=0)
	{
		if((Linear.Ye)>=0)
		{
			Linear.Quadrant=1;//����R1�岹��Linear.Xe>=0,Linear.Ye>=0;		
		}
		else
		{
			Linear.Quadrant=4;//����R4�岹��Linear.Xe>=0,Linear.Ye<0;
		}
	}
	else if(Linear.Ye>=0)
	{
		Linear.Quadrant=2;//����R2�岹��Linear.Xe<0,Linear.Ye>=0;		
	}
	else
	{
		Linear.Quadrant=3;//����R3�岹��Linear.Xe<0,Linear.Ye<0;
	}
}

u8 Linear_Interpolation(double x,double y)
{
	Linear_StepCalculate(x,y);				//λ��ת��Ϊ����(��������һ�г���֮ǰ�����������б����)
	Linear_QuadrantJudge();
	Linear.L=abs(Linear.Xe)+abs(Linear.Ye);	
	Linear.F=0;
	
	if(Linear.Quadrant==1)//����R1�岹
	{
		while(Linear.L!=0)
		{
			if(Linear.F>=0)
			{
					Xzheng();
					Linear.F=Linear.F-Linear.Ye;		
			}
			else
			{
					Yzheng();					
					Linear.F=Linear.F+Linear.Xe;		
			}
			Linear.L--;
		}
	}
	else if(Linear.Quadrant==2)//����R2�岹
	{
		while(Linear.L!=0)
		{
			if(Linear.F>=0)
			{
				Xni();
				Linear.F=Linear.F-abs(Linear.Ye);		
			}
			else
			{
				Yzheng();
				Linear.F=Linear.F+abs(Linear.Xe);		
			}
			Linear.L--;
		}
	}
	else if(Linear.Quadrant==3)//����R3�岹
	{
		while(Linear.L!=0)
		{
			if(Linear.F>=0)
			{
					Xni();
					Linear.F=Linear.F-abs(Linear.Ye);		
			}
			else
			{
					Yni();
					Linear.F=Linear.F+abs(Linear.Xe);
			}
			Linear.L--;
		}
	}
	else if(Linear.Quadrant==4)//����R4�岹
	{
		while(Linear.L!=0)
		{
			if(Linear.F>=0)
			{
				Xzheng();
				Linear.F=Linear.F-abs(Linear.Ye);
			}
			else
			{
				Yni();
				Linear.F=Linear.F+abs(Linear.Xe);
			}
			Linear.L--;
		}
	}
	return 0;
}
//u8 Circula_SetTarget(double x,double y,double i,double j)
//{
//	Target.x = (s32)(Itp.x*Settings.stepnum); //��������������������Ծ���֧�ֵ�С�������λ
//	Target.y = (s32)(Itp.y*Settings.stepnum);
//	CirclePosition.i=(s32)(Itp.i*Settings.stepnum);
//	CirclePosition.j=(s32)(Itp.j*Settings.stepnum);
//	return 0;
//}
/*void mc_arc(bool isclockwise)
{
	double center_axis0 = Position.Current.x + Itp.i;
	double center_axis1 = Position.Current.y + Itp.j;
	double linear_travel = 0; //target[axis_linear] - position[axis_linear];
	double r_axis0 = -Itp.i;  // Radius vector from center to current location
	double r_axis1 = -Itp.j;
	double rt_axis0 = Itp.x - center_axis0;
	double rt_axis1 = Itp.y - center_axis1;
	double radius=Itp.i*Itp.i+Itp.j+Itp.j;
	double angular_travel = atan2(r_axis0*rt_axis1-r_axis1*rt_axis0, r_axis0*rt_axis0+r_axis1*rt_axis1);
	double millimeters_of_travel;
	
	u16 segments;
	
	double theta_per_segment;
	double linear_per_segment;
	double cos_T; // Small angle approximation
	double sin_T;

	double arc_target[3];
	double sin_Ti;
	double cos_Ti;
	double r_axisi;
	u16 i;
	s8 count = 0;
	if (isclockwise) 
	{ 
		// Correct atan2 output per direction   
		if (angular_travel >= 0)
		{
			angular_travel -= 2*M_PI; 
		}
	} 
	else 
	{
		if (angular_travel <= 0)
		{
			angular_travel += 2*M_PI; 
		}
	}
	
	millimeters_of_travel = hypot(angular_travel*radius, linear_travel);
	
	if (millimeters_of_travel == 0.0)
	{
		return; 
	}
	
	segments = floor(millimeters_of_travel/Settings.mm_per_arc_segment);
	
//	if (invert_feed_rate)
//	{ 
//		feed_rate *= segments; 
//	}
 
	theta_per_segment = angular_travel/segments;
	linear_per_segment = linear_travel/segments;
	cos_T = 1-0.5*theta_per_segment*theta_per_segment; // Small angle approximation
	sin_T = theta_per_segment;

	
	arc_target[2] = 0;//position[axis_linear];
	for (i = 1; i<segments; i++) 
	{
		if (count < Settings.n_arc_correction)
		{
			// Apply vector rotation matrix 
			r_axisi = r_axis0*sin_T + r_axis1*cos_T;
			r_axis0 = r_axis0*cos_T - r_axis1*sin_T;
			r_axis1 = r_axisi;
			count++;
		} 
		else 
		{
			// Arc correction to radius vector. Computed only every n_arc_correction increments.
			// Compute exact location by applying transformation matrix from initial radius vector(=-offset).
			cos_Ti = cos(i*theta_per_segment);
			sin_Ti = sin(i*theta_per_segment);
			r_axis0 = -Itp.i*cos_Ti + Itp.j*sin_Ti;
			r_axis1 = -Itp.i*sin_Ti - Itp.j*cos_Ti;
			count = 0;
		}
		
		// Update arc_target location
		arc_target[X_AXIS] = center_axis0 + r_axis0;
		arc_target[Y_AXIS] = center_axis1 + r_axis1;
		arc_target[2] += linear_per_segment;
		
		Linear_Interpolation(arc_target[X_AXIS], arc_target[Y_AXIS]);
	}
	Linear_Interpolation(arc_target[X_AXIS], arc_target[Y_AXIS]);	
}*/
u8 Circula_SetSNR(u8 type)
{
	Circula.type=type;
	return 0;
}
static void Circula_StepCalculate(double x,double y)
{
	//�������岹���貽��ֵ
	Circula.Xe=(s32)(x*Settings.stepnum)-Current.x;//fabs()���󸡵����ľ���ֵ��abs()���������ľ���ֵ
	Circula.Ye=(s32)(y*Settings.stepnum)-Current.y;
}
static void kuaxiangxian(double x,double y,double i,double j)
{
	//��ʼ�������Բ�ĵ�X���꣬Y����(RXC,RYC)
	s32 RXC=0;
	s32 RYC=0;
	//�յ������Բ�ĵ�X���꣬Y����(RXT,RYT)
	s32 RXT=0; 
	s32 RYT=0;
	Circula_StepCalculate(x,y);//λ��ת��Ϊ����
	RXC=(s32)(i*Settings.stepnum);
	RYC=(s32)(j*Settings.stepnum);
	RXT=(s32)(Circula.Xe+RXC);
	RYT=(s32)(Circula.Ye+RYC);
	
	Circula.Centre.x=(s32)(i*Settings.stepnum)+Current.x;
	Circula.Centre.y=(s32)(j*Settings.stepnum)+Current.y;
}
//Բ���岹 
u8 Circula_Interpolation(double x,double y,double i,double j)
{
	//��ʼ�������Բ�ĵ�X���꣬Y����(RXC,RYC)
	s32 RXC=0;
	s32 RYC=0;
	//�յ������Բ�ĵ�X���꣬Y����(RXT,RYT)
	s32 RXT=0; 
	s32 RYT=0;
	Circula_StepCalculate(x,y);//λ��ת��Ϊ����
	RXC=(s32)(i*Settings.stepnum);
	RYC=(s32)(j*Settings.stepnum);
	RXT=(s32)(Circula.Xe+RXC);
	RYT=(s32)(Circula.Ye+RYC);
	if(((RXC*RXT<0)||(RYC*RYT<0)))
	{
		Linear_Interpolation(x,y);
		return 0;
	}
	Circula.L=abs(Circula.Xe)+abs(Circula.Ye);
	Circula.F=0;//ÿ�ν��в岹ʱ��������
	
	if(Circula.type==CR)
	{
		//�����ʼ������Բ��Ϊԭ�������ϵ��Y����
		if(RXC==0)
		{
			//�����ʼ������Բ��Ϊԭ�������ϵ��ԭ���ϣ�����ʼ����Բ���غϣ�Բ������
			if(RYC==0)
			{
				printf("\r\n�ֳ���\r\n");
			}
			//�����ʼ������Բ��Ϊԭ�������ϵ��Y���������ϣ�RXC=0,RYC>0,����е�һ����˳Բ�岹
			else if(RYC>0)
			{
				while(Circula.L!=0)
				{
					if(Circula.F>=0)
					{
						Yni();													
						Circula.F=Circula.F-2*RYC+1;
						RYC--;
					}
					else
					{
						Xzheng();
						Circula.F=Circula.F+2*RXC+1;
						RXC++;
					}
					Circula.L--;
				}
			}
			//�����ʼ������Բ��Ϊԭ�������ϵ��Y�Ḻ�����ϣ�RXC=0,RYC<0������е�������˳Բ�岹
			else
			{
				while(Circula.L!=0)
				{					
					if(Circula.F>=0)
					{
						Yzheng();
						Circula.F=Circula.F-2*abs(RYC)+1;
						RYC=abs(RYC)-1;																		
					}
					else
					{
						Xni();
						Circula.F=Circula.F+2*abs(RXC)+1;
						RXC=abs(RXC)+1;
					}
					Circula.L--;
				}				
			}		
		}
		//�����ʼ������Բ��Ϊԭ�������ϵ��Y����ұ�(��һ��������)
		else if(RXC>0)
		{
			//�����ʼ������Բ��Ϊԭ�������ϵ��X������������������ϣ�RXC>0��RYC<=0������е�������˳Բ�岹
			if(RYC<=0)
			{
				while(Circula.L!=0)
				{
					if(Circula.F>=0)
					{
						Xni();
						Circula.F=Circula.F-2*abs(RXC)+1;
						RXC=abs(RXC)-1;
					}
					else
					{
						Yni();
						Circula.F=Circula.F+2*abs(RYC)+1;
						RYC=abs(RYC)+1;
					}
					Circula.L--;
				}
			}
			//�����ʼ������Բ��Ϊԭ�������ϵ�ĵ�һ�����ϣ�RXC>0��RYC>0������е�һ����˳Բ�岹
			else
			{
				while(Circula.L!=0)
				{
					if(Circula.F>=0)
					{
						Yni();													
						Circula.F=Circula.F-2*RYC+1;
						RYC--;
					}
					else
					{
						Xzheng();
						Circula.F=Circula.F+2*RXC+1;
						RXC++;
					}
					Circula.L--;
				}
			}
		}
		//�����ʼ������Բ��Ϊԭ�������ϵ�ĵڶ����޻�X��ĸ�������RXC<0��RYC>=0������еڶ�����˳Բ�岹
		else if(RYC>=0)
		{
			while(Circula.L!=0)
			{
				if(Circula.F>=0)
				{
					Xzheng();
					Circula.F=Circula.F-2*abs(RXC)+1;
					RXC=abs(RXC)-1;
				}
				else
				{
					Yzheng();
					Circula.F=Circula.F+2*abs(RYC)+1;
					RYC=abs(RYC)+1;
				}
				Circula.L--;
			}
		}
		//�����ʼ������Բ��Ϊԭ�������ϵ�ĵ�������RXC<0��RYC<0������еڶ�����˳Բ�岹
		else
		{
			while(Circula.L!=0)
			{
				if(Circula.F>=0)
				{
					Yzheng();
					Circula.F=Circula.F-2*abs(RYC)+1;
					RYC=abs(RYC)-1;																		
				}
				else
				{
					Xni();
					Circula.F=Circula.F+2*abs(RXC)+1;
					RXC=abs(RXC)+1;
				}
				Circula.L--;
			}		
		}
	}
	else if(Circula.type==NR)
	{
		//�����ʼ������Բ��Ϊԭ�������ϵ��Y����	
		if(RXC==0)
		{
			//�����ʼ������Բ��Ϊԭ�������ϵ��ԭ���ϣ�����ʼ����Բ���غϣ�Բ������
			if(RYC==0)
			{
				printf("\r\n�ֳ���\r\n");
			}
			//�����ʼ������Բ��Ϊԭ�������ϵ��Y���������ϣ�RXC=0,RYC>0,����еڶ�������Բ�岹
			else if(RYC>0)
			{
				while(Circula.L!=0)
				{
					if(Circula.F>=0)
					{
						Yni();
						Circula.F=Circula.F-2*abs(RYC)+1;
						RYC=abs(RYC)-1;
					}
					else 
					{
						Xni();
						Circula.F=Circula.F+2*abs(RXC)+1;
						RXC=abs(RXC)+1;
					}
					Circula.L--;
				}
			}
			//�����ʼ������Բ��Ϊԭ�������ϵ��Y�Ḻ�����ϣ�RXC=0,RYC<0������е���������Բ�岹
			else 
			{
				while(Circula.L!=0)
				{
					if(Circula.F>=0)
					{
						Yzheng();
						Circula.F=Circula.F-2*abs(RYC)+1;
						RYC=abs(RYC)-1;
					}
					else 
					{
						Xzheng();
						Circula.F=Circula.F+2*abs(RXC)+1;
						RXC=abs(RXC)+1;
					}
					Circula.L--;
				}
			}		
		}
		//�����ʼ������Բ��Ϊԭ�������ϵ��Y����ұ�(��һ��������)
		else if(RXC>0)
		{
			//�����ʼ������Բ��Ϊԭ�������ϵ��X�����������һ�����ϣ�RXC>0��RYC>=0������е�һ������Բ�岹
			if(RYC>=0)
			{
				while(Circula.L!=0)
				{
					if(Circula.F>=0)
					{
						Xni();
						Circula.F=Circula.F-2*RXC+1;
						RXC--;
					}
					else 
					{
						Yzheng();
						Circula.F=Circula.F+2*RYC+1;
						RYC++;
					}
					Circula.L--;
				}
			}
			//�����ʼ������Բ��Ϊԭ�������ϵ�ĵ��������ϣ�RXC>0��RYC<0������е���������Բ�岹
			else
			{
				while(Circula.L!=0)
				{
					if(Circula.F>=0)
					{
						Yzheng();
						Circula.F=Circula.F-2*abs(RYC)+1;
						RYC=abs(RYC)-1;
					}
					else 
					{
						Xzheng();
						Circula.F=Circula.F+2*abs(RXC)+1;
						RXC=abs(RXC)+1;
					}
					Circula.L--;
				}
			}
		}
		//�����ʼ������Բ��Ϊԭ�������ϵ�ĵ������޻�X��ĸ������ϣ�RXC<0��RYC<=0������е���������Բ�岹
		else if(RYC<=0)
		{
					//����
			while(Circula.L!=0)
			{
				if(Circula.F>=0)
				{
					Xzheng();
					Circula.F=Circula.F-2*abs(RXC)+1;
					RXC=abs(RXC)-1;
				}
				else 
				{
					Yni();
					Circula.F=Circula.F+2*abs(RYC)+1;
					RYC=abs(RYC)+1;
				}
				Circula.L--;
			}
		}
		//�����ʼ������Բ��Ϊԭ�������ϵ�ĵڶ�����,RXC<0��RYC>0������еڶ�������Բ�岹
		else
		{
			//�ڶ�
			while(Circula.L!=0)
			{
				if(Circula.F>=0)
				{
					Yni();
					Circula.F=Circula.F-2*abs(RYC)+1;
					RYC=abs(RYC)-1;
				}
				else 
				{
					Xni();
					Circula.F=Circula.F+2*abs(RXC)+1;
					RXC=abs(RXC)+1;
				}
				Circula.L--;
			}
		}
	}
	return 0;//�岹������������ֵ
}
