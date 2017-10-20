// SignalTimeView.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "SignalTimeView.h"
#include "commonFile.h"


// CSignalTimeView

IMPLEMENT_DYNCREATE(CSignalTimeView, CFormView)

CSignalTimeView::CSignalTimeView()
	: CFormView(CSignalTimeView::IDD)
{

}

CSignalTimeView::~CSignalTimeView()
{
}

void CSignalTimeView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TIME, m_SignalTime);
	//  DDX_Text(pDX, IDC_SELPATH_EDIT, m_iPathOrderEdit);
}

BEGIN_MESSAGE_MAP(CSignalTimeView, CFormView)
	ON_MESSAGE(WM_TIME_MSG, OnSignalTime)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_SELPATH_EDIT, &CSignalTimeView::OnEnChangeSelpathEdit)
END_MESSAGE_MAP()


// CSignalTimeView diagnostics

#ifdef _DEBUG
void CSignalTimeView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSignalTimeView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSignalTimeView message handlers


void CSignalTimeView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	m_SignalTime.MoveWindow(100*XCOFF,20*YCOFF,1150*XCOFF,780*YCOFF);
	UpdateData(FALSE);
}

LRESULT CSignalTimeView::OnSignalTime(WPARAM wparam, LPARAM lparam)
{
	CTime tm = CTime ::GetCurrentTime();
	m_SignalTime.DisplayTime(tm.GetTime(),lparam,0);
	return 0;
}

void CSignalTimeView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CFormView::OnPaint() for painting messages
	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(rect);	
	dc.FillSolidRect(&rect,SYSTEM_COLOR);//设置你需要的颜色
}


void CSignalTimeView::OnEnChangeSelpathEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
}
