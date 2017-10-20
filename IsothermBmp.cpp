// IsothermBmp.cpp : implementation file
//

#include "stdafx.h"
#include "AcousticTherm.h"
#include "IsothermBmp.h"
#include "commonFile.h"
#include <GL/GLAux.h>


// CIsothermBmp

IMPLEMENT_DYNAMIC(CIsothermBmp, CStatic)

CIsothermBmp::CIsothermBmp()
{
	
}

CIsothermBmp::~CIsothermBmp()
{

	//�ͷ���Դ
	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(m_hRC);
	if (m_pDC)
	{
		delete m_pDC;
		m_pDC = NULL;
	}
	
}


BEGIN_MESSAGE_MAP(CIsothermBmp, CStatic)
//	ON_WM_SIZE()
END_MESSAGE_MAP()

// CIsothermBmp message handlers
void CIsothermBmp::DisplayIsotherm(WPARAM wparam, LPARAM lparam,CClientDC* pDC,CRect rRect)
{
	CRect rRectClient	;
	GetClientRect(rRectClient);

	wglMakeCurrent(m_pDC->m_hDC,m_hRC);//ʹ RC �뵱ǰ DC �����
	glClear(GL_COLOR_BUFFER_BIT);
	// ���Ƴ���
	glViewport(0,0,rRectClient.Width(),rRectClient.Height());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,rRectClient.Width(),0,rRectClient.Height());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	Draw(m_pDC,wparam,lparam,rRectClient);

	glFlush();
	glPopMatrix();
	// ����������
	SwapBuffers(wglGetCurrentDC());
	wglMakeCurrent(m_pDC->m_hDC,NULL);//�ͷ� RC���Ա����� DC ���л�ͼ
	
}


void CIsothermBmp::Draw(CDC* pDC, WPARAM wparam, LPARAM lparam,CRect rRect)
{
//	int OldMode = pDC->SetMapMode(MM_LOENGLISH);

	CRect rClientRect(rRect);
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	if(m_Bmp.GetSafeHandle()==NULL)
		m_Bmp.CreateCompatibleBitmap(pDC,rClientRect.Width(),rClientRect.Height());

	CBitmap *pOldBmp;
	pOldBmp=memDC.SelectObject(&m_Bmp);

	CBrush backBrush(RED_COLOR);
	memDC.FillRect(rClientRect,&backBrush);  //�����ʾ���򱳾�ɫ

	int nBkMode=memDC.SetBkMode(TRANSPARENT);

	/*****************************��ʾ����***********************************/
	DrawCurve(&memDC,rClientRect,wparam,lparam);
	/************************************************************************/

	pDC->BitBlt(0,0,rClientRect.Width(),rClientRect.Height(),&memDC,0,0,SRCCOPY);
	memDC.SetBkMode(nBkMode);
	memDC.SelectObject(pOldBmp);
//	pDC->SetMapMode(OldMode);
}


void CIsothermBmp::DrawCurve(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam)
{
	m_contour.DrawContour(pDC,rRect,wparam,lparam);
}


void CIsothermBmp::DoDataExchange(CDataExchange* pDX)
{
	// TODO: Add your specialized code here and/or call the base class

	CStatic::DoDataExchange(pDX);
}


BOOL CIsothermBmp::SetDCPixelFormat(CDC*pDC)
{
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),//�ṹ��С
		1,//�ṹ�İ汾
		PFD_DRAW_TO_WINDOW |         // �ڴ���(������λͼ)�л�ͼ
		PFD_SUPPORT_OPENGL |         // ֧���ڴ����н���OpenGL����
		PFD_DOUBLEBUFFER,           // ˫����ģʽ
		PFD_TYPE_RGBA,            //RGBA��ɫģʽ
		32,                  // ��Ҫ32λ��ɫ
		0, 0, 0, 0, 0, 0,           //������ѡ��ģʽ
		0,                  // No Alpha Buffer
		0,                  // Shift Bit Ignored
		0,                  // No Accumulation Buffer
		0, 0, 0, 0,              // Accumulation Bits Ignored
		16,                  // ��Ȼ������Ĵ�С
		1,                  // No Stencil Buffer
		0,                  // No Auxiliary Buffer
		PFD_MAIN_PLANE,            // Main Drawing Layer
		0,                  // Reserved
		0, 0, 0                // Layer Masks Ignored
	};
	int nPixelFormat = ChoosePixelFormat(pDC->GetSafeHdc(),&pfd);
	if (nPixelFormat == 0)
	{
		return FALSE;
	}

	return SetPixelFormat(pDC->GetSafeHdc(),nPixelFormat,&pfd);
}


BOOL CIsothermBmp::InitOpenGL(CWnd *pWnd)
{
	//////////////////////////////����OpenGL������///////////////////////////////////////////////
	//����DC
	m_pDC = new CClientDC(this);
	ASSERT(m_pDC!=NULL);

	//ѡ�����ظ�ʽ
	if (!SetDCPixelFormat(m_pDC))
	{
		return FALSE;
	}
	//������Ⱦ��������ʹ���Ϊ��ǰ��Ⱦ����
	m_hRC = wglCreateContext(m_pDC->GetSafeHdc());
	if (m_hRC == NULL)
	{
		return FALSE;
	}
	if (wglMakeCurrent(m_pDC->GetSafeHdc(),m_hRC) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

//void CIsothermBmp::OnSize(UINT nType, int cx, int cy)
//{
//	CStatic::OnSize(nType, cx, cy);
//
//	// TODO: Add your message handler code here
//	GLsizei width,height;
//	GLdouble aspect;
//	width = cx;
//	height = cy;
//
//	if(cy==0)
//	{
//		aspect = (GLdouble)width;
//	}
//	else
//	{
//		aspect = (GLdouble)width/(GLdouble)height;
//	}
//	glViewport(0,0,width,height);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//
//	//gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);//͸��ͶӰ
//
//	gluOrtho2D(0.0,width,-10.0,height);
//	gluOrtho2D(0.0,width-420*XCOFF,-10.0,height);
//	//	int w = 1000*XCOFF;
//	//	int h = 640*YCOFF;
//	//	gluOrtho2D(0.0,w,0.0,h);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//}