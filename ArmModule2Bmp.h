#pragma once


// CArmModule2Bmp

class CArmModule2Bmp : public CStatic
{
	DECLARE_DYNAMIC(CArmModule2Bmp)

public:
	CArmModule2Bmp();
	virtual ~CArmModule2Bmp();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
public:
	CBitmap m_Bmp;
};


