#pragma once


typedef CList<float,float>ContourValueList;//�洢��ֵ��ֵ

class CContourValues
{
public:
	CContourValues(void);
	~CContourValues(void);
	// ��ӵ�ֵ��ֵ
	void AddContourValue(float newValue);
	ContourValueList m_ContourValueList;
	// �õ��������������ֵ�ĸ���
	int GetNumOfContourValues(void);
	float GetContourValueFromList(int iOrder);
	// ��������ǰ��Ԥ����
//	void PreProcess(CRect rRect, WPARAM wparam, LPARAM lparam);
};

