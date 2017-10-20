#pragma once

#include "IsothermBmp.h"
#include "afxwin.h"
#include <GL/GLAux.h>

// CIsothermView form view

class CIsothermView : public CFormView
{
	DECLARE_DYNCREATE(CIsothermView)

public:
	CIsothermView();           // protected constructor used by dynamic creation
	virtual ~CIsothermView();
protected:
public:
	enum { IDD = IDD_ISOTHERM_DLG };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	CIsothermBmp m_Isotherm;
protected:
	afx_msg LRESULT OnIsotherm(WPARAM wparam,LPARAM  lparam);
public:
//	CContourOCX m_ContourOCX;
//	void DrawContourLines(void);
private:
//	CBitmap m_Bmp;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	// OpenGL»­Í¼
	void DrawScene(WPARAM wparam,LPARAM  lparam);
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnUpdateReturn(CCmdUI *pCmdUI);
	afx_msg void OnReturn();
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


