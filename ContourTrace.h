#pragma once
#include "commonFile.h"

typedef CArray<ContourPoint,ContourPoint&> CCurve; //一条等值线曲线的点集的坐标
typedef CTypedPtrList<CPtrList, CCurve*> CCurveList; //等值线曲线链表(给定某值的等值线不限于一条)


typedef struct _TracePoint{   //追踪等值点
	int i; //该等值点所在行号
	int j;//该等值点所在列号
	BOOL  bHorV;//1-横边，0-纵边

	float x; //记录该等值点坐标
	float y;;
}TracePoint;


class CContourTrace
{
public:
	CContourTrace(void);
	~CContourTrace(void);
	void SetCurveList(CCurveList* pCurveList);
	// 追踪等值线
	BOOL ExecuteTracing(float contourValue);
protected:
	// 存储追踪出来的等值线链的坐标
	CCurveList* m_pCurveList;
private:
	// //栅格数据
	float** m_ppGridData;
public:
	// 获得网格信息
	void GetGridBasicInfo(GridBaiscInfo dataInfo);
private:
	// 储存栅格信息
	GridBaiscInfo m_gridDataInfo;
public:
	// 得到栅格数据
	void GetGridData(float** ppGridData);
private:
	// 当前追踪的等值线值
	float m_valueTracing;
public:
	void AllocateMemory(void);
	// 追踪等值点
	void InterpolateTracingValue(void);
	// 追踪开曲线
	void TracingNonClosedContour(void);
	// 追踪闭曲线
	void TracingClosedContour(void);
private:
	// 标识横边是否有当前等值点
	float** xSide;
	// 标识纵边有当前等值点
	float** ySide;
	// 分配内存
	BOOL AllocMemory(float**& pp, UINT row, UINT col);
	// 当前等值线
	CCurve* m_currentCurveLine;

private:
	TracePoint PreviousPoint,CurrentPoint,NextPoint;
public:
	// 计算等值点坐标
	void CalcAndSaveOnePointCoord(int i, int j, BOOL bHorizon, float& x, float& y);
private:
	// 追踪一个开曲线
	void TracingOneNonClosedContour(void);
public:
	// 判断是否是等值点
	BOOL IsContourPoint(float r);
private:
	// 追踪下一个点
	void TracingNextPoint(void);
public:
	// 从下到上追踪
	void FromBottom2TopTracing(void);
	// 从左到右
	void FromLeft2RightTracing(void);
	// 从上到下追踪
	void FromTop2BottomTracing(void);
	// 从右到左
	void FromRight2LeftTracing(void);
	// //当下一个等值点找到后做相应的处理,计算坐标
	void HandlingAfterNextPointFounded(int i, int j, bool bHorizon);
	// 追踪一条等值线
	void TracingOneClosedContour(int startPt_i, int startPt_j);
	void FreeMemory(float**& pData);
};

