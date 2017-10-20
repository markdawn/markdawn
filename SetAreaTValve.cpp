// SetAreaTValve.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "SetAreaTValve.h"
#include "afxdialogex.h"



// CSetAreaTValve dialog

IMPLEMENT_DYNAMIC(CSetAreaTValve, CDialog)

CSetAreaTValve::CSetAreaTValve(CWnd* pParent /*=NULL*/)
	: CDialog(CSetAreaTValve::IDD, pParent)
{

	m_iHighLimitEdit = 1200;
	m_iLowLimitEdit = 120;
	m_uSaveSelArea = 0;
	m_AreaTParaSet = new AreaTParaSet[AREA_BLOCK_NUM];
	for (int i = 0;i<AREA_BLOCK_NUM;i++)
	{
		m_AreaTParaSet[i].iLowTLimit = 1;
		m_AreaTParaSet[i].iHighTLimit = 1600;
	}
}

CSetAreaTValve::~CSetAreaTValve()
{
	if (m_AreaTParaSet)
	{
		delete [] m_AreaTParaSet;
		m_AreaTParaSet = NULL;
	}
}

void CSetAreaTValve::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AREA_COB, m_SelAreaCob);
	DDX_Text(pDX, IDC_HIGHLIMIT_EDIT, m_iHighLimitEdit);
	DDX_Text(pDX, IDC_LOWLIMIT_EDIT, m_iLowLimitEdit);
}


BEGIN_MESSAGE_MAP(CSetAreaTValve, CDialog)
	ON_CBN_SELCHANGE(IDC_AREA_COB, &CSetAreaTValve::OnSelchangeAreaCob)
	ON_EN_CHANGE(IDC_LOWLIMIT_EDIT, &CSetAreaTValve::OnChangeLowlimitEdit)
	ON_EN_CHANGE(IDC_HIGHLIMIT_EDIT, &CSetAreaTValve::OnChangeHighlimitEdit)
END_MESSAGE_MAP()


// CSetAreaTValve message handlers


BOOL CSetAreaTValve::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString strAera;	 
	for (int i = 1;i<=AREA_BLOCK_NUM;i++)
	{
		strAera.Format(_T("%02d"), i);
		m_SelAreaCob.AddString(strAera);
	}
	m_SelAreaCob.SetCurSel(0);

	m_iHighLimitEdit = m_AreaTParaSet[0].iHighTLimit;
	m_iLowLimitEdit = m_AreaTParaSet[0].iLowTLimit;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

AreaTParaSet* CSetAreaTValve::GetAreaTParaSetPointer(void)
{
	ASSERT(m_AreaTParaSet);
	return m_AreaTParaSet;
}

void CSetAreaTValve::OnSelchangeAreaCob()
{
	// TODO: Add your control notification handler code here
	int iSel = m_SelAreaCob.GetCurSel();
	if (iSel != CB_ERR)
	{
		m_uSaveSelArea= iSel;
		m_iHighLimitEdit = m_AreaTParaSet[m_uSaveSelArea].iHighTLimit;
		m_iLowLimitEdit = m_AreaTParaSet[m_uSaveSelArea].iLowTLimit;
		UpdateData(FALSE);
	}
}


void CSetAreaTValve::OnChangeLowlimitEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData(TRUE);
	m_AreaTParaSet[m_uSaveSelArea].iLowTLimit = m_iLowLimitEdit;
	// TODO:  Add your control notification handler code here
}


void CSetAreaTValve::OnChangeHighlimitEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
	m_AreaTParaSet[m_uSaveSelArea].iHighTLimit = m_iHighLimitEdit;
}


void CSetAreaTValve::SetIAreaTLimit(AreaTParaSet* setAreaTLimit,UINT nAreaNum)
{
	if (nAreaNum>AREA_BLOCK_NUM || nAreaNum<0)
	{
		AfxMessageBox(_T("保存区域平均温度阀值设置错误"));
		return;
	}

	for (UINT i = 0; i<nAreaNum; i++)
	{
		m_AreaTParaSet[i].iLowTLimit = setAreaTLimit[i].iLowTLimit;
		m_AreaTParaSet[i].iHighTLimit = setAreaTLimit[i].iHighTLimit;
	}

}