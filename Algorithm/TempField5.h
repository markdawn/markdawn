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
	void BuildAreaT_LSM(bool sflag);				// 计算区域温度值
	void BuildAreaT();
private:
	float fabs(float fnum);
	float sigmoid(float x);
	float sigmoid(float x,float sigtemp,float toptemp);
	void ProcessResult();			// 对区域温度结果进行二次处理
	float P[8];						//P为飞渡时间转化为1维矩阵，方便矩阵计算，8为飞渡路径数量
	float DP[8];					//P的冗余路径
	float averTold[8];

public:
	int NumTempField;
	float MyTraveTime[M][M];		// 输入飞渡时间
	float Ttime[M+1][M+1];
	float da[2*M][4*M];
	float daOld[M];
	float f_da[M];
	float Z;
	float AreaT[M];					// 区域温度值
	float S[6][8];
	float aver_T_P[16];
	float aver_P[16];
	//以下1、2、3区域温度单算
	//float ib[3][3];                //【12，25，15】
	//float AreaTB[3];
	//float PB[3];
	float SigmoidTemp;
};

