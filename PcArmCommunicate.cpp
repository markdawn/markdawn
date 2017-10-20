#include "StdAfx.h"
#include "PcArmCommunicate.h"

//ARM��PCͨ���߳�
DWORD WINAPI PcArmCommThreadFunc( LPVOID lpParam );
volatile BOOL gbPcArmCommThreadFlag = TRUE;

CPcArmCommunicate::CPcArmCommunicate(void)
	: m_bErrorSelfCheckFlag(FALSE)
	, m_iRecordErrorState(NULL)
{
	m_hCommucateThread = INVALID_HANDLE_VALUE;
	m_iRecordErrorState = new int[ARM_NUM];
	memset(m_iRecordErrorState,0,ARM_NUM*sizeof(int));
	m_clientSocket = INVALID_SOCKET;
}


CPcArmCommunicate::~CPcArmCommunicate(void)
{
	gbPcArmCommThreadFlag = FALSE;
	delete[] m_iRecordErrorState;
	m_iRecordErrorState = NULL;
	CloseTcpipComm();
}


// ����PC��ARMͨ���߳�
void CPcArmCommunicate::StartCmmunicateThread(LPVOID lpParameter)
{
	if(INVALID_HANDLE_VALUE != m_hCommucateThread)
		CloseHandle(m_hCommucateThread);

	m_hCommucateThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PcArmCommThreadFunc, 
		lpParameter, CREATE_SUSPENDED, NULL);

	if(m_hCommucateThread)
	{
		::SetThreadPriority(m_hCommucateThread, THREAD_PRIORITY_BELOW_NORMAL);//�����߳����ȼ�
		::ResumeThread(m_hCommucateThread);  //�ָ��߳�
	}
	CloseHandle(m_hCommucateThread);
}


DWORD WINAPI PcArmCommThreadFunc( LPVOID lpParam ) 
{
	CPcArmCommunicate* pWnd = (CPcArmCommunicate*)lpParam;
	if (!pWnd)
	{
		return -1;
	}
	while (gbPcArmCommThreadFlag)
	{
		pWnd->m_bErrorSelfCheckFlag = pWnd->JudgeSystemIsFault();//�ж��Ƿ��й���
		while (!(pWnd->m_bErrorSelfCheckFlag))
		{
			//û�й���
			::Sleep(100);
		}
		::Sleep(100);
	}
	return 0;
}

// ϵͳÿ��ARMģ���Լ죺PC��ARM��ͨ�ţ�ARMģ���Լ�
//0-�޹��ϣ�����-����
int CPcArmCommunicate::SystemSelfCheck(int iArm)
{

	return 0;
}


// �ж�ϵͳ�Ƿ����,ֻҪ������һ��ARMģ���й��ϣ�����ϵͳ����
BOOL CPcArmCommunicate::JudgeSystemIsFault(void)
{
	int i = 0;
	BOOL	bRet = FALSE;
	for (i = 0;i<ARM_NUM;i++)
	{
		m_iRecordErrorState[i] = SystemSelfCheck(i);
		if (m_iRecordErrorState[i]  != 0)
		{
			bRet = TRUE;
		}
	}
	return bRet;
}


// �õ�ϵͳ����״̬
BOOL CPcArmCommunicate::GetSystemFaultState(void)
{
	return m_bErrorSelfCheckFlag;
}


// �õ���iArmģ��Ĺ��ϴ���,iArm:1~8
int CPcArmCommunicate::GetiArmFaultCode(int iArm)
{
	int i = iArm-1;
	if (i <0 || i >ARM_NUM)
	{
		return -100;
	}
	return m_iRecordErrorState[i];
}

//��ʼ��TCPIPͨ��
BOOL CPcArmCommunicate::InitTcpipComm(void)
{
	m_TcpipComm.LoadSocketBase(); //�����׽��ֿ�
	m_clientSocket = m_TcpipComm.CreateClientSocket();//�����׽���
	if (m_clientSocket == INVALID_SOCKET)
	{
		return FALSE;
	}

	return m_TcpipComm.SetTcpipNoBlockMode(m_clientSocket);//����Ϊ������ģʽ
}

//���ӷ�������
BOOL CPcArmCommunicate::ConnectServer(DWORD ipWord,u_short portNum,UINT retryTimes)
{
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(ipWord);//inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port=htons(portNum);
	return m_TcpipComm.TcpipConnect(m_clientSocket,addrSrv,sizeof(SOCKADDR),retryTimes);
}

//��������˷�������,���ط��͵����ݸ���
int CPcArmCommunicate::SendCmdToARM(char* cmd, int len)
{
	return m_TcpipComm.TcpipSendData(m_clientSocket,cmd,len);
}

//PC�ӷ������˽�������
int CPcArmCommunicate::RecvDataFromARM(char* recvBuf, int len)
{
	return m_TcpipComm.TcpipReCeiveData(m_clientSocket,recvBuf,len);
}
//�ر�TCPIPͨ��
void CPcArmCommunicate::CloseTcpipComm()
{
	m_TcpipComm.TcpipClose(m_clientSocket);
}