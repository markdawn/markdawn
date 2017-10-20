#pragma once
#ifndef ARMMODULE_H
#define ARMMODULE_H
#endif
#include "Communication.h"
#include "alcommon.h"
class ArmModule:public Communication
{
public:
	ArmModule(void);
	~ArmModule(void);
public:
	//static const int N=5120;
	float VMax;									// ���������ֵ
public:
	int Num;									// Armģ����
	bool SensorFlag;							// ��������־
												// 0--���� 1--�쳣
	bool ArmFlag;								// ģ���־
												// 0--���� 1--�쳣
	int ArmStatus;								// Arm����ģ��״̬
												// 0--�ȴ�  1--���� 2--��������
	int GasStatus;								// ��ɨ�����Ʒ�״̬
												// 0--����  1--����
	bool ArmDataFlag;							// ���ݱ�־
												// 0--����  1--�쳣
	float fData[fN];							// ����
	float Volt;									// ��ѹֵ
public:
	int AMInit(int num,CString addr,int port,int timeout);		// ��ʼ�� ��ģ���š�����ַ�����˿ڡ�
	int RecData();								// ��������
	int Sound();								// ����
	int GasOpen();								// ������
	int GasClose();								// ������
private:
	void TestSensor();							// ����̽ͷ
};

