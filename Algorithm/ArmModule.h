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
	float VMax;									// 采样器最大值
public:
	int Num;									// Arm模块编号
	bool SensorFlag;							// 传感器标志
												// 0--正常 1--异常
	bool ArmFlag;								// 模块标志
												// 0--正常 1--异常
	int ArmStatus;								// Arm控制模块状态
												// 0--等待  1--发声 2--发送数据
	int GasStatus;								// 吹扫气控制阀状态
												// 0--关气  1--开气
	bool ArmDataFlag;							// 数据标志
												// 0--正常  1--异常
	float fData[fN];							// 数据
	float Volt;									// 电压值
public:
	int AMInit(int num,CString addr,int port,int timeout);		// 初始化 【模块编号】【地址】【端口】
	int RecData();								// 接收数据
	int Sound();								// 发声
	int GasOpen();								// 开气阀
	int GasClose();								// 关气阀
private:
	void TestSensor();							// 测试探头
};

