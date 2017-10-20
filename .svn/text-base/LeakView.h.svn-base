#pragma once

#include "LeakBmp.h"

// CLeakView form view

class CLeakView : public CFormView
{
	DECLARE_DYNCREATE(CLeakView)

public:
	CLeakView();           // protected constructor used by dynamic creation
	virtual ~CLeakView();
protected:
public:
	enum { IDD = IDD_LEAK_DLG };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	CLeakBmp m_LeakBmp;

protected:
	afx_msg LRESULT OnLeak(WPARAM wparam, LPARAM lparam);
private:
	time_t m_tEndTime;
	time_t m_tStartTime;
public:
	afx_msg void OnPaint();
};


