#pragma once

#include "SysSetTap.h"
#include "SetIpAddrDlg.h"
// CSysSetDlg dialog

enum _SYSSET_TAB{IPSET,PARA_SET,DATASTORAGE_SET,DCS_SET,PASSWORD_SET};


class CSysSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CSysSetDlg)

public:
	CSysSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSysSetDlg();

// Dialog Data
	enum { IDD = IDD_SYSSET_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CSetIpAddrDlg*  m_SetIpAddrDlg;
	// 	
//	CSysSetTap m_sysSetTap;
	CSysSetTap m_sysSetTap;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedApplyBtn();
	afx_msg void OnBnClickedOk();
};
