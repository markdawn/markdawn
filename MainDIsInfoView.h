#pragma once
#include "MainDisBmp.h"
#include "LogoBmp.h"
#include "BoilerBmp.h"
#include "ArmModuleBmp.h"
#include "ArmModule2Bmp.h"
#include "VavleBmp.h"
#include "IsothermBmp.h"
#include "AreaAvrgTemperatureBmp.h"
#include "led.h"


// CMainDIsInfoView form view

class CMainDIsInfoView : public CFormView
{
	DECLARE_DYNCREATE(CMainDIsInfoView)

protected:
	CMainDIsInfoView();           // protected constructor used by dynamic creation
	virtual ~CMainDIsInfoView();

public:
	enum { IDD = IDD_MAINDIS_DLG };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg LRESULT OnDisplayAllPartsBmp(WPARAM wparam,LPARAM  lparam);
public:
	void InitMainDisInfo();
	void InitBoilerPointPos();
	void InitArmModule();
	void InitIsothermView();
	void InitAreaAvrgTView();
	void InitValveAndPipe();
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
public:
	CMainDisBmp m_MainDispalyBmp;
	CLogoBmp m_LogoDisplayBmp;
	CArmModuleBmp m_ArmModuleDisplayBmp;
	CAreaAvrgTemperatureBmp m_AreaTDisplayBmp;
	CIsothermBmp m_IsothermDisplayBmp;
	CBoilerBmp m_BolierDisplayBmp;
	CVavleBmp m_ValveDisplayBmp;
	CArmModule2Bmp m_ArmModule2DisplayBmp;

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
public:
	// OpenGL��ͼ
	void DrawScene(WPARAM wparam,LPARAM  lparam);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	

	//POINT* Led_Point;
	CLed m_Led1;
	CLed m_Led2;
	CLed m_Led3;
	CLed m_Led4;
	CLed m_Led5;
	CLed m_Led6;
	CLed m_Led7;
	CLed m_Led8;
	CLed m_Led9;
	CLed m_Led10;
	CLed m_MainLed;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	void StartTimer(UINT_PTR nIDEvent ,UINT nElapse);
	void StopTimer(UINT_PTR nIDEvent );
};


