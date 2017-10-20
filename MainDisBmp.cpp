// MainDisBmp.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "MainDisBmp.h"


// CMainDisBmp

IMPLEMENT_DYNAMIC(CMainDisBmp, CStatic)

CMainDisBmp::CMainDisBmp()
{
	m_Bmp.LoadBitmap(IDB_MAINDIS_BMP);
}

CMainDisBmp::~CMainDisBmp()
{
}


BEGIN_MESSAGE_MAP(CMainDisBmp, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CMainDisBmp message handlers




void CMainDisBmp::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CBitmap *pOldBmp;
	BITMAP bitmap;
	pOldBmp=NULL;
	pOldBmp=memDC.SelectObject(&m_Bmp);

	CRect rClient;
	GetClientRect(rClient);
	m_Bmp.GetBitmap(&bitmap);
	dc.StretchBlt(rClient.left,rClient.top,rClient.Width(),rClient.Height(),&memDC,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
//	dc.BitBlt(rClient.left+2,rClient.top+2,bitmap.bmWidth,bitmap.bmHeight,&memDC,0,0,SRCCOPY);
	memDC.SelectObject(pOldBmp);
}
