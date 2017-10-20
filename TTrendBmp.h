#pragma once
#include "afxwin.h"
#include "commonFile.h"

// CTTrendBmp

class CTTrendBmp : public CStatic
{
	DECLARE_DYNAMIC(CTTrendBmp)

public:
	CTTrendBmp();
	virtual ~CTTrendBmp();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void DisplayTTrend(WPARAM wparam, LPARAM lparam, UINT iCh1, UINT iCh2, UINT iCh3, UINT iCh4, UINT iCh5, UINT iCh6, UINT iCh7, UINT iCh8, UINT iCh9, UINT iCh10,  UINT iCh11, UINT iCh12);
	void Draw(CDC* pDC, WPARAM wparam, LPARAM lparam, UINT iCh1, UINT iCh2, UINT iCh3, UINT iCh4, UINT iCh5, UINT iCh6, UINT iCh7, UINT iCh8, UINT iCh9, UINT iCh10, UINT iCh11, UINT iCh12);
private:
	CBitmap m_Bmp;
	// Ϊ���¶�����׼������
	void PrePrepareForDraw(UINT iChanel, UINT* pBuffer, WPARAM wparam, LPARAM lparam);
public:
	void DrawTTrendCurve(CDC* pDC, CRect rRect, UINT iChanel, WPARAM wparam, LPARAM lparam, COLORREF color);
	// �����߱�����
	void DrawCurveFrame(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam);
};


