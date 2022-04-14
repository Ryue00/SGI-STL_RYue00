#pragma once
/**
 * Copyright (c) 2021, INTRE All rights reserved.
 *
 * @Description: IEEE754 数据转换
 * @author: RYue00
 * @date: 2021-10-20
 * @version: v1.1
 */

#include <iostream>
#include<math.h>

//========================================================================================
//                    IEEE754 --> float  将16进制IEEE754数据转换成单精度浮点型
//========================================================================================

/**
 * 将十进制数据整数部分转换为二进制格式
 *
 * @param x
 * @return void
 */

void Decimal_Integer_To_Binary(int x, int* pa, int* pb)
{
	int m, i;
	for (i = 0;; i++)
	{
		*(pa + i) = x % 2;
		m = x / 2;
		x = m;
		if (m == 0)
		{
			// 此时的i比实际的位数个数少1
			*(pa + 32) = i;  
			break;
		}
	}
	for (int j(0); j <= i; j++)
		*(pb + j) = *(pa + i - j);
}

/**
 * 将接收到的四个字节的IEEE754格式数据转换浮点数
 *
 * @param input  16进制数 0x3e 80 00 00  --> 1.000000
 * @return 单精度浮点数
 */
float IEEE754_To_Float(int* input_result)
{
	// 将四个字节16进制数 转成 int型 数组
	//int input_result[4] = { 0 };
	// input_result[0] = (input & 0xff000000) >> 24;
	// input_result[1] = (input & 0x00ff0000) >> 16;
	// input_result[2] = (input & 0x0000ff00) >> 8;
	// input_result[3] = (input & 0x000000ff) >> 0;
	
	int i = 0, j = 0, flag = 0, Exp_Part = 0, exp_part = 0;
	int	ieee754_result[33] = { 0 }, Decimal_Part[33] = { 0 };
	__int64 Integer_Part[33] = { 0 };
	float back_float = 0.0, multi0_5 = 1.0;
	int a[33], b[33], * pa = a, * pb = b;
	for (i = 0; i < 4; i++)
	{
		Decimal_Integer_To_Binary(*(input_result + i),pa,pb);
		flag = a[32];
		for (j = 0; j <= flag; j++)
			// ieee754_result[1]~ieee754_result[32]
			ieee754_result[i * 8 + (8 - flag + j)] = b[j];					
	}
	for (i = 2; i <= 9; i++)
	{
		Exp_Part += ieee754_result[i] * (1 << (9 - i));
	}
	// 取得指数及移位个数
	exp_part = Exp_Part - 127;											
	if (exp_part >= 0)
	{
		//1~32
		Integer_Part[1] = 1;											
		for (i = 0; i < exp_part; i++)
		{
			// ieee754_result[]维数不能超过32，Integer_Part[]就不能超过24
			if (i >= 23)  break;
			// 装入整数部分
			Integer_Part[i + 2] = ieee754_result[i + 10];					
		}
		// 整数部分的个数
		Integer_Part[32] = exp_part + 1;							
		if (exp_part < 23)
		{
			for (i = 0; i < 23 - exp_part; i++)
			{
				// 1~32,装入小数部分
				Decimal_Part[i + 1] = ieee754_result[10 + exp_part + i];	
			}
			// 小数部分的个数
			Decimal_Part[32] = 23 - exp_part;								
		}
	}
	else
	{
		exp_part = -exp_part;
		// 整数部分的个数
		Integer_Part[32] = 0;											
		for (i = 0; i < exp_part; i++)
		{
			// 1~32,装入小数部分.先全部填0，最后在exp_part处填1替换
			Decimal_Part[i + 1] = 0;									
		}
		Decimal_Part[exp_part] = 1;
		for (i = 0; i < 23; i++)
		{
			// 1~32,装入小数部分
			Decimal_Part[exp_part + i + 1] = ieee754_result[10 + i];		
		}
		// 小数部分的个数	
		Decimal_Part[32] = exp_part + 23;									
	}
	// 获得整数部分
	for (i = 1; i <= Integer_Part[32]; i++)								
	{
		// Integer_Part不能超过24
		if (i > 24) break;												
		back_float += Integer_Part[i] * (1 << (Integer_Part[32] - i));
	}
	// 获得小数部分
	for (i = 1; i <= Decimal_Part[32]; i++)								
	{
		if (i > 24) break;
		for (j = 1; j <= i; j++)
		{
			multi0_5 *= 0.5;
		}
		back_float += Decimal_Part[i] * multi0_5;
		multi0_5 = 1.0;
	}

	if (1 == ieee754_result[1])
	{
		back_float = -back_float;
	}
	for (i = 0; i < 33; i++)
	{
		*(pa + i) = 0;
		*(pb + i) = 0;
	}
	return back_float;
}


//========================================================================================
//                     float --> IEEE754 将单精度浮点型数据转换成IEEE754数据
//========================================================================================


// #include<iostream>
#include<math.h>
// using namespace std;
int aa[66], bb[66], * paa = aa, * pbb = bb;

/**
 * 将十进制数据小数部分转换为二进制格式
 */
void Decimal_Decimal_To_Binary(float x)
{
	int i;
	for (i = 0; i < 66; i++)
	{
		*(paa + i) = 0;
		*(pbb + i) = 0;
	}
	double m = x * 2;
	for (i = 0; i <= 22; i++)
	{
		if (m < 1)
		{
			*(pbb + i) = 0;
			m *= 2;
			continue;
		}
		if (m >= 1)
		{
			*(pbb + i) = 1;
			m -= 1;
			if (0.0000001 >= m) break;
			m *= 2;

		}
	}
}
//***********test函数*************//
void test(int ieee754_result[])
{
	IEEE754_To_Float(ieee754_result);
}



