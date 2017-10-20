#include "StdAfx.h"
#include "RebuildAreaT.h"
#include "Algorithm/interpolate.h"
#include "MainFrm.h"

#pragma warning(disable:4305)
CRebuildAreaT::CRebuildAreaT(void)
{
	int i=0,j= 0;
	counter=0;
	m_fSavePairSignalData = new float*[2];
	for ( i = 0; i < 2; i++)
	{
		m_fSavePairSignalData[i] = new float[N_DATALENTH];
	}

	m_fSaveFlightTime = new float*[1];
	for ( i = 0; i < 1; i++)
	{
		m_fSaveFlightTime[i] = new float[PATH_NUM];
	}
	
}

CRebuildAreaT::~CRebuildAreaT(void)
{
	for (int i = 0; i < 2; i++)
	{
		delete m_fSavePairSignalData[i];
	}
	delete[]m_fSavePairSignalData;
	delete[]m_fSaveFlightTime;

}




// 计算网格数据:输入区域平均温度，输出网格数据以及网格数据信息GridBaiscInfo
BOOL CRebuildAreaT::CalTGridData(UINT* fAreaT, float** fPGridBuff, int nAreaNum, GridBaiscInfo* gridInfo)
{
	gridInfo->col = GRID_COLUMN_NUM;
	gridInfo->row = GRID_ROW_NUM;
	gridInfo->width = GRID_COLUMN_NUM - 1;
	gridInfo->hight = GRID_ROW_NUM - 1;
	gridInfo->maxValue = 2000;
	gridInfo->minValue = 1;

	//m_getIsothermData.ReadIsothermData(fPGridBuff, gridInfo);
	int i, j = 0;
	//计算网格数据
	if (fAreaT[0] == 0 && fAreaT[1] == 0 && fAreaT[2] == 0 && fAreaT[3] == 0 && fAreaT[4] == 0 && fAreaT[5] == 0)
	{
		//SideLineT = 0.0f;
		for (i = 0; i < GRID_ROW_NUM; i++)
		{
			for (j = 0; j < GRID_COLUMN_NUM - 25; j++)
			{
				fPGridBuff[i][j] = 0.0f;
			}
		}
		InterTGridDataFlag[0] = FALSE;
	}
	else
	{
		//左半边插值计算
		SideLineT = SIDETEMP;
		InterTGridDataCol(fAreaT, fPGridBuff);
		InterTGridDataRow(fAreaT, fPGridBuff, gridInfo);
		InterTGridDataFlag[0] = TRUE;
	}

	if (fAreaT[6] == 0 && fAreaT[7] == 0 && fAreaT[8] == 0 && fAreaT[9] == 0 && fAreaT[10] == 0 && fAreaT[11] == 0)
	{
		//SideLineT = 0.0f;
		for (i = 0; i < GRID_ROW_NUM; i++)
		{
			for (j = 26; j < GRID_COLUMN_NUM; j++)
			{
				fPGridBuff[i][j] = 0.0f;
			}
		}
		InterTGridDataFlag[1] = FALSE;
	}
	else
	{
		//右半边插值计算
		SideLineT = SIDETEMP;
		InterTGridDataCol2(fAreaT, fPGridBuff);
		InterTGridDataRow2(fAreaT, fPGridBuff, gridInfo);
		InterTGridDataFlag[1] = TRUE;
	}
	
	//网格数据不为零，保存网格数据
	if (InterTGridDataFlag[0] == TRUE || InterTGridDataFlag[1] == TRUE)
	{
		m_getIsothermData.SaveIsothermData(fPGridBuff, gridInfo);
		counter++;
	}

	if (counter >= 1000)
	{
		counter = 5;
	}
	return TRUE;
}

//对2, 7，19列进行插值
void CRebuildAreaT::InterTGridDataCol(UINT* fAreaT, float** fPGridBuff)
{

	double x19[40] = { 0, 8, 16, 24, 32, 0, 0, 0, 0, 0 };
	double y19[40] = { fAreaT[3]*0.9, fAreaT[3], fAreaT[4], fAreaT[5], fAreaT[5]*0.9, 0, 0, 0, 0, 0 };
	int i, j = 0;
	interpolate interp1DCol19;
	i = interp1DCol19.init(x19, y19, 5);
	i = interp1DCol19.interp(33);
	ASSERT(fPGridBuff != NULL);

	for (i = 0; i < 31; i++)
	{
		fPGridBuff[i][19] = float(interp1DCol19.yout[i]);
	}
	
	double x7[40] = { 0, 8, 16, 24, 32, 0, 0, 0, 0, 0 };
	double y7[40] = { fAreaT[1] *0.9, fAreaT[1], fAreaT[4], fAreaT[2], fAreaT[2]*0.9, 0, 0, 0, 0, 0 };
	

	interpolate interp1DCol7;
	i = interp1DCol7.init(x7, y7, 5);
	i = interp1DCol7.interp(33);
	for (i = 0; i < 31; i++)
	{
		fPGridBuff[i][7] = float(interp1DCol7.yout[i]);
	}


	double x2[40] = { 0, 8, 16, 24, 32, 0, 0, 0, 0, 0 };
	double y2[40] = { fAreaT[1] *0.9, fAreaT[1] *0.95, fAreaT[0], fAreaT[2], fAreaT[2]*0.9, 0, 0, 0, 0, 0 };

	interpolate interp1DCol2;
	i = interp1DCol2.init(x2, y2, 5);
	i = interp1DCol2.interp(33);
	for (i = 0; i < 31; i++)
	{
		fPGridBuff[i][2] = float(interp1DCol2.yout[i]);
	}
}

//对左半边行进行插值
void CRebuildAreaT::InterTGridDataRow(UINT* fAreaT, float** fPGridBuff, GridBaiscInfo* gridInfo)
{
	UINT cols = gridInfo->col;
	UINT rows = gridInfo->row;
	double offsetT = 100;
	double x[40] = { 0, 8, 16, 24, 0, 0, 0, 0, 0, 0 };
	interpolate interp1DRow;
	UINT i, j;
	ASSERT(fPGridBuff != NULL);

	for (i = 0; i < rows; i++)
	{
		double rowT0 = fPGridBuff[i][2];
		double rowT1 = fPGridBuff[i][7];
		double rowT2 = fPGridBuff[i][19];
		double y[40] = { SideLineT, rowT0, rowT1, rowT2 };

		j = interp1DRow.init(x, y, 4);
		j = interp1DRow.interp(25);
		for (j = 0; j < 25; j++)
		{
			fPGridBuff[i][j] = interp1DRow.yout[j];
		}
	}
}

//对32，44,48列进行插值
void CRebuildAreaT::InterTGridDataCol2(UINT* fAreaT, float** fPGridBuff)
{
	double x32[40] = { 0, 8, 16, 24, 32, 0, 0, 0, 0, 0 };
	double y32[40] = { fAreaT[9] *0.9, fAreaT[9], fAreaT[10], fAreaT[11], fAreaT[11]*0.9, 0, 0, 0, 0, 0 };
	int i;
	interpolate interp1DCol32;
	i = interp1DCol32.init(x32, y32, 5);
	i = interp1DCol32.interp(33);
	ASSERT(fPGridBuff != NULL);

	for (i = 0; i < 31; i++)
	{
		fPGridBuff[i][32] = interp1DCol32.yout[i];
	}

	double x44[40] = { 0, 8, 16, 24, 32, 0, 0, 0, 0, 0 };
	double y44[40] = { fAreaT[7] *0.9, fAreaT[7], fAreaT[10], fAreaT[8], fAreaT[8]*0.9, 0, 0, 0, 0, 0 };
	
	interpolate interp1DCol44;
	i = interp1DCol44.init(x44, y44, 5);
	i = interp1DCol44.interp(33);
	for (i = 0; i < 31; i++)
	{
		fPGridBuff[i][44] = interp1DCol44.yout[i];
	}

	double x48[40] = { 0, 8, 16, 24, 32, 0, 0, 0, 0, 0 };
	double y48[40] = { fAreaT[6] *0.9, fAreaT[8] *0.95, fAreaT[6], fAreaT[7] *0.9, fAreaT[6] *0.9, 0, 0, 0, 0, 0 };

	interpolate interp1DCol48;
	i = interp1DCol48.init(x48, y48, 5);
	i = interp1DCol48.interp(33);
	for (i = 0; i < 31; i++)
	{
		fPGridBuff[i][48] = interp1DCol48.yout[i];
	}

}

//对右半边行进行插值
void CRebuildAreaT::InterTGridDataRow2(UINT* fAreaT, float** fPGridBuff, GridBaiscInfo* gridInfo)
{
	UINT cols = gridInfo->col;
	UINT rows = gridInfo->row;
	double offsetT = 100;
	double x[40] = { 0, 8, 16, 24, 0, 0, 0, 0, 0, 0 };
	UINT i, j;
	interpolate interp1DRow;
	ASSERT(fPGridBuff != NULL);

	for (i = 0; i < rows; i++)
	{
		double rowT0 = fPGridBuff[i][32];
		double rowT1 = fPGridBuff[i][44];
		double rowT2 = fPGridBuff[i][48];
		double y[40] = { rowT0, rowT1, rowT2, SideLineT };

		j = interp1DRow.init(x, y, 4);
		j = interp1DRow.interp(25);
		for (j = 26; j < cols; j++)
		{
			fPGridBuff[i][j] = interp1DRow.yout[j - 26];
		}
	}
}





