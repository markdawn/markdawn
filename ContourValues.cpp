#include "StdAfx.h"
#include "ContourValues.h"


CContourValues::CContourValues(void)
{
}


CContourValues::~CContourValues(void)
{

}


// ��ӵ�ֵ��ֵ
void CContourValues::AddContourValue(float newValue)
{
	if (m_ContourValueList.IsEmpty())
	{
		m_ContourValueList.AddHead(newValue);
	}
	else
	{
		float valueTmp = 0.0f;
		POSITION pos = m_ContourValueList.GetHeadPosition();
		valueTmp = m_ContourValueList.GetAt(pos);

		if (valueTmp > newValue)
		{
			m_ContourValueList.AddHead(newValue);
			return;
		}

		pos = m_ContourValueList.GetTailPosition();
		valueTmp = m_ContourValueList.GetAt(pos);
		if (valueTmp<newValue)
		{
			m_ContourValueList.AddTail(newValue);
			return;
		}

		POSITION nextPos = m_ContourValueList.GetHeadPosition();
		pos = nextPos;
		float nextVal = 0.0f;
		while(nextPos!=NULL)
		{
			valueTmp = m_ContourValueList.GetNext(nextPos);//nextPos+1
			if (valueTmp == newValue)
			{
				return;
			}
			if (nextPos != NULL)
				return;

			nextVal = m_ContourValueList.GetAt(nextPos);
			if (nextVal == newValue)
			{
				return;
			}
			if (newValue>valueTmp && newValue<nextVal)
			{
				m_ContourValueList.InsertAfter(pos,newValue);
				return;
			}
			pos = nextPos;//��¼Ҫ�����pos
		}
	}
	
}


// �õ��������������ֵ�ĸ���
int CContourValues::GetNumOfContourValues(void)
{
	int num = m_ContourValueList.GetCount();
	return num;
}

// ���б��еõ�ָ��λ�õĵ���ֵ
float CContourValues::GetContourValueFromList(int iOrder)
{
	float fValue = 0.0f;
	ASSERT( iOrder>=0 && iOrder<GetNumOfContourValues() );
	POSITION pos = m_ContourValueList.FindIndex(iOrder);
	fValue = m_ContourValueList.GetAt(pos);
	return fValue;
}

