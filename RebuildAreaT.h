#pragma once
#include "commonFile.h"
#include "getisothremdata.h"

#include "Algorithm\eqation.h"




//重构温度场，计算区域平均温度,网格数据
class CRebuildAreaT
{
public:
	CRebuildAreaT(void);
	~CRebuildAreaT(void);
public:
	//温度场栅格数据计算
	// 计算网格数据，输入区域平均温度
	BOOL CalTGridData(UINT* fAreaT, float** fPGridBuff, int nAreaNum, GridBaiscInfo* gridInfo);
	// 读和写网格数据
	CGetIsothremData m_getIsothermData;
public:
	//区域平均温度计算
	//BOOL CalAreaAvrageT(int*** piTimeIn, UINT* iAreaT, int nAreaNum, float* f_da, BOOL* bTravelTimeFlag);

	//CCalAreaAverageT m_CalAreaAverageT;
	float** m_fSavePairSignalData;//保存路径两端的一对数据，用来计算一个路径飞渡时间
	//BOOL CalAllPathFlightTime(int***piTimeIn, UINT* iAreaT, float* f_da, BOOL* bTravelTimeFlag);



	float** m_fSaveFlightTime;
	void InterTGridDataCol(UINT* fAreaT, float** fPGridBuff);
	void InterTGridDataRow(UINT* fAreaT, float** fPGridBuff, GridBaiscInfo* gridInfo);
	void InterTGridDataCol2(UINT* fAreaT, float** fPGridBuff);
	void InterTGridDataRow2(UINT* fAreaT, float** fPGridBuff, GridBaiscInfo* gridInfo);

	//float** m_pCalTGridData;

	double SideLineT;
	//DataClass myData;
	//TravelTime m_TraveTime;
	//eqation EQ;
	//eqation EQ_B;
	//float MyTimeData[8][8];
	//float MyTraveTime[8][8];
	//float Ttime[9][9];
	int counter;
	//float daOld[5];
	//float daOld_B[5];
	bool InterTGridDataFlag[2];
};

