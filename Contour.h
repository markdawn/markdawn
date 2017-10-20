#pragma once
#include "commonFile.h"
#include "GetIsothremData.h"
#include "contourvalues.h"
#include "contourtrace.h"
#include "contourdraw.h"

typedef  CArray<ContourPoint,ContourPoint&>CCurve;//动态存储一条等温线的信息
typedef CTypedPtrList<CPtrList, CCurve*>CCurveList;//动态存储一个温度值有多个等温线
typedef CTypedPtrArray<CPtrArray,CCurveList*>CContourArray;//存储所有等值线

class CContour
{
public:
	CContour(void);
	~CContour(void);
	CGetIsothremData* m_getIsothermData;
private:
	// //存储温度网格数据
//	float** m_pIsothermData;

public:
	void GenerateContourValues(void);
	CContourValues m_ContourValues;
	// 画等温线
	void DrawContour(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam);
	// 产生等值线
	void GenerateContours(void);
	void OnDraw(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam);
protected:
	// //等温线集合
	CContourArray m_ContourCurves;
	// 判断等温线集合是否为空
	BOOL ArrayIsEmpty(void);
	// 清空等温线集合
	void RemoveAllContours(void);
private:
	// 网格数据信息
	GridBaiscInfo m_gridDataInfo;
protected:
	// 追踪等温线类对象
	CContourTrace m_ContourTrace;
public:
	// 从列表中得到指定位置的等温值
//	float GetContourValueFromList(int iOrder);
protected:
	// //画等温线类
	CContourDraw m_ContourDraw;
public:
	// 得到栅格信息
	GridBaiscInfo GetGridBaseInfo(void);
	CContourArray& GetContours(void);
	// 获取栅格数据指针
//	float** GetGridDataPointer(void);
};

