#include "StdAfx.h"
#include "PcArmCommunicate.h"

//ARM与PC通信线程
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


// 开启PC与ARM通信线程
void CPcArmCommunicate::StartCmmunicateThread(LPVOID lpParameter)
{
	if(INVALID_HANDLE_VALUE != m_hCommucateThread)
		CloseHandle(m_hCommucateThread);

	m_hCommucateThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PcArmCommThreadFunc, 
		lpParameter, CREATE_SUSPENDED, NULL);

	if(m_hCommucateThread)
	{
		::SetThreadPriority(m_hCommucateThread, THREAD_PRIORITY_BELOW_NORMAL);//设置线程优先级
		::ResumeThread(m_hCommucateThread);  //恢复线程
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
		pWnd->m_bErrorSelfCheckFlag = pWnd->JudgeSystemIsFault();//判断是否有故障
		while (!(pWnd->m_bErrorSelfCheckFlag))
		{
			//没有故障
			::Sleep(100);
		}
		::Sleep(100);
	}
	return 0;
}

// 系统每个ARM模块自检：PC与ARM的通信，ARM模块自检
//0-无故障，负数-故障
int CPcArmCommunicate::SystemSelfCheck(int iArm)
{

	return 0;
}


// 判断系统是否故障,只要其中有一个ARM模块有故障，则标记系统故障
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


// 得到系统故障状态
BOOL CPcArmCommunicate::GetSystemFaultState(void)
{
	return m_bErrorSelfCheckFlag;
}


// 得到第iArm模块的故障代码,iArm:1~8
int CPcArmCommunicate::GetiArmFaultCode(int iArm)
{
	int i = iArm-1;
	if (i <0 || i >ARM_NUM)
	{
		return -100;
	}
	return m_iRecordErrorState[i];
}

//初始化TCPIP通信
BOOL CPcArmCommunicate::InitTcpipComm(void)
{
	m_TcpipComm.LoadSocketBase(); //加载套接字库
	m_clientSocket = m_TcpipComm.CreateClientSocket();//创建套接字
	if (m_clientSocket == INVALID_SOCKET)
	{
		return FALSE;
	}

	return m_TcpipComm.SetTcpipNoBlockMode(m_clientSocket);//设置为非阻塞模式
}

//连接服务器端
BOOL CPcArmCommunicate::ConnectServer(DWORD ipWord,u_short portNum,UINT retryTimes)
{
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(ipWord);//inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port=htons(portNum);
	return m_TcpipComm.TcpipConnect(m_clientSocket,addrSrv,sizeof(SOCKADDR),retryTimes);
}

//向服务器端发送命令,返回发送的数据个数
int CPcArmCommunicate::SendCmdToARM(char* cmd, int len)
{
	return m_TcpipComm.TcpipSendData(m_clientSocket,cmd,len);
}

//PC从服务器端接收数据
int CPcArmCommunicate::RecvDataFromARM(char* recvBuf, int len)
{
	return m_TcpipComm.TcpipReCeiveData(m_clientSocket,recvBuf,len);
}
//关闭TCPIP通信
void CPcArmCommunicate::CloseTcpipComm()
{
	m_TcpipComm.TcpipClose(m_clientSocket);
}