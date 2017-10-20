
// AcousticThermView.cpp : CAcousticThermView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CAcousticThermView 构造/析构

CAcousticThermView::CAcousticThermView()
{
	// TODO: 在此处添加构造代码

}

CAcousticThermView::~CAcousticThermView()
{
}

BOOL CAcousticThermView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CAcousticThermView 绘制

void CAcousticThermView::OnDraw(CDC* /*pDC*/)
{
	CAcousticThermDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CAcousticThermView 打印

BOOL CAcousticThermView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CAcousticThermView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CAcousticThermView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CAcousticThermView 诊断

#ifdef _DEBUG
void CAcousticThermView::AssertValid() const
{
	CView::AssertValid();
}

void CAcousticThermView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAcousticThermDoc* CAcousticThermView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAcousticThermDoc)));
	return (CAcousticThermDoc*)m_pDocument;
}
#endif //_DEBUG


// CAcousticThermView 消息处理程序
