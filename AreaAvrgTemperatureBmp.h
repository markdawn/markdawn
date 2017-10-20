#pragma once
#include "afxwin.h"
#include "commonFile.h"

// CAreaAvrgTemperatureBmp

typedef struct  _AreaPoint{
	POINT startPoint;
	POINT endPoint;
}AreaPoint;


class CAreaAvrgTemperatureBmp : public CStatic
{
	DECLARE_DYNAMIC(CAreaAvrgTemperatureBmp)

public:
	CAreaAvrgTemperatureBmp();
	virtual ~CAreaAvrgTemperatureBmp();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void DisplayAreaAvrgT(WPARAM wparam, LPARAM lparam);
	void Draw(CDC* pDc, WPARAM wparam, LPARAM lparam);
private:
	CBitmap m_Bmp;
public:
	afx_msg void OnPaint();
	void DrawAreaAvrgT(CDC* pDc, CRect rRect);
	void DrawFrame(CDC* pDc, CRect rRect);
	void DrawOneAreaT(CDC* pDc, AreaPoint point, COLORREF color);
private:
	AreaPoint *m_AreaPoint;
	COLORREF *m_AreaColor;
	// ����ÿ�������ƽ���¶�
	UINT* m_iPerAreaT;
public:
	void PreDrawAreaAvrgT(CRect rRect, WPARAM wparam, LPARAM lparam);
	// ���ÿ����������
	void GetAreaCoor(AreaPoint* areaPoint,CRect rRect);
	// ��ȡ�����Ӧ���¶���ɫ
	void GetAreaColor(COLORREF* color);
	void DrawAreaTText(CDC* pDc,UINT* pBuffer);
	BOOL  bAreaTOverLimitFlag[AREA_BLOCK_NUM];
	UINT m_uFlashTimerCounter[AREA_BLOCK_NUM];
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

