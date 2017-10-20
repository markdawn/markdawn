#pragma once
#ifndef FTIME_H
#define FTIME_H
#endif

#include "alcommon.h"
class FlightTime
{
public:
	FlightTime(void);
	~FlightTime(void);

public:
	int iTime[iNumArm][iNumSound];					// iTime[sound][receive][times]
	float FTime[iNumArm][iNumArm];					// Flight Time 结果返回值
public:

	int CalcFlightTime(int arithflag);						// 计算飞渡时间
	int iSound;										// 发声模块Num
	int ValidCount[iNumArm][iNumArm];				// the valid values in 5 times sounds
private:
	int pFTime[iNumArm][iNumArm];
	void AfterProcessData(int iSound,int iRec);
	int PreProcessData(int n);						// 路径数据飞渡处理函数
	float ThreeInFive(int* data,int num,int imode);	// 每条路径多次飞渡时间优化值	
	int Max(int* data,int num);					// int最大值
	float Max(float* data,int num);				// float最大值
	int Min(int* data,int num);					// int最小值
	float Min(float* data,int num);				// float最小值
	int Pdata[iNumArm][iNumArm][iNumSound];			// one couple flight time of every sound
	int MyData[2][iNumSound];						// 飞渡时间计算中间存储单元
};
