// ExitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "ExitDlg.h"
#include "afxdialogex.h"
#include "PassWordDlg.h"
#include "MainFrm.h"

// CExitDlg dialog

IMPLEMENT_DYNAMIC(CExitDlg, CDialog)

CExitDlg::CExitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExitDlg::IDD, pParent)
	, m_strPassword(_T(""))
{

}

CExitDlg::~CExitDlg()
{
}

void CExitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_PASSWORD, m_strPassword);
}


BEGIN_MESSAGE_MAP(CExitDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CExitDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SETPASS_BTN, &CExitDlg::OnBnClickedSetpassBtn)
END_MESSAGE_MAP()


// CExitDlg message handlers


void CExitDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void CExitDlg::OnBnClickedSetpassBtn()
{
	// TODO: Add your control notification handler code here
	CPassWordDlg dlg;
loop1:
	UpdateData(TRUE);
	CMainFrame *pWnd = (CMainFrame*)AfxGetMainWnd();
	if (!pWnd)
	{
		return;
	}

	if (dlg.DoModal()==IDOK)
	{
		if(dlg.m_strOldPass==pWnd->GetSysExitPassword())
		{
			if(dlg.m_strNewPass==dlg.m_strConfirmPass)
			{
				m_strPassword=dlg.m_strNewPass;
				if (RecordPassword(pWnd->GetMySQLHandle(),m_strPassword,pWnd->GetSysExitPassword()))
				{
					AfxMessageBox(_T("Succeed!"),MB_ICONINFORMATION);
					pWnd->SetSysExitPassword(m_strPassword);
				}
				else
					AfxMessageBox(_T("保存密码失败!"),MB_ICONINFORMATION);
			}
			else
			{
				AfxMessageBox(_T("设置密码失败!"),MB_ICONSTOP);
				goto loop1;
			}
		}
		else
		{
			AfxMessageBox(_T("密码输入错误"),MB_ICONSTOP);
			goto loop1;
		}
	}

}

BOOL CExitDlg::RecordPassword(MYSQL sqlHandle,CString strNewPass,CString strOldPasss)
{

	CString strCmd;
	strCmd.Format(_T("UPDATE Password SET ExitPass='%s' WHERE ExitPass='%s'"),strNewPass,strOldPasss);


	UINT ilen = strCmd.GetLength();
	strCmd.ReleaseBuffer();
	if(mysql_real_query(&sqlHandle,(char*)(LPCTSTR)strCmd,ilen)!=0) //执行查询
	{ 
		AfxMessageBox(_T("数据库中表格出错")); 
		return FALSE;
	}
	return TRUE;
}