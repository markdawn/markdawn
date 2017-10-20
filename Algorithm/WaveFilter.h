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
	void MyFilter();						// 滤波函数
	void SetFilterPara(						// 设置滤波参数
					float pa[],float pb[],int Nl);
public:	
	float InData[fN];						// 输入数据
	float OutData[fN];						// 输出数据
	float a[IIR_N],b[IIR_N];						// 参数
	UINT iNlength;							
	unsigned int Fil_N;
};
