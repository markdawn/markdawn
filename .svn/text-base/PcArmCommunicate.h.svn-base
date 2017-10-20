#pragma once
#include "tcpipcommunication.h"
#include "commonFile.h"

/////////////////PC与ARM通信类
class CPcArmCommunicate
{
public:
	CPcArmCommunicate(void);
	~CPcArmCommunicate(void);
	// 开启PC与ARM通信线程
	void StartCmmunicateThread(LPVOID lpParameter);
private:
	// 通信句柄
	HANDLE m_hCommucateThread;
public:
	// 系统每个ARM模块自检：PC与ARM的通信，ARM模块自检
	int SystemSelfCheck(int iArm);
	// TCP/IP通信
	CTcpipCommunication m_TcpipComm;
	
	// 判断系统是否故障
	BOOL JudgeSystemIsFault(void);
protected:
	// 记录每个模块的故障状态，0-无错误，负数-错误
	int* m_iRecordErrorState;
	
public:
	BOOL m_bErrorSelfCheckFlag;// 系统故障状态
	// 得到系统故障状态
	BOOL GetSystemFaultState(void);
	// 得到第iArm模块的故障代码
	int GetiArmFaultCode(int iArm);
public:
	BOOL InitTcpipComm(void);
	SOCKET m_clientSocket;
	BOOL ConnectServer(DWORD ipWord,u_short portNum,UINT retryTimes);
	int SendCmdToARM(char* cmd, int len);
	int RecvDataFromARM(char* recvBuf, int len);
	void CloseTcpipComm();
};

