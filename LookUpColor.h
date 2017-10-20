#pragma once
#include <gl\gl.h> 
#include <gl\glu.h> 
#include <gl\glaux.h>

/**************************************************
火焰颜色相对应的温度最低可见红色,RGB(11,0,0)RGB(255,0,0)
温度是475℃最低可见红色到深红色:RGB(255,0,0) RGB(196,0,0)
温度是475～650℃深红色到樱桃红色,RGB(196,0,0) RGB(255,33,21)??
温度是650～750℃樱桃红色到发亮樱桃红色,RGB(255,33,21) RGB(255,84,74)??
温度是750～825℃发亮樱桃红色到橙色,RGB(255,84,74) RGB(234,85,32)??
温度是825～900℃橙色到黄色,RGB(234,85,32) RGB(255,255,0)
温度是900～1090℃黄色到浅黄色,RGB(255,255,0) RGB(255,255,153)
温度是1090～1320℃浅黄色到白色,RGB(255,255,153) RGB(255,255,209)
***************************************************************/
#define  NO_FLAME						RGB(11,0,0)
#define  RED_FLAME						RGB(255,0,0)
#define  CHERRY_FLAME				RGB(255,33,21)
#define  CHERRYLIGHT_FLAME	RGB(255,84,74)
#define  ORANGE_FLAME			RGB(234,85,32)
#define  YELLOW_FLAME			RGB(255,255,0)
#define  LIGHTYELLOW_FlAME  RGB(255,255,153)
#define  WHITE_FLAME                RGB(255,255,209)


typedef struct _ColorNode{
	float pos;
	COLORREF color;
}ColorNode;



class CLookUpColor
{
public:
	CLookUpColor(void);
	~CLookUpColor(void);
	// 输入温度，得到相对应的火焰颜色
	COLORREF GetFlameColor(int tValue);
	// 计算对应的color值
	COLORREF CalColorValue(int tValue,float minValue, float maxValue, COLORREF minColorValue, COLORREF maxColorValue,int colorLevel);
	// 获取颜色
	void GetColor(int tValue,GLfloat* vdColor);
};

