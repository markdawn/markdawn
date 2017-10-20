#pragma once
#include "afxwin.h"
#include "contour.h"
//#include "e:\xn_ebcwork\声波测温\code\acoustictherm\isothermfile\contourocx.h"


// CIsothermBmp

class CIsothermBmp : public CStatic
{
	DECLARE_DYNAMIC(CIsothermBmp)

public:
	CIsothermBmp();
	virtual ~CIsothermBmp();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void DisplayIsotherm(WPARAM wparam, LPARAM lparam,CClientDC* pDC,CRect rRect);
private:
	void Draw(CDC* pDC, WPARAM wparam, LPARAM lparam,CRect rRect);
public:
	CBitmap m_Bmp;
	
//	CContourOCX m_ContourOCX;// 等温线
public:
	void DrawCurve(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam);
private:
//	void DrawContourLines(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam);
	virtual void DoDataExchange(CDataExchange* pDX);
	CContour m_contour;
public:
	BOOL SetDCPixelFormat(CDC*pDC);
	BOOL InitOpenGL(CWnd *pWnd);
	// 对应OpenGL渲染环境
	HGLRC m_hRC;

	// 记录当前View的DC
	CClientDC* m_pDC;
//	afx_msg void OnSize(UINT nType, int cx, int cy);
};


