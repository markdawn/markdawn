// SelDateTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "SelDateTimeDlg.h"
#include "afxdialogex.h"


// CSelDateTimeDlg dialog

IMPLEMENT_DYNAMIC(CSelDateTimeDlg, CDialog)

CSelDateTimeDlg::CSelDateTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelDateTimeDlg::IDD, pParent)
{
	m_iSaveSelStartHour = 0;
	m_iSaveSelEndHour = 23;
}

CSelDateTimeDlg::~CSelDateTimeDlg()
{
}

void CSelDateTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MONTHCALENDAR, m_MonthCalendar);
	DDX_Control(pDX, IDC_ENDHOUR_CMB, m_selEndHourCob);
	DDX_Control(pDX, IDC_STARTHOUR_CMB, m_selStartHourCob);
}


BEGIN_MESSAGE_MAP(CSelDateTimeDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_STARTHOUR_CMB, &CSelDateTimeDlg::OnSelchangeStarthourCmb)
	ON_CBN_SELCHANGE(IDC_ENDHOUR_CMB, &CSelDateTimeDlg::OnSelchangeEndhourCmb)
	ON_NOTIFY(MCN_SELECT, IDC_MONTHCALENDAR, &CSelDateTimeDlg::OnSelectMonthcalendar)
END_MESSAGE_MAP()


// CSelDateTimeDlg message handlers


BOOL CSelDateTimeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here


	// EXCEPTION: OCX Property Pages should return FALSE
	//��ʼ��Сʱ
	CString strHour;	 
	for (int i = 0;i<24;i++)
	{
		strHour.Format(_T("%02d"),i);
		m_selStartHourCob.AddString(strHour);
	}
	m_selStartHourCob.SetCurSel(0);

	for (int i = 0;i<24;i++)
	{
		strHour.Format(_T("%02d"),i);
		m_selEndHourCob.AddString(strHour);
	}
	m_selEndHourCob.SetCurSel(23);
	CTime tm = CTime::GetCurrentTime();
	CTime temp(tm.GetYear(),tm.GetMonth(),tm.GetDay(),0,0,0);
	m_tRecordSelDate = temp.GetTime();
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CSelDateTimeDlg::OnSelchangeStarthourCmb()
{
	// TODO: Add your control notification handler code here
	int iSel = m_selStartHourCob.GetCurSel();
	if (iSel != CB_ERR)
	{
		m_iSaveSelStartHour= iSel;
	}
}


void CSelDateTimeDlg::OnSelchangeEndhourCmb()
{
	// TODO: Add your control notification handler code here
	int iSel = m_selEndHourCob.GetCurSel();
	if (iSel != CB_ERR)
	{
		m_iSaveSelEndHour = iSel;
	}
}


void CSelDateTimeDlg::OnSelectMonthcalendar(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	COleDateTime m_dSelRq;
	m_dSelRq=COleDateTime(((NMSELCHANGE*)pNMHDR)->stSelStart);
	CTime temp(m_dSelRq.GetYear(),m_dSelRq.GetMonth(),m_dSelRq.GetDay(),0,0,0);
	 m_tRecordSelDate = temp.GetTime();
	*pResult = 0;
}
