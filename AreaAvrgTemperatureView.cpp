// AreaAvrgTemperatureView.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "AreaAvrgTemperatureView.h"
#include "commonFile.h"
#include "MainFrm.h"
#include "SetAreaTValve.h"


// CAreaAvrgTemperatureView

IMPLEMENT_DYNCREATE(CAreaAvrgTemperatureView, CFormView)

CAreaAvrgTemperatureView::CAreaAvrgTemperatureView()
	: CFormView(CAreaAvrgTemperatureView::IDD)
{

}

CAreaAvrgTemperatureView::~CAreaAvrgTemperatureView()
{
}

void CAreaAvrgTemperatureView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_AREAAVRGT, m_AreaAvrgTBmp);
}

BEGIN_MESSAGE_MAP(CAreaAvrgTemperatureView, CFormView)
	ON_MESSAGE(WM_AREAARVGT_MSG,OnAreaArvgT)
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_UPDATE_COMMAND_UI(ID_RETURN, &CAreaAvrgTemperatureView::OnUpdateReturn)
	ON_UPDATE_COMMAND_UI(ID_REALTREND, &CAreaAvrgTemperatureView::OnUpdateRealtrend)
	ON_UPDATE_COMMAND_UI(ID_TINFO, &CAreaAvrgTemperatureView::OnUpdateTinfo)
	ON_UPDATE_COMMAND_UI(ID_SETTING, &CAreaAvrgTemperatureView::OnUpdateSetting)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CAreaAvrgTemperatureView diagnostics

#ifdef _DEBUG
void CAreaAvrgTemperatureView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAreaAvrgTemperatureView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAreaAvrgTemperatureView message handlers


void CAreaAvrgTemperatureView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CRect rect;
	GetWindowRect(&rect);
	CRect rect1;
	GetClientRect(&rect1);
	m_AreaAvrgTBmp.MoveWindow(&AverageT_Rect);
//	m_AreaAvrgTBmp.MoveWindow(&rect);
}

LRESULT CAreaAvrgTemperatureView::OnAreaArvgT(WPARAM wparam,LPARAM lparam)
{
	m_AreaAvrgTBmp.DisplayAreaAvrgT(wparam,lparam);
	return 0;
}



void CAreaAvrgTemperatureView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CFormView::OnPaint() for painting messages
	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(rect);	
	dc.FillSolidRect(&rect,SYSTEM_COLOR);//设置你需要的颜色
}


void CAreaAvrgTemperatureView::OnContextMenu(CWnd* pWnd, CPoint point)
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
	VERIFY(menu.LoadMenu(IDR_AREAT_MENU));        //CG_IDR_POPUP_TYPING_TEST_VIEW为菜单ID
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;
	//	while (pWndPopupOwner->GetStyle() & WS_CHILD)  
	//		pWndPopupOwner = pWndPopupOwner->GetParent();
//	ClientToScreen(&point);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,pWndPopupOwner);
}


void CAreaAvrgTemperatureView::OnUpdateReturn(CCmdUI *pCmdUI)
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


void CAreaAvrgTemperatureView::OnUpdateRealtrend(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CMainFrame *pMain =(CMainFrame *)AfxGetMainWnd();
	if (pMain == NULL)
	{
		return ;
	}
	pMain->SelView(TTREND_VIEW);
	pMain->m_currentMsg = WM_TTREND_MSG;
}


void CAreaAvrgTemperatureView::OnUpdateTinfo(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CMainFrame *pMain =(CMainFrame *)AfxGetMainWnd();
	if (pMain == NULL)
	{
		return ;
	}
	pMain->SelView(AVRGTINFO_VIEW);
	pMain->m_currentMsg = WM_AREATINFO_MSG;
}


BOOL CAreaAvrgTemperatureView::PreTranslateMessage(MSG* pMsg)
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


void CAreaAvrgTemperatureView::OnUpdateSetting(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CSetAreaTValve setTDlg;
	CMainFrame *pMain =(CMainFrame *)AfxGetMainWnd();
	if (pMain == NULL)
	{
		return ;
	}

	setTDlg.SetIAreaTLimit(pMain->GetAreaTParaSetPointer(),AREA_BLOCK_NUM);
	if (setTDlg.DoModal() == IDOK)
	{
		pMain->SaveAreaTLimitSetting(setTDlg.GetAreaTParaSetPointer(),AREA_BLOCK_NUM);
		pMain->SetIAreaTLimit(setTDlg.GetAreaTParaSetPointer(),AREA_BLOCK_NUM);
	}
}


void CAreaAvrgTemperatureView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CFormView::OnTimer(nIDEvent);
}
