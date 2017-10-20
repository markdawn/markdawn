#pragma once
#include "afxwin.h"


// CLeakBmp

class CLeakBmp : public CStatic
{
	DECLARE_DYNAMIC(CLeakBmp)

public:
	CLeakBmp();
	virtual ~CLeakBmp();

protected:
	DECLARE_MESSAGE_MAP()

public:
	void DisplayLeak(WPARAM wparam, LPARAM lparam);
private:
	void Draw(CDC* pDC, WPARAM wparam, LPARAM lparam);
	CBitmap m_Bmp;
	void DrawCurveFrame(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam);
};


