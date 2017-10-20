#pragma once


// CLogoBmp

class CLogoBmp : public CStatic
{
	DECLARE_DYNAMIC(CLogoBmp)

public:
	CLogoBmp();
	virtual ~CLogoBmp();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
public:
	CBitmap m_Bmp;
};


