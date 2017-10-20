#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CSelDateTimeDlg dialog

class CSelDateTimeDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelDateTimeDlg)

public:
	CSelDateTimeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelDateTimeDlg();

// Dialog Data
	enum { IDD = IDD_DATETIME_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
//	CComboBox m_selHourCob;
	CMonthCalCtrl m_MonthCalendar;
	CComboBox m_selEndHourCob;
	CComboBox m_selStartHourCob;
	afx_msg void OnSelchangeStarthourCmb();
	afx_msg void OnSelchangeEndhourCmb();
public:
	UINT m_iSaveSelStartHour;
	UINT m_iSaveSelEndHour;
	afx_msg void OnSelectMonthcalendar(NMHDR *pNMHDR, LRESULT *pResult);
	time_t m_tRecordSelDate;
};
