#pragma once


typedef CList<float,float>ContourValueList;//存储等值线值

class CContourValues
{
public:
	CContourValues(void);
	~CContourValues(void);
	// 添加等值线值
	void AddContourValue(float newValue);
	ContourValueList m_ContourValueList;
	// 得到链表里面等温线值的个数
	int GetNumOfContourValues(void);
	float GetContourValueFromList(int iOrder);
	// 画等温线前的预处理
//	void PreProcess(CRect rRect, WPARAM wparam, LPARAM lparam);
};

