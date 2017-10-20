// SignalTimeBmp.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "SignalTimeBmp.h"
#include "commonFile.h"
#include "MainFrm.h"


// CSignalTimeBmp

IMPLEMENT_DYNAMIC(CSignalTimeBmp, CStatic)

CSignalTimeBmp::CSignalTimeBmp()
{
	int j = 0;
	int k = 0;
	m_pfSignalData = NULL;
	/*
		m_pfSignalData= new int**[ARM_NUM];
		for (j=0;j<ARM_NUM;j++)
		{
		m_pfSignalData[j]=new int*[SOUNDDEV_NUM];
		for (k= 0;k<SOUNDDEV_NUM;k++)
		{
		m_pfSignalData[j][k]=new int[PERMODULE_DATA_LENTH];
		}
		}
		int m = 0;
		for (j=0;j<ARM_NUM;j++)
		{
		for (k= 0;k<SOUNDDEV_NUM;k++)
		{
		for(m = 0;m<PERMODULE_DATA_LENTH;m++)
		{
		m_pfSignalData[j][k][m]=0.0f;
		}
		}
		}
		*/
}

CSignalTimeBmp::~CSignalTimeBmp()
{
	/*
		int j = 0,k = 0;
		for (j=0;j<ARM_NUM;j++)
		{
		for (k= 0;k<SOUNDDEV_NUM;k++)
		{
		delete m_pfSignalData[j][k];
		}
		delete m_pfSignalData[j];
		}
		delete m_pfSignalData;*/
}


BEGIN_MESSAGE_MAP(CSignalTimeBmp, CStatic)
END_MESSAGE_MAP()



// ��ʾһ��·�����շ�ʱ���ź�
void CSignalTimeBmp::DisplayTime(WPARAM wparam, LPARAM lparam, UINT iSendArm, UINT iRecArm, BOOL bRec, BOOL bSend)
{
	CDC *pDC = GetDC();
	Draw(pDC, wparam, lparam, iSendArm, iRecArm, bRec, bSend);
	ReleaseDC(pDC);
}


void CSignalTimeBmp::Draw(CDC* pDC, WPARAM wparam, LPARAM lparam, UINT iSendArm, UINT iRecArm, BOOL bRec, BOOL bSend)
{
	CRect rClientRect;
	GetClientRect(rClientRect);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	if (m_Bmp.GetSafeHandle() == NULL)
		m_Bmp.CreateCompatibleBitmap(pDC, rClientRect.Width(), rClientRect.Height());

	CBitmap *pOldBmp;
	pOldBmp = memDC.SelectObject(&m_Bmp);

	CBrush backBrush(BLACK_COLOR);
	memDC.FillRect(rClientRect, &backBrush);  //�����ʾ���򱳾�ɫ

	int nBkMode = memDC.SetBkMode(TRANSPARENT);

	/*****************************��ʾ����***********************************/
	DrawTimeBackgnd(&memDC, rClientRect, wparam, lparam, iSendArm, iRecArm);
	DrawTimeCurve(&memDC, m_DisCurveArea, wparam, lparam, iSendArm, iSendArm, bRec, BLUE_COLOR);//send
	DrawTimeCurve(&memDC, m_DisCurveArea, wparam, lparam, iSendArm, iRecArm, bSend, RED_COLOR);//receive
	/************************************************************************/

	pDC->BitBlt(0, 0, rClientRect.Width(), rClientRect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SetBkMode(nBkMode);
	memDC.SelectObject(pOldBmp);
}


void CSignalTimeBmp::DrawTimeBackgnd(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam, UINT iSendArm, UINT iRecArm)
{
	UINT OFFSET = 40;
	float TIME_Y_LAGESCALE = VDD / 1000.0f;
	float TIME_Y_SMALLSCALE = 5.0f;
	float TIME_DISPLAY_POINTNUM = 4096;
	/************************************************************************/
	/* �����������ڿ�                                                                     */
	/************************************************************************/
	int iBias = 10;//б��
	CRect rFreMainRect, tempRect;
	tempRect.left = rRect.left + OFFSET;
	tempRect.top = rRect.top + OFFSET;
	tempRect.right = rRect.right - OFFSET;
	tempRect.bottom = rRect.bottom - OFFSET;

	CString strTemp;
	CSize sizeTemp;
	CTime tmTemp(lparam);

	/************************************************************************/
	/*     ������                                                                 */
	/************************************************************************/
	CPen greenPen, *pOldPen;
	UINT iTimeWidth = 0, iTimeHight = 0;
	float ySmallScale = tempRect.Height() / (TIME_Y_LAGESCALE*TIME_Y_SMALLSCALE); //y����С�̶ȵĿ��
	iTimeHight = TIME_Y_SMALLSCALE*ySmallScale;//

	rFreMainRect.left = tempRect.left;//��������xϵ�����
	rFreMainRect.right = tempRect.right;//��������Xϵ���յ�
	rFreMainRect.top = tempRect.top; //����Yϵ���յ�
	rFreMainRect.bottom = rFreMainRect.top + TIME_Y_LAGESCALE*iTimeHight;//����Yϵ�����
	m_DisCurveArea = rFreMainRect;
	float xcoff = (float)rFreMainRect.Width() / TIME_DISPLAY_POINTNUM;
	float yScale = iTimeHight;

	greenPen.CreatePen(PS_SOLID, 1, GRIDLINE_COLOR);
	pOldPen = pDC->SelectObject(&greenPen);

	pDC->MoveTo(rFreMainRect.left, rFreMainRect.top);
	pDC->LineTo(rFreMainRect.left, rFreMainRect.bottom);
	pDC->MoveTo(rFreMainRect.left, rFreMainRect.bottom);
	pDC->LineTo(rFreMainRect.right, rFreMainRect.bottom);

	pDC->SelectObject(pOldPen);
	/************************************************************************/
	/* ��ʾʱ��                                                                     */
	/************************************************************************/
	//	strTemp.Format(_T("         ����·��%d�շ��ź�ʱ������(V)"),iPathOrder+1);
	if (iSendArm < SOUNDDEV_NUM - 4)
	{
		strTemp = (_T("         A��¯�������շ��ź�ʱ������(V)"));
	}
	else if (iSendArm >= SOUNDDEV_NUM - 4)
	{
		strTemp = (_T("         B��¯�������շ��ź�ʱ������(V)"));
	}
	
	strTemp = tmTemp.Format(_T("%Y-%m-%d %H:%M:%S")) + strTemp;
	sizeTemp = pDC->GetTextExtent(strTemp);
	COLORREF crColor = pDC->SetTextColor(WHITE_COLOR);
	pDC->TextOut(rFreMainRect.left + (rFreMainRect.Width() - sizeTemp.cx) / 2, rRect.top + sizeTemp.cy, strTemp);
	/************************************************************************/
	/* ��Y�����̶�                                                                     */
	/************************************************************************/
	CPen coorPen, *oldCoorPen;
	coorPen.CreatePen(PS_SOLID, 1, GRIDLINE_COLOR);
	oldCoorPen = pDC->SelectObject(&coorPen);
	for (int i = 0; i < (int)TIME_Y_LAGESCALE + 1; i++)
	{
		strTemp.Format(_T("%2d V"), i);
		sizeTemp = pDC->GetTextExtent(strTemp);
		pDC->MoveTo(rFreMainRect.left, rFreMainRect.bottom - yScale*i);//
		pDC->LineTo(rFreMainRect.left - iBias, rFreMainRect.bottom - yScale*i);
		pDC->TextOut(rFreMainRect.left - iBias - sizeTemp.cx - 2, rFreMainRect.bottom - i*yScale - sizeTemp.cy / 2, strTemp);
	}


	/************************************************************************/
	/* ��Y����С�̶�                                                                     */
	/************************************************************************/
	for (int i = 0; i < (int)TIME_Y_LAGESCALE; i++)
	{
		for (UINT j = 0; j < 5; j++)
		{
			pDC->MoveTo(rFreMainRect.left, rFreMainRect.bottom - yScale*i - j*ySmallScale);
			pDC->LineTo(rFreMainRect.left - 5, rFreMainRect.bottom - yScale*i - j*ySmallScale);
		}
	}

	pDC->SelectObject(oldCoorPen);
	/************************************************************************/
	/* ����������                                                                     */
	/************************************************************************/
	CPen dotPen, *pOldDotPen;
	dotPen.CreatePen(PS_DOT, 1, GRIDLINE_COLOR);
	pOldDotPen = pDC->SelectObject(&dotPen);
	for (int i = 0; i < (int)TIME_Y_LAGESCALE + 1; i++)
	{
		pDC->MoveTo(rFreMainRect.left, rFreMainRect.bottom - yScale*i);
		pDC->LineTo(rFreMainRect.right, rFreMainRect.bottom - yScale*i);
	}

	pDC->MoveTo(rFreMainRect.right, rFreMainRect.top);
	pDC->LineTo(rFreMainRect.right, rFreMainRect.bottom);

	pDC->SetTextColor(crColor);
	pDC->SelectObject(pOldDotPen);

	/************************************************************************/
	/* ��ʮ����                                                                     */
	/************************************************************************/
	//CPoint pointTmp;
	//pointTmp.x = pTimeWnd->m_point.x-29*XCOFF;
	//pointTmp.y = pTimeWnd->m_point.y-73*YCOFF;

	//if (rFreMainRect.PtInRect(pointTmp))
	//{
	//	pDC->MoveTo(rFreMainRect.left,pointTmp.y);
	//	pDC->LineTo(rFreMainRect.right,pointTmp.y);
	//	pDC->MoveTo(pointTmp.x,rFreMainRect.top);
	//	pDC->LineTo(pointTmp.x,rFreMainRect.bottom);
	//}
}


void CSignalTimeBmp::DrawTimeCurve(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam, UINT iArmOrder, UINT iDataModule, BOOL bDis, COLORREF rColor)
{
	if (!bDis)
	{
		return;
	}
	//	CMainFrame *pWnd=(CMainFrame *)AfxGetMainWnd();
	//	if(!pWnd)
	//		return ;
	m_pfSignalData = (int***)wparam;// pWnd->GetAcousticData();
	if (!m_pfSignalData)
	{
		return;
	}
	/************************************************************************/
	/* ��ʱ������                                                                     */
	/************************************************************************/

	CPen redPen, *pOldPen;
	redPen.CreatePen(PS_SOLID, 1, rColor);
	pOldPen = pDC->SelectObject(&redPen);

	float yPercent = float(rRect.Height() / (VDD));
	float xcoff = (float)rRect.Width() / 1280.0f;//PERMODULE_DATA_LENTH;
	int y = 0;
	int i = 0;
	//	y=int(m_pfSignalData[iArmOrder][iDataModule][0]*yPercent*vCoff);

	y = int(m_pfSignalData[iArmOrder][iDataModule][0] * yPercent*vCoff);
	pDC->MoveTo(rRect.left + i / 2, rRect.bottom - y);
	for (i = 1; i < 1280; i++)
	{
		y = int((m_pfSignalData[iArmOrder][iDataModule][i] * yPercent*vCoff));
		pDC->LineTo(rRect.left + i*xcoff, rRect.bottom - y);
		//	i+=1;
	}
	pDC->SelectObject(pOldPen);
}
