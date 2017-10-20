// LeakView.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "LeakView.h"
#include "commonFile.h"

// CLeakView

IMPLEMENT_DYNCREATE(CLeakView, CFormView)

CLeakView::CLeakView()
	: CFormView(CLeakView::IDD)
{

}

CLeakView::~CLeakView()
{
}

void CLeakView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LEAK, m_LeakBmp);
}

BEGIN_MESSAGE_MAP(CLeakView, CFormView)
	ON_MESSAGE(WM_LEAK_MSG, OnLeak)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CLeakView diagnostics

#ifdef _DEBUG
void CLeakView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLeakView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLeakView message handlers


void CLeakView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
//	m_LeakBmp.MoveWindow(0*XCOFF,0*YCOFF,RESOLUTION_X*XCOFF,RESOLUTION_Y*YCOFF);
	m_LeakBmp.MoveWindow(10*XCOFF,10*YCOFF,(RESOLUTION_X-30)*XCOFF,(RESOLUTION_Y-205)*YCOFF);
	CTime tm = CTime ::GetCurrentTime();
	CTime tmStart(tm.GetYear(),tm.GetMonth(),tm.GetDay(),0,0,0);
	CTime tmEnd(tm.GetYear(),tm.GetMonth(),tm.GetDay(),23,59,59);

	m_tEndTime = tmEnd.GetTime()+1;
	m_tStartTime = tmStart.GetTime();
}

LRESULT CLeakView::OnLeak(WPARAM wparam, LPARAM lparam)
{
	m_LeakBmp.DisplayLeak(m_tEndTime,m_tStartTime);
	return 0;
}

void CLeakView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CFormView::OnPaint() for painting messages
	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(rect);	
	dc.FillSolidRect(&rect,SYSTEM_COLOR);//设置你需要的颜色
}
