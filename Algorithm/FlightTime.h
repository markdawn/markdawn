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
	float FTime[iNumArm][iNumArm];					// Flight Time �������ֵ
public:

	int CalcFlightTime(int arithflag);						// ����ɶ�ʱ��
	int iSound;										// ����ģ��Num
	int ValidCount[iNumArm][iNumArm];				// the valid values in 5 times sounds
private:
	int pFTime[iNumArm][iNumArm];
	void AfterProcessData(int iSound,int iRec);
	int PreProcessData(int n);						// ·�����ݷɶɴ�����
	float ThreeInFive(int* data,int num,int imode);	// ÿ��·����ηɶ�ʱ���Ż�ֵ	
	int Max(int* data,int num);					// int���ֵ
	float Max(float* data,int num);				// float���ֵ
	int Min(int* data,int num);					// int��Сֵ
	float Min(float* data,int num);				// float��Сֵ
	int Pdata[iNumArm][iNumArm][iNumSound];			// one couple flight time of every sound
	int MyData[2][iNumSound];						// �ɶ�ʱ������м�洢��Ԫ
};
