// LeakBmp.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "LeakBmp.h"
#include "commonFile.h"
#pragma warning(disable:4018)

// CLeakBmp

IMPLEMENT_DYNAMIC(CLeakBmp, CStatic)

CLeakBmp::CLeakBmp()
{

}

CLeakBmp::~CLeakBmp()
{
}


BEGIN_MESSAGE_MAP(CLeakBmp, CStatic)
END_MESSAGE_MAP()



// 显示泄漏信息
void CLeakBmp::DisplayLeak(WPARAM wparam, LPARAM lparam)
{
	CDC *pDC=GetDC();
	Draw(pDC,wparam,lparam);
	ReleaseDC(pDC);
}


void CLeakBmp::Draw(CDC* pDC, WPARAM wparam, LPARAM lparam)
{
	CRect rClientRect;
	GetClientRect(rClientRect);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	if(m_Bmp.GetSafeHandle()==NULL)
		m_Bmp.CreateCompatibleBitmap(pDC,rClientRect.Width(),rClientRect.Height());

	CBitmap *pOldBmp;
	pOldBmp=memDC.SelectObject(&m_Bmp);

	CBrush backBrush(WHITE_COLOR);
	memDC.FillRect(rClientRect,&backBrush);  //填充显示区域背景色

	int nBkMode=memDC.SetBkMode(TRANSPARENT);

	/*****************************显示内容***********************************/
	DrawCurveFrame(&memDC,rClientRect, wparam,lparam);
	//	DrawTimeCurve(&memDC,rClientRect,wparam,lparam,iPathOrder);
	/************************************************************************/

	pDC->BitBlt(0,0,rClientRect.Width(),rClientRect.Height(),&memDC,0,0,SRCCOPY);

	memDC.SetBkMode(nBkMode);
	memDC.SelectObject(pOldBmp);
}


void CLeakBmp::DrawCurveFrame(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam)
{
	int OFFSET = 50;
	float REAL_X_LAGESCALE = 24.0f;//second
	float REAL_X_SMALLSCALE = 60.0f; //minute
	float LEAK_RANGE_Y  = 100.0f;//泄漏幅值
	float REAL_Y_LAGESCALE = 10.0f;//Y一个刻度对应10

	int iBias=10;//斜线
	CRect rHistimeMainRect ,tempRect;
	tempRect.left = rRect.left + OFFSET;
	tempRect.top = rRect.top + OFFSET;
	tempRect.right = rRect.right - OFFSET;
	tempRect.bottom = rRect.bottom - OFFSET;	

	CString strTemp;
	CSize sizeTemp;

	CTime tmStartTemp(lparam);
	CTime tmEndTemp(wparam-1);
	CTime tmTemp(lparam);
	//画坐标
	CPen greenPen,*pOldPen;
	UINT iRealHight = 0; 
	float iRealWidth=0;

	int  nDayNum = (wparam - lparam)/(REAL_X_LAGESCALE*REAL_X_SMALLSCALE*60);

	float xSmallScale = tempRect.Width()/(nDayNum*REAL_X_LAGESCALE*REAL_X_SMALLSCALE);//x轴最小刻度的宽度:nDayNum天，REAL_X_LAGESCALE小时，REAL_X_SMALLSCALE分钟
	float ySmallScale = tempRect.Height()/(LEAK_RANGE_Y); //y轴最小刻度的宽度

	iRealWidth = REAL_X_SMALLSCALE*xSmallScale; //每小时映射的宽度
	iRealHight = REAL_Y_LAGESCALE*ySmallScale;//映射频率幅值

	rHistimeMainRect.left = tempRect.left;//计算坐标x系的起点
	rHistimeMainRect.right=tempRect.right;//计算坐标X系的终点
	rHistimeMainRect.top = tempRect.top; //坐标Y系的终点
	rHistimeMainRect.bottom = rHistimeMainRect.top + 10*iRealHight;//坐标Y系的起点

	float xcoff = xSmallScale;
	float yScale=iRealHight;
	float xScale = iRealWidth;

	greenPen.CreatePen(PS_SOLID,1,BLACK_COLOR);
	pOldPen=pDC->SelectObject(&greenPen);

	pDC->MoveTo(rHistimeMainRect.left,rHistimeMainRect.top);
	pDC->LineTo(rHistimeMainRect.left,rHistimeMainRect.bottom);
	pDC->MoveTo(rHistimeMainRect.left,rHistimeMainRect.bottom);
	pDC->LineTo(rHistimeMainRect.right,rHistimeMainRect.bottom);
	pDC->SelectObject(pOldPen);

	/************************************************************************/
	/* 显示时间                                                                     */
	/************************************************************************/
	strTemp.Format(_T("         泄漏检测"));
	strTemp=tmStartTemp.Format("%Y-%m-%d 00:00:00") + "----" + tmEndTemp.Format("%Y-%m-%d %H:%M:%S")+strTemp;
	sizeTemp=pDC->GetTextExtent(strTemp);
	COLORREF crColor=pDC->SetTextColor(BLACK_COLOR);
	pDC->TextOut(rHistimeMainRect.left+(rHistimeMainRect.Width()-sizeTemp.cx)/2,rRect.top +sizeTemp.cy,strTemp);


	/************************************************************************/
	/* 画Y坐标大刻度                                                                     */
	/************************************************************************/
	CPen coorPen,*oldCoorPen;
	coorPen.CreatePen(PS_SOLID,1,BLACK_COLOR);
	oldCoorPen = pDC->SelectObject(&coorPen);
	for(int i=0;i<11;i++)
	{
		strTemp.Format(_T("%d "),i*10);
		strTemp += _T("%");
		sizeTemp=pDC->GetTextExtent(strTemp);
		pDC->MoveTo(rHistimeMainRect.left,rHistimeMainRect.bottom-yScale*i);//
		pDC->LineTo(rHistimeMainRect.left-iBias,rHistimeMainRect.bottom-yScale*i);
		pDC->TextOut(rHistimeMainRect.left/*- iBias*/-sizeTemp.cx-5,rHistimeMainRect.bottom-i*yScale-sizeTemp.cy/2,strTemp);
	}

	/************************************************************************/
	/* 画Y坐标小刻度                                                                     */
	/************************************************************************/
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<5;j++)
		{		
			pDC->MoveTo(rHistimeMainRect.left,rHistimeMainRect.bottom-yScale*i-j*ySmallScale*2);
			pDC->LineTo(rHistimeMainRect.left-5,rHistimeMainRect.bottom-yScale*i-j*ySmallScale*2);
		}
	}

	/************************************************************************/
	/* 画X坐标大刻度                                                                     */
	/************************************************************************/
//	CString strTimeCoor;
	int hour = 0;
	strTemp.Format(_T("%02d"),hour);
	int nCount = 0;
	int nCountDay = 0;

	for (int i = 0; i <= 24*nDayNum; i++)
	{

		if (nCount>=24)
			nCount = 0;

		pDC->MoveTo(rHistimeMainRect.left + xScale*i,rHistimeMainRect.bottom);//
		pDC->LineTo(rHistimeMainRect.left + xScale*i,rHistimeMainRect.bottom+iBias);

		if (nCount == 0)
		{
			strTemp.Format(_T("%02d"),nCount);
			sizeTemp=pDC->GetTextExtent(strTemp);
			pDC->TextOut(rHistimeMainRect.left + xScale*i-sizeTemp.cx/2,rHistimeMainRect.bottom+iBias,strTemp);//display hour

			strTemp.Format(_T("(%02d-%02d)"),tmStartTemp.GetMonth(),tmStartTemp.GetDay()+nCountDay);
			sizeTemp=pDC->GetTextExtent(strTemp);
			pDC->TextOut(rHistimeMainRect.left + xScale*i-sizeTemp.cx/2,rHistimeMainRect.bottom+2*iBias+2,strTemp);//display date
			nCountDay ++;
		}
		if (!(nCount%4))
		{
			strTemp.Format(_T("%02d"),nCount);
			sizeTemp=pDC->GetTextExtent(strTemp);
			pDC->TextOut(rHistimeMainRect.left + xScale*i-sizeTemp.cx/2,rHistimeMainRect.bottom+iBias,strTemp);//display hour
		}

		nCount++;
	}
	pDC->SelectObject(oldCoorPen);
	pDC->SetTextColor(crColor);

	/************************************************************************/
	/* 画虚线网络                                                                     */
	/************************************************************************/
	CPen dotPen ,*pOldDotPen;
	dotPen.CreatePen(PS_DOT,1,BLACK_COLOR);
	pOldDotPen = pDC->SelectObject(&dotPen);
	UINT i = 0;
	for(i=0;i<11;i++)
	{
		pDC->MoveTo(rHistimeMainRect.left,rHistimeMainRect.bottom-yScale*i);
		pDC->LineTo(rHistimeMainRect.right,rHistimeMainRect.bottom-yScale*i);
	}

	for (i = 0; i < 24*nDayNum; i++)
	{
		pDC->MoveTo(rHistimeMainRect.left + xScale*i,rHistimeMainRect.top);//
		pDC->LineTo(rHistimeMainRect.left + xScale*i,rHistimeMainRect.bottom);
	}
	pDC->MoveTo(rHistimeMainRect.right,rHistimeMainRect.top);
	pDC->LineTo(rHistimeMainRect.right,rHistimeMainRect.bottom);

	pDC->SetTextColor(crColor);
	pDC->SelectObject(pOldDotPen);
}
