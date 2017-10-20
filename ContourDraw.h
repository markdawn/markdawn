#pragma once

#include "commonFile.h"
#include "atltypes.h"
#include "lookupcolor.h"

typedef  CArray<ContourPoint,ContourPoint&>CCurve;//��̬�洢һ�������ߵ���Ϣ
typedef CTypedPtrList<CPtrList, CCurve*>CCurveList;//��̬�洢һ���¶�ֵ�ж��������


class CContourDraw
{
public:
	CContourDraw(void);
	~CContourDraw(void);
	// ����ֵ��
	void DrawContours(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam,LPVOID lpvoid);
	// ��������ǰ��Ԥ����
	void PreProcess(CRect rRect,WPARAM wparam, LPARAM lparam);
protected:
	// դ����Ϣ
	GridBaiscInfo m_GridInfo;
public:
	// �������߱����߿�
	void DrawBackground(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam);
protected:
	// �����������򣬰�ʵ�ʱ���
	CRect m_rect;
public:
	// ��������
	void DrawContourCurve(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam,LPVOID lpvoid);
	void DrawContourCurveText(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam, LPVOID lpvoid);
	// ���������߶����Է��㻭������
	void CreateContourObj(WPARAM wparam,LPARAM lparam,LPVOID lpvoid);
	// //�������е����߶����Ա㻭������
	CCurveList* m_pCurveList;
	CLookUpColor m_LookUpColor;
private:
	// դ������
	float** m_ppGridData;
public:
	void ReMoveAll(void);
	void drawString(const char* str);
//	CCurve* m_pCurve;
};

