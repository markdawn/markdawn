// TTrendBmp.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "TTrendBmp.h"



// CTTrendBmp

IMPLEMENT_DYNAMIC(CTTrendBmp, CStatic)

CTTrendBmp::CTTrendBmp()
{

}

CTTrendBmp::~CTTrendBmp()
{
}


BEGIN_MESSAGE_MAP(CTTrendBmp, CStatic)
END_MESSAGE_MAP()



// ��ʾ�����¶�����
void CTTrendBmp::DisplayTTrend(WPARAM wparam, LPARAM lparam,UINT iCh1,UINT iCh2,UINT iCh3,UINT iCh4,
	UINT iCh5, UINT iCh6, UINT iCh7, UINT iCh8, UINT iCh9, UINT iCh10, UINT iCh11, UINT iCh12)
{
	CDC *pDC=GetDC();
	//Draw(pDC, wparam, lparam, iCh1, iCh2, iCh3, iCh4, iCh5, iCh6, iCh7, iCh8, iCh9, iCh10, iCh11, iCh12);
	ReleaseDC(pDC);
}


void CTTrendBmp::Draw(CDC* pDC, WPARAM wparam, LPARAM lparam, UINT iCh1, UINT iCh2, UINT iCh3, UINT iCh4, 
	UINT iCh5, UINT iCh6, UINT iCh7, UINT iCh8, UINT iCh9, UINT iCh10, UINT iCh11, UINT iCh12)
{
	CRect rClientRect;
	GetClientRect(rClientRect);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	if(m_Bmp.GetSafeHandle()==NULL)
		m_Bmp.CreateCompatibleBitmap(pDC,rClientRect.Width(),rClientRect.Height());

	CBitmap *pOldBmp;
	pOldBmp=memDC.SelectObject(&m_Bmp);

	CBrush backBrush(BLACK_COLOR);
	memDC.FillRect(rClientRect,&backBrush);  //�����ʾ���򱳾�ɫ

	int nBkMode=memDC.SetBkMode(TRANSPARENT);

	/*****************************��ʾ����***********************************/
	DrawCurveFrame(&memDC,rClientRect, wparam,lparam);
	DrawTTrendCurve(&memDC, rClientRect, iCh1, wparam, lparam, RED_COLOR);
	DrawTTrendCurve(&memDC, rClientRect, iCh2, wparam, lparam, REDWINE_COLOR);
	DrawTTrendCurve(&memDC, rClientRect, iCh3, wparam, lparam, LIGHT_BLUE_COLOR);
	DrawTTrendCurve(&memDC, rClientRect, iCh4, wparam, lparam, GREEN_COLOR);
	DrawTTrendCurve(&memDC, rClientRect, iCh5, wparam, lparam, PURPLE_COLOR);
	DrawTTrendCurve(&memDC, rClientRect, iCh6, wparam, lparam, ORANGE_COLOR);
	DrawTTrendCurve(&memDC, rClientRect, iCh7, wparam, lparam, GRAY_COLOR);
	DrawTTrendCurve(&memDC, rClientRect, iCh8, wparam, lparam, NAVY_COLOR);
	DrawTTrendCurve(&memDC, rClientRect, iCh9, wparam, lparam, ORANGE_COLOR);
	DrawTTrendCurve(&memDC, rClientRect, iCh10, wparam, lparam, REDWINE_COLOR);
	DrawTTrendCurve(&memDC, rClientRect, iCh11, wparam, lparam, GREEN_COLOR_1);
	DrawTTrendCurve(&memDC, rClientRect, iCh12, wparam, lparam, GREEN_COLOR_2);
	/************************************************************************/

	pDC->BitBlt(0,0,rClientRect.Width(),rClientRect.Height(),&memDC,0,0,SRCCOPY);

	memDC.SetBkMode(nBkMode);
	memDC.SelectObject(pOldBmp);
}


// Ϊ���¶�����׼������
void CTTrendBmp::PrePrepareForDraw(UINT iChanel, UINT* pBuffer, WPARAM wparam, LPARAM lparam)
{
	
}

//��ƽ���¶���������
void CTTrendBmp::DrawTTrendCurve(CDC* pDC, CRect rRect, UINT iChanel, WPARAM wparam, LPARAM lparam, COLORREF color)
{
	int OFFSET = 50;
	float REAL_X_LAGESCALE = 24.0f;//second
	float REAL_X_SMALLSCALE = 60.0f; //minute
	float TEMPER_RANGE_Y  = 2000.0f;//����¶�3500���϶�
	float REAL_Y_LAGESCALE = 400.0f;//Yһ���̶ȶ�Ӧ500��

	CRect rHistimeMainRect ,tempRect;
	tempRect.left = rRect.left + OFFSET;
	tempRect.top = rRect.top + OFFSET;
	tempRect.right = rRect.right - OFFSET;
	tempRect.bottom = rRect.bottom - OFFSET;	

	StartEndTime* pStartEndTime = (StartEndTime*)lparam;
	MYSQL* hMysql = (MYSQL*)wparam;

	int  nDayNum = (pStartEndTime->endTime - pStartEndTime->startTIme)/(REAL_X_LAGESCALE*REAL_X_SMALLSCALE*60);
	float xSmallScale = tempRect.Width()/(nDayNum*REAL_X_LAGESCALE*REAL_X_SMALLSCALE);//x����С�̶ȵĿ���:nDayNum�죬REAL_X_LAGESCALEСʱ��REAL_X_SMALLSCALE����
	float ySmallScale = tempRect.Height()/(TEMPER_RANGE_Y/**REAL_Y_SMALLSCALE*/); //y����С�̶ȵĿ���
	/****************��ȡƽ���¶ȵ�һ�����ݱ����ʱ��*************************/
	char *ch_query;
	CString sQueryT;
	sQueryT.Format(_T("select TimeParam, Area%dAvrgTValue from areaavrgtvalue where  TimeParam>= %d and TimeParam <= %d"),iChanel,int(pStartEndTime->startTIme),int(pStartEndTime->endTime));
	ch_query =   (LPSTR)(LPCTSTR)sQueryT.GetBuffer(sQueryT.GetLength());
	sQueryT.ReleaseBuffer();
	if(mysql_real_query(hMysql,ch_query,(UINT)strlen(ch_query))!=0) //ִ�в�ѯ
	{ 
		CString strError(_T("�����ݿ���� !"));
		CSize sizeError=pDC->GetTextExtent(strError);
		pDC->TextOut(tempRect.left+(tempRect.Width()-sizeError.cx)/2,tempRect.top+(tempRect.Height()-sizeError.cy)/2,strError);
		//AfxMessageBox(_T("���ݿ��б������")); 
		return;
	}

	MYSQL_RES *result;
	MYSQL_ROW row;
	if(!(result=mysql_use_result(hMysql)))//��ʼ������
	{ 
		CString strError(_T("��ʼ��ƽ���¶����ݼ�ʧ�� !"));
		CSize sizeError=pDC->GetTextExtent(strError);
		pDC->TextOut(tempRect.left+(tempRect.Width()-sizeError.cx)/2,tempRect.top+(tempRect.Height()-sizeError.cy)/2,strError);
	//	AfxMessageBox(_T("��ʼ��ƽ���¶����ݼ�ʧ��")); 		
		return;
	}

	int iLastTemperature = 0;
	int iNextTemperature = 0;
	int iLastTime = 0;
	int iNextTime = 0;
	float fXPoint = 0.0f;
	float fYPoint = 0.0f;

	float xScale=tempRect.Width()/100.0f;//(REAL_X_LAGESCALE*REAL_X_SMALLSCALE*60.0f)/;
	float yScale=tempRect.Height()/100.0f;

	//int xPoint,yPoint;
	CPen drawCurvePen ,*pOldPen;
	drawCurvePen.CreatePen(PS_SOLID,1,color);
	pOldPen = pDC->SelectObject(&drawCurvePen);

	int nColumn =	mysql_num_fields(result); //��ȡ������

	if (nColumn!=2)
	{
		CString strError;
		strError.Format(_T("��ȡ��%dͨ�����ݴ���"),iChanel);
	//	AfxMessageBox(strError); 	
		mysql_free_result(result);
		CSize sizeError=pDC->GetTextExtent(strError);
		pDC->TextOut(tempRect.left+(tempRect.Width()-sizeError.cx)/2,tempRect.top+(tempRect.Height()-sizeError.cy)/2,strError);

		return;
	}

	if (!(row=mysql_fetch_row(result)))
	{
		//������
		CString strError(_T("������ !"));
		CSize sizeError=pDC->GetTextExtent(strError);
		pDC->TextOut(tempRect.left+(tempRect.Width()-sizeError.cx)/2,tempRect.top+(tempRect.Height()-sizeError.cy)/2,strError);
		mysql_free_result(result);
		return;
	}
	else
	{
		if (row[1]!=NULL)
		{
			iLastTime = atoi(row[0]);
			iLastTemperature = atoi(row[1]);//��ȡ�ڶ�����Ϣ����ƽ���¶����ݴ洢��ʱ��
			fXPoint = ((iLastTime-pStartEndTime->startTIme)/60.0f)*xSmallScale + tempRect.left;
			fYPoint = tempRect.bottom - (iLastTemperature*ySmallScale);
			pDC->MoveTo(fXPoint,fYPoint);
		}
	}

	while(row=mysql_fetch_row(result))  //��ȡÿһ����ָ��    
	{
		if (row[1]==NULL)
		{
			continue;
		}
		iNextTime = atoi(row[0]);
		iNextTemperature = atoi(row[1]);//��ȡ�ڶ�����Ϣ����ƽ���¶����ݴ洢��ʱ��
		if (iNextTime-iLastTime>180) //����3��������û�����ݵĵ�
		{
			fXPoint = ((iNextTime-pStartEndTime->startTIme)/60.0f)*xSmallScale + tempRect.left;
			fYPoint = tempRect.bottom - (iNextTemperature*ySmallScale);
			pDC->MoveTo(fXPoint,fYPoint);
			iLastTime = iNextTime;
			continue;
		}

		fXPoint = ((iNextTime-pStartEndTime->startTIme)/60.0f)*xSmallScale + tempRect.left;
		fYPoint = tempRect.bottom - (iNextTemperature*ySmallScale);
		pDC->LineTo(fXPoint,fYPoint);
		iLastTime = iNextTime;
	}
	mysql_free_result(result);
	pDC->SelectObject(pOldPen);
}


// �����߱�����
void CTTrendBmp::DrawCurveFrame(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam)
{
	int OFFSET = 50;
	float REAL_X_LAGESCALE = 24.0f;//second
	float REAL_X_SMALLSCALE = 60.0f; //minute
	float TEMPER_RANGE_Y  = 2000.0f;//����¶�2500���϶�
	float REAL_Y_LAGESCALE = 400.0f;//Yһ���̶ȶ�Ӧ500��

	int iBias=10;//б��
	CRect rHistimeMainRect ,tempRect;
	tempRect.left = rRect.left + OFFSET;
	tempRect.top = rRect.top + OFFSET;
	tempRect.right = rRect.right - OFFSET;
	tempRect.bottom = rRect.bottom - OFFSET;	

	CString strTemp;
	CSize sizeTemp;

	StartEndTime* pStartEndTime = (StartEndTime*)lparam;

	CTime tmStartTemp(pStartEndTime->startTIme);
	CTime tmEndTemp(pStartEndTime->endTime);
	CTime tmTemp(pStartEndTime->startTIme);
	//������
//	tempRect +=REAL_DISPALY_OFFSET; 
	CPen greenPen,*pOldPen;
	UINT iRealHight = 0; 
	float iRealWidth=0;

	int  nDayNum = (pStartEndTime->endTime+1 - pStartEndTime->startTIme)/(REAL_X_LAGESCALE*REAL_X_SMALLSCALE*60);

	float xSmallScale = tempRect.Width()/(nDayNum*REAL_X_LAGESCALE*REAL_X_SMALLSCALE);//x����С�̶ȵĿ���:nDayNum�죬REAL_X_LAGESCALEСʱ��REAL_X_SMALLSCALE����
	float ySmallScale = tempRect.Height()/(TEMPER_RANGE_Y/**REAL_Y_SMALLSCALE*/); //y����С�̶ȵĿ���

	iRealWidth = REAL_X_SMALLSCALE*xSmallScale; //ÿСʱӳ��Ŀ���
	iRealHight = REAL_Y_LAGESCALE*ySmallScale;//ӳ��Ƶ�ʷ�ֵ

	rHistimeMainRect.left = tempRect.left;//��������xϵ�����
	rHistimeMainRect.right=tempRect.right;//��������Xϵ���յ�
	rHistimeMainRect.top = tempRect.top; //����Yϵ���յ�
	rHistimeMainRect.bottom = rHistimeMainRect.top + 5*iRealHight;//����Yϵ�����

	float xcoff = xSmallScale;
	float yScale=iRealHight;
	float xScale = iRealWidth;

	greenPen.CreatePen(PS_SOLID,1,GRIDLINE_COLOR);
	pOldPen=pDC->SelectObject(&greenPen);

	pDC->MoveTo(rHistimeMainRect.left,rHistimeMainRect.top);
	pDC->LineTo(rHistimeMainRect.left,rHistimeMainRect.bottom);
	pDC->MoveTo(rHistimeMainRect.left,rHistimeMainRect.bottom);
	pDC->LineTo(rHistimeMainRect.right,rHistimeMainRect.bottom);
//	m_rHistimeDisArea = rHistimeMainRect;
	pDC->SelectObject(pOldPen);

	/************************************************************************/
	/* ��ʾʱ��                                                                     */
	/************************************************************************/
	strTemp.Format(_T("         ����ƽ���¶���������"));
	strTemp=tmStartTemp.Format("%Y-%m-%d 00:00:00") + "----" + tmEndTemp.Format("%Y-%m-%d %H:%M:%S")+strTemp;
	sizeTemp=pDC->GetTextExtent(strTemp);
	COLORREF crColor=pDC->SetTextColor(WHITE_COLOR);
	pDC->TextOut(rHistimeMainRect.left+(rHistimeMainRect.Width()-sizeTemp.cx)/2,rRect.top +sizeTemp.cy,strTemp);

	
	/************************************************************************/
	/* ��Y�����̶�                                                                     */
	/************************************************************************/
	CPen coorPen,*oldCoorPen;
	coorPen.CreatePen(PS_SOLID,1,GRIDLINE_COLOR);
	oldCoorPen = pDC->SelectObject(&coorPen);
	for(int i=0;i<=5;i++)
	{
		strTemp.Format(_T("%d "),i*400);
		sizeTemp=pDC->GetTextExtent(strTemp);
		pDC->MoveTo(rHistimeMainRect.left,rHistimeMainRect.bottom-yScale*i);//
		pDC->LineTo(rHistimeMainRect.left-iBias,rHistimeMainRect.bottom-yScale*i);
		pDC->TextOut(rHistimeMainRect.left/*- iBias*/-sizeTemp.cx-5,rHistimeMainRect.bottom-i*yScale-sizeTemp.cy/2,strTemp);
	}

	/************************************************************************/
	/* ��Y����С�̶�                                                                     */
	/************************************************************************/
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{		
			pDC->MoveTo(rHistimeMainRect.left,rHistimeMainRect.bottom-yScale*i-j*ySmallScale*100);
			pDC->LineTo(rHistimeMainRect.left-5,rHistimeMainRect.bottom-yScale*i-j*ySmallScale*100);
		}
	}




	/************************************************************************/
	/* ��X�����̶�                                                                     */
	/************************************************************************/
	CString strTimeCoor;
	int hour = 0;
	strTimeCoor.Format(_T("%02d"),hour);
	int nCount = 0;

	if (nDayNum>0 && nDayNum<=8)
	{
		for (int i = 0; i <= 24*nDayNum; i++)
		{
			CTime tm(tmStartTemp + i*3600);
			if (nCount>=24)
				nCount = 0;

			pDC->MoveTo(rHistimeMainRect.left + xScale*i,rHistimeMainRect.bottom);//
			pDC->LineTo(rHistimeMainRect.left + xScale*i,rHistimeMainRect.bottom+iBias);

			if (nCount == 0)
			{
				strTimeCoor.Format(_T("(%02d-%02d)"),tm.GetMonth(),tm.GetDay());
				sizeTemp=pDC->GetTextExtent(strTimeCoor);
				pDC->TextOut(rHistimeMainRect.left + xScale*i-sizeTemp.cx/2,rHistimeMainRect.bottom+2*iBias+2,strTimeCoor);//display date
			}
			if (!(nCount%4))
			{
				strTimeCoor.Format(_T("%02d"),nCount);
				sizeTemp=pDC->GetTextExtent(strTimeCoor);
				pDC->TextOut(rHistimeMainRect.left + xScale*i-sizeTemp.cx/2,rHistimeMainRect.bottom+iBias,strTimeCoor);//display hour
			}

			nCount++;
		}
	}
	else if (nDayNum>8 && nDayNum <= 16)
	{
		float fXScall = xScale * REAL_X_LAGESCALE;
		for (int i = 0; i <= nDayNum; i++)
		{
			CTime tm(tmStartTemp + i*24*3600);
			pDC->MoveTo(rHistimeMainRect.left + fXScall*i,rHistimeMainRect.bottom);//
			pDC->LineTo(rHistimeMainRect.left + fXScall*i,rHistimeMainRect.bottom+iBias);

			strTimeCoor.Format(_T("(%02d-%02d)"),tm.GetMonth(),tm.GetDay());
			sizeTemp=pDC->GetTextExtent(strTimeCoor);
			pDC->TextOut(rHistimeMainRect.left + fXScall*i-sizeTemp.cx/2,rHistimeMainRect.bottom+2*iBias+2,strTimeCoor);//display date
		}
	}
	else
	{
		float fXScall = xScale * REAL_X_LAGESCALE;
		int i = 0; 
		for (i = 0; i <= nDayNum; i = i+2)
		{
			CTime tm(tmStartTemp + i*24*3600);
			pDC->MoveTo(rHistimeMainRect.left + fXScall*i,rHistimeMainRect.bottom);//
			pDC->LineTo(rHistimeMainRect.left + fXScall*i,rHistimeMainRect.bottom+iBias);

			strTimeCoor.Format(_T("(%02d-%02d)"),tm.GetMonth(),tm.GetDay());
			sizeTemp=pDC->GetTextExtent(strTimeCoor);
			pDC->TextOut(rHistimeMainRect.left + fXScall*i-sizeTemp.cx/2,rHistimeMainRect.bottom+2*iBias+2,strTimeCoor);//display date

		}
	}

	pDC->SelectObject(oldCoorPen);
	pDC->SetTextColor(crColor);

	/************************************************************************/
	/* ����������                                                                     */
	/************************************************************************/
	CPen dotPen ,*pOldDotPen;
	dotPen.CreatePen(PS_DOT,1,GRIDLINE_COLOR);
	pOldDotPen = pDC->SelectObject(&dotPen);
	//UINT i = 0;
	int i = 0;
	for(i=0;i<=5;i++)
	{
		pDC->MoveTo(rHistimeMainRect.left,rHistimeMainRect.bottom-yScale*i);
		pDC->LineTo(rHistimeMainRect.right,rHistimeMainRect.bottom-yScale*i);
	}

	if (nDayNum>0 && nDayNum<=8)
	{
		for (i = 0; i < 24*nDayNum; i++)
		{
			pDC->MoveTo(rHistimeMainRect.left + xScale*i,rHistimeMainRect.top);//
			pDC->LineTo(rHistimeMainRect.left + xScale*i,rHistimeMainRect.bottom);
		}
	}
	else if (nDayNum>8 && nDayNum<=16)
	{
		float fXScall = xScale * REAL_X_LAGESCALE;
		for (i = 0; i<nDayNum; i++)
		{
			pDC->MoveTo(rHistimeMainRect.left + fXScall*i,rHistimeMainRect.top);//
			pDC->LineTo(rHistimeMainRect.left + fXScall*i,rHistimeMainRect.bottom);
		}
	}
	else
	{
		float fXScall = xScale * REAL_X_LAGESCALE;
		for (i = 0; i<nDayNum; i=i+2)
		{
			pDC->MoveTo(rHistimeMainRect.left + fXScall*i,rHistimeMainRect.top);//
			pDC->LineTo(rHistimeMainRect.left + fXScall*i,rHistimeMainRect.bottom);
		}
	}
	pDC->MoveTo(rHistimeMainRect.right,rHistimeMainRect.top);
	pDC->LineTo(rHistimeMainRect.right,rHistimeMainRect.bottom);

	pDC->SetTextColor(crColor);
	pDC->SelectObject(pOldDotPen);
}