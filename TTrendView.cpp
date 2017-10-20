// TTrendView.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "TTrendView.h"
#include "commonFile.h"
#include "MainFrm.h"


// CTTrendView

IMPLEMENT_DYNCREATE(CTTrendView, CFormView)

CTTrendView::CTTrendView()
	: CFormView(CTTrendView::IDD)
{
	m_iChanelA = 1;
	m_iChanelB = 2;
	m_iChanelC = 3;
	m_iChanelD = 4;
	m_iChanelE = 5;
	m_iChanelF = 6;
	m_iChanelG = 7;
	m_iChanelH = 8;
	m_iChanelI = 9;
	m_iChanelJ = 10;
	m_iChanelK = 11;
	m_iChanelL = 12;
}

CTTrendView::~CTTrendView()
{
	m_bDisAreaT1CurveFlag = TRUE;
	m_bDisAreaT2CurveFlag = TRUE;
	m_bDisAreaT3CurveFlag = TRUE;
	m_bDisAreaT4CurveFlag = TRUE;
	m_bDisAreaT5CurveFlag = TRUE;
	m_bDisAreaT6CurveFlag = TRUE;
	m_bDisAreaT7CurveFlag = TRUE;
	m_bDisAreaT8CurveFlag = TRUE;
	m_bDisAreaT9CurveFlag = TRUE;
	m_bDisAreaT10CurveFlag = TRUE;
	m_bDisAreaT11CurveFlag = TRUE;
	m_bDisAreaT12CurveFlag = TRUE;
	//m_bDisAreaT13CurveFlag = TRUE;
	//m_bDisAreaT14CurveFlag = TRUE;
	//m_bDisAreaT15CurveFlag = TRUE;
	//m_bDisAreaT16CurveFlag = TRUE;
}

void CTTrendView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TTREND, m_TTrendBmp);
}

BEGIN_MESSAGE_MAP(CTTrendView, CFormView)
	ON_MESSAGE(WM_TTREND_MSG,OnTTrend)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_AREAT1_BTN, &CTTrendView::OnBnClickedAreat1Btn)
	ON_BN_CLICKED(IDC_AREAT2_BTN, &CTTrendView::OnBnClickedAreat2Btn)
	ON_BN_CLICKED(IDC_AREAT3_BTN, &CTTrendView::OnBnClickedAreat3Btn)
	ON_BN_CLICKED(IDC_AREAT4_BTN, &CTTrendView::OnBnClickedAreat4Btn)
	ON_BN_CLICKED(IDC_AREAT5_BTN, &CTTrendView::OnBnClickedAreat5Btn)
	ON_BN_CLICKED(IDC_AREAT6_BTN, &CTTrendView::OnBnClickedAreat6Btn)
	ON_BN_CLICKED(IDC_AREAT7_BTN, &CTTrendView::OnBnClickedAreat7Btn)
	ON_BN_CLICKED(IDC_AREAT8_BTN, &CTTrendView::OnBnClickedAreat8Btn)
	ON_BN_CLICKED(IDC_AREAT9_BTN, &CTTrendView::OnBnClickedAreat9Btn)
	ON_BN_CLICKED(IDC_AREAT10_BTN, &CTTrendView::OnBnClickedAreat10Btn)
	ON_BN_CLICKED(IDC_AREAT11_BTN, &CTTrendView::OnBnClickedAreat11Btn)
	ON_BN_CLICKED(IDC_AREAT12_BTN, &CTTrendView::OnBnClickedAreat12Btn)
	//ON_BN_CLICKED(IDC_AREAT13_BTN, &CTTrendView::OnBnClickedAreat13Btn)
	//ON_BN_CLICKED(IDC_AREAT14_BTN, &CTTrendView::OnBnClickedAreat14Btn)
	//ON_BN_CLICKED(IDC_AREAT15_BTN, &CTTrendView::OnBnClickedAreat15Btn)
	//ON_BN_CLICKED(IDC_AREAT16_BTN, &CTTrendView::OnBnClickedAreat16Btn)
	ON_BN_CLICKED(IDC_SELTIME_BTN, &CTTrendView::OnBnClickedSeltimeBtn)
	ON_WM_CONTEXTMENU()
	ON_UPDATE_COMMAND_UI(ID_RETURN, &CTTrendView::OnUpdateReturn)
END_MESSAGE_MAP()


// CTTrendView diagnostics

#ifdef _DEBUG
void CTTrendView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTTrendView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTTrendView message handlers


void CTTrendView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
//	m_TTrendBmp.MoveWindow(60*XCOFF,20*YCOFF,1200*XCOFF,780*YCOFF);

	m_TTrendBmp.MoveWindow(&TrendT_Rect);
	GetDlgItem(IDC_SELTIME_BTN)->MoveWindow(30*XCOFF,80*YCOFF,100*XCOFF,50*YCOFF);
	int yOffset = 75; 
	int nWidth = 100;
	int nHight = 50;
	int xStart = 30;
	int yStart = 160;
	GetDlgItem(IDC_AREAT1_BTN)->MoveWindow(xStart * XCOFF, yStart * YCOFF, nWidth*XCOFF, nHight * YCOFF);
	GetDlgItem(IDC_AREAT2_BTN)->MoveWindow(xStart * XCOFF, yStart * YCOFF + yOffset, nWidth*XCOFF, nHight * YCOFF);
	GetDlgItem(IDC_AREAT3_BTN)->MoveWindow(xStart * XCOFF, yStart * YCOFF + yOffset * 2, nWidth*XCOFF, nHight * YCOFF);
	GetDlgItem(IDC_AREAT4_BTN)->MoveWindow(xStart * XCOFF, yStart * YCOFF + yOffset * 3, nWidth*XCOFF, nHight * YCOFF);
	GetDlgItem(IDC_AREAT5_BTN)->MoveWindow(xStart * XCOFF, yStart * YCOFF + yOffset * 4, nWidth*XCOFF, nHight * YCOFF);
	GetDlgItem(IDC_AREAT6_BTN)->MoveWindow(xStart * XCOFF, yStart * YCOFF + yOffset * 5, nWidth*XCOFF, nHight * YCOFF);
	GetDlgItem(IDC_AREAT7_BTN)->MoveWindow(xStart * XCOFF, yStart * YCOFF + yOffset * 6, nWidth*XCOFF, nHight * YCOFF);
	GetDlgItem(IDC_AREAT8_BTN)->MoveWindow(xStart * XCOFF, yStart * YCOFF + yOffset * 7, nWidth*XCOFF, nHight * YCOFF);
	GetDlgItem(IDC_AREAT9_BTN)->MoveWindow(xStart * XCOFF, yStart * YCOFF + yOffset * 8, nWidth*XCOFF, nHight * YCOFF);
	GetDlgItem(IDC_AREAT10_BTN)->MoveWindow(xStart * XCOFF, yStart * YCOFF + yOffset * 9, nWidth*XCOFF, nHight * YCOFF);
	GetDlgItem(IDC_AREAT11_BTN)->MoveWindow(xStart * XCOFF, yStart * YCOFF + yOffset * 10, nWidth*XCOFF, nHight * YCOFF);
	GetDlgItem(IDC_AREAT12_BTN)->MoveWindow(xStart * XCOFF, yStart * YCOFF + yOffset * 11, nWidth*XCOFF, nHight * YCOFF);
	//GetDlgItem(IDC_AREAT13_BTN)->MoveWindow(30*XCOFF,100*YCOFF+yOffset*12,40*XCOFF,30*YCOFF);
	//GetDlgItem(IDC_AREAT14_BTN)->MoveWindow(30*XCOFF,100*YCOFF+yOffset*13,40*XCOFF,30*YCOFF);
	//GetDlgItem(IDC_AREAT15_BTN)->MoveWindow(30*XCOFF,100*YCOFF+yOffset*14,40*XCOFF,30*YCOFF);
	//GetDlgItem(IDC_AREAT16_BTN)->MoveWindow(30*XCOFF,100*YCOFF+yOffset*15,40*XCOFF,30*YCOFF);
	/************************************************************************/
	/* ��ʼ����ǰʱ���00:00:00--23:59:59Ϊ��ʷ������ʾʱ���                                                                   */
	/************************************************************************/
	CTime tm = CTime ::GetCurrentTime();
	CTime tmStart(tm.GetYear(),tm.GetMonth(),tm.GetDay(),0,0,0);
	CTime tmEnd(tm.GetYear(),tm.GetMonth(),tm.GetDay(),23,59,59);

	m_tStartEndTime.startTIme = tmStart.GetTime();
	m_tStartEndTime.endTime = tmEnd.GetTime(); 

	((CButton*)GetDlgItem(IDC_AREAT1_BTN))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_AREAT2_BTN))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_AREAT3_BTN))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_AREAT4_BTN))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_AREAT5_BTN))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_AREAT6_BTN))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_AREAT7_BTN))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_AREAT8_BTN))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_AREAT9_BTN))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_AREAT10_BTN))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_AREAT11_BTN))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_AREAT12_BTN))->SetCheck(TRUE);
	//((CButton*)GetDlgItem(IDC_AREAT13_BTN))->SetCheck(TRUE);
	//((CButton*)GetDlgItem(IDC_AREAT14_BTN))->SetCheck(TRUE);
	//((CButton*)GetDlgItem(IDC_AREAT15_BTN))->SetCheck(TRUE);
	//((CButton*)GetDlgItem(IDC_AREAT16_BTN))->SetCheck(TRUE);

	/*LOGFONT LogFont;
	lstrcpy((LPSTR)LogFont.lfFaceName, (LPSTR)"΢���ź�");
	LogFont.lfWeight = 50;
	LogFont.lfWidth = 12;
	LogFont.lfHeight = 30;
	LogFont.lfEscapement = 0;
	LogFont.lfUnderline = FALSE;
	LogFont.lfItalic = FALSE;
	LogFont.lfStrikeOut = FALSE;
	LogFont.lfCharSet = ANSI_CHARSET;
	CFont m_font;
	m_font.CreateFontIndirect(&LogFont);
	
	GetDlgItem(IDC_SELTIME_BTN)->SetFont(&m_font);
	GetDlgItem(IDC_AREAT1_BTN)->SetFont(&m_font);
	GetDlgItem(IDC_AREAT2_BTN)->SetFont(&m_font);
	GetDlgItem(IDC_AREAT3_BTN)->SetFont(&m_font);
	GetDlgItem(IDC_AREAT4_BTN)->SetFont(&m_font);
	GetDlgItem(IDC_AREAT5_BTN)->SetFont(&m_font);
	GetDlgItem(IDC_AREAT6_BTN)->SetFont(&m_font);
	GetDlgItem(IDC_AREAT7_BTN)->SetFont(&m_font);
	GetDlgItem(IDC_AREAT8_BTN)->SetFont(&m_font);
	GetDlgItem(IDC_AREAT9_BTN)->SetFont(&m_font);
	GetDlgItem(IDC_AREAT10_BTN)->SetFont(&m_font);*/

	
}

LRESULT CTTrendView::OnTTrend(WPARAM wparam,LPARAM lparam)
{
	m_TTrendBmp.DisplayTTrend(wparam, (LPARAM)&m_tStartEndTime, m_iChanelA, m_iChanelB, m_iChanelC, m_iChanelD, m_iChanelE, m_iChanelF, m_iChanelG, m_iChanelH, m_iChanelI, m_iChanelJ, m_iChanelK, m_iChanelL);
	return 0;
}


void CTTrendView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CFormView::OnPaint() for painting messages
	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(rect);	
	dc.FillSolidRect(&rect,SYSTEM_COLOR);//��������Ҫ����ɫ
}


void CTTrendView::OnBnClickedAreat1Btn()
{
	// TODO: Add your control notification handler code here
	m_bDisAreaT1CurveFlag = !m_bDisAreaT1CurveFlag;
}


void CTTrendView::OnBnClickedAreat2Btn()
{
	// TODO: Add your control notification handler code here
	m_bDisAreaT2CurveFlag = !m_bDisAreaT2CurveFlag;
}


void CTTrendView::OnBnClickedAreat3Btn()
{
	// TODO: Add your control notification handler code here
	m_bDisAreaT3CurveFlag = !m_bDisAreaT3CurveFlag;
}


void CTTrendView::OnBnClickedAreat4Btn()
{
	// TODO: Add your control notification handler code here
	m_bDisAreaT4CurveFlag = !m_bDisAreaT4CurveFlag;
}


void CTTrendView::OnBnClickedAreat5Btn()
{
	// TODO: Add your control notification handler code here
	m_bDisAreaT5CurveFlag = !m_bDisAreaT5CurveFlag;
}


void CTTrendView::OnBnClickedAreat6Btn()
{
	// TODO: Add your control notification handler code here
	m_bDisAreaT6CurveFlag = !m_bDisAreaT6CurveFlag;
}


void CTTrendView::OnBnClickedAreat7Btn()
{
	// TODO: Add your control notification handler code here
	m_bDisAreaT7CurveFlag = !m_bDisAreaT7CurveFlag;
}


void CTTrendView::OnBnClickedAreat8Btn()
{
	// TODO: Add your control notification handler code here
	m_bDisAreaT8CurveFlag = !m_bDisAreaT8CurveFlag;
}


void CTTrendView::OnBnClickedAreat9Btn()
{
	// TODO: Add your control notification handler code here
	m_bDisAreaT9CurveFlag = !m_bDisAreaT9CurveFlag;
}


void CTTrendView::OnBnClickedAreat10Btn()
{
	// TODO: Add your control notification handler code here
	m_bDisAreaT10CurveFlag = !m_bDisAreaT10CurveFlag;
}


void CTTrendView::OnBnClickedAreat11Btn()
{
	// TODO: Add your control notification handler code here
	m_bDisAreaT11CurveFlag = !m_bDisAreaT11CurveFlag;
}


void CTTrendView::OnBnClickedAreat12Btn()
{
	// TODO: Add your control notification handler code here
	m_bDisAreaT12CurveFlag = !m_bDisAreaT12CurveFlag;
}


//void CTTrendView::OnBnClickedAreat13Btn()
//{
//	// TODO: Add your control notification handler code here
//	m_bDisAreaT13CurveFlag = !m_bDisAreaT13CurveFlag;
//}
//
//
//void CTTrendView::OnBnClickedAreat14Btn()
//{
//	// TODO: Add your control notification handler code here
//	m_bDisAreaT14CurveFlag = !m_bDisAreaT14CurveFlag;
//}
//
//
//void CTTrendView::OnBnClickedAreat15Btn()
//{
//	// TODO: Add your control notification handler code here
//	m_bDisAreaT15CurveFlag = !m_bDisAreaT15CurveFlag;
//}
//
//
//void CTTrendView::OnBnClickedAreat16Btn()
//{
//	// TODO: Add your control notification handler code here
//	m_bDisAreaT16CurveFlag = !m_bDisAreaT16CurveFlag;
//}


void CTTrendView::OnBnClickedSeltimeBtn()
{
	// TODO: Add your control notification handler code here
	CSelTTrendTimeDlg m_SelTTrendTimeDlg;
	m_SelTTrendTimeDlg.m_StartTime = m_tStartEndTime.startTIme ;
	m_SelTTrendTimeDlg.m_EndTime = m_tStartEndTime.endTime;
	if (m_SelTTrendTimeDlg.DoModal()==IDOK)
	{
		if (m_SelTTrendTimeDlg.m_EndTime - m_SelTTrendTimeDlg.m_StartTime>30*24*3600)
		{
			AfxMessageBox(_T("ѡ��ʱ�䳬����Χ��������ѡ��ʱ��Σ�"));
			return;
		}
		m_tStartEndTime.startTIme = m_SelTTrendTimeDlg.m_StartTime;
		m_tStartEndTime.endTime = m_SelTTrendTimeDlg.m_EndTime+1;
	}
}


void CTTrendView::OnContextMenu(CWnd* pWnd, CPoint point)
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
	VERIFY(menu.LoadMenu(IDR_MENU));        //CG_IDR_POPUP_TYPING_TEST_VIEWΪ�˵�ID
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;
	//	while (pWndPopupOwner->GetStyle() & WS_CHILD)  
	//		pWndPopupOwner = pWndPopupOwner->GetParent();
	//	ClientToScreen(&point);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,pWndPopupOwner);
}


void CTTrendView::OnUpdateReturn(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CMainFrame *pMain =(CMainFrame *)AfxGetMainWnd();
	if (pMain == NULL)
	{
		return ;
	}
	pMain->SelView(AREAAVRGT_VIEW);
	pMain->m_currentMsg = WM_AREAARVGT_MSG;
}


BOOL CTTrendView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	 if(pMsg->wParam==VK_ESCAPE)   
	 {
		 CMainFrame *pMain =(CMainFrame *)AfxGetMainWnd();
		 if (pMain == NULL)
		 {
			 return FALSE;
		 }
		 pMain->SelView(AREAAVRGT_VIEW);
		 pMain->m_currentMsg = WM_AREAARVGT_MSG;
	 }
	return CFormView::PreTranslateMessage(pMsg);
}