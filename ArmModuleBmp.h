#pragma once


// CArmModuleBmp

class CArmModuleBmp : public CStatic
{
	DECLARE_DYNAMIC(CArmModuleBmp)

public:
	CArmModuleBmp();
	virtual ~CArmModuleBmp();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
public:
	CBitmap m_Bmp;
};


