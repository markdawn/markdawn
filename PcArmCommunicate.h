#pragma once
#include "tcpipcommunication.h"
#include "commonFile.h"

/////////////////PC��ARMͨ����
class CPcArmCommunicate
{
public:
	CPcArmCommunicate(void);
	~CPcArmCommunicate(void);
	// ����PC��ARMͨ���߳�
	void StartCmmunicateThread(LPVOID lpParameter);
private:
	// ͨ�ž��
	HANDLE m_hCommucateThread;
public:
	// ϵͳÿ��ARMģ���Լ죺PC��ARM��ͨ�ţ�ARMģ���Լ�
	int SystemSelfCheck(int iArm);
	// TCP/IPͨ��
	CTcpipCommunication m_TcpipComm;
	
	// �ж�ϵͳ�Ƿ����
	BOOL JudgeSystemIsFault(void);
protected:
	// ��¼ÿ��ģ��Ĺ���״̬��0-�޴��󣬸���-����
	int* m_iRecordErrorState;
	
public:
	BOOL m_bErrorSelfCheckFlag;// ϵͳ����״̬
	// �õ�ϵͳ����״̬
	BOOL GetSystemFaultState(void);
	// �õ���iArmģ��Ĺ��ϴ���
	int GetiArmFaultCode(int iArm);
public:
	BOOL InitTcpipComm(void);
	SOCKET m_clientSocket;
	BOOL ConnectServer(DWORD ipWord,u_short portNum,UINT retryTimes);
	int SendCmdToARM(char* cmd, int len);
	int RecvDataFromARM(char* recvBuf, int len);
	void CloseTcpipComm();
};

