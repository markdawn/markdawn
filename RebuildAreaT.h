#pragma once
#include "commonFile.h"
#include "getisothremdata.h"

#include "Algorithm\eqation.h"




//�ع��¶ȳ�����������ƽ���¶�,��������
class CRebuildAreaT
{
public:
	CRebuildAreaT(void);
	~CRebuildAreaT(void);
public:
	//�¶ȳ�դ�����ݼ���
	// �����������ݣ���������ƽ���¶�
	BOOL CalTGridData(UINT* fAreaT, float** fPGridBuff, int nAreaNum, GridBaiscInfo* gridInfo);
	// ����д��������
	CGetIsothremData m_getIsothermData;
public:
	//����ƽ���¶ȼ���
	//BOOL CalAreaAvrageT(int*** piTimeIn, UINT* iAreaT, int nAreaNum, float* f_da, BOOL* bTravelTimeFlag);

	//CCalAreaAverageT m_CalAreaAverageT;
	float** m_fSavePairSignalData;//����·�����˵�һ�����ݣ���������һ��·���ɶ�ʱ��
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

