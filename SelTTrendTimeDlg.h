#pragma once
#include "afxdtctl.h"


// CSelTTrendTimeDlg dialog

class CSelTTrendTimeDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelTTrendTimeDlg)

public:
	CSelTTrendTimeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelTTrendTimeDlg();

// Dialog Data
	enum { IDD = IDD_SELTRENDTIME_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CDateTimeCtrl m_SelStartTimeCtrl;
	CDateTimeCtrl m_SelEndTimeCtrl;
	afx_msg void OnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDatetimechangeDatetimepicker2(NMHDR *pNMHDR, LRESULT *pResult);
public:
	time_t m_StartTime;
	time_t m_EndTime;
};
