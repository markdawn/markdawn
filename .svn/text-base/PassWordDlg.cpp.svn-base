// PassWordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "PassWordDlg.h"
#include "afxdialogex.h"


// CPassWordDlg dialog

IMPLEMENT_DYNAMIC(CPassWordDlg, CDialogEx)

CPassWordDlg::CPassWordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPassWordDlg::IDD, pParent)
	, m_strOldPass(_T(""))
	, m_strNewPass(_T(""))
	, m_strConfirmPass(_T(""))
{

}

CPassWordDlg::~CPassWordDlg()
{
}

void CPassWordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_OLDPASS_EDIT, m_strOldPass);
	DDX_Text(pDX, IDC_NEWPASS_EDIT, m_strNewPass);
	DDX_Text(pDX, IDC_SURENEWPASS_EDIT, m_strConfirmPass);
}


BEGIN_MESSAGE_MAP(CPassWordDlg, CDialogEx)
END_MESSAGE_MAP()


// CPassWordDlg message handlers
