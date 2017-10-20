#pragma once


// CSetIpAddrDlg dialog
//…Ë÷√IPµÿ÷∑
class CSetIpAddrDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetIpAddrDlg)

public:
	CSetIpAddrDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetIpAddrDlg();

// Dialog Data
	enum { IDD = IDD_SETIPADRR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	UINT m_iPort1;
	UINT m_iPort2;
	UINT m_iPort3;
	UINT m_iPort4;
	UINT m_iPort5;
	UINT m_iPort6;
	UINT m_iPort7;
	UINT m_iPort8;
	CIPAddressCtrl m_ipAddress1;
	CIPAddressCtrl m_ipAddress2;
	CIPAddressCtrl m_ipAddress3;
	CIPAddressCtrl m_ipAddress4;
	CIPAddressCtrl m_ipAddress5;
	CIPAddressCtrl m_ipAddress6;
	CIPAddressCtrl m_ipAddress7;
	CIPAddressCtrl m_ipAddress8;
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditPort1();
	afx_msg void OnChangeEditPort2();
	afx_msg void OnChangeEditPort3();
	afx_msg void OnChangeEditPort4();
	afx_msg void OnChangeEditPort5();
	afx_msg void OnChangeEditPort6();
	afx_msg void OnChangeEditPort7();
	afx_msg void OnChangeEditPort8();
};
