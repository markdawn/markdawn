// IsothermView.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "IsothermView.h"
#include "commonFile.h"
#include "AcousticThermDoc.h"
#include <gl\gl.h> 
#include <gl\glu.h> 
#include <gl\glaux.h>

#include "MainFrm.h"

//#include <gl\glut.h> 


// CIsothermView

IMPLEMENT_DYNCREATE(CIsothermView, CFormView)

CIsothermView::CIsothermView()
	: CFormView(CIsothermView::IDD)
{

}

CIsothermView::~CIsothermView()
{

}

void CIsothermView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_STATIC_ISOTHERM, m_Isotherm);
	DDX_Control(pDX, IDC_STATIC_ISOTHERM, m_Isotherm);
}

BEGIN_MESSAGE_MAP(CIsothermView, CFormView)
	ON_MESSAGE(WM_ISOTHERM_MSG,OnIsotherm)
	ON_WM_CREATE()
	ON_WM_DESTROY()
//	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_UPDATE_COMMAND_UI(ID_RETURN, &CIsothermView::OnUpdateReturn)
	ON_COMMAND(ID_RETURN, &CIsothermView::OnReturn)
END_MESSAGE_MAP()


// CIsothermView diagnostics

#ifdef _DEBUG
void CIsothermView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CIsothermView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CIsothermView message handlers


void CIsothermView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	GetDlgItem(IDC_STATIC_COLOR)->MoveWindow(10*XCOFF,10*YCOFF,50*XCOFF,500*YCOFF);
   m_Isotherm.MoveWindow(&IsothermT_Rect);
	m_Isotherm.InitOpenGL(this);
}

LRESULT CIsothermView::OnIsotherm(WPARAM wparam,LPARAM  lparam)
{
	DrawScene(wparam,lparam);
	return 0;
}



int CIsothermView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	
	return 0;
}


void CIsothermView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: Add your message handler code here
}

BOOL CIsothermView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	return CFormView::PreCreateWindow(cs);
}


// OpenGL画图
void CIsothermView::DrawScene(WPARAM wparam,LPARAM  lparam)
{
	CRect rRectClient	(&IsothermT_Rect); //设置图形显示界面大小
	m_Isotherm.DisplayIsotherm(wparam,lparam,NULL,rRectClient);
}


void CIsothermView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CFormView::OnPaint() for painting messages
	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(rect);	
	dc.FillSolidRect(&rect,SYSTEM_COLOR);//设置你需要的颜色
}


void CIsothermView::OnContextMenu(CWnd* pWnd, CPoint point)
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


void CIsothermView::OnUpdateReturn(CCmdUI *pCmdUI)
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


void CIsothermView::OnReturn()
{
	// TODO: Add your command handler code here
	CMainFrame *pMain =(CMainFrame *)AfxGetMainWnd();
	if (pMain == NULL)
	{
		return ;
	}
	pMain->SelView(MAINDIS_VIEW);
	pMain->m_currentMsg = WM_MAINDIS_MSG;
}


BOOL CIsothermView::PreTranslateMessage(MSG* pMsg)
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
