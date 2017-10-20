// MainDIsInfoView.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "MainDIsInfoView.h"
#include "commonFile.h"
#include "MainFrm.h"

#include <gl\gl.h> 
#include <gl\glu.h> 
#include <gl\glaux.h>


// CMainDIsInfoView

IMPLEMENT_DYNCREATE(CMainDIsInfoView, CFormView)
#define LEDTIMER 400

CMainDIsInfoView::CMainDIsInfoView()
	: CFormView(CMainDIsInfoView::IDD)
{

}

CMainDIsInfoView::~CMainDIsInfoView()
{

}

void CMainDIsInfoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAINDIS_BMP, m_MainDispalyBmp);
	DDX_Control(pDX, IDC_LOGO_STATIC, m_LogoDisplayBmp);
	DDX_Control(pDX, IDC_ARM_STATIC, m_ArmModuleDisplayBmp);
	DDX_Control(pDX, IDC_AREAT_STATIC, m_AreaTDisplayBmp);
	DDX_Control(pDX, IDC_ISOTHERM_STATIC, m_IsothermDisplayBmp);
	DDX_Control(pDX, IDC_POS_STATIC, m_BolierDisplayBmp);
	DDX_Control(pDX, IDC_VALVA_STATIC, m_ValveDisplayBmp);
	DDX_Control(pDX, IDC_ARM2_STATIC, m_ArmModule2DisplayBmp);
	DDX_Control(pDX, IDC_LED1, m_Led1);
	DDX_Control(pDX, IDC_LED2, m_Led2);
	DDX_Control(pDX, IDC_LED3, m_Led3);
	DDX_Control(pDX, IDC_LED4, m_Led4);
	DDX_Control(pDX, IDC_LED5, m_Led5);
	DDX_Control(pDX, IDC_LED6, m_Led6);
	DDX_Control(pDX, IDC_LED7, m_Led7);
	DDX_Control(pDX, IDC_LED8, m_Led8);
	DDX_Control(pDX, IDC_LED9, m_Led9);
	DDX_Control(pDX, IDC_LED10, m_Led10);
	DDX_Control(pDX, IDC_MAINLED, m_MainLed);
}

BEGIN_MESSAGE_MAP(CMainDIsInfoView, CFormView)
	ON_MESSAGE(WM_MAINDIS_MSG,OnDisplayAllPartsBmp)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
//	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMainDIsInfoView diagnostics

#ifdef _DEBUG
void CMainDIsInfoView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMainDIsInfoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMainDIsInfoView message handlers
void CMainDIsInfoView::InitMainDisInfo()
{
	GetDlgItem(IDC_LOGO_STATIC)->MoveWindow(&Logo_Rect);
	//InitBoilerPointPos();
	InitArmModule();
	InitIsothermView();
	InitAreaAvrgTView();
    InitValveAndPipe();

	CRect rRect;
	GetClientRect(&rRect);
//	m_MainDispalyBmp.MoveWindow(2*XCOFF,5*YCOFF,1900*XCOFF,920*YCOFF);
//	m_MainDispalyBmp.MoveWindow(0,0,rRect.right,rRect.bottom-50*YCOFF);
}
void CMainDIsInfoView::InitBoilerPointPos()
{
	GetDlgItem(IDC_POS_STATIC)->MoveWindow(&BoilerPos_Rect);
	//m_BolierDisplayBmp.MoveWindow(&BoilerPos_Rect);
}

void CMainDIsInfoView::InitArmModule()
{
	//GetDlgItem(IDC_ARM_STATIC)->MoveWindow(&ArmModule1_Rect);
	//GetDlgItem(IDC_ARM2_STATIC)->MoveWindow(&ArmModule2_Rect);
}
void CMainDIsInfoView::InitIsothermView()
{
//	GetDlgItem(IDC_ISOTHERM_STATIC)->MoveWindow(&Isotherm_Rect);
	m_IsothermDisplayBmp.MoveWindow(&Isotherm_Rect);
	m_IsothermDisplayBmp.InitOpenGL(this);
}
void CMainDIsInfoView::InitAreaAvrgTView()
{
	
	GetDlgItem(IDC_AREAT_STATIC)->MoveWindow(&AreaT_Rect);
}
void CMainDIsInfoView::InitValveAndPipe()
{
	GetDlgItem(IDC_VALVA_STATIC)->MoveWindow(&ValvePipe_Rect);
}

void CMainDIsInfoView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
//	glClearColor(100/255.0f,0/255.0f,0/255.0f,1);
	InitMainDisInfo();

	CRect rRectLed, rTmpRect;
	
	m_Led1.GetClientRect(rTmpRect);
	rRectLed.left = Led_Point.x ;
	rRectLed.top = Led_Point.y;
	rRectLed.right = rRectLed.left + rTmpRect.Width();
	rRectLed.bottom = rRectLed.top + rTmpRect.Height();
	m_Led1.MoveWindow(rRectLed);

	m_Led2.GetClientRect(rTmpRect);
	rRectLed.left = Led_Point.x + 25 * XCOFF;
	rRectLed.top = Led_Point.y - 173 * YCOFF;
	rRectLed.right = rRectLed.left + rTmpRect.Width();
	rRectLed.bottom = rRectLed.top + rTmpRect.Height();
	m_Led2.MoveWindow(rRectLed);

	m_Led3.GetClientRect(rTmpRect);
	rRectLed.left = Led_Point.x + 225 * XCOFF;
	rRectLed.top = Led_Point.y - 173 * YCOFF;
	rRectLed.right = rRectLed.left + rTmpRect.Width();
	rRectLed.bottom = rRectLed.top + rTmpRect.Height();
	m_Led3.MoveWindow(rRectLed);

	m_Led4.GetClientRect(rTmpRect);
	rRectLed.left = Led_Point.x + 225 * XCOFF;
	rRectLed.top = Led_Point.y + 173 * YCOFF;
	rRectLed.right = rRectLed.left + rTmpRect.Width();
	rRectLed.bottom = rRectLed.top + rTmpRect.Height();
	m_Led4.MoveWindow(rRectLed);

	m_Led5.GetClientRect(rTmpRect);
	rRectLed.left = Led_Point.x + 25 * XCOFF;
	rRectLed.top = Led_Point.y + 173 * YCOFF;
	rRectLed.right = rRectLed.left + rTmpRect.Width();
	rRectLed.bottom = rRectLed.top + rTmpRect.Height();
	m_Led5.MoveWindow(rRectLed);

	m_Led6.GetClientRect(rTmpRect);
	rRectLed.left = Led_Point.x + 647 * XCOFF;
	rRectLed.top = Led_Point.y ;
	rRectLed.right = rRectLed.left + rTmpRect.Width();
	rRectLed.bottom = rRectLed.top + rTmpRect.Height();
	m_Led6.MoveWindow(rRectLed);

	m_Led7.GetClientRect(rTmpRect);
	rRectLed.left = Led_Point.x + 625 * XCOFF;
	rRectLed.top = Led_Point.y - 173 * YCOFF;
	rRectLed.right = rRectLed.left + rTmpRect.Width();
	rRectLed.bottom = rRectLed.top + rTmpRect.Height();
	m_Led7.MoveWindow(rRectLed);

	m_Led8.GetClientRect(rTmpRect);
	rRectLed.left = Led_Point.x + 425 * XCOFF;
	rRectLed.top = Led_Point.y - 173 * YCOFF;
	rRectLed.right = rRectLed.left + rTmpRect.Width();
	rRectLed.bottom = rRectLed.top + rTmpRect.Height();
	m_Led8.MoveWindow(rRectLed);

	m_Led9.GetClientRect(rTmpRect);
	rRectLed.left = Led_Point.x + 425 * XCOFF;
	rRectLed.top = Led_Point.y + 173 * YCOFF;
	rRectLed.right = rRectLed.left + rTmpRect.Width();
	rRectLed.bottom = rRectLed.top + rTmpRect.Height();
	m_Led9.MoveWindow(rRectLed);

	m_Led10.GetClientRect(rTmpRect);
	rRectLed.left = Led_Point.x + 625 * XCOFF;
	rRectLed.top = Led_Point.y + 173 * YCOFF;
	rRectLed.right = rRectLed.left + rTmpRect.Width();
	rRectLed.bottom = rRectLed.top + rTmpRect.Height();
	m_Led10.MoveWindow(rRectLed);

	m_Led1.SetLed(CLed::LED_COLOR_RED, CLed::LED_OFF, CLed::LED_ROUND);

	StartTimer(2, LEDTIMER);
	
}


void CMainDIsInfoView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CMainFrame *pMain =(CMainFrame *)AfxGetMainWnd();

	if (pMain == NULL)
	{
		return ;
	}
	if(PtInRect(&BoilerPos_Rect,point))
	{
		pMain->SelView(POSITION_VIEW);
		pMain->m_currentMsg = WM_POSITION_MSG;
//		AfxMessageBox(_T("进入锅炉+测点显示"));
	}
	else if (PtInRect(&ValvePipe_Rect, point))
	{
		pMain->SelView(TIME_VIEW);
		pMain->m_currentMsg = WM_TIME_MSG;
//		AfxMessageBox(_T("进入ARM模块信息显示"));
	}
	else if (PtInRect(&Isotherm_Rect,point))
	{
		pMain->SelView(ISOTHERM_VIEW);
		pMain->m_currentMsg = WM_ISOTHERM_MSG;
//		AfxMessageBox(_T("进入温度场界面显示"));
	}
	else if (PtInRect(&ValvePipe_Rect,point))
	{
	//	pMain->SelView(ISOTHERM_VIEW);
	//	pMain->m_currentMsg = WM_ISOTHERM_MSG;
//		AfxMessageBox(_T("进入电磁阀及管道显示"));
	}
	else if (PtInRect(&AreaT_Rect,point))
	{
		pMain->SelView(AREAAVRGT_VIEW);
		pMain->m_currentMsg = WM_AREAARVGT_MSG;
//		AfxMessageBox(_T("进入区域平均温度显示"));
	}


	CFormView::OnLButtonDblClk(nFlags, point);
}


HBRUSH CMainDIsInfoView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CMainDIsInfoView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CFormView::OnPaint() for painting messages
	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(rect);	
	dc.FillSolidRect(&rect,SYSTEM_COLOR);//设置你需要的颜色
}

LRESULT CMainDIsInfoView::OnDisplayAllPartsBmp(WPARAM wparam,LPARAM  lparam)
{
	CMainFrame *pWnd=(CMainFrame *)wparam;
	if (!pWnd)
	{
		return -1;
	}
	
	//m_ValveDisplayBmp.UpdateState(wparam, lparam);
	
	m_AreaTDisplayBmp.DisplayAreaAvrgT(WPARAM(pWnd->GetAreaAvrgTValue()),LPARAM(pWnd->GetAreaTParaSetPointer()));
	DrawScene(WPARAM(pWnd->GetGridData()),(LPARAM)pWnd->GetGridDataInfo());


	return 0;
}

int CMainDIsInfoView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	return 0;
}

void CMainDIsInfoView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: Add your message handler code here
}


BOOL CMainDIsInfoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	return CFormView::PreCreateWindow(cs);
}

// OpenGL画图
void CMainDIsInfoView::DrawScene(WPARAM wparam,LPARAM  lparam)
{
	CRect rRectClient	(&IsothermT_Rect); //设置图形显示界面大小
	m_IsothermDisplayBmp.DisplayIsotherm(wparam,lparam,NULL,rRectClient);


}

void CMainDIsInfoView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CMainFrame *pWnd=(CMainFrame *)AfxGetMainWnd();
	if(!pWnd)
		return ;


	m_ValveDisplayBmp.OnPaint();

	if (nIDEvent == 2)
	{		
		if (pWnd->GetOneArmState(0) == TRUE)
		{
			m_Led1.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);
		}
		else
			m_Led1.SetLed(CLed::LED_COLOR_GREEN,CLed::LED_OFF,CLed::LED_ROUND);
		m_Led1.Ping(400);

		if (pWnd->GetOneArmState(1) == TRUE)
		{
			m_Led2.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);
		}
		else
			m_Led2.SetLed(CLed::LED_COLOR_GREEN,CLed::LED_OFF,CLed::LED_ROUND);
		m_Led2.Ping(400);

		if (pWnd->GetOneArmState(2) == TRUE)
		{
			m_Led3.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);
		}
		else
			m_Led3.SetLed(CLed::LED_COLOR_GREEN,CLed::LED_OFF,CLed::LED_ROUND);
		m_Led3.Ping(400);

		if (pWnd->GetOneArmState(3) == TRUE)
		{
			m_Led4.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);
		}
		else
			m_Led4.SetLed(CLed::LED_COLOR_GREEN,CLed::LED_OFF,CLed::LED_ROUND);
		m_Led4.Ping(400);
		
		if (pWnd->GetOneArmState(4) == TRUE)
		{
			m_Led5.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);	
		}
		else
			m_Led5.SetLed(CLed::LED_COLOR_GREEN,CLed::LED_OFF,CLed::LED_ROUND);
		m_Led5.Ping(400);

		if (pWnd->GetOneArmStateB(0) == TRUE)
		{
			m_Led6.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);	
		}
		else
			m_Led6.SetLed(CLed::LED_COLOR_GREEN,CLed::LED_OFF,CLed::LED_ROUND);
		m_Led6.Ping(400);

		if (pWnd->GetOneArmStateB(1) == TRUE)
		{
			m_Led7.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);	
		}
		else
			m_Led7.SetLed(CLed::LED_COLOR_GREEN,CLed::LED_OFF,CLed::LED_ROUND);
		m_Led7.Ping(400);

		if (pWnd->GetOneArmStateB(2) == TRUE)
		{
			m_Led8.SetLed(CLed::LED_COLOR_RED,CLed::LED_OFF,CLed::LED_ROUND);	
		}
		else
			m_Led8.SetLed(CLed::LED_COLOR_GREEN,CLed::LED_OFF,CLed::LED_ROUND);
		m_Led8.Ping(400);

		if (pWnd->GetOneArmStateB(3) == TRUE)
		{
			m_Led9.SetLed(CLed::LED_COLOR_RED, CLed::LED_OFF, CLed::LED_ROUND);
		}
		else
			m_Led9.SetLed(CLed::LED_COLOR_GREEN, CLed::LED_OFF, CLed::LED_ROUND);
		m_Led9.Ping(400);
	
		if (pWnd->GetOneArmStateB(4) == TRUE)
		{
			m_Led10.SetLed(CLed::LED_COLOR_RED, CLed::LED_OFF, CLed::LED_ROUND);
		}
		else
			m_Led10.SetLed(CLed::LED_COLOR_GREEN, CLed::LED_OFF, CLed::LED_ROUND);
		m_Led10.Ping(400);
	}


	CFormView::OnTimer(nIDEvent);
}

//ms
void CMainDIsInfoView::StartTimer(UINT_PTR nIDEvent ,UINT nElapse)
{
	SetTimer(nIDEvent,nElapse,NULL); //报警
}

void CMainDIsInfoView::StopTimer(UINT_PTR nIDEvent )
{
	KillTimer(nIDEvent);
}