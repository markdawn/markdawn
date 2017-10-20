// SelTTrendTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "SelTTrendTimeDlg.h"
#include "afxdialogex.h"


// CSelTTrendTimeDlg dialog

IMPLEMENT_DYNAMIC(CSelTTrendTimeDlg, CDialog)

CSelTTrendTimeDlg::CSelTTrendTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelTTrendTimeDlg::IDD, pParent)
{

}

CSelTTrendTimeDlg::~CSelTTrendTimeDlg()
{
}

void CSelTTrendTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_SelStartTimeCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_SelEndTimeCtrl);
}


BEGIN_MESSAGE_MAP(CSelTTrendTimeDlg, CDialog)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CSelTTrendTimeDlg::OnDatetimechangeDatetimepicker1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, &CSelTTrendTimeDlg::OnDatetimechangeDatetimepicker2)
END_MESSAGE_MAP()


// CSelTTrendTimeDlg message handlers


BOOL CSelTTrendTimeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here


	CTime tempStart(m_StartTime);
	m_SelStartTimeCtrl.SetTime(&tempStart);
	CTime tempEnd(m_EndTime);
	m_SelEndTimeCtrl.SetTime(&tempEnd);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSelTTrendTimeDlg::OnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	CTime tm(pDTChange->st.wYear,pDTChange->st.wMonth,pDTChange->st.wDay,0,0,0);
	m_StartTime = tm.GetTime();
	*pResult = 0;
}


void CSelTTrendTimeDlg::OnDatetimechangeDatetimepicker2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	CTime tm(pDTChange->st.wYear,pDTChange->st.wMonth,pDTChange->st.wDay,0,0,0);
	m_EndTime = tm.GetTime();
	*pResult = 0;
}
