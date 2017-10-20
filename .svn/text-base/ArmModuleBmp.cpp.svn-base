// ArmModuleBmp.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "ArmModuleBmp.h"


// CArmModuleBmp

IMPLEMENT_DYNAMIC(CArmModuleBmp, CStatic)

CArmModuleBmp::CArmModuleBmp()
{
	m_Bmp.LoadBitmap(IDB_ARMMODULE1_BMP);
}

CArmModuleBmp::~CArmModuleBmp()
{
}


BEGIN_MESSAGE_MAP(CArmModuleBmp, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CArmModuleBmp message handlers




void CArmModuleBmp::OnPaint()
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
