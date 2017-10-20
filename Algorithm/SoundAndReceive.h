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
	int SoundOrder[iNumArm];										// 发声顺序
	int GasDelayTime;												// second
	int iArmModuleNum;												// Arm控制模块实际数量
	float ArmSoundData[iNumArm][iNumArm][iNumSound][fN];			// 一个循环中收到的数据【发声】【接收】【次数】
	ArmModule *SnRModule;											// Arm控制模块对象指针
public:
	int SnR_Init(int *order,CString *ip,int *port,int n,int GasTime,int timeout);	// order: the order of sound
																	// ip: the Arm Module IP address
																	// port: the Arm Module Communication Port
																	// n:the number of Arm Module
	int SnR_Circle(int ArmSound);									// 开始发声循环
private:
	void DbgCallOutArmStatus(int n);								// 调试用函数
};

