#pragma once


// CExitDlg dialog

class CExitDlg : public CDialog
{
	DECLARE_DYNAMIC(CExitDlg)

public:
	CExitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CExitDlg();

// Dialog Data
	enum { IDD = IDD_EXIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSetpassBtn();
	CString m_strPassword;//当前输入的退出密码
	CString m_strSysExitPass;//系统退出密码，保存在数据库
public:
	BOOL RecordPassword(MYSQL sqlHandle,CString strNewPass,CString strOldPasss);
};
