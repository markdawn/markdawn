#pragma once
#ifndef WaveFILTER_H
#define WaveFILTER_H
#endif
#include "alcommon.h"
#define IIR_N 9
class WaveFilter
{
public:
	//static const int fN=5120;
	WaveFilter(void);
	~WaveFilter(void);
	void MyFilter();						// �˲�����
	void SetFilterPara(						// �����˲�����
					float pa[],float pb[],int Nl);
public:	
	float InData[fN];						// ��������
	float OutData[fN];						// �������
	float a[IIR_N],b[IIR_N];						// ����
	UINT iNlength;							
	unsigned int Fil_N;
};
