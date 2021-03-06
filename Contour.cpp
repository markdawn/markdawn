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



// 画等温线
void CContour::DrawContour(CDC* pDC, CRect rRect, WPARAM wparam, LPARAM lparam)
{
	/**********************(1)得到网格数据及数据基本信息***********************************/

	ASSERT(wparam);
	m_ContourTrace.GetGridData((float**) wparam);//设置追踪等温线时需要用到的栅格数据
	ASSERT(lparam);
	GridBaiscInfo *pGridInfo = (GridBaiscInfo*)lparam;
	m_gridDataInfo = *pGridInfo;
	if (m_gridDataInfo.col == 0 || m_gridDataInfo.row == 0)
	{
		return;
	}
	m_ContourTrace.GetGridBasicInfo(m_gridDataInfo);//设置追踪等温线时需要用到的栅格信息
	
	/**********************(2)计算等温线值及等温线条数*************************************/
	GenerateContourValues();
	/**********************(3)产生等值线***************************************************/
	GenerateContours();
	/**********************(4)画等值线*****************************************************/
	OnDraw(pDC,rRect,wparam,lparam);
}

//产生10个给定的等值线值
void CContour::GenerateContourValues(void)
{
	ASSERT(m_getIsothermData != NULL);
	float zMin = m_gridDataInfo.minValue;
	float zMax = m_gridDataInfo.maxValue;

	float inc = (zMax - zMin) / 10; //10等份

	float val = 0.0f;

	for (val = zMin; val <= zMax; val += inc)
	{
		m_ContourValues.AddContourValue(val);
	}
}
// 产生等值线
void CContour::GenerateContours(void)
{
	/*******************得到所需要显示温度的个数**********************************/
	int contourNum = m_ContourValues.GetNumOfContourValues();   //5个
	if (contourNum<=0)
	{
		return;
	}

	/*******************先清空等温线集合**************************************/
	if (ArrayIsEmpty()) 
	{
		RemoveAllContours();
	}

	/********************追踪等值线********************************************/
	CCurveList *pCurveList;
	for(int i=0; i<contourNum; i++)
	{
		pCurveList = new CCurveList;
		if(pCurveList==NULL)
		{
			AfxMessageBox(_T("内存分配失败!"));
			delete pCurveList;
			return;
		}
		m_ContourTrace.SetCurveList(pCurveList);
		//根据当前设定的温度执行追踪等值线
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


// 判断等温线集合是否为空
BOOL CContour::ArrayIsEmpty(void)
{
	BOOL bFlag = FALSE;
	if (m_ContourCurves.GetSize()>0)
	{
		bFlag = TRUE;
	}
	return bFlag;
}


// 清空等温线集合
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



// 得到栅格信息
GridBaiscInfo CContour::GetGridBaseInfo(void)
{
	return m_gridDataInfo;
}


CContourArray& CContour::GetContours(void)
{
	return m_ContourCurves;
}


// 获取栅格数据指针
/*
float** CContour::GetGridDataPointer(void)
{
	//TODO: insert return statement here
   ASSERT(m_pIsothermData);
	return m_pIsothermData;
}
*/
