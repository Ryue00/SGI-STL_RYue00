#pragma once
/**
 * Copyright (c) 2021, INTRE All rights reserved.
 *
 * @Description: RGB --> xyz --> XY
 * @author: RYue00
 * @date: 2021-03-08
 * @version: v1.1
 */

#include "TypeDef.h"
#define MAX3(X,Y,Z)     ((Y) >= (Z) ? ((X) >= (Y) ? (X) : (Y)) : ((X) >= (Z) ? (X) : (Z)))

/**
 * 伽马校准
 */
uint16 GammaCorrection(int16 numberRGB);

/**
 * RGB 转换成 XY
 */

/**
 * 获取RGB
 */
void XY2RGB(uint16 currentX, uint16 currentY, uint16* r, uint16* g, uint16* b);

/**
 * 创建 RGB转xyY的矩阵
 */
bool CreateCrx(double Crx[3][3]);

/**
 * 创建 xyY转RGB的矩阵
 */
bool CreateCxr(float Cxr[3][3]);

/**
 * RGB 转换成 XY
 */
bool RGB2XY(const uint16 red, const uint16 green, const uint16 blue, uint16& currentX, uint16& currentY);

/**
 * 判断值是否等于 0
 */
bool IsEqualToZero(const double value)
{
	const double EPSINON = 0.000000000001;

	if ((value >= -EPSINON) && (value <= EPSINON))
	{
		return true;
	}
	return false;
}

/**
 * XYZ 转换成 RGB
 */
bool XYZ2RGB(
		    float  afMatrix[3][3],
		    float  fX,
		    float  fY,
		    float  fZ,
		    float* pfRed,
		    float* pfGreen,
		    float* pfBlue);