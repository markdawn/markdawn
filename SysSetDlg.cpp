// SysSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "SysSetDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CSysSetDlg dialog

IMPLEMENT_DYNAMIC(CSysSetDlg, CDialog)

CSysSetDlg::CSysSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSysSetDlg::IDD, pParent)
	, m_SetIpAddrDlg(NULL)
{
	//m_SetIpAddrDlg = new CSetIpAddrDlg;
}

CSysSetDlg::~CSysSetDlg()
{
//	if (m_SetIpAddrDlg)
//	{
//		delete m_SetIpAddrDlg;
//		m_SetIpAddrDlg = NULL;
//	}
}

void CSysSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SYSSET_TAB, m_sysSetTap);
}


BEGIN_MESSAGE_MAP(CSysSetDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_APPLY_BTN, &CSysSetDlg::OnBnClickedApplyBtn)
	ON_BN_CLICKED(IDOK, &CSysSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSysSetDlg message handlers


BOOL CSysSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_SetIpAddrDlg= new CSetIpAddrDlg;
	m_SetIpAddrDlg->Create(CSetIpAddrDlg::IDD,&m_sysSetTap);
	m_sysSetTap.AddItem(m_SetIpAddrDlg,_T("IP设置"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSysSetDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	if (m_SetIpAddrDlg!=NULL)
	{
		m_SetIpAddrDlg->DestroyWindow();
		delete m_SetIpAddrDlg;
	}
}


void CSysSetDlg::OnBnClickedApplyBtn()
{
	// TODO: Add your control notification handler code here
	//应用功能：设置变量，不写入数据库
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	if (!pWnd)
	{
		return;
	}
	UpdateData(TRUE);
	int iSelCur = m_sysSetTap.GetCurSel();
	if (iSelCur == IPSET)
	{
		DWORD m_dwIPAddr;
		u_short iPort = m_SetIpAddrDlg->m_iPort1;
		m_SetIpAddrDlg->m_ipAddress1.GetAddress(m_dwIPAddr);
		pWnd->SetIPAddressAndPort(m_dwIPAddr,iPort,0);

		iPort = m_SetIpAddrDlg->m_iPort2;
		m_SetIpAddrDlg->m_ipAddress2.GetAddress(m_dwIPAddr);
		pWnd->SetIPAddressAndPort(m_dwIPAddr,iPort,1);

		iPort = m_SetIpAddrDlg->m_iPort3;
		m_SetIpAddrDlg->m_ipAddress3.GetAddress(m_dwIPAddr);
		pWnd->SetIPAddressAndPort(m_dwIPAddr,iPort,2);

		iPort = m_SetIpAddrDlg->m_iPort4;
		m_SetIpAddrDlg->m_ipAddress4.GetAddress(m_dwIPAddr);
		pWnd->SetIPAddressAndPort(m_dwIPAddr,iPort,3);

		/*iPort = m_SetIpAddrDlg->m_iPort5;
		m_SetIpAddrDlg->m_ipAddress5.GetAddress(m_dwIPAddr);
		pWnd->SetIPAddressAndPort(m_dwIPAddr,iPort,4);

		iPort = m_SetIpAddrDlg->m_iPort6;
		m_SetIpAddrDlg->m_ipAddress6.GetAddress(m_dwIPAddr);
		pWnd->SetIPAddressAndPort(m_dwIPAddr,iPort,5);

		iPort = m_SetIpAddrDlg->m_iPort7;
		m_SetIpAddrDlg->m_ipAddress7.GetAddress(m_dwIPAddr);
		pWnd->SetIPAddressAndPort(m_dwIPAddr,iPort,6);

		iPort = m_SetIpAddrDlg->m_iPort8;
		m_SetIpAddrDlg->m_ipAddress8.GetAddress(m_dwIPAddr);
		pWnd->SetIPAddressAndPort(m_dwIPAddr,iPort,7);*/
	}

}


void CSysSetDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//写入数据库
	CDialog::OnOK();
}
