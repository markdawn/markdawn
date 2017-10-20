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
	void DC_ReadData(float data[]);								// ��������
	void DC_GetData(float data[]);								// �õ�����
	int DC_Init(bool FF,bool NF,bool VF,int number,int SR);		// ��ʼ��
	void DC_GetVolt(float data[],int number);					// �õ���ѹ
	int DC_CalcTime(float Value,int SPoint,int EPoint,int SoundFlag);			// �������ݴ���
	unsigned int Nlength;

	int RMS();													//RMS 
	int RMSLen1,RMSLen2;										//len1:RMSβ���ض� len2:RMS����
	float VmsData[fN];											//Wave after RMS
	void VmsNto1(int Nbegin,int Nend);								// ��һ��
	int sub_2(int StartPoint,int EndPoint);
protected:
	void Nto1(int Nbegin,int Nend);								// ��һ��
	void Amplifer(unsigned int Nlen,int SFlag);					// �Ŵ�
private:
	float Vdata[fN];											// ��ѹֵ
	int SampRate;
	bool Nto1Flag,VoltFlag,FilterFlag;							// ��һ����־����ѹ��־���˲���־
	float DataMax;
	float coef;													// �Ŵ�ϵ��
};

