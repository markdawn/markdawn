#pragma once
#include "TTrendBmp.h"
#include "selttrendtimedlg.h"


// CTTrendView form view

class CTTrendView : public CFormView
{
	DECLARE_DYNCREATE(CTTrendView)

public:
	CTTrendView();           // protected constructor used by dynamic creation
	virtual ~CTTrendView();
protected:
public:
	enum { IDD = IDD_TTREND_DLG };
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
	CTTrendBmp m_TTrendBmp;
	virtual void OnInitialUpdate();

protected:
	afx_msg LRESULT OnTTrend(WPARAM wparam,LPARAM lparam);
private:
	// A通道......
	UINT m_iChanelA;
	UINT m_iChanelB;
	UINT m_iChanelC;
	UINT m_iChanelD;
	UINT m_iChanelE;
	UINT m_iChanelF;
	UINT m_iChanelG;
	UINT m_iChanelH;
	UINT m_iChanelI;
	UINT m_iChanelJ;
	UINT m_iChanelK;
	UINT m_iChanelL;
public:
	// 平均温度趋势显示的起始时间
	StartEndTime m_tStartEndTime;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedAreat1Btn();
	afx_msg void OnBnClickedAreat2Btn();
	afx_msg void OnBnClickedAreat3Btn();
	afx_msg void OnBnClickedAreat4Btn();
	afx_msg void OnBnClickedAreat5Btn();
	afx_msg void OnBnClickedAreat6Btn();
	afx_msg void OnBnClickedAreat7Btn();
	afx_msg void OnBnClickedAreat8Btn();
	afx_msg void OnBnClickedAreat9Btn();
	afx_msg void OnBnClickedAreat10Btn();
	afx_msg void OnBnClickedAreat11Btn();
	afx_msg void OnBnClickedAreat12Btn();
	//afx_msg void OnBnClickedAreat13Btn();
	//afx_msg void OnBnClickedAreat14Btn();
	//afx_msg void OnBnClickedAreat15Btn();
	//afx_msg void OnBnClickedAreat16Btn();
private:
	BOOL  m_bDisAreaT1CurveFlag;
	BOOL  m_bDisAreaT2CurveFlag;
	BOOL  m_bDisAreaT3CurveFlag;
	BOOL  m_bDisAreaT4CurveFlag;
	BOOL  m_bDisAreaT5CurveFlag;
	BOOL  m_bDisAreaT6CurveFlag;
	BOOL  m_bDisAreaT7CurveFlag;
	BOOL  m_bDisAreaT8CurveFlag;
	BOOL  m_bDisAreaT9CurveFlag;
	BOOL  m_bDisAreaT10CurveFlag;
	BOOL  m_bDisAreaT11CurveFlag;
	BOOL  m_bDisAreaT12CurveFlag;
	//BOOL  m_bDisAreaT13CurveFlag;
	//BOOL  m_bDisAreaT14CurveFlag;
	//BOOL  m_bDisAreaT15CurveFlag;
	//BOOL  m_bDisAreaT16CurveFlag;

public:
	afx_msg void OnBnClickedSeltimeBtn();
	
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnUpdateReturn(CCmdUI *pCmdUI);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


