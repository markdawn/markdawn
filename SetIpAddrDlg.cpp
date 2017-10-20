// SetIpAddrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "SetIpAddrDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CSetIpAddrDlg dialog

IMPLEMENT_DYNAMIC(CSetIpAddrDlg, CDialog)

CSetIpAddrDlg::CSetIpAddrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetIpAddrDlg::IDD, pParent)
{

	m_iPort1 = 6000;
	m_iPort2 = 6000;
	m_iPort3 = 6000;
	m_iPort4 = 6000;
	m_iPort5 = 6000;
	m_iPort6 = 6000;
	m_iPort7 = 6000;
	m_iPort8 = 6000;
}

CSetIpAddrDlg::~CSetIpAddrDlg()
{
}

void CSetIpAddrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT1, m_iPort1);
	DDX_Text(pDX, IDC_EDIT_PORT2, m_iPort2);
	DDX_Text(pDX, IDC_EDIT_PORT3, m_iPort3);
	DDX_Text(pDX, IDC_EDIT_PORT4, m_iPort4);
	DDX_Text(pDX, IDC_EDIT_PORT5, m_iPort5);
	DDX_Text(pDX, IDC_EDIT_PORT6, m_iPort6);
	DDX_Text(pDX, IDC_EDIT_PORT7, m_iPort7);
	DDX_Text(pDX, IDC_EDIT_PORT8, m_iPort8);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ipAddress1);
	DDX_Control(pDX, IDC_IPADDRESS2, m_ipAddress2);
	DDX_Control(pDX, IDC_IPADDRESS3, m_ipAddress3);
	DDX_Control(pDX, IDC_IPADDRESS4, m_ipAddress4);
	DDX_Control(pDX, IDC_IPADDRESS5, m_ipAddress5);
	DDX_Control(pDX, IDC_IPADDRESS6, m_ipAddress6);
	DDX_Control(pDX, IDC_IPADDRESS7, m_ipAddress7);
	DDX_Control(pDX, IDC_IPADDRESS8, m_ipAddress8);
}


BEGIN_MESSAGE_MAP(CSetIpAddrDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT_PORT1, &CSetIpAddrDlg::OnChangeEditPort1)
	ON_EN_CHANGE(IDC_EDIT_PORT2, &CSetIpAddrDlg::OnChangeEditPort2)
	ON_EN_CHANGE(IDC_EDIT_PORT3, &CSetIpAddrDlg::OnChangeEditPort3)
	ON_EN_CHANGE(IDC_EDIT_PORT4, &CSetIpAddrDlg::OnChangeEditPort4)
	//ON_EN_CHANGE(IDC_EDIT_PORT5, &CSetIpAddrDlg::OnChangeEditPort5)
	//ON_EN_CHANGE(IDC_EDIT_PORT6, &CSetIpAddrDlg::OnChangeEditPort6)
	//ON_EN_CHANGE(IDC_EDIT_PORT7, &CSetIpAddrDlg::OnChangeEditPort7)
	//ON_EN_CHANGE(IDC_EDIT_PORT8, &CSetIpAddrDlg::OnChangeEditPort8)
END_MESSAGE_MAP()


// CSetIpAddrDlg message handlers


BOOL CSetIpAddrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	if (!pWnd)
	{
		return FALSE;
	}
	m_iPort1 = pWnd->GetPortNum(0);
	m_iPort2 = pWnd->GetPortNum(1);
	m_iPort3 = pWnd->GetPortNum(2);
    m_iPort4 = pWnd->GetPortNum(3);
	//m_iPort5 = pWnd->GetPortNum(4);
	//m_iPort6 = pWnd->GetPortNum(5);
	//m_iPort7 = pWnd->GetPortNum(6);
	//m_iPort8 = pWnd->GetPortNum(7);

	m_ipAddress1.SetAddress(pWnd->GetIPAddress(0));
	m_ipAddress2.SetAddress(pWnd->GetIPAddress(1));
	m_ipAddress3.SetAddress(pWnd->GetIPAddress(2));
	m_ipAddress4.SetAddress(pWnd->GetIPAddress(3));
	//m_ipAddress5.SetAddress(pWnd->GetIPAddress(4));
	//m_ipAddress6.SetAddress(pWnd->GetIPAddress(5));
	//m_ipAddress7.SetAddress(pWnd->GetIPAddress(6));
	//m_ipAddress8.SetAddress(pWnd->GetIPAddress(7));
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSetIpAddrDlg::OnChangeEditPort1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
}


void CSetIpAddrDlg::OnChangeEditPort2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
}


void CSetIpAddrDlg::OnChangeEditPort3()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
}


void CSetIpAddrDlg::OnChangeEditPort4()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
}

/*
void CSetIpAddrDlg::OnChangeEditPort5()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
}


void CSetIpAddrDlg::OnChangeEditPort6()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
}


void CSetIpAddrDlg::OnChangeEditPort7()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
}


void CSetIpAddrDlg::OnChangeEditPort8()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
}
*/
