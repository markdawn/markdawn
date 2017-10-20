#pragma once

#include "PostionBmp.h"


// CPositionView form view

class CPositionView : public CFormView
{
	DECLARE_DYNCREATE(CPositionView)

public:
	CPositionView();           // protected constructor used by dynamic creation
	virtual ~CPositionView();
protected:
public:
	enum { IDD = IDD_POSITION_DLG };
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
	CPositionBmp m_PositionBmp;
	virtual void OnInitialUpdate();

protected:
	afx_msg LRESULT OnPosition(WPARAM wparam, LPARAM lparam);
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	CBrush m_brush;
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnUpdateReturn(CCmdUI *pCmdUI);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


