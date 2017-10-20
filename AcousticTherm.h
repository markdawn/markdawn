
// AcousticTherm.h : AcousticTherm 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CAcousticThermApp:
// 有关此类的实现，请参阅 AcousticTherm.cpp
//

class CAcousticThermApp : public CWinApp
{
public:
	CAcousticThermApp();
	~CAcousticThermApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	// 数据库指针
	//_ConnectionPtr  m_pConnection;

private:
//	CSingleDocTemplate* m_pDocTemplate;
};

extern CAcousticThermApp theApp;
