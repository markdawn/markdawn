#pragma once
#include "afxwin.h"
#include "atltypes.h"


// CSignalTimeBmp

class CSignalTimeBmp : public CStatic
{
	DECLARE_DYNAMIC(CSignalTimeBmp)

public:
	CSignalTimeBmp();
	virtual ~CSignalTimeBmp();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void DisplayTime(WPARAM wparam, LPARAM lparam, UINT iSendArm,UINT iRecArm,BOOL bRec,BOOL bSend);
private:
	void Draw(CDC* pDC, WPARAM wparam, LPARAM lparam, UINT iSendArm,UINT iRecArm,BOOL bRec,BOOL bSend);
	CBitmap m_Bmp;
	void DrawTimeBackgnd(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam,UINT iSendArm,UINT iRecArm);
	void DrawTimeCurve(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam, UINT iArmOrder,UINT iDataModule,BOOL bDis,COLORREF rColor);
public:
	int*** m_pfSignalData;
	CRect m_DisCurveArea;
};


