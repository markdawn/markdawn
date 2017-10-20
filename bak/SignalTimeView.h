#pragma once

#include "SignalTimeBmp.h"

// CSignalTimeView form view

class CSignalTimeView : public CFormView
{
	DECLARE_DYNCREATE(CSignalTimeView)

public:
	CSignalTimeView();           // protected constructor used by dynamic creation
	virtual ~CSignalTimeView();
protected:
public:
	enum { IDD = IDD_TIME_DLG };
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

protected:
	afx_msg LRESULT OnSignalTime(WPARAM wparam, LPARAM lparam);
public:
	CSignalTimeBmp m_SignalTime;
	afx_msg void OnPaint();
	afx_msg void OnEnChangeSelpathEdit();
public:
	// Â·¾¶ÐòºÅ
//	int m_iPathOrderEdit;
};


