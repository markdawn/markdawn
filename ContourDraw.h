#pragma once

#include "commonFile.h"
#include "atltypes.h"
#include "lookupcolor.h"

typedef  CArray<ContourPoint,ContourPoint&>CCurve;//动态存储一条等温线的信息
typedef CTypedPtrList<CPtrList, CCurve*>CCurveList;//动态存储一个温度值有多个等温线


class CContourDraw
{
public:
	CContourDraw(void);
	~CContourDraw(void);
	// 画等值线
	void DrawContours(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam,LPVOID lpvoid);
	// 画等温线前的预处理
	void PreProcess(CRect rRect,WPARAM wparam, LPARAM lparam);
protected:
	// 栅格信息
	GridBaiscInfo m_GridInfo;
public:
	// 画等温线背景线框
	void DrawBackground(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam);
protected:
	// 画等温线区域，按实际比例
	CRect m_rect;
public:
	// 画等温线
	void DrawContourCurve(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam,LPVOID lpvoid);
	void DrawContourCurveText(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam, LPVOID lpvoid);
	// 创建等温线对像，以方便画等温线
	void CreateContourObj(WPARAM wparam,LPARAM lparam,LPVOID lpvoid);
	// //储存所有等温线对象，以便画等温线
	CCurveList* m_pCurveList;
	CLookUpColor m_LookUpColor;
private:
	// 栅格数据
	float** m_ppGridData;
public:
	void ReMoveAll(void);
	void drawString(const char* str);
//	CCurve* m_pCurve;
};

