// DisAreaAvrgTInfoView.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "DisAreaAvrgTInfoView.h"
#include "commonFile.h"
#include "MainFrm.h"

#define N 5120
// CDisAreaAvrgTInfoView

IMPLEMENT_DYNCREATE(CDisAreaAvrgTInfoView, CFormView)

CDisAreaAvrgTInfoView::CDisAreaAvrgTInfoView()
	: CFormView(CDisAreaAvrgTInfoView::IDD)
{
	m_pGridCtrl = NULL;
	m_brush.CreateSolidBrush(SYSTEM_COLOR);
}

CDisAreaAvrgTInfoView::~CDisAreaAvrgTInfoView()
{
	 if(m_pGridCtrl)
		 delete m_pGridCtrl; 
	 m_tStartSearchTime = 0;
	 m_tEndSearchTime = 0;
}

void CDisAreaAvrgTInfoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDisAreaAvrgTInfoView, CFormView)
//	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_QUERY_BTN, &CDisAreaAvrgTInfoView::OnBnClickedQueryBtn)
ON_BN_CLICKED(IDC_SETTIME_BTN, &CDisAreaAvrgTInfoView::OnBnClickedSettimeBtn)
ON_WM_CTLCOLOR()
ON_WM_CONTEXTMENU()
ON_UPDATE_COMMAND_UI(ID_RETURN, &CDisAreaAvrgTInfoView::OnUpdateReturn)
END_MESSAGE_MAP()


// CDisAreaAvrgTInfoView diagnostics

#ifdef _DEBUG
void CDisAreaAvrgTInfoView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDisAreaAvrgTInfoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDisAreaAvrgTInfoView message handlers




void CDisAreaAvrgTInfoView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	InitGrid();
	CTime tm = CTime::GetCurrentTime();
	CTime temp(tm.GetYear(),tm.GetMonth(),tm.GetDay(),0,0,0);
	m_tStartSearchTime = temp.GetTime();
	m_tEndSearchTime = temp.GetTime()+24*3600-1;
}


void CDisAreaAvrgTInfoView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	 if(m_pGridCtrl->GetSafeHwnd())    
	 {      
		 CRect rect;      
		 GetClientRect(rect);     
	//	 m_pGridCtrl->MoveWindow(rect);  
	 }
}


//  计算区域平均温度统计信息
void CDisAreaAvrgTInfoView::CalAllAreaTinfo(time_t tStartTime, time_t tEndTime, UINT iAreaNum)
{
	CMainFrame *pWnd=(CMainFrame *)AfxGetMainWnd();
	if (!pWnd)
	{
		return;
	}

	MYSQL mysql;
	mysql = pWnd->GetMySQLHandle();

	//int i = 0;
	UINT i = 0;
	for (i = 1;i <= iAreaNum;i++)
	{
		CalPerAreaTinfo(mysql,tStartTime, tEndTime,i);
	}
	m_pGridCtrl->Refresh();
}
void CDisAreaAvrgTInfoView::CalPerAreaTinfo(MYSQL mysql,time_t tStartTime, time_t tEndTime, UINT iArea)
{
	CTime tm(tStartTime);
	CTime tmend(tEndTime);
	CString str;
	str.Format(_T("查询时间段%d%d%d %d:%d:%d----%d%d%d %d:%d:%d"),tm.GetYear(),tm.GetMonth(),tm.GetDay(),
		tm.GetHour(),tm.GetMinute(),tm.GetSecond(),tmend.GetYear(),tmend.GetMonth(),tmend.GetDay(),
		tmend.GetHour(),tmend.GetMinute(),tmend.GetSecond());
	((CStatic *)GetDlgItem(IDC_STC_QUERYTIME))->SetWindowText(str);
	UpdateData(TRUE);

	CString ch_query;
	ch_query.Format(_T("select Area%dAvrgTValue from areaavrgtvalue where  TimeParam>= %d and TimeParam <= %d"),iArea,int(tStartTime),int(tEndTime));
	UINT ilen = ch_query.GetLength();
	ch_query.ReleaseBuffer();
	if(mysql_real_query(&mysql,(char*)(LPCTSTR)ch_query,ilen)!=0)
	{
		CString strError;
		strError.Format(_T("无区域%d数据 !"),iArea);
		AfxMessageBox(strError);
		return;
	}

	MYSQL_RES *result;
	MYSQL_ROW row;
	if(!(result=mysql_store_result(&mysql)))
	{
		CString strError;
		strError.Format(_T("读取区域%d温度数据集失败 !"),iArea);
		AfxMessageBox(strError);
		return;
	}

	long timeOrder = 0;
	double totalD = 0;
	int iRow = 1;
	int nCountMysqlRow = 0;
	nCountMysqlRow = mysql_num_rows(result);
	if (nCountMysqlRow == 0)
	{
		CString strError;
		strError.Format(_T("无区域%d数据 !"),iArea);
		AfxMessageBox(strError);
		mysql_free_result(result);
		return;
	}
	int* T = new int[nCountMysqlRow];
	memset(T,0,nCountMysqlRow*sizeof(int));
	int i = 0;
	int Tmin = 0;
	int Tmax = 0;
	int Ttsh = 1000; //温度阀值
	int  nCounterOfOverT = 0;//分钟
	float sumT = 0.0f;
	float avrgT = 0.0f;
	if (row=mysql_fetch_row(result))
	{
		sumT = Tmin = Tmax = atoi(row[0]);
		i = 1;
	}
	while(row=mysql_fetch_row(result))
	{
		if (row[1]==NULL)
		{
			continue;
		}
		T[i] = atoi(row[0]);
		sumT = sumT + T[i]; //求和
		if (T[i] <Tmin)  //find the min T 
		{
			Tmin = T[i];
		}

		if (T[i]>Tmax)//find the max T 
		{
			Tmax = T[i];
		}

		if (T[i]>Ttsh)
		{
			nCounterOfOverT++; //记录超温时间
		}
		i++;
	}
	mysql_free_result(result);

	int iN = i;
	avrgT = sumT / iN; //计算平均温度
	str.Format(_T("%d"),(int)avrgT);
	m_pGridCtrl->SetItemText(iArea,1,str);//平均温度

	str.Format(_T("%d%%"),100);
	m_pGridCtrl->SetItemText(iArea,3,str);//变化率

	str.Format(_T("%d"),Tmin);
	m_pGridCtrl->SetItemText(iArea,4,str);//最低温度

	str.Format(_T("%d"),Tmax);
	m_pGridCtrl->SetItemText(iArea,5,str);//最高温度

	str.Format(_T("%d"),Ttsh);
	m_pGridCtrl->SetItemText(iArea,6,str);//温度阀值

	str.Format(_T("%d"),nCounterOfOverT);
	m_pGridCtrl->SetItemText(iArea,7,str);//超温时间

	float sum = 0;
	for (i = 0;i<iN;i++)
	{
		sum = sum +  (T[i]-avrgT)*(T[i]-avrgT);
	}
	float standardD = 0.0f;
	standardD = sqrtf(sum/N);
	str.Format(_T("%d"),(int)standardD);
	m_pGridCtrl->SetItemText(iArea,2,str);//计算标准差

	delete [] T;
	T = NULL;
}


void CDisAreaAvrgTInfoView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CFormView::OnPaint() for painting messages
	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(rect);	
	dc.FillSolidRect(&rect,SYSTEM_COLOR);//设置你需要的颜色
}

void CDisAreaAvrgTInfoView::OnBnClickedQueryBtn()
{
	// TODO: Add your control notification handler code here
	CSelDateTimeDlg m_SelDateTimeDlg;

	if (m_SelDateTimeDlg.DoModal() == IDOK)
	{
		m_tStartSearchTime = m_SelDateTimeDlg.m_tRecordSelDate+m_SelDateTimeDlg.m_iSaveSelStartHour*3600;
		m_tEndSearchTime = m_SelDateTimeDlg.m_tRecordSelDate+(m_SelDateTimeDlg.m_iSaveSelEndHour+1)*3600 - 1;
		CalAllAreaTinfo(m_tStartSearchTime, m_tEndSearchTime,AREA_BLOCK_NUM);
	}

}



void CDisAreaAvrgTInfoView::OnBnClickedSettimeBtn()
{
	// TODO: Add your control notification handler code here
/*
	if (m_SelDateTimeDlg.DoModal() == IDOK)
	{
		m_tStartSearchTime = m_SelDateTimeDlg.m_tRecordSelDate+m_SelDateTimeDlg.m_iSaveSelStartHour*3600;
		m_tEndSearchTime = m_SelDateTimeDlg.m_tRecordSelDate+(m_SelDateTimeDlg.m_iSaveSelEndHour+1)*3600 - 1;
	}*/
}


HBRUSH CDisAreaAvrgTInfoView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	int  nID = pWnd-> GetDlgCtrlID();
	if (nID ==IDC_STC_QUERYTIME || nID ==IDC_STC_INFO)
	{
		pDC->SetBkMode(TRANSPARENT);  
		pDC->SetTextColor(WHITE_COLOR);
		return m_brush;
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CDisAreaAvrgTInfoView::InitGrid(void)
{
	//初始化GridCtrl控件
	if(m_pGridCtrl!=NULL)
	{
		delete m_pGridCtrl;
		m_pGridCtrl=NULL;
	}

	if (m_pGridCtrl == NULL)
	{
		// Create the Gridctrl object
		m_pGridCtrl = new CGridCtrl;
		if (!m_pGridCtrl)
			return;

		// Create the Gridctrl window
		CRect rect(40*XCOFF,100*YCOFF,1240*XCOFF,800*YCOFF);

		m_pGridCtrl->Create(QueryT_Rect, this, 100);
		// fill it up with stuff
		m_pGridCtrl->SetEditable(false);
		m_pGridCtrl->SetTextBkColor(RGB(0xFF, 0xFF, 0xE0)); //黄色背景
		m_pGridCtrl->EnableDragAndDrop(false);

		int iRow = 17;  //17行
		int iCol = 8;//8列
		try {
			m_pGridCtrl->SetRowCount(iRow); //设置行数
			m_pGridCtrl->SetColumnCount(iCol);   //设置列数
			m_pGridCtrl->SetFixedRowCount(1);   //标题行为一行
			m_pGridCtrl->SetFixedColumnCount(1);  //同上
		}
		catch (CMemoryException* e)
		{
			e->ReportError();
			e->Delete();
			return;
		}

		//填充列标题
		for(int col=0;col<iCol;col++)
		{
			GV_ITEM Item;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = 0;//第0行
			Item.col = col;
			if(col==0){
				Item.nFormat = DT_CENTER|DT_WORDBREAK;
				Item.strText.Format(_T("【区域】"),col);
			}
			else if(col==1){
				Item.nFormat = DT_LEFT|DT_WORDBREAK;
				Item.strText.Format(_T("平均温度"),col);
			}
			else if(col==2){
				Item.nFormat = DT_LEFT|DT_WORDBREAK;
				Item.strText.Format(_T("标准差"),col);
			}
			else if(col==3){
				Item.nFormat = DT_LEFT|DT_WORDBREAK;
				Item.strText.Format(_T("最大温度变化率"),col);
			}
			else if(col==4){
				Item.nFormat = DT_LEFT|DT_WORDBREAK;
				Item.strText.Format(_T("最低温度"),col);
			}
			else if(col==5){
				Item.nFormat = DT_LEFT|DT_WORDBREAK;
				Item.strText.Format(_T("最高温度"),col);
			}
			else if(col==6){
				Item.nFormat = DT_LEFT|DT_WORDBREAK;
				Item.strText.Format(_T("温度阀值"),col);
			}
			else if(col==7){
				Item.nFormat = DT_LEFT|DT_WORDBREAK;
				Item.strText.Format(_T("超温时间"),col);
			}
			m_pGridCtrl->SetItem(&Item);
		}

		// fill rows/cols with text
		CString str;
		for (int row = 1; row < iRow; row++)
		{
			GV_ITEM Item;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = row;
			Item.col = 0;//第0列

			Item.nFormat = DT_CENTER|DT_VCENTER
				|DT_SINGLELINE|DT_END_ELLIPSIS
				|DT_NOPREFIX;
			Item.strText.Format(_T("%d"),row);
			m_pGridCtrl->SetItem(&Item);
		}
		m_pGridCtrl->AutoSize();

		//--------------设置行距------------------
		int iHight = (QueryT_Rect.bottom-QueryT_Rect.top-30)/16;
		m_pGridCtrl->SetRowHeight(0,30);  //设置第0行高
		for(int i=1;i<iRow;i++)
			m_pGridCtrl->SetRowHeight(i,iHight);  //设置各行高

		//--------------设置列距------------------
		int iWindth = (QueryT_Rect.right-QueryT_Rect.left-70)/7;
		m_pGridCtrl->SetColumnWidth(0,70); //设置第0列宽
		for (int j= 1; j < iCol;j++)
			m_pGridCtrl->SetColumnWidth(j,iWindth); //设置各列宽
	}
}







void CDisAreaAvrgTInfoView::OnContextMenu(CWnd* pWnd, CPoint point)
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


void CDisAreaAvrgTInfoView::OnUpdateReturn(CCmdUI *pCmdUI)
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


BOOL CDisAreaAvrgTInfoView::PreTranslateMessage(MSG* pMsg)
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
