
// AcousticThermView.h : CAcousticThermView ��Ľӿ�
//

#pragma once


class CAcousticThermView : public CView
{
protected: // �������л�����
	CAcousticThermView();
	DECLARE_DYNCREATE(CAcousticThermView)

// ����
public:
	CAcousticThermDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CAcousticThermView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // AcousticThermView.cpp �еĵ��԰汾
inline CAcousticThermDoc* CAcousticThermView::GetDocument() const
   { return reinterpret_cast<CAcousticThermDoc*>(m_pDocument); }
#endif

