// VavleBmp.cpp : implementation file
//

#include "StdAfx.h"
#include "AcousticTherm.h"
#include "VavleBmp.h"
#include "commonFile.h"
#include "MainFrm.h"
#include "Led.h"

// CVavleBmp

IMPLEMENT_DYNAMIC(CVavleBmp, CStatic)


CVavleBmp::CVavleBmp()
: m_point(NULL)
{

	m_point = new POINT[10];
	memset(m_point, 0, 10 * sizeof(POINT));
	lineDis = 80 * XCOFF;
	m_pOldFont = NULL;
	m_newFont.CreatePointFont(120, _T("宋体"));

}

CVavleBmp::~CVavleBmp()
{
	delete[] m_point;

}


BEGIN_MESSAGE_MAP(CVavleBmp, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CVavleBmp message handlers

void CVavleBmp::UpdateState(WPARAM wparam, LPARAM lparam)
{

	CRect rClient;
	GetClientRect(rClient);


	InvalidateRect(rClient, 0); //使窗口无效
	UpdateWindow();           //更新窗口
}


void CVavleBmp::OnPaint()
{
	CDC *pDC = GetDC();
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CRect rClientRect;
	GetClientRect(rClientRect);


	if (m_Bmp.GetSafeHandle() == NULL)
		m_Bmp.CreateCompatibleBitmap(pDC, rClientRect.Width(), rClientRect.Height());

	CBitmap *pOldBmp;
	pOldBmp = memDC.SelectObject(&m_Bmp);

	CBrush backBrush(SYSTEM_COLOR);
	memDC.FillRect(rClientRect, &backBrush);  //填充显示区域背景色

	int nBkMode = memDC.SetBkMode(TRANSPARENT);


	/*****************************显示内容***********************************/
	DisCommState(&memDC);
	DrawFrame(&memDC, rClientRect);
	DrawSounder(&memDC, m_point);
	DrawSensor(&memDC, m_point);
	DrawVavle(&memDC, rClientRect);
	/************************************************************************/

	pDC->BitBlt(0, 0, rClientRect.Width(), rClientRect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SetBkMode(nBkMode);
	memDC.SelectObject(pOldBmp);
	ReleaseDC(pDC);

}

void CVavleBmp::DisCommState(CDC* dc)
{
	CMainFrame *pWnd = (CMainFrame *)AfxGetMainWnd();
	if (!pWnd)
		return;

	LONG disErrorPointX = 0;
	LONG disErrorPointY = 0;
	disErrorPointX = m_point[2].x + (m_point[7].x - m_point[2].x) / 2 - 50 * XCOFF;
	disErrorPointY = m_point[2].y + 100 * YCOFF;
	m_pOldFont = dc->SelectObject(&m_newFont);
	dc->SetBkMode(TRANSPARENT);
	dc->SetTextColor(RGB(255, 0, 0));

	if (pWnd->GetOneArmState(0) == TRUE)
	{
		dc->TextOutA(disErrorPointX * XCOFF, disErrorPointY * YCOFF, _T("模块1通信中断"));
	}
	else
	{
		dc->TextOutA(disErrorPointX * XCOFF, disErrorPointY * YCOFF, _T("              "));
	}

	if (pWnd->GetOneArmState(1) == TRUE)
	{
		dc->TextOutA(disErrorPointX * XCOFF, disErrorPointY * YCOFF + 25 * YCOFF, _T("模块2通信中断"));
	}
	else
		dc->TextOutA(disErrorPointX * XCOFF, disErrorPointY * YCOFF + 25 * YCOFF, _T("              "));

	if (pWnd->GetOneArmState(2) == TRUE)
	{
		dc->TextOutA(disErrorPointX * XCOFF, disErrorPointY * YCOFF + 50 * YCOFF, _T("模块3通信中断"));
	}
	else
		dc->TextOutA(disErrorPointX * XCOFF, disErrorPointY * YCOFF + 50 * YCOFF, _T("              "));

	if (pWnd->GetOneArmState(3) == TRUE)
	{
		dc->TextOutA(disErrorPointX * XCOFF, disErrorPointY * YCOFF + 75 * YCOFF, _T("模块4通信中断"));
	}
	else
		dc->TextOutA(disErrorPointX * XCOFF, disErrorPointY * YCOFF + 75 * YCOFF, _T("              "));

	if (pWnd->GetOneArmState(4) == TRUE)
	{
		dc->TextOutA(disErrorPointX * XCOFF, disErrorPointY * YCOFF + 100 * YCOFF, _T("模块5通信中断"));
	}
	else
		dc->TextOutA(disErrorPointX * XCOFF, disErrorPointY * YCOFF + 100 * YCOFF, _T("              "));

	if (pWnd->GetOneArmStateB(0) == TRUE)
	{
		dc->TextOutA(disErrorPointX * XCOFF, disErrorPointY * YCOFF + 125 * YCOFF, _T("模块6通信中断"));
	}
	else
		dc->TextOutA(disErrorPointX * XCOFF, disErrorPointY * YCOFF + 125 * YCOFF, _T("              "));
	if (pWnd->GetOneArmStateB(1) == TRUE)
	{
		dc->TextOutA(disErrorPointX * XCOFF, disErrorPointY * YCOFF + 150 * YCOFF, _T("模块7通信中断"));
	}
	else
		dc->TextOutA(disErrorPointX * XCOFF, disErrorPointY * YCOFF + 150 * YCOFF, _T("              "));

	if (pWnd->GetOneArmStateB(2) == TRUE)
	{
		dc->TextOutA(disErrorPointX * XCOFF, disErrorPointY * YCOFF + 175 * YCOFF, _T("模块8通信中断"));
	}
	else
		dc->TextOutA(disErrorPointX * XCOFF, disErrorPointY * YCOFF + 175 * YCOFF, _T("              "));
	if (pWnd->GetOneArmStateB(3) == TRUE)
	{
		dc->TextOutA(disErrorPointX * XCOFF, disErrorPointY * YCOFF + 200 * YCOFF, _T("模块9通信中断"));
	}
	else
		dc->TextOutA(disErrorPointX * XCOFF, disErrorPointY * YCOFF + 200 * YCOFF, _T("              "));
	if (pWnd->GetOneArmStateB(4) == TRUE)
	{
		dc->TextOutA(disErrorPointX * XCOFF, disErrorPointY * YCOFF + 225 * YCOFF, _T("模块10通信中断"));
	}
	else
		dc->TextOutA(disErrorPointX * XCOFF, disErrorPointY * YCOFF + 225 * YCOFF, _T("              "));

	dc->SelectObject(m_pOldFont);
}

void CVavleBmp::DrawFrame(CDC* pDC, CRect rRect)
{
	

	UINT OFFSET = 50;
	UINT iBias = 10;//斜线

	CRect rHistimeMainRect, tempRect;
	tempRect.left = rRect.left + 4 * OFFSET*XCOFF;
	tempRect.top = rRect.top + 2 * OFFSET*YCOFF;
	tempRect.right = rRect.right - 2 * OFFSET*XCOFF;
	tempRect.bottom = rRect.bottom - 2 * OFFSET*XCOFF;

	//画坐标
	UINT iRealHight = 0;
	UINT iRealWidth = 0;

	iRealWidth = tempRect.Width() / 4;//
	iRealHight = tempRect.Height() / 4; //

	rHistimeMainRect.left = tempRect.left;//计算坐标x系的起点
	rHistimeMainRect.right = rHistimeMainRect.left + 4 * iRealWidth;//计算坐标X系的终点
	rHistimeMainRect.top = tempRect.top; //坐标Y系的终点
	rHistimeMainRect.bottom = rHistimeMainRect.top + 4 * iRealHight;//坐标Y系的起点
	

	CPen framePen, *pOldPen;
	framePen.CreatePen(PS_SOLID, 10, WHITE_COLOR);
	pOldPen = pDC->SelectObject(&framePen);


	pDC->MoveTo(rHistimeMainRect.left, rHistimeMainRect.top);
	pDC->LineTo(rHistimeMainRect.right, rHistimeMainRect.top);
	pDC->LineTo(rHistimeMainRect.right, rHistimeMainRect.bottom);
	pDC->LineTo(rHistimeMainRect.left, rHistimeMainRect.bottom);
	pDC->LineTo(rHistimeMainRect.left, rHistimeMainRect.top);

	pDC->MoveTo(rHistimeMainRect.left, rHistimeMainRect.top + 2 * iRealHight - (2 * OFFSET + 10)*YCOFF);
	pDC->LineTo(rHistimeMainRect.left - (2 * OFFSET + iBias) * YCOFF, rHistimeMainRect.top + 2 * iRealHight - (2 * OFFSET + iBias)*YCOFF);





	pDC->SelectObject(pOldPen);

	GetPoint(m_point, rHistimeMainRect);

}



void CVavleBmp::GetPoint(POINT* pPoint, CRect rRect)
{
	UINT iRealHight = 0;
	UINT iRealWidth = 0;

	iRealWidth = rRect.Width() / 8;//
	iRealHight = rRect.Height() / 2; //

	pPoint[1].x = rRect.left + iRealWidth;
	pPoint[1].y = rRect.top;
	pPoint[2].x = rRect.left + 3 * iRealWidth;
	pPoint[2].y = rRect.top;
	pPoint[7].x = rRect.left + 5 * iRealWidth;
	pPoint[7].y = rRect.top;
	pPoint[6].x = rRect.left + 7 * iRealWidth;
	pPoint[6].y = rRect.top;

	pPoint[4].x = rRect.left + iRealWidth;
	pPoint[4].y = rRect.bottom;
	pPoint[3].x = rRect.left + 3 * iRealWidth;
	pPoint[3].y = rRect.bottom;
	pPoint[8].x = rRect.left + 5 * iRealWidth;
	pPoint[8].y = rRect.bottom;
	pPoint[9].x = rRect.left + 7 * iRealWidth;
	pPoint[9].y = rRect.bottom;

	pPoint[0].x = rRect.left;
	pPoint[0].y = rRect.top + iRealHight;
	pPoint[5].x = rRect.right;
	pPoint[5].y = rRect.top + iRealHight;

}

void CVavleBmp::DrawSounder(CDC* pDC, POINT* pPoint)
{
	CMainFrame *pWnd = (CMainFrame *)AfxGetMainWnd();
	if (!pWnd)
		return;
	

	UINT iRadius = 30;
	CPen pen(PS_SOLID, 1, GRAY_COLOR), *pOldPen;
	CBrush brush(GRAY_COLOR), brush1(RED_COLOR), brush2(GREEN_COLOR), *pOldBrush;  //0等待灰色，1发声红色，2发送数据绿色

	int iSounder = 0;
	for (iSounder = 0; iSounder < 10; iSounder++)
	{
		if (iSounder < 5)
		{
			switch (pWnd->GetOneSounderState(iSounder))
			{
			case 0:
				pOldBrush = pDC->SelectObject(&brush);
				break;
			case 1:
				pOldBrush = pDC->SelectObject(&brush1);
				break;
			case 2:
				pOldBrush = pDC->SelectObject(&brush2);
				break;
			default:
				pOldBrush = pDC->SelectObject(&brush);
				break;
			}
		}
		else
		{
			switch (pWnd->GetOneSounderStateB(iSounder-5))
			{
			case 0:
				pOldBrush = pDC->SelectObject(&brush);
				break;
			case 1:
				pOldBrush = pDC->SelectObject(&brush1);
				break;
			case 2:
				pOldBrush = pDC->SelectObject(&brush2);
				break;
			default:
				pOldBrush = pDC->SelectObject(&brush);
				break;
			}
		}
		

		pOldPen = pDC->SelectObject(&pen);
		COLORREF oldColor = pDC->SetTextColor(WHITE_COLOR);

		CRect rectTmp;
		CString strTmp;

		pDC->BeginPath();

		if (iSounder == 0)
		{
			pDC->MoveTo(m_point[iSounder].x - iRadius * XCOFF, m_point[iSounder].y);
			pDC->LineTo(m_point[iSounder].x + 2 * iRadius * XCOFF, m_point[iSounder].y - 3 * iRadius * YCOFF / 2);
			pDC->LineTo(m_point[iSounder].x + 2 * iRadius * XCOFF, m_point[iSounder].y + 3 * iRadius * YCOFF / 2);
			pDC->LineTo(m_point[iSounder].x - iRadius * XCOFF, m_point[iSounder].y);
		
		
		}
		if (iSounder == 5)
		{
			pDC->MoveTo(m_point[iSounder].x + iRadius * XCOFF, m_point[iSounder].y);
			pDC->LineTo(m_point[iSounder].x - 2 * iRadius * XCOFF, m_point[iSounder].y + 3 * iRadius * YCOFF / 2);
			pDC->LineTo(m_point[iSounder].x - 2 * iRadius * XCOFF, m_point[iSounder].y - 3 * iRadius * YCOFF / 2);
			pDC->LineTo(m_point[iSounder].x + iRadius * XCOFF, m_point[iSounder].y);
		}

		if (iSounder == 1 || iSounder == 2 || iSounder == 6 || iSounder == 7)
		{
			pDC->MoveTo(m_point[iSounder].x, m_point[iSounder].y - iRadius * YCOFF);
			pDC->LineTo(m_point[iSounder].x + 3 * iRadius * XCOFF / 2, m_point[iSounder].y + 2 * iRadius * YCOFF);
			pDC->LineTo(m_point[iSounder].x - 3 * iRadius * XCOFF / 2, m_point[iSounder].y + 2 * iRadius * YCOFF);
			pDC->LineTo(m_point[iSounder].x, m_point[iSounder].y - iRadius * YCOFF);
		}
		if (iSounder == 3 || iSounder == 4 || iSounder == 8 || iSounder == 9)
		{
			pDC->MoveTo(m_point[iSounder].x, m_point[iSounder].y + iRadius * YCOFF);
			pDC->LineTo(m_point[iSounder].x - 3 * iRadius * XCOFF / 2, m_point[iSounder].y - 2 * iRadius * YCOFF);
			pDC->LineTo(m_point[iSounder].x + 3 * iRadius * XCOFF / 2, m_point[iSounder].y - 2 * iRadius * YCOFF);
			pDC->LineTo(m_point[iSounder].x, m_point[iSounder].y + iRadius * YCOFF);
		}

		pDC->EndPath();
		pDC->FillPath();

		strTmp.Format(_T("SOU %d"), iSounder + 1);
		CSize sizeError = pDC->GetTextExtent(strTmp);
		if (iSounder == 0 )
		{
			pDC->TextOut(m_point[iSounder].x + iRadius * XCOFF - sizeError.cx / 2 - 5, m_point[iSounder].y - sizeError.cy / 2, strTmp);
		}
		if (iSounder == 5)
		{
			pDC->TextOut(m_point[iSounder].x - iRadius * XCOFF + 5 - sizeError.cx / 2, m_point[iSounder].y - sizeError.cy / 2, strTmp);
		}

		if (iSounder == 1 || iSounder == 2 || iSounder == 6 || iSounder == 7)
		{
			pDC->TextOut(m_point[iSounder].x - sizeError.cx / 2, m_point[iSounder].y - sizeError.cy / 2 + iRadius * YCOFF + 10, strTmp);
		}
		if (iSounder == 3 || iSounder == 4 || iSounder == 8 || iSounder == 9)
		{
			pDC->TextOut(m_point[iSounder].x - sizeError.cx / 2, m_point[iSounder].y - sizeError.cy / 2 - iRadius * YCOFF - 10, strTmp);
		}

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		pDC->SetTextColor(oldColor);

		UINT* lineAverageTemp = pWnd->GetLineAvrgTValue();
		UINT* lineAverageTempB = pWnd->GetLineAvrgTValueB();
	    CPen linePen, *pOldPen;
		linePen.CreatePen(PS_SOLID, 1, RED_COLOR);
		pOldPen = pDC->SelectObject(&linePen);
		
		//12
		pDC->MoveTo(m_point[0].x + 2 * iRadius * XCOFF, m_point[0].y);
		pDC->LineTo(m_point[1].x, m_point[1].y + 2 * iRadius * YCOFF);
		strTmp.Format(_T("[1,2] %d"), lineAverageTemp[0]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[0].x + 2 * iRadius * XCOFF + m_point[1].x) / 2 - sizeError.cx / 2 - 20 * XCOFF	, (m_point[0].y + m_point[1].y + 2 * iRadius * YCOFF) / 2 - sizeError.cy, strTmp);
		strTmp.Format(_T("[2,1] %d"), lineAverageTemp[8]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[0].x + 2 * iRadius * XCOFF + m_point[1].x) / 2 - sizeError.cx / 2 - 20 * XCOFF, (m_point[0].y + m_point[1].y + 2 * iRadius * YCOFF) / 2 + sizeError.cy, strTmp);
		
		//13
		pDC->MoveTo(m_point[0].x + 2 * iRadius * XCOFF, m_point[0].y);
		pDC->LineTo(m_point[2].x, m_point[2].y + 2 * iRadius * YCOFF);
		strTmp.Format(_T("[1,3] %d"), lineAverageTemp[1]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[0].x + 2 * iRadius * XCOFF + m_point[2].x) / 2 - sizeError.cx/2, (m_point[0].y + m_point[2].y + 2 * iRadius * YCOFF) / 2 - 3*sizeError.cy, strTmp);
		strTmp.Format(_T("[3,1] %d"), lineAverageTemp[9]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[0].x + 2 * iRadius * XCOFF + m_point[2].x) / 2 - sizeError.cx / 2, (m_point[0].y + m_point[2].y + 2 * iRadius * YCOFF) / 2 - sizeError.cy, strTmp);

		//15
		pDC->MoveTo(m_point[0].x + 2 * iRadius * XCOFF, m_point[0].y);
		pDC->LineTo(m_point[4].x  , m_point[4].y - 2 * iRadius * YCOFF);

		strTmp.Format(_T("[1,5] %d"), lineAverageTemp[2]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[0].x + 2 * iRadius * XCOFF + m_point[4].x) / 2 - sizeError.cx / 2 - 20 * XCOFF, (m_point[0].y + m_point[4].y - 2 * iRadius * YCOFF) / 2 - sizeError.cy, strTmp);
		strTmp.Format(_T("[5,1] %d"), lineAverageTemp[10]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[0].x + 2 * iRadius * XCOFF + m_point[4].x) / 2 - sizeError.cx / 2 - 20 * XCOFF, (m_point[0].y + m_point[4].y - 2 * iRadius * YCOFF) / 2 + sizeError.cy, strTmp);

		//14
		pDC->MoveTo(m_point[0].x + 2 * iRadius * XCOFF, m_point[0].y);
		pDC->LineTo(m_point[3].x, m_point[3].y - 2 * iRadius * YCOFF);

		strTmp.Format(_T("[1,4] %d"), lineAverageTemp[3]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[0].x + 2 * iRadius * XCOFF + m_point[3].x) / 2 - sizeError.cx/2, (m_point[0].y + m_point[3].y - 2 * iRadius * YCOFF) / 2 + sizeError.cy, strTmp);
		strTmp.Format(_T("[4,1] %d"), lineAverageTemp[11]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[0].x + 2 * iRadius * XCOFF + m_point[3].x) / 2 - sizeError.cx / 2, (m_point[0].y + m_point[3].y - 2 * iRadius * YCOFF) / 2 + 3 * sizeError.cy, strTmp);

		//25
		pDC->MoveTo(m_point[1].x, m_point[1].y + 2 * iRadius * YCOFF);
		pDC->LineTo(m_point[4].x, m_point[4].y - 2 * iRadius * YCOFF);

		strTmp.Format(_T("[2,5] %d"), lineAverageTemp[4]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[1].x + m_point[4].x) / 2 /*+ sizeError.cx*/, (m_point[1].y  + m_point[4].y ) / 2 - 2*sizeError.cy, strTmp);
		strTmp.Format(_T("[5,2] %d"), lineAverageTemp[12]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[1].x + m_point[4].x) / 2 /*+ sizeError.cx*/, (m_point[1].y  + m_point[4].y ) / 2 + sizeError.cy, strTmp);

		//24
		pDC->MoveTo(m_point[1].x, m_point[1].y + 2 * iRadius * YCOFF);
		pDC->LineTo(m_point[3].x, m_point[3].y - 2 * iRadius * YCOFF);

		strTmp.Format(_T("[2,4] %d"), lineAverageTemp[5]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[1].x + m_point[3].x) / 2 + sizeError.cx / 2 + 20 * XCOFF, (m_point[1].y  + m_point[3].y ) / 2 + 4 * sizeError.cy, strTmp);
		strTmp.Format(_T("[4,2] %d"), lineAverageTemp[13]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[1].x + m_point[3].x) / 2 + sizeError.cx / 2 + 20 * XCOFF, (m_point[1].y  + m_point[3].y ) / 2 + 6 * sizeError.cy, strTmp);

		//34
		pDC->MoveTo(m_point[2].x, m_point[2].y + 2 * iRadius * YCOFF);
		pDC->LineTo(m_point[3].x, m_point[3].y - 2 * iRadius * YCOFF);

		strTmp.Format(_T("[3,4] %d"), lineAverageTemp[6]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[2].x + m_point[3].x) / 2 - sizeError.cx/2, (m_point[2].y   + m_point[3].y  ) / 2 - sizeError.cy, strTmp);
		strTmp.Format(_T("[4,3] %d"), lineAverageTemp[14]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[2].x + m_point[3].x) / 2 - sizeError.cx/2, (m_point[2].y   + m_point[3].y  ) / 2 + sizeError.cy, strTmp);

		//35
		pDC->MoveTo(m_point[2].x, m_point[2].y + 2 * iRadius * YCOFF);
		pDC->LineTo(m_point[4].x, m_point[4].y - 2 * iRadius * YCOFF);

		strTmp.Format(_T("[3,5] %d"), lineAverageTemp[7]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[2].x + m_point[4].x) / 2 + sizeError.cx / 2 + 20 * XCOFF, (m_point[2].y   + m_point[4].y  ) / 2 - 6 * sizeError.cy, strTmp);
		strTmp.Format(_T("[5,3] %d"), lineAverageTemp[15]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[2].x + m_point[4].x) / 2 + sizeError.cx / 2 + 20 * XCOFF, (m_point[2].y   + m_point[4].y  ) / 2 - 4 * sizeError.cy, strTmp);

		////右半边
		//67
		pDC->MoveTo(m_point[5].x - 2 * iRadius * XCOFF, m_point[5].y);
		pDC->LineTo(m_point[6].x, m_point[6].y + 2 * iRadius * YCOFF);
		strTmp.Format(_T("[6,7] %d"), lineAverageTempB[0]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[5].x + 2 * iRadius * XCOFF + m_point[6].x) / 2 - sizeError.cx / 2 - 20 * XCOFF, (m_point[5].y + m_point[6].y + 2 * iRadius * YCOFF) / 2 - sizeError.cy, strTmp);
		strTmp.Format(_T("[7,6] %d"), lineAverageTempB[8]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[5].x + 2 * iRadius * XCOFF + m_point[6].x) / 2 - sizeError.cx / 2 - 20 * XCOFF, (m_point[5].y + m_point[6].y + 2 * iRadius * YCOFF) / 2 + sizeError.cy, strTmp);

		//68
		pDC->MoveTo(m_point[5].x - 2 * iRadius * XCOFF, m_point[5].y);
		pDC->LineTo(m_point[7].x, m_point[7].y + 2 * iRadius * YCOFF);
		strTmp.Format(_T("[6,8] %d"), lineAverageTempB[1]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[5].x + 2 * iRadius * XCOFF + m_point[7].x) / 2 - sizeError.cx * 3 / 2, (m_point[5].y + m_point[7].y + 2 * iRadius * YCOFF) / 2 - 3 * sizeError.cy, strTmp);
		strTmp.Format(_T("[8,6] %d"), lineAverageTempB[9]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[5].x + 2 * iRadius * XCOFF + m_point[7].x) / 2 - sizeError.cx * 3 / 2, (m_point[5].y + m_point[7].y + 2 * iRadius * YCOFF) / 2 - sizeError.cy, strTmp);

		//6-10
		pDC->MoveTo(m_point[5].x - 2 * iRadius * XCOFF, m_point[5].y);
		pDC->LineTo(m_point[9].x, m_point[9].y - 2 * iRadius * YCOFF);

		strTmp.Format(_T("[6,10] %d"), lineAverageTempB[2]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[5].x + 2 * iRadius * XCOFF + m_point[9].x) / 2 - sizeError.cx / 2 - 20 * XCOFF, (m_point[5].y + m_point[9].y - 2 * iRadius * YCOFF) / 2 - sizeError.cy, strTmp);
		strTmp.Format(_T("[10,6] %d"), lineAverageTempB[10]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[5].x + 2 * iRadius * XCOFF + m_point[9].x) / 2 - sizeError.cx / 2 - 20 * XCOFF, (m_point[5].y + m_point[9].y - 2 * iRadius * YCOFF) / 2 + sizeError.cy, strTmp);

		//69
		pDC->MoveTo(m_point[5].x - 2 * iRadius * XCOFF, m_point[5].y);
		pDC->LineTo(m_point[8].x, m_point[8].y - 2 * iRadius * YCOFF);

		strTmp.Format(_T("[6,9] %d"), lineAverageTempB[3]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[5].x + 2 * iRadius * XCOFF + m_point[8].x) / 2 - sizeError.cx*3/2 , (m_point[5].y + m_point[8].y - 2 * iRadius * YCOFF) / 2 + sizeError.cy, strTmp);
		strTmp.Format(_T("[9,6] %d"), lineAverageTempB[11]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[5].x + 2 * iRadius * XCOFF + m_point[8].x) / 2 - sizeError.cx * 3 / 2, (m_point[5].y + m_point[8].y - 2 * iRadius * YCOFF) / 2 + 3 * sizeError.cy, strTmp);

		//7-10
		pDC->MoveTo(m_point[6].x, m_point[6].y + 2 * iRadius * YCOFF);
		pDC->LineTo(m_point[9].x, m_point[9].y - 2 * iRadius * YCOFF);

		strTmp.Format(_T("[7,10] %d"), lineAverageTempB[4]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[6].x + m_point[9].x) / 2 - sizeError.cx, (m_point[6].y + m_point[9].y) / 2 -  sizeError.cy, strTmp);
		strTmp.Format(_T("[10,7] %d"), lineAverageTempB[12]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[6].x + m_point[9].x) / 2 - sizeError.cx, (m_point[6].y + m_point[9].y) / 2 + sizeError.cy, strTmp);

		//79
		pDC->MoveTo(m_point[6].x, m_point[6].y + 2 * iRadius * YCOFF);
		pDC->LineTo(m_point[8].x, m_point[8].y - 2 * iRadius * YCOFF);

		strTmp.Format(_T("[7,9] %d"), lineAverageTempB[5]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[6].x + m_point[8].x) / 2 - sizeError.cx + 20 * XCOFF, (m_point[6].y + m_point[8].y) / 2 + 3 * sizeError.cy, strTmp);
		strTmp.Format(_T("[9,7] %d"), lineAverageTempB[13]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[6].x + m_point[8].x) / 2 - sizeError.cx + 20 * XCOFF, (m_point[6].y + m_point[8].y) / 2 + 5 * sizeError.cy, strTmp);

		//89
		pDC->MoveTo(m_point[7].x, m_point[7].y + 2 * iRadius * YCOFF);
		pDC->LineTo(m_point[8].x, m_point[8].y - 2 * iRadius * YCOFF);

		strTmp.Format(_T("[8,9] %d"), lineAverageTempB[6]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[7].x + m_point[8].x) / 2 - sizeError.cx / 2, (m_point[7].y + m_point[8].y) / 2 - sizeError.cy, strTmp);
		strTmp.Format(_T("[9,8] %d"), lineAverageTempB[14]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[7].x + m_point[8].x) / 2 - sizeError.cx / 2, (m_point[7].y + m_point[8].y) / 2 + sizeError.cy, strTmp);

		//8-10
		pDC->MoveTo(m_point[7].x, m_point[7].y + 2 * iRadius * YCOFF);
		pDC->LineTo(m_point[9].x, m_point[9].y - 2 * iRadius * YCOFF);

		strTmp.Format(_T("[8,10] %d"), lineAverageTempB[7]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[7].x + m_point[9].x) / 2 - sizeError.cx  + 20 * XCOFF, (m_point[7].y + m_point[9].y) / 2 - 5 * sizeError.cy, strTmp);
		strTmp.Format(_T("[10,8] %d"), lineAverageTempB[15]);
		sizeError = pDC->GetTextExtent(strTmp);
		pDC->TextOut((m_point[7].x + m_point[9].x) / 2 - sizeError.cx + 20 * XCOFF, (m_point[7].y + m_point[9].y) / 2 - 3 * sizeError.cy, strTmp);


		pDC->SelectObject(pOldPen);





	}

}

void CVavleBmp::DrawSensor(CDC* pDC, POINT* pPoint)
{
	CMainFrame *pWnd = (CMainFrame *)AfxGetMainWnd();
	if (!pWnd)
		return;

	UINT iRadius = 30;//圆半径

	CPen pen(PS_SOLID, 1, GREEN_COLOR), *pOldPen;
	CBrush brush(GREEN_COLOR), brush1(GRAY_COLOR), *pOldBrush;
	int iSensor = 0;
	for (iSensor = 0; iSensor < 10; iSensor++)
	{
		if (iSensor < 5)
		{
			if (!pWnd->GetOneSersorState(iSensor))
			{
				pOldBrush = pDC->SelectObject(&brush);
			}
			if (pWnd->GetOneSersorState(iSensor))
			{
				pOldBrush = pDC->SelectObject(&brush1);
			}
		}
		else
		{
			if (!pWnd->GetOneSersorState(iSensor-5))
			{
				pOldBrush = pDC->SelectObject(&brush);
			}
			if (pWnd->GetOneSersorState(iSensor-5))
			{
				pOldBrush = pDC->SelectObject(&brush1);
			}
		}
		
		pOldPen = pDC->SelectObject(&pen);
		COLORREF oldColor = pDC->SetTextColor(WHITE_COLOR);

		CRect rectTmp;
		CString strTmp;

		strTmp.Format(_T("SEN %d"), iSensor + 1);
		if (iSensor == 0)
		{
			rectTmp.left = m_point[iSensor].x - 3 * iRadius * XCOFF;
			rectTmp.top = m_point[iSensor].y - iRadius * YCOFF;
			rectTmp.right = m_point[iSensor].x - iRadius * XCOFF;
			rectTmp.bottom = m_point[iSensor].y + iRadius * YCOFF;
		}
		if (iSensor == 5)
		{
			rectTmp.left = m_point[iSensor].x + iRadius * XCOFF;
			rectTmp.top = m_point[iSensor].y - iRadius * YCOFF;
			rectTmp.right = m_point[iSensor].x + 3 * iRadius * XCOFF;
			rectTmp.bottom = m_point[iSensor].y + iRadius * YCOFF;
		}

		if (iSensor == 1 || iSensor == 2 || iSensor == 6 || iSensor == 7)
		{
			rectTmp.left = m_point[iSensor].x - iRadius * XCOFF;
			rectTmp.top = m_point[iSensor].y - 3 * iRadius * YCOFF;
			rectTmp.right = m_point[iSensor].x + iRadius * XCOFF;
			rectTmp.bottom = m_point[iSensor].y - iRadius * YCOFF;
		}
		if (iSensor == 3 || iSensor == 4 || iSensor == 8 || iSensor == 9)
		{
			rectTmp.left = m_point[iSensor].x - iRadius * XCOFF;
			rectTmp.top = m_point[iSensor].y + iRadius * YCOFF;
			rectTmp.right = m_point[iSensor].x + iRadius * XCOFF;
			rectTmp.bottom = m_point[iSensor].y + 3 * iRadius * YCOFF;
		}

		pDC->Rectangle(rectTmp);
		CSize sizeError = pDC->GetTextExtent(strTmp);
		if (iSensor == 0 )
		{
			pDC->TextOut(m_point[iSensor].x - 2 * iRadius * XCOFF - sizeError.cx / 2, m_point[iSensor].y - sizeError.cy / 2, strTmp);
		}
		if (iSensor==5)
		{
			pDC->TextOut(m_point[iSensor].x + 2 * iRadius * XCOFF - sizeError.cx / 2, m_point[iSensor].y - sizeError.cy / 2, strTmp);
		}
		if (iSensor == 1 || iSensor == 2 || iSensor == 6 || iSensor == 7)
		{
			pDC->TextOut(m_point[iSensor].x - sizeError.cx / 2, m_point[iSensor].y - 2 * iRadius * YCOFF - sizeError.cy / 2, strTmp);
		}
		if (iSensor == 3 || iSensor == 4 || iSensor == 8 || iSensor == 9)
		{
			pDC->TextOut(m_point[iSensor].x - sizeError.cx / 2, m_point[iSensor].y + 2 * iRadius * YCOFF - sizeError.cy / 2, strTmp);
		}

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		pDC->SetTextColor(oldColor);
	}

}

void CVavleBmp::DrawVavle(CDC* pDC, CRect rRect)
{
	CMainFrame *pWnd = (CMainFrame *)AfxGetMainWnd();
	if (!pWnd)
		return;

	UINT OFFSET1 = 50;
	UINT OFFSET = 15;
	UINT iRealHight = 0;
	UINT iRealWidth = 0;

	iRealWidth = rRect.Width() / 4;//
	iRealHight = rRect.Height() / 4; //

	CPen pen(PS_SOLID, 1, GREEN_COLOR), *pOldPen;
	CBrush brush(GREEN_COLOR), brush1(RED_COLOR), *pOldBrush;
	//pOldBrush = pDC->SelectObject(&brush);
	if (pWnd->GetGasControlState())
	{
		pOldBrush = pDC->SelectObject(&brush); //气阀开 GREEN
	}
	else
	{
		pOldBrush = pDC->SelectObject(&brush1);  //气阀关闭 RED
	}

	pOldPen = pDC->SelectObject(&pen);
	COLORREF oldColor = pDC->SetTextColor(WHITE_COLOR);

	CRect rectTmp;
	CString strTmp;

	pDC->BeginPath();
	pDC->MoveTo(rRect.left + lineDis + OFFSET * XCOFF, rRect.top + 2 * iRealHight - 2 * OFFSET1 * YCOFF - 2 * OFFSET * YCOFF);
	pDC->LineTo(rRect.left + lineDis - 5 * OFFSET * XCOFF, rRect.top + 2 * iRealHight - 2 * OFFSET1 * YCOFF - 2 * OFFSET * YCOFF);
	pDC->LineTo(rRect.left + lineDis - 5 * OFFSET * XCOFF, rRect.top + 2 * iRealHight - 6 * OFFSET * YCOFF);
	pDC->LineTo(rRect.left + lineDis + OFFSET * XCOFF, rRect.top + 2 * iRealHight - 6 * OFFSET * YCOFF);
	pDC->LineTo(rRect.left + lineDis + OFFSET * XCOFF, rRect.top + 2 * iRealHight - 2 * OFFSET1 * YCOFF - 2 * OFFSET * YCOFF);

	pDC->MoveTo(rRect.left + lineDis - 2 * OFFSET * XCOFF, rRect.top + 2 * iRealHight - 2 * OFFSET1 * YCOFF - 2 * OFFSET * YCOFF);
	pDC->LineTo(rRect.left + lineDis, rRect.top + 2 * iRealHight - OFFSET1 * YCOFF - 8*OFFSET * YCOFF);
	pDC->LineTo(rRect.left + lineDis - 4 * OFFSET * XCOFF, rRect.top + 2 * iRealHight - OFFSET1 * YCOFF - 8 * OFFSET * YCOFF);
	pDC->LineTo(rRect.left + lineDis - 2 * OFFSET * XCOFF, rRect.top + 2 * iRealHight - 2 * OFFSET1 * YCOFF - 2 * OFFSET * YCOFF);

	pDC->MoveTo(rRect.left + lineDis - 2 * OFFSET * XCOFF, rRect.top + 2 * iRealHight - 6 * OFFSET * YCOFF);
	pDC->LineTo(rRect.left + lineDis - 4 * OFFSET * XCOFF, rRect.top + 2 * iRealHight -  OFFSET1 * YCOFF);
	pDC->LineTo(rRect.left + lineDis, rRect.top + 2 * iRealHight -  OFFSET1 * YCOFF);
	pDC->LineTo(rRect.left + lineDis - 2 * OFFSET * XCOFF, rRect.top + 2 * iRealHight - 6 * OFFSET * YCOFF);
	pDC->EndPath();
	pDC->FillPath();

	strTmp.Format(_T("吹扫"));
	pDC->Rectangle(rectTmp);
	CSize sizeError = pDC->GetTextExtent(strTmp);
	pDC->TextOut(rRect.left + lineDis - 2 * OFFSET * XCOFF - sizeError.cx / 2, rRect.top + 2 * iRealHight - (OFFSET1 + 4 * OFFSET)* YCOFF - sizeError.cy / 2, strTmp);


	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	pDC->SetTextColor(oldColor);
}

//void CVavleBmp::DrawPath(CDC* pDC, POINT pPointStart, POINT pPointEnd, UINT m, UINT n, UINT tempValue)
//{
//	
//	CString strTemp;
//	CPen linePen(PS_SOLID,2,RED_COLOR), *pOldPen;
//	pOldPen = pDC->SelectObject(&linePen);
//	COLORREF oldColor = pDC->SetTextColor(BLUE_COLOR);
//	
//	pDC->MoveTo(pPointStart.x,pPointStart.y);
//	pDC->LineTo(pPointEnd.x, pPointEnd.y);
//	
//	strTemp.Format(_T("(%d,%d) %d"), m, n, tempValue);
//	pDC->TextOut((pPointStart.x + pPointEnd.x) / 2, (pPointStart.y + pPointEnd.y) / 2,strTemp);
//
//	pDC->SelectObject(pOldPen);
//	pDC->SetTextColor(oldColor);
//
//
//
//
//}