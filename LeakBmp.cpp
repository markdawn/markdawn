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



// ��ʾй©��Ϣ
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
	memDC.FillRect(rClientRect,&backBrush);  //�����ʾ���򱳾�ɫ

	int nBkMode=memDC.SetBkMode(TRANSPARENT);

	/*****************************��ʾ����***********************************/
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
	float LEAK_RANGE_Y  = 100.0f;//й©��ֵ
	float REAL_Y_LAGESCALE = 10.0f;//Yһ���̶ȶ�Ӧ10

	int iBias=10;//б��
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
	//������
	CPen greenPen,*pOldPen;
	UINT iRealHight = 0; 
	float iRealWidth=0;

	int  nDayNum = (wparam - lparam)/(REAL_X_LAGESCALE*REAL_X_SMALLSCALE*60);

	float xSmallScale = tempRect.Width()/(nDayNum*REAL_X_LAGESCALE*REAL_X_SMALLSCALE);//x����С�̶ȵĿ��:nDayNum�죬REAL_X_LAGESCALEСʱ��REAL_X_SMALLSCALE����
	float ySmallScale = tempRect.Height()/(LEAK_RANGE_Y); //y����С�̶ȵĿ��

	iRealWidth = REAL_X_SMALLSCALE*xSmallScale; //ÿСʱӳ��Ŀ��
	iRealHight = REAL_Y_LAGESCALE*ySmallScale;//ӳ��Ƶ�ʷ�ֵ

	rHistimeMainRect.left = tempRect.left;//��������xϵ�����
	rHistimeMainRect.right=tempRect.right;//��������Xϵ���յ�
	rHistimeMainRect.top = tempRect.top; //����Yϵ���յ�
	rHistimeMainRect.bottom = rHistimeMainRect.top + 10*iRealHight;//����Yϵ�����

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
	/* ��ʾʱ��                                                                     */
	/************************************************************************/
	strTemp.Format(_T("         й©���"));
	strTemp=tmStartTemp.Format("%Y-%m-%d 00:00:00") + "----" + tmEndTemp.Format("%Y-%m-%d %H:%M:%S")+strTemp;
	sizeTemp=pDC->GetTextExtent(strTemp);
	COLORREF crColor=pDC->SetTextColor(BLACK_COLOR);
	pDC->TextOut(rHistimeMainRect.left+(rHistimeMainRect.Width()-sizeTemp.cx)/2,rRect.top +sizeTemp.cy,strTemp);


	/************************************************************************/
	/* ��Y�����̶�                                                                     */
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
	/* ��Y����С�̶�                                                                     */
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
	/* ��X�����̶�                                                                     */
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
	/* ����������                                                                     */
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
