#pragma once
#ifndef CTIME_H
#define CTIME_H
#endif

#include "DataClass.h"
#include "alcommon.h"
class CalTime:public DataClass
{
protected:

private:
	float RawData[iNumArm][iNumSound][fN];							// 原始读入值
	int CTPreProcessData(bool SToSFlag);								// 预处理函数

public:	
	int SaLen,SaCut,SabLen;											// 子串长度，子串起始位置，样本子串长度
	float lVal;														// 门限电平值
	int Time[iNumArm][iNumSound];									// 飞渡时间
	int MainSound;													// 发声模块号
	int iNArm,iNSound;												// 控制模块数量，发声次数
public:
	CalTime(void);
	~CalTime(void);
	void CalcTime();												// 计算飞渡时间
	int OneCalcTime(float value,int SPoint,int EPoint,int SoundFlag);				// 计算一条路径
	int ReadData(													// 读入数据
				float mydata[iNumArm][iNumArm][iNumSound][fN],		// 接收原始数据
				int iArm,int iSound);								// 模块数量， 发声次数
	void Init(unsigned int uSL,unsigned int uSC,					//初始化
				unsigned int Sb,float SF,float lV);
protected:
	int Find_First_Big(float data[],int num);
};

