#pragma once


// CPassWordDlg dialog

class CPassWordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPassWordDlg)

public:
	CPassWordDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPassWordDlg();

// Dialog Data
	enum { IDD = IDD_PASSWORD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strOldPass;
	CString m_strNewPass;
	CString m_strConfirmPass;
};
