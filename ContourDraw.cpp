#include "StdAfx.h"
#include "ContourDraw.h"
#include "Contour.h"
#include <gl\gl.h> 
#include <gl\glu.h> 
#include <gl\glaux.h>
#include<GL\glut.h>

#define MAX_CHAR  128
CContourDraw::CContourDraw(void)
	: m_ppGridData(NULL)
{
	m_pCurveList = new CCurveList;
	//		m_pCurve = new CCurve;
	
}


CContourDraw::~CContourDraw(void)
{
	int num = m_pCurveList->GetCount();
	if (num > 0)
	{
		m_pCurveList->RemoveAll();
		if (m_pCurveList)
		{
			delete m_pCurveList;
		}
	}


}


// ����ֵ��
void CContourDraw::DrawContours(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam, LPVOID lpvoid)
{

	ASSERT(lparam);
	CRect rPictureRect;
	//Ԥ����դ����Ϣ
	PreProcess(rRect, wparam, lparam);
	//�������߱����߿�
	DrawBackground(pDC, m_rect, wparam, lparam);

	//////////////////////////////////////////////��ɫ���--Debug///////////////////////////////////////////////////////////////////////////

	int i = 0;
	int j = 0;
	ASSERT(wparam);
	m_GridInfo = *((GridBaiscInfo*)lparam); //�õ�դ�������Ϣ
	m_ppGridData = (float**)wparam;
	ASSERT(m_ppGridData);
	//COLORREF colorValue ;
	//	CRect fillRect;
	//	CBrush newBrush,*pOldBrush;
	POINT startPoint, endPoint;
	float deltX = m_GridInfo.width / (m_GridInfo.col - 1);
	float deltY = m_GridInfo.hight / (m_GridInfo.row - 1);
	float x = 0.0f, y = 0.0f;

	GLfloat vdColor[3];
	int Value = 0;
	for (i = 1; i < 31 /*m_GridInfo.row*/; i++)
	{
		for (j = 1; j < 51/*m_GridInfo.col*/; j++)
		{
			x = j* deltX;
			y = i*deltY;
			endPoint.x = m_rect.left + x*m_rect.Width() / 50.0f; //50Ϊʵ��¯����
			// endPoint.y =m_rect.bottom -  y*m_rect.Height()/30.0f;//30Ϊʵ��¯��
			endPoint.y = y*m_rect.Height() / 30.0f;//30Ϊʵ��¯��

			x = (j - 1)*deltX;
			y = (i - 1)*deltY;
			startPoint.x = m_rect.left + x*m_rect.Width() / 50.0f;
			//	 startPoint.y =m_rect.bottom -  y*m_rect.Height()/30.0f;
			startPoint.y = y*m_rect.Height() / 30.0f;

			glBegin(GL_POLYGON); //���һ������
			Value = m_ppGridData[i - 1][j - 1]; //���¶������½ǵ�λ�ü���ɫ����
			m_LookUpColor.GetColor(Value, vdColor);
			glColor3fv(vdColor);
			glVertex2f(startPoint.x, startPoint.y);
			Value = m_ppGridData[i - 1][j]; //���¶������½ǵ�λ�ü���ɫ����
			m_LookUpColor.GetColor(Value, vdColor);
			glColor3fv(vdColor);
			glVertex2f(endPoint.x, startPoint.y);
			Value = m_ppGridData[i][j]; //���¶������Ͻǵ�λ�ü���ɫ����
			m_LookUpColor.GetColor(Value, vdColor);
			glColor3fv(vdColor);
			glVertex2f(endPoint.x, endPoint.y);
			Value = m_ppGridData[i][j - 1]; //�����Ƕ������Ͻǵ�λ�ü���ɫ����
			m_LookUpColor.GetColor(Value, vdColor);
			glColor3fv(vdColor);
			glVertex2f(startPoint.x, endPoint.y);
			glEnd(); //���һ���������
		}
	}

	
/*#else
	int i = 0;
	int j =0;
	//	m_GridInfo = pWnd->GetGridBaseInfo(); //�õ�դ�������Ϣ
	//	m_ppGridData = pWnd->GetGridDataPointer();

	ASSERT(m_ppGridData);
	COLORREF startColorValue ;
	COLORREF endColorValue;
	COLORREF fillClr;
	//	CRect fillRect;
	POINT startPoint,endPoint;
	float deltX = m_GridInfo.width/ ( m_GridInfo.col - 1 );
	float deltY = m_GridInfo.hight / ( m_GridInfo.row - 1 );
	float x = 0.0f,y = 0.0f;

	BYTE  r = 0, g = 0,b = 0; 

	for (i = 1;i < m_GridInfo.row;i++)
	{
		for (j = 1;j<m_GridInfo.col;j++)
		{
			x =  j* deltX;
			y = i*deltY;
			endPoint.x =m_rect.left + x*m_rect.Width()/50.0f ;
			endPoint.y =m_rect.bottom - y*m_rect.Height()/30.0f;

			x = (j-1)*deltX;
			y = (i-1)*deltY;
			startPoint.x =m_rect.left + x*m_rect.Width()/50.0f ;
			startPoint.y =m_rect.bottom -  y*m_rect.Height()/30.0f;

			endColorValue = m_LookUpColor.GetFlameColor(m_ppGridData[i][j]*10);
			startColorValue = m_LookUpColor.GetFlameColor(m_ppGridData[i-1][j-1]*10);

			int m = 0, n = 0;
			for (m = startPoint.y; m>endPoint.y;m--)
			{
				for (n = startPoint.x;n < endPoint.x;n++)
				{
					int width = (endPoint.x-startPoint.x) ;
					r = n*( GetRValue(endColorValue) - GetRValue(startColorValue) ) / width + ( endPoint.x*GetRValue(startColorValue) - startPoint.x*GetRValue(endColorValue) )/width;
					g = n*( GetGValue(endColorValue) - GetGValue(startColorValue) ) / width + ( endPoint.x*GetGValue(startColorValue) - startPoint.x*GetGValue(endColorValue) )/width;
					b = n*( GetBValue(endColorValue) - GetBValue(startColorValue) ) / width + ( endPoint.x*GetBValue(startColorValue) - startPoint.x*GetBValue(endColorValue) )/width;
					fillClr = RGB(r,g,b);
					pDC->SetPixel(n,m,fillClr);
				}
			}


		}
	}
#endif*/
	////////////////////////////////////��������//////////////////////////////////
	CreateContourObj(wparam, lparam, lpvoid);
	DrawContourCurve(pDC, m_rect, wparam, lparam, lpvoid);
	DrawContourCurveText(pDC, m_rect, wparam, lparam, lpvoid);

	glFlush();
}


// ��������ǰ��Ԥ����
void CContourDraw::PreProcess(CRect rRect, WPARAM wparam, LPARAM lparam)
{

	ASSERT(lparam);
	m_GridInfo = *((GridBaiscInfo*)lparam); //�õ�դ�������Ϣ

	//�������
	float m_radioCoff = (m_GridInfo.width) / (m_GridInfo.hight);
	float m_radioCoff2 = (float)(rRect.Width()) / (rRect.Height());

	int OFFSET = 10;

	//	m_rect.top = rRect.top + OFFSET;
	//	m_rect.left = rRect.left + OFFSET;
	//	m_rect.right = rRect.right - OFFSET;
	//	m_rect.bottom = rRect.bottom - OFFSET;

	m_rect.top = rRect.top;
	m_rect.left = rRect.left;
	m_rect.right = rRect.right;
	m_rect.bottom = rRect.bottom;
	/*
		if (m_radioCoff>=m_radioCoff2)
		{
		m_rect.bottom = m_rect.top + m_rect.Width()/m_radioCoff;
		}
		else
		{
		m_rect.right = m_rect.left+m_rect.Height()*m_radioCoff;
		}
		*/


}


// �������߱����߿�
void CContourDraw::DrawBackground(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam)
{
	CPen blackPen;
	blackPen.CreatePen(PS_SOLID, 1, GREEN_COLOR);
	CPen* oldPen = pDC->SelectObject(&blackPen);

	//	CGridDataInfo& dataInfo = m_pContourOwner->GetDataOwner()->GetDataInfo(); 
	//	CGeoRect mapRect(dataInfo.xMin,dataInfo.yMin,dataInfo.xMax,dataInfo.yMax);

	//	mapRect.Scale(m_ScaleRatio);
	//	mapRect.Translate(m_TranslateX,m_TranslateY);


	pDC->MoveTo(rRect.left + 10, rRect.top + 10);
	pDC->LineTo(rRect.right - 10, rRect.top + 10);
	pDC->LineTo(rRect.right - 10, rRect.bottom - 10);
	pDC->LineTo(rRect.left + 10, rRect.bottom - 10);
	pDC->LineTo(rRect.left + 10, rRect.top + 10);

	pDC->SelectObject(oldPen);

}


// ��������
void CContourDraw::DrawContourCurve(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam, LPVOID lpvoid)
{

	CContour* pWnd = (CContour*)lpvoid;
	if (!lpvoid)
	{
		return;
	}
	// ���������߶���
	//CreateContourObj(wparam,lparam,lpvoid);

	//��������
	int nCount = m_pCurveList->GetCount();
	POSITION pos = m_pCurveList->GetHeadPosition();
	CCurve* pCurve = NULL;
	float contourValue = 0.0f;
	CString strTemp;
	for (int i = 0; i < nCount; i++)
	{
		pCurve = m_pCurveList->GetNext(pos);
		int nSize = pCurve->GetSize();
		ASSERT(nSize >= 2);

		int x = int(pCurve->GetAt(1).x*rRect.Width() / 50.0f) + 10;
		int y = int(pCurve->GetAt(1).y*rRect.Height() / 30.0f);//openGL�����½�Ϊ��㣬��MFC�����Ͻ�Ϊ������

		glBegin(GL_LINE_STRIP); //���һ���߶�
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex2f(x, y);
		for (int j = 2; j < nSize; j++)
		{
			x = int(pCurve->GetAt(j).x*rRect.Width() / 50.0f) + 10;
			y = int(pCurve->GetAt(j).y*rRect.Height() / 30.0f);
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex2f(x, y);
		}
		glEnd();
	}
	
}

// ������������
void CContourDraw::DrawContourCurveText(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam, LPVOID lpvoid)
{
	CString strTemp;
	int nCount = m_pCurveList->GetCount();
	POSITION pos = m_pCurveList->GetHeadPosition();
	CCurve* pCurve = NULL;
	CCurve* pCurveBf = NULL;
	CCurve* pCurveAf = NULL;
	int cValue = 0.0f;
	int temp = 0;
	for (int i = 0; i < nCount; i++)
	{
		pCurve = m_pCurveList->GetNext(pos);
		int nSize = pCurve->GetSize();
		ASSERT(nSize >= 2);
		if (nSize <= 30)
		{
			int x = int(pCurve->GetAt(nSize / 2).x*rRect.Width() / 50.0f) + 10;
			int y = int(pCurve->GetAt(nSize / 2).y*rRect.Height() / 30.0f);
			cValue = pCurve->GetAt(0).x;    //���ߵ�ͷ�����������������¶�ֵ
			strTemp.Format(_T("%d"), cValue);
			glRasterPos2i(x, y);
			drawString(strTemp);
		}
		else
		{
			int x = int(pCurve->GetAt(nSize - 30).x*rRect.Width() / 50.0f) + 10;
			int y = int(pCurve->GetAt(nSize - 30).y*rRect.Height() / 30.0f);
			cValue = pCurve->GetAt(0).x;
			strTemp.Format(_T("%d"), cValue);
			glRasterPos2i(x, y);
			drawString(strTemp);
			//for (int j = 1; j < nSize; j++)
			//{
			//	if (j % 30 == 0 /*&& j % 30 <= 30*/)
			//	{
			//		int x = int(pCurve->GetAt(j).x*rRect.Width() / 50.0f) + 10;
			//		int y = int(pCurve->GetAt(j).y*rRect.Height() / 30.0f);
			//		cValue = pCurve->GetAt(0).x;
			//		strTemp.Format(_T("%d"), cValue);
			//		glRasterPos2i(x, y);
			//		drawString(strTemp);
			//	}
			//}
		}
	}
		
	

}


void CContourDraw::drawString(const char* str)
{
	static int isFirstCall = 1;
	static GLuint lists;
	if (isFirstCall) { // ����ǵ�һ�ε��ã�ִ�г�ʼ��
		// Ϊÿһ��ASCII�ַ�����һ����ʾ�б�
		isFirstCall = 0;
		// ����MAX_CHAR����������ʾ�б���
		lists = glGenLists(MAX_CHAR);
		// ��ÿ���ַ��Ļ������װ����Ӧ����ʾ�б���
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// ����ÿ���ַ���Ӧ����ʾ�б�����ÿ���ַ�
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

// ���������߶����Է��㻭������
void CContourDraw::CreateContourObj(WPARAM wparam, LPARAM lparam, LPVOID lpvoid)
{
	CContour* pWnd = (CContour*)lpvoid;
	if (!lpvoid)
	{
		return;
	}

	float contourValue = 0.0f;
	CCurveList* pCurveList;
	int numVals = pWnd->m_ContourValues.GetNumOfContourValues();
	int testNum = 0;
	ReMoveAll();
	for (int i = 0; i < numVals; i++)
	{
		pCurveList = pWnd->GetContours().GetAt(i);

		ASSERT(pCurveList != NULL);

		contourValue = pWnd->m_ContourValues.GetContourValueFromList(i);

		POSITION pos = pCurveList->GetHeadPosition();
		CCurve* pCtrLine = NULL;

		while (pos != NULL)
		{
			pCtrLine = pCurveList->GetNext(pos);
			ASSERT(pCtrLine->GetSize() >= 2); //һ����ֵ������������2����

			//����������
			m_pCurveList->AddTail(pCtrLine);
			testNum++;
		}
	}
	int num = testNum;
}


void CContourDraw::ReMoveAll(void)
{
	int num = m_pCurveList->GetCount();
	if (num > 0)
	{
		m_pCurveList->RemoveAll();
	}
}

