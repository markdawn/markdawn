#pragma once
#ifndef DCLASS_H
#define DCLASS_H
#endif
#include "WaveFilter.h"
#include "alcommon.h"
class DataClass:public WaveFilter
{
public:
	//static const int N=5120;
	DataClass(void);
	~DataClass(void);
	void DC_ReadData(float data[]);								// 读入数据
	void DC_GetData(float data[]);								// 得到数据
	int DC_Init(bool FF,bool NF,bool VF,int number,int SR);		// 初始化
	void DC_GetVolt(float data[],int number);					// 得到电压
	int DC_CalcTime(float Value,int SPoint,int EPoint,int SoundFlag);			// 接收数据处理
	unsigned int Nlength;

	int RMS();													//RMS 
	int RMSLen1,RMSLen2;										//len1:RMS尾部截断 len2:RMS长度
	float VmsData[fN];											//Wave after RMS
	void VmsNto1(int Nbegin,int Nend);								// 归一化
	int sub_2(int StartPoint,int EndPoint);
protected:
	void Nto1(int Nbegin,int Nend);								// 归一化
	void Amplifer(unsigned int Nlen,int SFlag);					// 放大
private:
	float Vdata[fN];											// 电压值
	int SampRate;
	bool Nto1Flag,VoltFlag,FilterFlag;							// 归一化标志，电压标志，滤波标志
	float DataMax;
	float coef;													// 放大系数
};

