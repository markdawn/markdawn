#include "StdAfx.h"
#include "Contour.h"
#include "MainFrm.h"

CContour::CContour(void)
{
/*
	if (m_pIsothermData!=NULL)
	{
		m_pIsothermData = NULL;
	}

	m_pIsothermData = new float*[GRID_ROW_NUM];
	for (int i = 0; i<GRID_ROW_NUM;i++)
	{
		m_pIsothermData[i] = new float[GRID_COLUMN_NUM];
	}
*/
}


CContour::~CContour(void)
{
/*
	for (int i = 0;i<GRID_ROW_NUM;i++)
	{
		delete[] m_pIsothermData[i];
	}
	delete[] m_pIsothermData;
	m_pIsothermData = NULL;
	RemoveAllContours();
*/
}



// ��������
void CContour::DrawContour(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam)
{
	/**********************(1)�õ��������ݼ����ݻ�����Ϣ***********************************/

	ASSERT(wparam);
	m_ContourTrace.GetGridData((float**) wparam);//����׷�ٵ�����ʱ��Ҫ�õ���դ������
	ASSERT(lparam);
	GridBaiscInfo *pGridInfo = (GridBaiscInfo*)lparam;
	m_gridDataInfo = *pGridInfo;
	if (m_gridDataInfo.col == 0 || m_gridDataInfo.row == 0)
	{
		return;
	}
	m_ContourTrace.GetGridBasicInfo(m_gridDataInfo);//����׷�ٵ�����ʱ��Ҫ�õ���դ����Ϣ
	
	/**********************(2)���������ֵ������������*************************************/
	GenerateContourValues();
	/**********************(3)������ֵ��***************************************************/
	GenerateContours();
	/**********************(4)����ֵ��*****************************************************/
	OnDraw(pDC,rRect,wparam,lparam);
}

//����10�������ĵ�ֵ��ֵ
void CContour::GenerateContourValues(void)
{
	ASSERT(m_getIsothermData != NULL);
	float zMin = m_gridDataInfo.minValue;
	float zMax = m_gridDataInfo.maxValue;

	float inc = (zMax - zMin) / 10; //10�ȷ�

	float val = 0.0f;

	for (val = zMin; val <= zMax; val += inc)
	{
		m_ContourValues.AddContourValue(val);
	}
}
// ������ֵ��
void CContour::GenerateContours(void)
{
	/*******************�õ�����Ҫ��ʾ�¶ȵĸ���**********************************/
	int contourNum = m_ContourValues.GetNumOfContourValues();   //5��
	if (contourNum<=0)
	{
		return;
	}

	/*******************����յ����߼���**************************************/
	if (ArrayIsEmpty()) 
	{
		RemoveAllContours();
	}

	/********************׷�ٵ�ֵ��********************************************/
	CCurveList *pCurveList;
	for(int i=0; i<contourNum; i++)
	{
		pCurveList = new CCurveList;
		if(pCurveList==NULL)
		{
			AfxMessageBox(_T("�ڴ����ʧ��!"));
			delete pCurveList;
			return;
		}
		m_ContourTrace.SetCurveList(pCurveList);
		//���ݵ�ǰ�趨���¶�ִ��׷�ٵ�ֵ��
		if(!m_ContourTrace.ExecuteTracing(m_ContourValues.GetContourValueFromList(i))) 
		{
			delete pCurveList;
		}
		else
		{
			m_ContourCurves.Add(pCurveList);
		}		
		
	}

}


void CContour::OnDraw(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam)
{
	m_ContourDraw.DrawContours(pDC,rRect,wparam,lparam,(LPVOID)this);
}


// �жϵ����߼����Ƿ�Ϊ��
BOOL CContour::ArrayIsEmpty(void)
{
	BOOL bFlag = FALSE;
	if (m_ContourCurves.GetSize()>0)
	{
		bFlag = TRUE;
	}
	return bFlag;
}


// ��յ����߼���
void CContour::RemoveAllContours(void)
{
	int num = m_ContourCurves.GetSize();
	if( num > 0 )
	{
		CCurveList* pCurveList;
		for(int i=0; i<num; i++)
		{
			pCurveList = m_ContourCurves.GetAt(i);	
			ASSERT( pCurveList!=NULL );

			POSITION pos = pCurveList->GetHeadPosition();						
			while (pos!=NULL) 
			{
				delete pCurveList->GetNext(pos);
			}
			delete pCurveList;
		}

		m_ContourCurves.RemoveAll();
	}
}



// �õ�դ����Ϣ
GridBaiscInfo CContour::GetGridBaseInfo(void)
{
	return m_gridDataInfo;
}


CContourArray& CContour::GetContours(void)
{
	return m_ContourCurves;
}


// ��ȡդ������ָ��
/*
float** CContour::GetGridDataPointer(void)
{
	//TODO: insert return statement here
   ASSERT(m_pIsothermData);
	return m_pIsothermData;
}
*/