#pragma once

#include "AreaAvrgTemperatureBmp.h"

// CAreaAvrgTemperatureView form view

class CAreaAvrgTemperatureView : public CFormView
{
	DECLARE_DYNCREATE(CAreaAvrgTemperatureView)

public:
	CAreaAvrgTemperatureView();           // protected constructor used by dynamic creation
	virtual ~CAreaAvrgTemperatureView();
protected:
public:
	enum { IDD = IDD_AVRGT_DLG };
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
	CAreaAvrgTemperatureBmp m_AreaAvrgTBmp;

protected:
	afx_msg LRESULT OnAreaArvgT(WPARAM wparam,LPARAM lparam);
public:

	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnUpdateReturn(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRealtrend(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTinfo(CCmdUI *pCmdUI);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnUpdateSetting(CCmdUI *pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

