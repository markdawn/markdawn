#pragma once

#include "GridCtrlFile\GridCtrl.h"
#include "seldatetimedlg.h"


// CDisAreaAvrgTInfoView form view

class CDisAreaAvrgTInfoView : public CFormView
{
	DECLARE_DYNCREATE(CDisAreaAvrgTInfoView)

public:
	CDisAreaAvrgTInfoView();           // protected constructor used by dynamic creation
	virtual ~CDisAreaAvrgTInfoView();
protected:
public:
	enum { IDD = IDD_TINFO_DLG };
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
	CGridCtrl * m_pGridCtrl;
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//  计算区域平均温度统计信息
	void CalPerAreaTinfo(MYSQL mysql,time_t tStartTime, time_t tEndTime, UINT iArea);
	void CalAllAreaTinfo(time_t tStartTime, time_t tEndTime, UINT iAreaNum);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedQueryBtn();
	
	afx_msg void OnBnClickedSettimeBtn();
	time_t m_tStartSearchTime;
	time_t m_tEndSearchTime;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	CBrush m_brush;
	void InitGrid(void);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnUpdateReturn(CCmdUI *pCmdUI);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


