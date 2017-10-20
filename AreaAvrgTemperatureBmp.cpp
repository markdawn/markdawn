// AreaAvrgTemperatureBmp.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "AreaAvrgTemperatureBmp.h"

#include "MainFrm.h"

#pragma warning(disable:4018)

// CAreaAvrgTemperatureBmp

IMPLEMENT_DYNAMIC(CAreaAvrgTemperatureBmp, CStatic)

CAreaAvrgTemperatureBmp::CAreaAvrgTemperatureBmp()
{
	m_AreaPoint = new AreaPoint[AREA_BLOCK_NUM];
	m_AreaColor = new COLORREF[AREA_BLOCK_NUM];
	m_iPerAreaT = new UINT[AREA_BLOCK_NUM];

	memset(m_AreaPoint, 0, AREA_BLOCK_NUM*sizeof(AreaPoint));
	memset(m_AreaColor, 0, AREA_BLOCK_NUM*sizeof(COLORREF));
	memset(m_iPerAreaT, 0, AREA_BLOCK_NUM*sizeof(UINT));
	for (int i = 0; i < AREA_BLOCK_NUM; i++)
	{
		bAreaTOverLimitFlag[i] = FALSE;
		m_uFlashTimerCounter[i] = 0;
	}
}

CAreaAvrgTemperatureBmp::~CAreaAvrgTemperatureBmp()
{
	if (m_AreaPoint)
	{
		delete[] m_AreaPoint;
		m_AreaPoint = NULL;
	}

	if (m_AreaColor)
	{
		delete[] m_AreaColor;
		m_AreaColor = NULL;
	}

	if (m_iPerAreaT)
	{
		delete[] m_iPerAreaT;
		m_iPerAreaT = NULL;
	}
}


BEGIN_MESSAGE_MAP(CAreaAvrgTemperatureBmp, CStatic)
	ON_WM_PAINT()
	//	ON_WM_TIMER()
END_MESSAGE_MAP()



// CAreaAvrgTemperatureBmp message handlers




void CAreaAvrgTemperatureBmp::DisplayAreaAvrgT(WPARAM wparam, LPARAM lparam)
{
	CDC *pDC = GetDC();
	Draw(pDC, wparam, lparam);
	ReleaseDC(pDC);
}


void CAreaAvrgTemperatureBmp::Draw(CDC* pDc, WPARAM wparam, LPARAM lparam)
{
	CRect rClientRect;
	GetClientRect(rClientRect);

	CDC memDC;
	memDC.CreateCompatibleDC(pDc);

	if (m_Bmp.GetSafeHandle() == NULL)
		m_Bmp.CreateCompatibleBitmap(pDc, rClientRect.Width(), rClientRect.Height());

	CBitmap *pOldBmp;
	pOldBmp = memDC.SelectObject(&m_Bmp);

	CBrush backBrush(SYSTEM_COLOR);
	memDC.FillRect(rClientRect, &backBrush);  //填充显示区域背景色

	int nBkMode = memDC.SetBkMode(TRANSPARENT);

	/*****************************显示内容***********************************/
	DrawFrame(&memDC, rClientRect);
	PreDrawAreaAvrgT(rClientRect, wparam, lparam);
	DrawAreaAvrgT(&memDC, rClientRect);
	DrawAreaTText(&memDC, m_iPerAreaT);
	/************************************************************************/

	pDc->BitBlt(0, 0, rClientRect.Width(), rClientRect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SetBkMode(nBkMode);     
	memDC.SelectObject(pOldBmp);
}

void CAreaAvrgTemperatureBmp::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	CDC memDC;
	CBitmap *pOldBmp;
	CRect rRect;

	memDC.CreateCompatibleDC(&dc);

	if (m_Bmp.GetSafeHandle())
	{
		dc.GetClipBox(rRect);
		pOldBmp = memDC.SelectObject(&m_Bmp);
		dc.BitBlt(rRect.left, rRect.top, rRect.Width(), rRect.Height(), &memDC, rRect.left, rRect.top, SRCCOPY);
		memDC.SelectObject(pOldBmp);
	}
}

void CAreaAvrgTemperatureBmp::DrawAreaAvrgT(CDC* pDc, CRect rRect)
{
	for (int i = 0; i < AREA_BLOCK_NUM; i++)
	{
		DrawOneAreaT(pDc, m_AreaPoint[i], m_AreaColor[i]);
		/*//if (i == 0 || i == 1 || i == 4 || i == 5 || i == 8 || i == 9 || i == 12 || i == 13)
		if (i < 5)
		{
		DrawOneAreaT( pDc, m_AreaPoint[i], m_AreaColor[i]);
		}
		else
		{
		DrawOneAreaT(pDc, m_AreaPoint[i], m_AreaColor[i]);
		}*/

	}
}

void CAreaAvrgTemperatureBmp::DrawFrame(CDC* pDc, CRect rRect)
{

	CPen framePen, *pOldPen;
	framePen.CreatePen(PS_SOLID, 1, WHITE_COLOR);
	pOldPen = pDc->SelectObject(&framePen);
	pDc->MoveTo(rRect.left, rRect.top);
	pDc->LineTo(rRect.right, rRect.top);
	pDc->LineTo(rRect.right, rRect.bottom - 1 * YCOFF);
	pDc->LineTo(rRect.left, rRect.bottom - 1 * YCOFF);
	pDc->LineTo(rRect.left, rRect.top);
	pDc->SelectObject(pOldPen);
}

void CAreaAvrgTemperatureBmp::DrawOneAreaT(CDC* pDc, AreaPoint point, COLORREF color)
{
	CBrush areaBrush(color), *pOldAreaBrush;
	CPen  areaPen, *pOldAreaPen;
	areaPen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

	pOldAreaPen = pDc->SelectObject(&areaPen);
	pOldAreaBrush = pDc->SelectObject(&areaBrush);

	pDc->Rectangle(CRect(point.startPoint, point.endPoint));

	pDc->SelectObject(pOldAreaBrush);
	pDc->SelectObject(pOldAreaPen);
}


void CAreaAvrgTemperatureBmp::PreDrawAreaAvrgT(CRect rRect, WPARAM wparam, LPARAM lparam)
{
	AreaTParaSet areaTLimit[AREA_BLOCK_NUM];
	for (int i = 0; i < AREA_BLOCK_NUM; i++)
	{
		m_iPerAreaT[i] = ((UINT*)wparam)[i];
		areaTLimit[i].iLowTLimit = ((AreaTParaSet*)lparam)[i].iLowTLimit;
		areaTLimit[i].iHighTLimit = ((AreaTParaSet*)lparam)[i].iHighTLimit;

		//判断是否超温
		if (bAreaTOverLimitFlag[i] == FALSE)
		{
			if (m_iPerAreaT[i] < areaTLimit[i].iLowTLimit)
			{
				bAreaTOverLimitFlag[i] = TRUE;
				m_AreaColor[i] = BLUE_COLOR;
				m_uFlashTimerCounter[i] = 10;
			}
			if (m_iPerAreaT[i] > areaTLimit[i].iHighTLimit)
			{
				bAreaTOverLimitFlag[i] = TRUE;
				//m_AreaColor[i] = RED_COLOR;
				m_AreaColor[i] = WHITE_COLOR;
				m_uFlashTimerCounter[i] = 10;
			}
		}
	}

	for (int i = 0; i < AREA_BLOCK_NUM; i++)
	{
		if (bAreaTOverLimitFlag[i] == TRUE)
		{
			m_uFlashTimerCounter[i] = m_uFlashTimerCounter[i] - 1;
			if (m_uFlashTimerCounter[i] <= 0)
			{
				bAreaTOverLimitFlag[i] = FALSE;
			}
		}
	}

	GetAreaCoor(m_AreaPoint, rRect); //获取坐标
	GetAreaColor(m_AreaColor);//获取颜色


}


// 获得每个区域坐标
void CAreaAvrgTemperatureBmp::GetAreaCoor(AreaPoint* areaPoint, CRect rRect)
{
	CRect rClientRect(rRect);

	int nWidth = rClientRect.Width() / 16;
	int nHight1 = rClientRect.Height() / 2;
	int nHight2 = rClientRect.Height() / 3;
	UINT i = 0;
	UINT j = 0;

	areaPoint[0].startPoint.x = rClientRect.left;
	areaPoint[0].endPoint.x = areaPoint[0].startPoint.x + nWidth;

	areaPoint[0].startPoint.y = rClientRect.top;
	areaPoint[0].endPoint.y = rClientRect.bottom;


	areaPoint[6].startPoint.x = rClientRect.left + 15 * nWidth;
	areaPoint[6].endPoint.x = rClientRect.right;

	areaPoint[6].startPoint.y = rClientRect.top;
	areaPoint[6].endPoint.y = rClientRect.bottom;


	//i=1
	for (j = 0; j < 2; j++)
	{
		areaPoint[j + 1].startPoint.x = rClientRect.left + nWidth;
		areaPoint[j + 1].endPoint.x = areaPoint[1].startPoint.x + 3 * 1 * nWidth;

		areaPoint[j + 1].startPoint.y = rClientRect.top + j * nHight1;
		areaPoint[j + 1].endPoint.y = areaPoint[j + 1].startPoint.y + nHight1 /*+ 1 * YCOFF*/;

	}
	//i=2
	for (j = 0; j < 3; j++)
	{
		areaPoint[j + 3].startPoint.x = rClientRect.left + 4 * nWidth;
		areaPoint[j + 3].endPoint.x = areaPoint[0].startPoint.x + 4 * 2 * nWidth - 1 * XCOFF;

		areaPoint[j + 3].startPoint.y = rClientRect.top + j * nHight2;
		areaPoint[j + 3].endPoint.y = areaPoint[j + 3].startPoint.y + nHight2 + 1 * YCOFF;

	}
	//i=3
	for (j = 0; j < 3; j++)
	{
		areaPoint[j + 9].startPoint.x = rClientRect.left + 4 * 2 * nWidth + 1 * XCOFF;
		areaPoint[j + 9].endPoint.x = areaPoint[0].startPoint.x + 4 * 3 * nWidth;

		areaPoint[j + 9].startPoint.y = rClientRect.top + j * nHight2;
		areaPoint[j + 9].endPoint.y = areaPoint[j + 9].startPoint.y + nHight2 + 1 * YCOFF;

	}
	//i=4
	for (j = 0; j < 2; j++)
	{
		areaPoint[j + 7].startPoint.x = rClientRect.left + 4 * 3 * nWidth;
		areaPoint[j + 7].endPoint.x = areaPoint[j + 7].startPoint.x + 3 * nWidth;

		areaPoint[j + 7].startPoint.y = rClientRect.top + j * nHight1;
		areaPoint[j + 7].endPoint.y = areaPoint[j + 7].startPoint.y + nHight1 /*+ 1 * YCOFF*/;
	}
}


// 获取区域对应的温度颜色
void CAreaAvrgTemperatureBmp::GetAreaColor(COLORREF* color)
{
	UINT i = 0;
	BYTE rStart = GetRValue(START_COLOR);
	BYTE gStart = GetGValue(START_COLOR);
	BYTE bStart = GetBValue(START_COLOR);

	BYTE rEnd = GetRValue(END_COLOR);
	BYTE gEnd = GetGValue(END_COLOR);
	BYTE bEnd = GetBValue(END_COLOR);

	double rStep, gStep, bStep;
	rStep = (double)(rEnd - rStart) / (1600);
	gStep = (double)(gEnd - gStart) / (1600);
	bStep = (double)(bEnd - bStart) / (1600);

	BYTE r = 0, g = 0, b = 0;

	for (i = 0; i < AREA_BLOCK_NUM; i++)
	{
		if (bAreaTOverLimitFlag[i])
		{
			continue;
		}
		r = rStart + m_iPerAreaT[i] * rStep;
		g = gStart + m_iPerAreaT[i] * gStep;
		b = bStart + m_iPerAreaT[i] * bStep;

		color[i] = RGB(r, g, b);
	}
}


void CAreaAvrgTemperatureBmp::DrawAreaTText(CDC* pDc, UINT* pBuffer)
{
	UINT i = 0;
	UINT x = 0;
	UINT y = 0;

	CString strTemp;
	CSize sizeTemp;
	for (i = 0; i < AREA_BLOCK_NUM; i++)
	{

		x = m_AreaPoint[i].startPoint.x + (m_AreaPoint[i].endPoint.x - m_AreaPoint[i].startPoint.x) / 2;
		y = m_AreaPoint[i].startPoint.y + (m_AreaPoint[i].endPoint.y - m_AreaPoint[i].startPoint.y) / 2;
		//if (bAreaTOverLimitFlag[i])
		//{
		//	strTemp.Format(_T("%d\nOverLimitTemp"), pBuffer[i]);
		//}
		//else
		//{
			strTemp.Format(_T("%d"), pBuffer[i]);
		//}
		
		sizeTemp = pDc->GetTextExtent(strTemp);
		pDc->TextOut(x - sizeTemp.cx / 2, y + sizeTemp.cy / 2, strTemp);
	}

}


//void CAreaAvrgTemperatureBmp::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	CStatic::OnTimer(nIDEvent);
//}
