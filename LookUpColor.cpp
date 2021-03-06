#include "StdAfx.h"
#include "LookUpColor.h"
#include <gl\gl.h> 
#include <gl\glu.h> 
#include <gl\glaux.h>
#pragma warning(disable:4305)
#pragma warning(disable:4244)
CLookUpColor::CLookUpColor(void)
{

}


CLookUpColor::~CLookUpColor(void)
{

}
#define  NO_FLAME						RGB(11,0,0)
#define  RED_FLAME						RGB(255,0,0)
#define  CHERRY_FLAME				RGB(255,33,21)
//#define  CHERRYLIGHT_FLAME	RGB(255,85,74)
#define  ORANGE_FLAME			RGB(234,85,32)
#define  YELLOW_FLAME			RGB(255,255,0)
#define  LIGHTYELLOW_FlAME  RGB(255,255,153)
#define  WHITE_FLAME                RGB(255,255,209)

// 输入温度，得到相对应的火焰颜色
COLORREF CLookUpColor::GetFlameColor(int tValue)
{
	COLORREF  startColor;
	COLORREF endColor;
	COLORREF colorValue;
	int colorLevel = -1;

	float minT = 0.0f;
	float maxT = 0.0f;
	
	if (tValue>=0 && tValue<475)
	{
		startColor = NO_FLAME;
		endColor = RED_FLAME;
		minT = 0.0;
		maxT = 475.0f;
		colorLevel = 1;
	}
	else if(tValue>=475 && tValue<650)
	{
		startColor = RED_FLAME;
		endColor = CHERRY_FLAME;
		minT = 475.0f;
		maxT = 650.0f;
		colorLevel = 2;
	}
	else if(tValue>=650 && tValue<750)
	{
		startColor = CHERRY_FLAME;
		endColor = CHERRYLIGHT_FLAME;
		minT = 650.0f;
		maxT = 750.0f;
		colorLevel = 3;
	}
	else if(tValue>=750 && tValue<825)
	{
		startColor = CHERRYLIGHT_FLAME;
		endColor = ORANGE_FLAME;
		minT = 750.0f;
		maxT = 825.0f;
		colorLevel = 4;
	}
	else if(tValue>=825 && tValue<900)
	{
		startColor = ORANGE_FLAME;
		endColor = YELLOW_FLAME;
		minT = 825.0f;
		maxT = 900.0f;
		colorLevel = 5;
	}
	else if(tValue>=900 && tValue<1090)
	{
		startColor = YELLOW_FLAME;
		endColor = LIGHTYELLOW_FlAME;
		minT = 900.0f;
		maxT = 1090.0f;
		colorLevel = 6;
	}
	else if(tValue>=1090 && tValue<1320)
	{
		startColor = LIGHTYELLOW_FlAME;
		endColor = WHITE_FLAME;
		minT = 1090.0f;
		maxT = 1320.0f;
		colorLevel = 7;
	}
	else if (tValue>=1320) //white
	{
		return WHITE_FLAME;
	}
	else  //black
	{
		return NO_FLAME;
	}
	colorValue = CalColorValue(tValue,minT,maxT,startColor,endColor,colorLevel);
	return colorValue;
}


// 计算对应的color值
COLORREF CLookUpColor::CalColorValue(int tValue,float minValue, float maxValue, COLORREF minColorValue, COLORREF maxColorValue, int colorLevel)
{
	COLORREF cValue;
	float rCoff = 0.0f;
	float gCoff = 0.0f;
	float bCoff = 0.0f;

	BYTE r = 0;
	BYTE g = 0;
	BYTE b = 0;

	BYTE rMin = GetRValue(minColorValue);
	BYTE gMin = GetGValue(minColorValue);
	BYTE bMin = GetBValue(minColorValue);

	BYTE rMax = GetRValue(maxColorValue);
	BYTE gMax = GetGValue(maxColorValue);
	BYTE bMax = GetBValue(maxColorValue);

	switch(colorLevel)
	{
		case 1:
			rCoff = (float)( rMax-rMin)/(maxValue - minValue);
			r = rMin + (BYTE)((tValue-minValue)*rCoff);
			g = gMin;
			b = bMin;
			break;
		case 2:
			gCoff = (float)(gMax-gMin)/(maxValue - minValue);
			bCoff = (float)(bMax-bMin)/(maxValue - minValue);
			r = rMin;
			g = gMin + (BYTE)((tValue-minValue)*gCoff);
			b = bMin + (BYTE)((tValue-minValue)*bCoff);
			break;
		case 3:
			gCoff = (float)(gMax-gMin)/(maxValue - minValue);
			bCoff = (float)(bMax-bMin)/(maxValue - minValue);
			r = rMin;
			g = gMin + (BYTE)((tValue-minValue)*gCoff);
			b = bMin + (BYTE)((tValue-minValue)*bCoff);
			break;
		case 4:
			rCoff = (float)( rMax-rMin)/(maxValue - minValue);
			bCoff = (float)(bMax-bMin)/(maxValue - minValue);
			r = rMin + (BYTE)((tValue-minValue)*rCoff);
			b = bMin + (BYTE)((tValue-minValue)*bCoff);
			g = gMin;
			break;
		case 5:
			rCoff = (float)( rMax-rMin)/(maxValue - minValue);
			gCoff = (float)(gMax-gMin)/(maxValue - minValue);
			bCoff = (float)(bMax-bMin)/(maxValue - minValue);
			r = rMin + (BYTE)((tValue-minValue)*rCoff);
			g = gMin + (BYTE)((tValue-minValue)*gCoff);
			b = bMin + (BYTE)((tValue-minValue)*bCoff);
			break;
		case 6:
			r = rMin;
			g = gMin;
			bCoff = (float)(bMax-bMin)/(maxValue - minValue);
			b = bMin + (BYTE)((tValue-minValue)*bCoff);
			break;
		case 7:
			r = rMin;
			g = gMin;
			bCoff = (float)(bMax-bMin)/(maxValue - minValue);
			b = bMin + (BYTE)((tValue-minValue)*bCoff);
			break;
		default:
			break;
		
	}
	cValue = RGB(r,g,b);

	return cValue;
}


// 获取颜色
void CLookUpColor::GetColor(int tValue,GLfloat* vdColor)
{
	COLORREF  startColor;
	COLORREF endColor;
	COLORREF  minColorValue;
	COLORREF maxColorValue;

	int colorLevel = -1;

	float minT = 0.0f;
	float maxT = 0.0f;

	if (tValue>=0 && tValue<475)
	{
		startColor = NO_FLAME;
		endColor = RED_FLAME;
		minT = 0.0;
		maxT = 475.0f;
		colorLevel = 1;
	}
	else if(tValue>=475 && tValue<650)
	{
		startColor = RED_FLAME;
		endColor = CHERRY_FLAME;
		minT = 475.0f;
		maxT = 650.0f;
		colorLevel = 2;
	}
	else if(tValue>=650 && tValue<750)
	{
		startColor = CHERRY_FLAME;
		endColor = CHERRYLIGHT_FLAME;
		minT = 650.0f;
		maxT = 750.0f;
		colorLevel = 3;
	}
	else if(tValue>=750 && tValue<825)
	{
		startColor = CHERRYLIGHT_FLAME;
		endColor = ORANGE_FLAME;
		minT = 750.0f;
		maxT = 825.0f;
		colorLevel = 4;
	}
	else if(tValue>=825 && tValue<900)
	{
		startColor = ORANGE_FLAME;
		endColor = YELLOW_FLAME;
		minT = 825.0f;
		maxT = 900.0f;
		colorLevel = 5;
	}
	else if(tValue>=900 && tValue<1090)
	{
		startColor = YELLOW_FLAME;
		endColor = LIGHTYELLOW_FlAME;
		minT = 900.0f;
		maxT = 1090.0f;
		colorLevel = 6;
	}
	else if(tValue>=1090 && tValue<1320)
	{
		startColor = LIGHTYELLOW_FlAME;
		endColor = WHITE_FLAME;
		minT = 1090.0f;
		maxT = 1320.0f;
		colorLevel = 7;
	}
	else if (tValue>=1320) //white
	{
		vdColor[0] = 1.0;
		vdColor[1] = 1.0;
		vdColor[2] = 0.9;
		return;
	}
	else  //black
	{
		vdColor[0] = 0.1;
		vdColor[1] = 0;
		vdColor[2] = 0;
		return;
	}

	float rCoff = 0.0f;
	float gCoff = 0.0f;
	float bCoff = 0.0f;

	BYTE r = 0;
	BYTE g = 0;
	BYTE b = 0;

	minColorValue =  startColor;
	maxColorValue = endColor;

	BYTE rMin = GetRValue(minColorValue);
	BYTE gMin = GetGValue(minColorValue);
	BYTE bMin = GetBValue(minColorValue);

	BYTE rMax = GetRValue(maxColorValue);
	BYTE gMax = GetGValue(maxColorValue);
	BYTE bMax = GetBValue(maxColorValue);

	switch(colorLevel)
	{
	case 1:
		rCoff = (float)( rMax-rMin)/(maxT - minT);
		r = rMin + (BYTE)((tValue-minT)*rCoff);
		g = gMin;
		b = bMin;
		break;
	case 2:
		gCoff = (float)(gMax-gMin)/(maxT - minT);
		bCoff = (float)(bMax-bMin)/(maxT - minT);
		r = rMin;
		g = gMin + (BYTE)((tValue-minT)*gCoff);
		b = bMin + (BYTE)((tValue-minT)*bCoff);
		break;
	case 3:
		gCoff = (float)(gMax-gMin)/(maxT - minT);
		bCoff = (float)(bMax-bMin)/(maxT - minT);
		r = rMin;
		g = gMin + (BYTE)((tValue-minT)*gCoff);
		b = bMin + (BYTE)((tValue-minT)*bCoff);
		break;
	case 4:
		rCoff = (float)( rMax-rMin)/(maxT - minT);
		bCoff = (float)(bMax-bMin)/(maxT - minT);
		r = rMin + (BYTE)((tValue-minT)*rCoff);
		b = bMin + (BYTE)((tValue-minT)*bCoff);
		g = gMin;
		break;
	case 5:
		rCoff = (float)( rMax-rMin)/(maxT - minT);
		gCoff = (float)(gMax-gMin)/(maxT - minT);
		bCoff = (float)(bMax-bMin)/(maxT - minT);
		r = rMin + (BYTE)((tValue-minT)*rCoff);
		g = gMin + (BYTE)((tValue-minT)*gCoff);
		b = bMin + (BYTE)((tValue-minT)*bCoff);
		break;
	case 6:
		r = rMin;
		g = gMin;
		bCoff = (float)(bMax-bMin)/(maxT - minT);
		b = bMin + (BYTE)((tValue-minT)*bCoff);
		break;
	case 7:
		r = rMin;
		g = gMin;
		bCoff = (float)(bMax-bMin)/(maxT - minT);
		b = bMin + (BYTE)((tValue-minT)*bCoff);
		break;
	default:
		break;

	}
	vdColor[0] = (GLfloat)r/255.0;
	vdColor[1] = (GLfloat)g/255.0;
	vdColor[2] = (GLfloat)b/255.0;
}
