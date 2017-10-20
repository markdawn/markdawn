#pragma once
#ifndef TFIELD_H
#define TFIELD_H
#endif
#include "eqation.h"
//#include "interpolate.h"
#include "alcommon.h"

class TempField:public eqation
{
private:
	//static const int M=5;
public:
	TempField(void);
	~TempField(void);
	void BuildAreaT();				// ���������¶�ֵ
private:
	void ProcessResult();			// �������¶Ƚ�����ж��δ���
public:
	int NumTempField;
	float MyTraveTime[M][M];		// ����ɶ�ʱ��
	float Ttime[M+1][M+1];
	float da[2*M][4*M];
	float daOld[M];
	float f_da[M];
	float Z;
	float AreaT[M];					// �����¶�ֵ
};