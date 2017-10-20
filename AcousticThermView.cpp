
// AcousticThermView.cpp : CAcousticThermView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "AcousticTherm.h"
#endif

#include "AcousticThermDoc.h"
#include "AcousticThermView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAcousticThermView

IMPLEMENT_DYNCREATE(CAcousticThermView, CView)

BEGIN_MESSAGE_MAP(CAcousticThermView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CAcousticThermView ����/����

CAcousticThermView::CAcousticThermView()
{
	// TODO: �ڴ˴���ӹ������

}

CAcousticThermView::~CAcousticThermView()
{
}

BOOL CAcousticThermView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CAcousticThermView ����

void CAcousticThermView::OnDraw(CDC* /*pDC*/)
{
	CAcousticThermDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CAcousticThermView ��ӡ

BOOL CAcousticThermView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CAcousticThermView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CAcousticThermView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CAcousticThermView ���

#ifdef _DEBUG
void CAcousticThermView::AssertValid() const
{
	CView::AssertValid();
}

void CAcousticThermView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAcousticThermDoc* CAcousticThermView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAcousticThermDoc)));
	return (CAcousticThermDoc*)m_pDocument;
}
#endif //_DEBUG


// CAcousticThermView ��Ϣ�������
