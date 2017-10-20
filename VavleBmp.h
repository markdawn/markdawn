#pragma once


// CVavleBmp

class CVavleBmp : public CStatic
{
	DECLARE_DYNAMIC(CVavleBmp)

public:
	CVavleBmp();
	virtual ~CVavleBmp();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
public:
	CBitmap m_Bmp;
	void GetPoint(POINT* pPoint, CRect rRect);
	

	void DrawFrame(CDC* pDC, CRect rRect);


	void DrawSounder(CDC* pDC, POINT* pPoint);
	void DrawSensor(CDC* pDC, POINT* pPoint);
	//void DrawPath(CDC* pDC, POINT pPointStart, POINT pPointEnd,UINT m,UINT n,UINT tempValue);
	void DrawVavle(CDC* pDC, CRect rRect);
	POINT* m_point;
	CFont m_newFont;
	CFont *m_pOldFont;
	int lineDis;
	void UpdateState(WPARAM wparam, LPARAM lparam);
	void DisCommState(CDC* pDC);



};


