// SysSetTap.cpp : implementation file
//

#include "stdafx.h"
#include "SysSetTap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysSetTap

CSysSetTap::CSysSetTap()
{
	m_itemcount=0;
	m_ptTabs.x=4;
	m_ptTabs.y=26;
}

CSysSetTap::~CSysSetTap()
{
	m_itemcount = 0;
}


BEGIN_MESSAGE_MAP(CSysSetTap, CTabCtrl)
	//{{AFX_MSG_MAP(CSysSetTap)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysSetTap message handlers

void CSysSetTap::AddItem(CWnd *pwnd, LPTSTR name)
{
	TCITEM item;
	item.mask = TCIF_TEXT|TCIF_PARAM;
	item.lParam = (LPARAM) pwnd;
	item.pszText = name;
	InsertItem(m_itemcount, &item);
	
	pwnd->SetWindowPos(NULL, m_ptTabs.x, m_ptTabs.y , 0, 0,
		SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER);

	if(m_itemcount==0)
	{
		pwnd->ShowWindow(SW_SHOW);//显示第一个子窗体
	}
	else
	{
		pwnd->ShowWindow(SW_HIDE);
	}
	
	m_itemcount++;
}


