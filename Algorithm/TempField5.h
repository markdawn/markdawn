#pragma once
#ifndef TFIELD5_H
#define TFIELD5_H
#endif
#include "eqation.h"
#include "alcommon.h"
class TempField5 :
	public eqation
{
public:
	TempField5(void);
	~TempField5(void);
	void BuildAreaT_LSM(bool sflag);				// ���������¶�ֵ
	void BuildAreaT();
private:
	float fabs(float fnum);
	float sigmoid(float x);
	float sigmoid(float x,float sigtemp,float toptemp);
	void ProcessResult();			// �������¶Ƚ�����ж��δ���
	float P[8];						//PΪ�ɶ�ʱ��ת��Ϊ1ά���󣬷��������㣬8Ϊ�ɶ�·������
	float DP[8];					//P������·��
	float averTold[8];

public:
	int NumTempField;
	float MyTraveTime[M][M];		// ����ɶ�ʱ��
	float Ttime[M+1][M+1];
	float da[2*M][4*M];
	float daOld[M];
	float f_da[M];
	float Z;
	float AreaT[M];					// �����¶�ֵ
	float S[6][8];
	float aver_T_P[16];
	float aver_P[16];
	//����1��2��3�����¶ȵ���
	//float ib[3][3];                //��12��25��15��
	//float AreaTB[3];
	//float PB[3];
	float SigmoidTemp;
};

