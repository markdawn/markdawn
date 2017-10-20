#pragma once
#ifndef SANDR_H
#define SANDR_H
#endif
#include "ArmModule.h"
#include "alcommon.h"
class SoundAndReceive
{
public:
	SoundAndReceive(void);
	~SoundAndReceive(void);

public:
	int SoundOrder[iNumArm];										// ����˳��
	int GasDelayTime;												// second
	int iArmModuleNum;												// Arm����ģ��ʵ������
	float ArmSoundData[iNumArm][iNumArm][iNumSound][fN];			// һ��ѭ�����յ������ݡ������������ա���������
	ArmModule *SnRModule;											// Arm����ģ�����ָ��
public:
	int SnR_Init(int *order,CString *ip,int *port,int n,int GasTime,int timeout);	// order: the order of sound
																	// ip: the Arm Module IP address
																	// port: the Arm Module Communication Port
																	// n:the number of Arm Module
	int SnR_Circle(int ArmSound);									// ��ʼ����ѭ��
private:
	void DbgCallOutArmStatus(int n);								// �����ú���
};

