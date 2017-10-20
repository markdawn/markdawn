// SignalTimeView.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "SignalTimeView.h"
#include "commonFile.h"
#include "MainFrm.h"
#include "SysSetDlg.h"


// CSignalTimeView

IMPLEMENT_DYNCREATE(CSignalTimeView, CFormView)

CSignalTimeView::CSignalTimeView()
	: CFormView(CSignalTimeView::IDD)
{

	m_iSelPathOrderEdit = 1;
	m_sReceiveModule = _T("ARM2模块");
	m_sSendModule = _T("ARM1模块");
	m_bSendModuleDisFlag = TRUE;
	m_bReceiveModuleDisFlag = TRUE;
	m_iSendArmModule = 0;
	m_iRecArmModule = 0;
	m_bSendModuleDisFlag2 = TRUE;
	m_bReceiveModuleDisFlag2 = TRUE;
	m_iSendArmModule2 = 4;
	m_iRecArmModule2 = 4;

	m_brush.CreateSolidBrush(SYSTEM_COLOR);
}

CSignalTimeView::~CSignalTimeView()
{
}

void CSignalTimeView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TIME, m_SignalTime);
	DDX_Control(pDX, IDC_STATIC_TIME2, m_SignalTime2);
	///////////////被隐藏////////////////
	DDX_Text(pDX, IDC_SELPATH_EDIT, m_iSelPathOrderEdit);
	DDX_Control(pDX, IDC_SELPATH_SPIN, m_selPathSpin);
	DDX_Text(pDX, IDC_RECEIVEMODULE_EDIT, m_sReceiveModule);
	DDX_Text(pDX, IDC_SENDMODULE_EDIT, m_sSendModule);
	///////////////被隐藏////////////////
	DDX_Control(pDX, IDC_RECEIVEMODULE_COB, m_SelRecArmCob);
	DDX_Control(pDX, IDC_SENDMODULE_COB, m_SelSendArmCob);
	DDX_Control(pDX, IDC_RECEIVEMODULE_COB2, m_SelRecArmCob2);
	DDX_Control(pDX, IDC_SENDMODULE_COB2, m_SelSendArmCob2);
}

BEGIN_MESSAGE_MAP(CSignalTimeView, CFormView)
	ON_MESSAGE(WM_TIME_MSG, OnSignalTime)
	ON_WM_PAINT()

	ON_EN_CHANGE(IDC_SELPATH_EDIT, &CSignalTimeView::OnChangeSelpathEdit)
	ON_BN_CLICKED(IDC_SENDDIS_BTN, &CSignalTimeView::OnBnClickedSenddisBtn)
	ON_BN_CLICKED(IDC_RECEIVEDIS_BTN, &CSignalTimeView::OnBnClickedReceivedisBtn)
	
	ON_WM_CONTEXTMENU()
	ON_UPDATE_COMMAND_UI(ID_RETURN, &CSignalTimeView::OnUpdateReturn)
	ON_UPDATE_COMMAND_UI(ID_SETTING, &CSignalTimeView::OnUpdateSetting)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_RECEIVEMODULE_COB, &CSignalTimeView::OnSelchangeReceivemoduleCob)
	ON_CBN_SELCHANGE(IDC_SENDMODULE_COB, &CSignalTimeView::OnSelchangeSendmoduleCob)
	ON_CBN_SELCHANGE(IDC_SENDMODULE_COB2, &CSignalTimeView::OnCbnSelchangeSendmoduleCob2)
	ON_CBN_SELCHANGE(IDC_RECEIVEMODULE_COB2, &CSignalTimeView::OnCbnSelchangeReceivemoduleCob2)
	ON_BN_CLICKED(IDC_SENDDIS_BTN2, &CSignalTimeView::OnBnClickedSenddisBtn2)
	ON_BN_CLICKED(IDC_RECEIVEDIS_BTN2, &CSignalTimeView::OnBnClickedReceivedisBtn2)
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
	m_SignalTime.MoveWindow(&Signal_Rect);
	m_SignalTime2.MoveWindow(&Signal_Rect2);
	m_selPathSpin.SetRange(1,28);
	m_selPathSpin.SetPos(1);
	GetDlgItem(IDC_HALF_STATIC)->MoveWindow(50 * XCOFF, 50 * YCOFF, 90 * XCOFF, 30 * YCOFF);
	GetDlgItem(IDC_SEND_STATIC)->MoveWindow(60 * XCOFF, 120 * YCOFF, 80 * XCOFF, 30 * YCOFF);
	GetDlgItem(IDC_SENDMODULE_COB)->MoveWindow(50 * XCOFF, 150 * YCOFF, 90 * XCOFF, 130 * YCOFF);
	GetDlgItem(IDC_SENDDIS_BTN)->MoveWindow(65 * XCOFF, 190 * YCOFF, 60 * XCOFF, 30 * YCOFF);
	GetDlgItem(IDC_RECV_STATIC)->MoveWindow(60 * XCOFF, 290 * YCOFF, 80 * XCOFF, 30 * YCOFF);
	GetDlgItem(IDC_RECEIVEMODULE_COB)->MoveWindow(50 * XCOFF, 320 * YCOFF, 90 * XCOFF, 130 * YCOFF);
	GetDlgItem(IDC_RECEIVEDIS_BTN)->MoveWindow(65 * XCOFF, 360 * YCOFF, 60 * XCOFF, 30 * YCOFF);
	GetDlgItem(IDC_HALF_STATIC2)->MoveWindow(50 * XCOFF, 500 * YCOFF, 90 * XCOFF, 30 * YCOFF);
	GetDlgItem(IDC_SEND_STATIC2)->MoveWindow(60 * XCOFF, 570 * YCOFF, 80 * XCOFF, 30 * YCOFF);
	GetDlgItem(IDC_SENDMODULE_COB2)->MoveWindow(50 * XCOFF, 600 * YCOFF, 90 * XCOFF, 130 * YCOFF);
	GetDlgItem(IDC_SENDDIS_BTN2)->MoveWindow(65 * XCOFF, 640 * YCOFF, 60 * XCOFF, 30 * YCOFF);
	GetDlgItem(IDC_RECV_STATIC2)->MoveWindow(60 * XCOFF, 750 * YCOFF, 80 * XCOFF, 30 * YCOFF);
	GetDlgItem(IDC_RECEIVEMODULE_COB2)->MoveWindow(50 * XCOFF, 780 * YCOFF, 90 * XCOFF, 130 * YCOFF);
	GetDlgItem(IDC_RECEIVEDIS_BTN2)->MoveWindow(65 * XCOFF, 810 * YCOFF, 60 * XCOFF, 30 * YCOFF);

	//初始化
	CString strSel;	
	int i = 0;
	for (int i = 1;i<=5/*ARM_NUM*/;i++)
	{
		strSel.Format(_T("Module %02d"),i);
		m_SelRecArmCob.AddString(strSel);
	}
	m_SelRecArmCob.SetCurSel(0);

	for (i = 1;i<=5/*ARM_NUM*/;i++)
	{
		strSel.Format(_T("Module %02d"),i);
		m_SelSendArmCob.AddString(strSel);
	}
	m_SelSendArmCob.SetCurSel(0);
	
	for (int i = 6; i <= 10; i++)
	{
		strSel.Format(_T("Module %02d"), i);
		m_SelRecArmCob2.AddString(strSel);
	}
	m_SelRecArmCob2.SetCurSel(0);

	for (i = 6; i <= 10; i++)
	{
		strSel.Format(_T("Module %02d"), i);
		m_SelSendArmCob2.AddString(strSel);
	}
	m_SelSendArmCob2.SetCurSel(0);


}

LRESULT CSignalTimeView::OnSignalTime(WPARAM wparam, LPARAM lparam)
{
	UpdateData(TRUE);
	m_SignalTime.DisplayTime(wparam,lparam,m_iSendArmModule,m_iRecArmModule,m_bReceiveModuleDisFlag,m_bSendModuleDisFlag);
	m_SignalTime2.DisplayTime(wparam, lparam, m_iSendArmModule2, m_iRecArmModule2, m_bReceiveModuleDisFlag2, m_bSendModuleDisFlag2);
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
	dc.FillSolidRect(&rect,SYSTEM_COLOR);
}


void CSignalTimeView::OnChangeSelpathEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (!m_selPathSpin)
	{
		return;
	}
	m_iSelPathOrderEdit = m_selPathSpin.GetPos();
	int order = m_iSelPathOrderEdit-1;
	m_sSendModule.Format(_T("ARM%d模块"),gPathOrder[order].iSendModule);
	m_sReceiveModule.Format(_T("ARM%d模块"),gPathOrder[order].iReveiveModule);


	UpdateData(FALSE);
}



void CSignalTimeView::OnBnClickedSenddisBtn()
{
	// TODO: Add your control notification handler code here
	m_bSendModuleDisFlag = !m_bSendModuleDisFlag;
}


void CSignalTimeView::OnBnClickedReceivedisBtn()
{
	// TODO: Add your control notification handler code here
	m_bReceiveModuleDisFlag = !m_bReceiveModuleDisFlag;
}


void CSignalTimeView::OnContextMenu(CWnd* pWnd, CPoint point)
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
	VERIFY(menu.LoadMenu(IDR_ARMMODULE_MENU));        //CG_IDR_POPUP_TYPING_TEST_VIEW为菜单ID
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;
	//	while (pWndPopupOwner->GetStyle() & WS_CHILD)  
	//		pWndPopupOwner = pWndPopupOwner->GetParent();
	//	ClientToScreen(&point);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,pWndPopupOwner);
}


void CSignalTimeView::OnUpdateReturn(CCmdUI *pCmdUI)
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


void CSignalTimeView::OnUpdateSetting(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CMainFrame *pMain =(CMainFrame *)AfxGetMainWnd();
	if (pMain == NULL)
	{
		return ;
	}

	CSysSetDlg setDlg;
	if (setDlg.DoModal()==IDOK)
	{
		pMain->SaveIPAndPortSetting(pMain->GetIPAddressPointer(),pMain->GetPortPointer(),ARM_NUM);
	}
}


HBRUSH CSignalTimeView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	int  nID = pWnd-> GetDlgCtrlID();
	if (nID == IDC_RECV_STATIC || nID == IDC_SEND_STATIC || nID == IDC_RECV_STATIC2 || nID == IDC_SEND_STATIC2 || nID == IDC_HALF_STATIC || nID == IDC_HALF_STATIC2)
	{
		pDC->SetBkMode(TRANSPARENT);  
		pDC->SetTextColor(WHITE_COLOR);
		return m_brush;
	}
	LOGFONT LogFont;
	lstrcpy((LPSTR)LogFont.lfFaceName, (LPSTR)"楷体_GB2312");
	LogFont.lfWeight = 700;
	LogFont.lfWidth = 12;
	LogFont.lfHeight = 30;
	LogFont.lfEscapement = 0;
	LogFont.lfUnderline = FALSE;
	LogFont.lfItalic = FALSE;
	LogFont.lfStrikeOut = FALSE;
	LogFont.lfCharSet = GB2312_CHARSET; 
	CFont m_font;
	m_font.CreateFontIndirect(&LogFont);
	GetDlgItem(IDC_HALF_STATIC)->SetFont(&m_font);
	GetDlgItem(IDC_HALF_STATIC2)->SetFont(&m_font);

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CSignalTimeView::OnSelchangeReceivemoduleCob()
{
	// TODO: Add your control notification handler code here
	int iSel = m_SelRecArmCob.GetCurSel();
	if (iSel != CB_ERR)
	{
		m_iRecArmModule= iSel;
	}
}

void CSignalTimeView::OnSelchangeSendmoduleCob()
{
	// TODO: Add your control notification handler code here
	int iSel = m_SelSendArmCob.GetCurSel();
	if (iSel != CB_ERR)
	{
		m_iSendArmModule= iSel;
	}
}


BOOL CSignalTimeView::PreTranslateMessage(MSG* pMsg)
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


void CSignalTimeView::OnCbnSelchangeSendmoduleCob2()
{
	// TODO:  在此添加控件通知处理程序代码
	int iSel = m_SelSendArmCob2.GetCurSel();
	if (iSel != CB_ERR)
	{
		m_iSendArmModule2 = iSel;
	}
}


void CSignalTimeView::OnCbnSelchangeReceivemoduleCob2()
{
	// TODO:  在此添加控件通知处理程序代码
	int iSel = m_SelRecArmCob2.GetCurSel();
	if (iSel != CB_ERR)
	{
		m_iRecArmModule2 = iSel;
	}
}


void CSignalTimeView::OnBnClickedSenddisBtn2()
{
	// TODO:  在此添加控件通知处理程序代码
	m_bSendModuleDisFlag2 = !m_bSendModuleDisFlag2;
}


void CSignalTimeView::OnBnClickedReceivedisBtn2()
{
	// TODO:  在此添加控件通知处理程序代码
	m_bReceiveModuleDisFlag2 = !m_bReceiveModuleDisFlag2;
}
