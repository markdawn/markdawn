#pragma once


// CBoilerBmp

class CBoilerBmp : public CStatic
{
	DECLARE_DYNAMIC(CBoilerBmp)

public:
	CBoilerBmp();
	virtual ~CBoilerBmp();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
public:
	CBitmap m_Bmp;

};


