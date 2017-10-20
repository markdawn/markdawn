#pragma once
#include "afxwin.h"


// CPositionBmp

class CPositionBmp : public CStatic
{
	DECLARE_DYNAMIC(CPositionBmp)

public:
	CPositionBmp();
	virtual ~CPositionBmp();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void DisplayPosition(WPARAM wparam, LPARAM lparam);
private:
	// 画测点位置
	void Draw(CDC* pDC, WPARAM wparam, LPARAM lparam);
	CBitmap m_Bmp;
	void DrawPictureFrame(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam);
public:
	// 保存测点的位置
	POINT* m_point;
private:
	// 获取传感器坐标
	void GetSensorPoint(POINT* pPoint, CRect rRect);
public:
	// //画sensor示意图：用一个圆表示
	void DrawSensorPoint(CDC* pDC, POINT* pPoint, WPARAM wparam , LPARAM lparam);
private:
	// 画声波传输路径
	void DrawAcousticPath(CDC* pDC, POINT* pPoint);
};


