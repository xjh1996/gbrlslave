#include "stringdeal.h"
#include "stepper.h"


u8 gc_execute_line(u8 *line) 
{
	u8 letter;
	double value;
	u8 int_value;
	u8 char_counter = 0; //������־
	while(line[char_counter] != '\0')  //ֱ���ַ�����β
	{
		letter=line[char_counter];
		char_counter++;
		read_float(line, &char_counter, &value);
		int_value=(u8)value;
		switch(letter)
		{
			case 'G':
				switch(int_value)
				{
					case 0: Itp.type = 0; break; // G0 ���ٶ�λ 
					case 1: Itp.type = 1; break; // G1 ֱ�߲岹
					case 2: Itp.type = 2; break; // G2 ˳ʱ��Բ���岹
					case 3: Itp.type = 3; break; // G3 ��ʱ��Բ���岹		
					case 75: Itp.type = 4;Itp.x = 0;Itp.y = 0;break; //�ص�����ԭ��
					case 30: Current.x = 0,Current.y = 0,Linear.Xe = 0,Linear.Ye = 0,Linear.L = 0,Linear.F = 0,Itp.x = 0,Itp.y = 0;break; //����Ϊ�������
				}
				break;
			case 'M':
				Itp.type=10;
				switch(int_value)
				{
					case 3: LASER = 1; break;  //���⿪
					case 5: LASER = 0; break;  //�����
				}
				break;
			case 'X':
				Itp.x = value;
				break;
			case 'Y':
				Itp.y = value;
				break;					
			case 'I':
				Itp.i = value;
				break;
			case 'J':
				Itp.j = value;
				break;					
			case 'F':
				break;					
		}
	}
	return 0;
}

u8 read_float(u8 *line, u8 *char_counter, double *float_ptr)                  
{
	u32 intval = 0;
	s8 exp = 0;
	u8 ndigit = 0;
	u8 c;
	double fval;
	bool isnegative = false; //������־λ
	bool isdecimal = false;  //С����־λ
	
	u8 *ptr = line + *char_counter; 
	//���һ���ַ�������ָ�롣
	c = *ptr++;
  
	// ��׽�������/����
 
	if (c == '-') 
	{
		isnegative = true;
		c = *ptr++;
	} 
	else if (c == '+') 
	{		
		c = *ptr++;
	}  
	// ��ȡ������������С��
	while(1) 
	{
		c -= '0';  //���ַ�ת��Ϊ���� ��ֵ��c
		if (c <= 9) 
		{
			ndigit++;
			if (MAX_INT_DIGITS <= 12) 
			{ 
				//�ж��Ƿ����λ��
				if (isdecimal) 
				{ 
					exp--; 
				}  //�����С��
				intval = (((intval << 2) + intval) << 1) + c; // intval*10 + c  �����ƽ�ʡʱ��
			} 
			else 
			{
				if (!(isdecimal)) { exp++; }  //���û��С��
			}
		}
		else if (c == (('.'-'0') & 0xff)  &&  !(isdecimal)) 
		{
			isdecimal = true;
		} 
		else 
		{
			break;
		}	
		c = *ptr++;
	}  
	// ���û�������� ���ؼ�
	if (!ndigit) 
	{ 
	return(false); 
	}
  
	//  ����ת���ɸ�������

	fval = (double)intval; //ǿ��ת��
  
	//Ӧ��С����Ӧִ�еĲ�������������˷� 
	// E0��E-4��Ԥ�ڷ�Χ��
	if (fval != 0) 
	{
		while (exp <= -2) 
		{
			fval *= 0.01; 
			exp += 2;
		}
		if (exp < 0) 
		{ 
			fval *= 0.1; 
		} 
		else if (exp > 0) 
		{
			do{
				fval *= 10.0;
			} while (--exp > 0);
		} 
	}   //����С�����λ�� 

	//   ָ������ֵ����ȷ�ı�־��
	if (isnegative) 
	{
		*float_ptr = -fval;
	} 
	else 
	{
		*float_ptr = fval;
	}

	*char_counter = ptr - line - 1; //  ����char_counter����һ�����
  
	return(true);
}
