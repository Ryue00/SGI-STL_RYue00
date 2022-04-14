#include "ConvertRGB2CIEXY.h"

#include <iostream>
#include <math.h>
#include <stdio.h>
uint16 GammaCorrection(int16 numberRGB)
{
    if (numberRGB > 0.04045)
    {
        return pow(((numberRGB + 0.055) / 1.055), 2.4);
    }
    else
    {
        return (numberRGB / 12.92);
    }
}

bool CreateCrx(double Crx[3][3])
{
    double fRedX = (0.68);
    double fRedY = (0.31);
    double fGreenX = (0.11);
    double fGreenY = (0.82);
    double fBlueX = (0.13);
    double fBlueY = (0.04);
    double fWhiteX = (0.33);
    double fWhiteY = (0.33);
	
    double fRedZ, fGreenZ, fBlueZ;
    double D, U, V, u, v, w, Determinant;
    // double Crx[3][3];
    // double Cxr[3][3];

    /* Calculate missing coordinates */
    fRedZ = (double)(1.0 - fRedX - fRedY);
    fGreenZ = (double)(1.0 - fGreenX - fGreenY);
    fBlueZ = (double)(1.0 - fBlueX - fBlueY);

    /* Do white point correction */
    D = (fRedX - fBlueX) * (fGreenY - fBlueY) - (fRedY - fBlueY) * (fGreenX - fBlueX);
    U = (fWhiteX - fBlueX) * (fGreenY - fBlueY) - (fWhiteY - fBlueY) * (fGreenX - fBlueX);
    V = (fRedX - fBlueX) * (fWhiteY - fBlueY) - (fRedY - fBlueY) * (fWhiteX - fBlueX);
    u = U / D;
    v = V / D;
    w = (double)(1.0 - u - v);

    /* Create the conversion matrix for RGB to XYZ (Crx) */
    Crx[0][0] = u * (fRedX / fWhiteY); Crx[0][1] = v * (fGreenX / fWhiteY); Crx[0][2] = w * (fBlueX / fWhiteY);
    Crx[1][0] = u * (fRedY / fWhiteY); Crx[1][1] = v * (fGreenY / fWhiteY); Crx[1][2] = w * (fBlueY / fWhiteY);
    Crx[2][0] = u * (fRedZ / fWhiteY); Crx[2][1] = v * (fGreenZ / fWhiteY); Crx[2][2] = w * (fBlueZ / fWhiteY);

	return true;
}
bool CreateCxr(float Cxr[3][3])
{

    double Crx[3][3] = { 0 };
    CreateCrx(Crx);
	
    /* Now we need to create the inverse matrix for XYZ to RGB (Cxr). Calculate the determinant  */
    double Determinant = Crx[0][0] * (Crx[1][1] * Crx[2][2] - Crx[2][1] * Crx[1][2])
        - Crx[0][1] * (Crx[1][0] * Crx[2][2] - Crx[1][2] * Crx[2][0])
        + Crx[0][2] * (Crx[1][0] * Crx[2][1] - Crx[1][1] * Crx[2][0]);

    /* Use the determinant to calculate the inverse matrix */
    Cxr[0][0] = (Crx[1][1] * Crx[2][2] - Crx[2][1] * Crx[1][2]) / Determinant;
    Cxr[1][0] = -(Crx[1][0] * Crx[2][2] - Crx[1][2] * Crx[2][0]) / Determinant;
    Cxr[2][0] = (Crx[1][0] * Crx[2][1] - Crx[2][0] * Crx[1][1]) / Determinant;
    Cxr[0][1] = -(Crx[0][1] * Crx[2][2] - Crx[0][2] * Crx[2][1]) / Determinant;
    Cxr[1][1] = (Crx[0][0] * Crx[2][2] - Crx[0][2] * Crx[2][0]) / Determinant;
    Cxr[2][1] = -(Crx[0][0] * Crx[2][1] - Crx[2][0] * Crx[0][1]) / Determinant;
    Cxr[0][2] = (Crx[0][1] * Crx[1][2] - Crx[0][2] * Crx[1][1]) / Determinant;
    Cxr[1][2] = -(Crx[0][0] * Crx[1][2] - Crx[1][0] * Crx[0][2]) / Determinant;
    Cxr[2][2] = (Crx[0][0] * Crx[1][1] - Crx[1][0] * Crx[0][1]) / Determinant;

	return true;
}


bool RGB2XY(const uint16 red, const uint16 green, const uint16 blue, uint16& currentX, uint16& currentY)
{
	
    double Crc[3][3] = { 0 };
    CreateCrx(Crc);
	
    double x = (red * Crc[0][0] + green * Crc[0][1] + blue * Crc[0][2]);
    double y = (red * Crc[1][0] + green * Crc[1][1] + blue * Crc[1][2]);
    double z = (red * Crc[2][0] + green * Crc[2][1] + blue * Crc[2][2]);

    if (IsEqualToZero(x) && IsEqualToZero(y) && IsEqualToZero(z))
    {
        currentX = 0;
        currentY = 0;
        return true;
    }

    if (IsEqualToZero(x + y + z))
    {
        return false;
    }
    double xx = ((x / (x + y + z)) * 65536);
    double yy = ((y / (x + y + z)) * 65536);
    currentX = xx;  // X��ֵ
    currentY = yy;  // Y��ֵ
	
    return true;
}

void XY2RGB(uint16 currentX, uint16 currentY, uint16* r, uint16* g, uint16* b)
{
    float x, y, bigY;
    float X, Y, Z;
    float R, G, B;

    /* Convert xyY to floating point values */
    x = (float)((float)currentX / 65535.0);
    y = (float)((float)currentY / 65535.0);
    bigY = 1.0;

    /* Convert xyY to XYZ colour space */
    float fz = (float)(1.0 - x - y);
    X = (x / y) * bigY;
    Y = bigY;
    Z = (fz / y) * bigY;

    /* Convert XYZ to RGB colour space */
    float Cxr[3][3] = { 0 };
    CreateCxr(Cxr);
	
    XYZ2RGB(Cxr, X, Y, Z, &R, &G, &B);
    *r = (uint16)(255.0 * R);
    *g = (uint16)(255.0 * G);
    *b = (uint16)(255.0 * B);
}

bool XYZ2RGB(float afMatrix[3][3], float fX, float fY, float fZ, float* pfRed, float* pfGreen, float* pfBlue)
{
    float R, G, B, Max;

    R = fX * afMatrix[0][0] + fY * afMatrix[0][1] + fZ * afMatrix[0][2];
    G = fX * afMatrix[1][0] + fY * afMatrix[1][1] + fZ * afMatrix[1][2];
    B = fX * afMatrix[2][0] + fY * afMatrix[2][1] + fZ * afMatrix[2][2];

    Max = MAX3(R, G, B);

    if (Max > 1.000) {
        R = R / Max;
        G = G / Max;
        B = B / Max;
    }

    if (R < 0.0) R = 0;
    if (G < 0.0) G = 0;
    if (B < 0.0) B = 0;

    *pfRed = R;
    *pfGreen = G;
    *pfBlue = B;
	
    return false;
}


int main()
{
    uint16 currentX = 0;
    uint16 currentY = 0;
    RGB2XY(0, 0, 0, currentX, currentY);

    printf("currentX=0x%x, currentY=0x%x", currentX, currentY);
    
    getchar();
	return 0;
}