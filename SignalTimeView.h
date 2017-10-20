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
	CSignalTimeBmp m_SignalTime2;
	afx_msg void OnPaint();
	int m_iSelPathOrderEdit;

	CSpinButtonCtrl m_selPathSpin;
	afx_msg void OnChangeSelpathEdit();
	CString m_sReceiveModule;
	CString m_sSendModule;
	afx_msg void OnBnClickedSenddisBtn();
	afx_msg void OnBnClickedReceivedisBtn();
private:
	BOOL m_bSendModuleDisFlag;
	BOOL m_bReceiveModuleDisFlag;
	BOOL m_bSendModuleDisFlag2;
	BOOL m_bReceiveModuleDisFlag2;
public:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnUpdateReturn(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSetting(CCmdUI *pCmdUI);
	CComboBox m_SelRecArmCob;
	CComboBox m_SelSendArmCob;
	CComboBox m_SelRecArmCob2;
	CComboBox m_SelSendArmCob2;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	CBrush m_brush;
	UINT m_iSendArmModule;
	UINT m_iRecArmModule;
	UINT m_iSendArmModule2;
	UINT m_iRecArmModule2;
	afx_msg void OnSelchangeReceivemoduleCob();
	afx_msg void OnSelchangeSendmoduleCob();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeSendmoduleCob2();
	afx_msg void OnCbnSelchangeReceivemoduleCob2();
	afx_msg void OnBnClickedSenddisBtn2();
	afx_msg void OnBnClickedReceivedisBtn2();
};


