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
	float RawData[iNumArm][iNumSound][fN];							// ԭʼ����ֵ
	int CTPreProcessData(bool SToSFlag);								// Ԥ������

public:	
	int SaLen,SaCut,SabLen;											// �Ӵ����ȣ��Ӵ���ʼλ�ã������Ӵ�����
	float lVal;														// ���޵�ƽֵ
	int Time[iNumArm][iNumSound];									// �ɶ�ʱ��
	int MainSound;													// ����ģ���
	int iNArm,iNSound;												// ����ģ����������������
public:
	CalTime(void);
	~CalTime(void);
	void CalcTime();												// ����ɶ�ʱ��
	int OneCalcTime(float value,int SPoint,int EPoint,int SoundFlag);				// ����һ��·��
	int ReadData(													// ��������
				float mydata[iNumArm][iNumArm][iNumSound][fN],		// ����ԭʼ����
				int iArm,int iSound);								// ģ�������� ��������
	void Init(unsigned int uSL,unsigned int uSC,					//��ʼ��
				unsigned int Sb,float SF,float lV);
protected:
	int Find_First_Big(float data[],int num);
};

