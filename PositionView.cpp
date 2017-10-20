// PositionView.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "PositionView.h"
#include "commonFile.h"
#include "MainFrm.h"


// CPositionView

IMPLEMENT_DYNCREATE(CPositionView, CFormView)

CPositionView::CPositionView()
	: CFormView(CPositionView::IDD)
{
	m_brush.CreateSolidBrush(SYSTEM_COLOR);
}

CPositionView::~CPositionView()
{
}

void CPositionView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_POSITION, m_PositionBmp);
}

BEGIN_MESSAGE_MAP(CPositionView, CFormView)
	ON_MESSAGE(WM_POSITION_MSG, OnPosition)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_CONTEXTMENU()
	ON_UPDATE_COMMAND_UI(ID_RETURN, &CPositionView::OnUpdateReturn)
END_MESSAGE_MAP()


// CPositionView diagnostics

#ifdef _DEBUG
void CPositionView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPositionView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPositionView message handlers


void CPositionView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	m_PositionBmp.MoveWindow(&PointPos_Rect);
}


LRESULT CPositionView::OnPosition(WPARAM wparam, LPARAM lparam)
{
	m_PositionBmp.DisplayPosition(wparam,lparam);
	return 0;
}

void CPositionView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CFormView::OnPaint() for painting messages
	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(rect);	
	dc.FillSolidRect(&rect,SYSTEM_COLOR);//设置你需要的颜色
}


HBRUSH CPositionView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
		int  nID = pWnd-> GetDlgCtrlID();
	if (nID ==IDC_POSTXT_STC)
	{
		pDC->SetBkMode(TRANSPARENT);  
		pDC->SetTextColor(SYSTEM_COLOR);
		return m_brush;
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CPositionView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	if (point.x == -1 && point.y == -1)
	{
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);
		point = rect.TopLeft();
		point.Offset(5, 5);
	}
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU));        //CG_IDR_POPUP_TYPING_TEST_VIEW为菜单ID
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;
	//	while (pWndPopupOwner->GetStyle() & WS_CHILD)  
	//		pWndPopupOwner = pWndPopupOwner->GetParent();
	//	ClientToScreen(&point);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,pWndPopupOwner);
}


void CPositionView::OnUpdateReturn(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CMainFrame *pMain =(CMainFrame *)AfxGetMainWnd();
	if (pMain == NULL)
	{
		return ;
	}
	pMain->SelView(MAINDIS_VIEW);
	pMain->m_currentMsg = WM_MAINDIS_MSG;
}


BOOL CPositionView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->wParam==VK_ESCAPE)
	{
		CMainFrame *pMain =(CMainFrame *)AfxGetMainWnd();
		if (pMain == NULL)
		{
			return FALSE;
		}
		pMain->SelView(MAINDIS_VIEW);
		pMain->m_currentMsg = WM_MAINDIS_MSG;
	}
	return CFormView::PreTranslateMessage(pMsg);
}
