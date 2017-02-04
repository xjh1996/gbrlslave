#include "stringdeal.h"
#include "stepper.h"


u8 gc_execute_line(u8 *line) 
{
	u8 letter;
	double value;
	u8 int_value;
	u8 char_counter = 0; //计数标志
	while(line[char_counter] != '\0')  //直到字符串结尾
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
					case 0: Itp.type = 0; break; // G0 快速定位 
					case 1: Itp.type = 1; break; // G1 直线插补
					case 2: Itp.type = 2; break; // G2 顺时针圆弧插补
					case 3: Itp.type = 3; break; // G3 逆时针圆弧插补		
					case 75: Itp.type = 4;Itp.x = 0;Itp.y = 0;break; //回到坐标原点
					case 30: Current.x = 0,Current.y = 0,Linear.Xe = 0,Linear.Ye = 0,Linear.L = 0,Linear.F = 0,Itp.x = 0,Itp.y = 0;break; //设置为坐标零点
				}
				break;
			case 'M':
				Itp.type=10;
				switch(int_value)
				{
					case 3: LASER = 1; break;  //激光开
					case 5: LASER = 0; break;  //激光关
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
	bool isnegative = false; //正负标志位
	bool isdecimal = false;  //小数标志位
	
	u8 *ptr = line + *char_counter; 
	//设第一个字符和增量指针。
	c = *ptr++;
  
	// 捕捉最初的正/负号
 
	if (c == '-') 
	{
		isnegative = true;
		c = *ptr++;
	} 
	else if (c == '+') 
	{		
		c = *ptr++;
	}  
	// 提取整数，并计算小数
	while(1) 
	{
		c -= '0';  //将字符转化为数字 赋值给c
		if (c <= 9) 
		{
			ndigit++;
			if (MAX_INT_DIGITS <= 12) 
			{ 
				//判断是否到最大位数
				if (isdecimal) 
				{ 
					exp--; 
				}  //如果是小数
				intval = (((intval << 2) + intval) << 1) + c; // intval*10 + c  用左移节省时间
			} 
			else 
			{
				if (!(isdecimal)) { exp++; }  //如果没有小树
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
	// 如果没有数字了 返回假
	if (!ndigit) 
	{ 
	return(false); 
	}
  
	//  整数转换成浮点数。

	fval = (double)intval; //强制转换
  
	//应用小数，应执行的不超过两个浮点乘法 
	// E0至E-4的预期范围。
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
	}   //计算小数点的位置 

	//   指定浮点值与正确的标志。
	if (isnegative) 
	{
		*float_ptr = -fval;
	} 
	else 
	{
		*float_ptr = fval;
	}

	*char_counter = ptr - line - 1; //  设置char_counter到下一条语句
  
	return(true);
}
