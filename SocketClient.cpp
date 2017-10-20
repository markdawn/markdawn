#include "StdAfx.h"
#include "SocketClient.h"
#include "MainFrm.h"


const DWORD DEFAULT_TIMEOUT = 100L;

CSocketClient::CSocketClient(void)
{
	m_SocketClient = INVALID_SOCKET;
	m_iErrorCode = NO_ERROR;
	m_hSocketThread = INVALID_HANDLE_VALUE;
	m_nPackageCounter = 0;
	for (int i = 0;i<PACKAGE_NUM;i++)
	{
		m_bRecvOK[i] = TRUE;  //一个周期内数据是否接收完标记
	}


	m_totalBuffer = new unsigned char[TOTLA_LENTH*2];                //修改
	memset(m_totalBuffer,0,2*TOTLA_LENTH*sizeof(unsigned char));	//修改

	m_bRecvDataOver = TRUE; 
	m_bTransDataFlag = FALSE;
	m_uID = 0;
}


CSocketClient::~CSocketClient(void)
{
	if (m_totalBuffer)
	{
		delete [] m_totalBuffer;
		m_totalBuffer = NULL;
	}
	CloseCommunicate();
	if (IsStart())
	{
		CloseHandle(m_hSocketThread);
		m_hSocketThread = INVALID_HANDLE_VALUE;
	}
}

///////////////////////////////////////////////////////////////////////////////
// IsOpen
BOOL CSocketClient::IsOpen() const
{
	return ( INVALID_SOCKET != m_SocketClient );
}

///////////////////////////////////////////////////////////////////////////////
// IsStart
BOOL CSocketClient::IsStart() const
{
	return ( INVALID_HANDLE_VALUE != m_hSocketThread );
}

///////////////////////////////////////////////////////////////////////////////
// IsRecvOver
BOOL CSocketClient::IsRecvOver() const
{
	return m_bRecvDataOver;
}

///////////////////////////////////////////////////////////////////////////////
// IsStartTransData
BOOL CSocketClient::IsStartTransData()const
{
	return  m_bTransDataFlag;
}

///////////////////////////////////////////////////////////////////////////////
// SetRecvDataOver 
void CSocketClient::SetRecvDataOver(BOOL bOver)
{
	m_bRecvDataOver = bOver;
}

///////////////////////////////////////////////////////////////////////////////
// SetRecvDataOver 
void CSocketClient::SetStartTransData(BOOL bOver)
{
	m_bTransDataFlag = bOver;
}
/*
BOOL CSocketClient::InitSocket()
{
	WORD wVersionRequested;
	WSADATA wsadata;
	int err = -1;
	wVersionRequested = MAKEWORD(2,2);

	err = WSAStartup(wVersionRequested,&wsadata);
	if (err!=0)
	{
		return FALSE;
	}

	if (LOBYTE(wsadata.wVersion)!=2 || HIBYTE(wsadata.wVersion)!=2)
	{
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}
*/

BOOL CSocketClient::SetBuffSize(SOCKET sock)
{
	//设置系统接收数据缓冲区大小为默认的BUF_TIMES倍
	int errorCode = 0;
	unsigned int uiRcvBuf;
	int uiRcvBufLen = sizeof(uiRcvBuf);

	//获取设置参数
	errorCode= getsockopt(sock, SOL_SOCKET, SO_RCVBUF,(char*)&uiRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == errorCode)
	{
		return FALSE;
	}
	//设置缓冲区大小
	uiRcvBuf *= BUF_TIMES;
	errorCode = setsockopt(sock, SOL_SOCKET, SO_RCVBUF,(char*)&uiRcvBuf, uiRcvBufLen);
	if (SOCKET_ERROR == errorCode)
	{
		return FALSE;
	}

	//检查设置系统接收数据缓冲区是否成功
	unsigned int uiNewRcvBuf;
	getsockopt(sock, SOL_SOCKET, SO_RCVBUF,(char*)&uiNewRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == errorCode || uiNewRcvBuf != uiRcvBuf)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CSocketClient::CreateSocket(DWORD dwIP,u_short usPort,int iRetryTimes)
{
	if (IsOpen()) //已创建
		return TRUE;
	
	SOCKET sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP); //创建套接字
	if (INVALID_SOCKET != sock)
	{
		if (FALSE == SetBuffSize(sock)) //设置
		{
			CloseCommunicate();
			return FALSE;
		}

		if (!Connect(sock,dwIP,usPort,iRetryTimes))//连接
		{
			CloseCommunicate();
			return FALSE;
		}
		m_SocketClient = sock; //已经连接到服务器后,套接字可以被使用
	}
	return (INVALID_SOCKET != sock);
}

BOOL CSocketClient::Connect(SOCKET sock, DWORD dwIP,u_short usPort,int iRetryTimes)
{
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(dwIP);//inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port=htons(usPort);

	//连接服务器
	int errorCode = 0;
	int i = 0;
	int retConn  = connect(sock,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	if (retConn == NO_ERROR)
		return TRUE;
	else
	{
		for (i= 0;i<iRetryTimes;i++) //重试
		{
			retConn = connect(sock,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
			if (retConn != SOCKET_ERROR)
				return TRUE;
		}
	}
	m_iErrorCode = WSAGetLastError(); //保存错误代码
	return FALSE;
}

BOOL CSocketClient::WatchCommunicate()
{
	if (!IsStart()) //线程是否已经创建
	{
		if (IsOpen()) //连接是否已经建立
		{
			HANDLE hThread;
			UINT uiThreadId = 0;
			hThread = (HANDLE)_beginthreadex(NULL,	// Security attributes
				0,	// stack
				SocketThreadProc,	// Thread proc
				this,	// Thread param
				CREATE_SUSPENDED,	// creation mode
				&uiThreadId);	// Thread ID

			if ( NULL != hThread)
			{
				//SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
				ResumeThread( hThread );
				m_hSocketThread = hThread;
				return TRUE;
			}
		}
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
// SocketThreadProc
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//     Socket Thread function.  This function is the main thread for socket
//     communication - Asynchronous mode.
// PARAMETERS:
//     LPVOID pParam : Thread parameter - a CSocketComm pointer
// NOTES:
///////////////////////////////////////////////////////////////////////////////
UINT WINAPI CSocketClient::SocketThreadProc(LPVOID pParam)
{
	CSocketClient* pThis = reinterpret_cast<CSocketClient*>( pParam );
	_ASSERTE( pThis != NULL );

	pThis->Run();

	return 1L;
} // end SocketThreadProc

///////////////////////////////////////////////////////////////////////////////
// Run
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//		This function runs the main thread loop
//		this implementation can be overloaded.
//      This function calls CSocketComm::OnDataReceived() (Virtual Function)
// PARAMETERS:
// NOTES:
//		You should not wait on the thread to end in this function or overloads
///////////////////////////////////////////////////////////////////////////////
void CSocketClient::Run()
{
//	stMessageProxy stMsgProxy;
	int	dwBytes  = 0;
	DWORD	dwTimeout = DEFAULT_TIMEOUT;
//	LPBYTE  lpData  = (LPBYTE)&stMsgProxy;
//	DWORD	dwSize  = sizeof(stMsgProxy);
//	LPBYTE  lpData; 
	BYTE Data[1028] = {0}; //这个要改成结构体


	Package recvPackage;
	while( IsStart()) //thread is not closed
	{
		Sleep(200);
		// Blocking mode: Wait for event
		if( !IsOpen() )  //is socket connected?
			continue;

		if (IsStartTransData())
		{
			int dwRes = OnSendTransDataCmd();
			if (dwRes<0)
			{
				Sleep(500);
				continue;
			}
		
			while (IsRecvOver() == FALSE)
			{
				memset(&recvPackage,PACKAGE_LENTH,0);
				dwBytes = OnDataReceived((LPBYTE)&recvPackage,PACKAGE_LENTH); //receive data
				// Error? - need to signal error
				if (dwBytes < 0) //error ocrrered :dwBytes=0,time out;<0,error or close socket;>0,data num
				{
					switch(dwBytes)
					{
					case -3:   //lenth error
					case -2: //format error     ----repeat the operator
						break;
					case -1: //connect error    --- connect again  ---reset all
						if (IsOpen())
							CloseCommunicate();
						SetRecvDataOver(TRUE);
						SetStartTransData(FALSE);
						m_nPackageCounter = 0; 
						OnEvent( EVT_CONDROP ); // lost connection
						break;
					default:
						break;
					}
				}

				// Chars received?
				if (dwBytes >4 && dwBytes <= PACKAGE_LENTH) //接收到了数据
//				if (dwBytes == PACKAGE_LENTH) //接收到了数据
				{
//					TRACE2(_T("dwBytes=%d, packagecounter=%d\n"),dwBytes,m_nPackageCounter);
					CopyDataIntoBuffer(recvPackage.data,m_totalBuffer,DATA_LENTH,m_nPackageCounter); // into buffer
				}
				Sleep(10);
			}

			if (IsOpen())
			{
				SetStartTransData(FALSE);
				OnEvent( EVT_TRANSOVER ); //Receiving data is over
			}
		}
		
	}
}

///////////////////////////////////////////////////////////////////////////////
// ReadSocket
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//		Reads the Socket Communication
// PARAMETERS:
//		LPBYTE lpBuffer: buffer to place new data
//		DWORD dwSize: maximum size of buffer
//		DWORD dwTimeout: timeout to use in millisecond
///////////////////////////////////////////////////////////////////////////////
int CSocketClient::ReadSocket(LPBYTE lpBuffer, DWORD dwSize, DWORD dwTimeout)
{
	if (!IsOpen())
	{
		return -1;
	}

	_ASSERTE( lpBuffer != NULL );

	if (lpBuffer == NULL || dwSize < 1L)
		return 0L;

	fd_set	fdRead  = { 0 };
	TIMEVAL	stTime;
	TIMEVAL	*pstTime = NULL;

	if ( INFINITE != dwTimeout ) {
		stTime.tv_sec = dwTimeout/1000;
		stTime.tv_usec = dwTimeout % 1000;
		pstTime = &stTime;
	}

	SOCKET s = m_SocketClient;
	// Set Descriptor
	if ( !FD_ISSET( s, &fdRead ) )
		FD_SET( s, &fdRead );

	// Select function set read timeout
	int dwBytesRead = 0L;
	int res = select(0, &fdRead, NULL, NULL, pstTime ); //res = 0,time out;=SOCKET_ERROR, error;>0,data coming
	if ( res > 0)
	{
		res = recv( s, (LPSTR)lpBuffer, dwSize, 0);//res = 0,socket close;=SOCKET_ERROR, error;>0,data num
		dwBytesRead = ((res > 0)?(res) : (-1));
	}
	return dwBytesRead;  //0-time out ; -1: close or error;>0-data lenth
}

///////////////////////////////////////////////////////////////////////////////
// OnEvent
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//				This function reports events & errors
// PARAMETERS:
//		UINT uEvent: can be one of the event value EVT_(events)
///////////////////////////////////////////////////////////////////////////////

void CSocketClient::OnEvent(UINT uEvent)
{
	CMainFrame *pWnd = (CMainFrame*)(AfxGetApp()->GetMainWnd());
	if (!pWnd)
	{
		return;
	}
	UINT ModuleID = GetTheModuleID();
	//pWnd->UpdataCommProcess(uEvent, ModuleID);
	
}

//receive data   res<0:error (-3:lenth error;-2:format error;-1:recv or send error);0:conn close;>0,data lenth
int CSocketClient::OnDataReceived(LPBYTE lpBuffer, DWORD dwCount)
{
	int res = ReadSocket(lpBuffer,dwCount,200);
	if (res>0) //data coming
	{
		switch (res)
		{
		case PACKAGE_LENTH:
			m_nPackageCounter ++;
			if ( /*lpBuffer[0] == 0x02 && */lpBuffer[2]== PACKAGE_NUM) 
			{
				if (m_nPackageCounter == lpBuffer[3])
				{
					
					m_bRecvOK[m_nPackageCounter-1] = TRUE; //the transmission is ok
					//if (lpBuffer[1] == 0x00 &&  m_nPackageCounter ==lpBuffer[2] )//data transmission is over
					if (lpBuffer[1] == 0x40 &&  m_nPackageCounter ==lpBuffer[2] )//data transmission is over

					{
						m_nPackageCounter = 0; //reset counter
						SetRecvDataOver(TRUE);
					}
				}
				else
				{
					//data is lost
					if (m_nPackageCounter<lpBuffer[3])//中间丢失了包裹
					{
						int i = 0;
						for (i = m_nPackageCounter;i<=lpBuffer[3];i++)
						{
							m_bRecvOK[i] = FALSE;
						}
						m_nPackageCounter = lpBuffer[3];
					}
				}
			}
			else
			{
				//the format of data package is wrong
				res = -2;
			}
			break;
		default:
			//lenth error
			res = -3;
			break;
		}
	}
	
	return res;
	// = 0 ，time out;     -1,error or close;      -2, format error;      -3,lenth error
}

///////////////////////////////////////////////////////////////////////////////
// WriteSocket
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//		Writes data to the Socket Communication
// PARAMETERS:
//		const LPBYTE lpBuffer: data to write
//		DWORD dwCount: maximum characters to write
//		DWORD dwTimeout: timeout to use in millisecond
///////////////////////////////////////////////////////////////////////////////
int CSocketClient::WriteSocket(const LPBYTE lpBuffer, DWORD dwCount, DWORD dwTimeout)
{
	_ASSERTE( NULL != lpBuffer );

	// Accept 0 bytes message
	if (!IsOpen() || NULL == lpBuffer)
		return 0L;

	fd_set	fdWrite  = { 0 };
	TIMEVAL	stTime;
	TIMEVAL	*pstTime = NULL;

	if ( INFINITE != dwTimeout ) {
		stTime.tv_sec = dwTimeout/1000;
		stTime.tv_usec = dwTimeout % 1000;
		pstTime = &stTime;
	}

	SOCKET s = m_SocketClient;
	// Set Descriptor
	if ( !FD_ISSET( s, &fdWrite ) )
		FD_SET( s, &fdWrite );

	// Select function set write timeout
	int  dwBytesWritten = 0L;
	int res = select( s+1, NULL, &fdWrite, NULL, pstTime );
	if ( res > 0)
	{
		res = send( s, (LPCSTR)lpBuffer, dwCount, 0);
		dwBytesWritten = ((res >= 0)?(res) : (-1));
	}
	return dwBytesWritten;
}

//ucData--data from socket,    ucBuffer--data buffer,  offset--data's offset in buffer
void CSocketClient::CopyDataIntoBuffer(LPBYTE ucData,LPBYTE ucBuffer,int iNumOfData,int offset)
{
	CString str;
	int i = 0;
	/*for ( i = 0; i < iNumOfData; i++)
	{
		str.Format("%02x", (BYTE)ucData[i]);
		TRACE3(_T("i = %d  ,ucData=%s   offset=%d\n"),i, str, offset);
	}*/
	
	if (iNumOfData<0 || iNumOfData>PACKAGE_LENTH)
	{
		return;
	}
	if (offset == 0)
	{
		offset = 9;
	}
	else
	{
		offset = offset - 1;
	}
	for(i = 0;i<iNumOfData;i++)
	{
		ucBuffer[offset*DATA_LENTH+i]=ucData[i];
		
	}
}

//发送吹扫气阀控制命令
int  CSocketClient::OnSendGasCtrlCmd(int iOrder)
{
	if (!IsOpen())
	{
		return -1;
	}
	BYTE cmdCode[CMD_LENTH] = { 0 };
	cmdCode[0] = 0x06;
	if (iOrder == 1)
	{
		cmdCode[1] = 0x01;
	}
	else
	{
		cmdCode[1] = 0x02;
	}
	cmdCode[2] = 0x0F;
	cmdCode[3] = 0x0F;
	int res = WriteSocket(cmdCode, CMD_LENTH, 200); //send the cmd of translating data 
	if (res<0)
	{
		//when error occurred,retry one times
		res = WriteSocket(cmdCode, CMD_LENTH, 200); //send the cmd of translating data
		if (res < 0)
		{
			//error
			CloseCommunicate();
			return res;
		}
	}
	Sleep(500);
	int iRetryTimes = 10;
	while (iRetryTimes--)
	{
		res = ReadSocket(cmdCode, CMD_LENTH, 200);
		if (res < 0)
		{
			//error, or connect is closed
			CloseCommunicate();
			break;
		}
		if (cmdCode[0] == 0x02 && cmdCode[1] == 0x0F)
		{
			break;
		}
		Sleep(100);
	}
	return res;
}

//发送数据采集命令
int  CSocketClient::OnSendSampleCmd()
{
	if (!IsOpen())
	{
		return 1;
	}
	BYTE cmdCode[CMD_LENTH] = {0};
	cmdCode[0] = 0x02;
	cmdCode[1] = 0x0F;
	cmdCode[2] = 0x0F;
	cmdCode[3] = 0x0F;
	int res = WriteSocket(cmdCode,CMD_LENTH,200); //send the cmd of translating data 
	if (res<0)
	{
		//when error occurred,retry one times
		res = WriteSocket(cmdCode,CMD_LENTH,200); //send the cmd of translating data
		if (res < 0)
		{
			//error
			CloseCommunicate();
			return res;
		}
	}
	Sleep(500);
	int iRetryTimes = 10;
	while (iRetryTimes--) 
	{
		res = ReadSocket(cmdCode, CMD_LENTH, 200);
		if (res < 0)
		{
			//error, or connect is closed
			CloseCommunicate();
			break;
		}
		if (cmdCode[0] == 0x02 && cmdCode[1] == 0x0F)
		{
			break;
		}
		Sleep(100);
	}
	return res;
}

//发送数据传输命令
int  CSocketClient::OnSendTransDataCmd()
{
	if (!IsOpen())
	{
		return -1;
	}

	BYTE cmdCode[CMD_LENTH] = {0};
	cmdCode[0] = 0x03;
	cmdCode[1] = 0x0F;
	cmdCode[2] = 0x0F;
	cmdCode[3] = 0x0F;
	int res = WriteSocket(cmdCode,CMD_LENTH,200); //send the cmd of translating data 
	if (res<0)
	{
		//when error occurred,retry one times
		res = WriteSocket(cmdCode,CMD_LENTH,200); //send the cmd of translating data
		if (res<0)
		{
			CloseCommunicate();
		}
	}

	return res;
}


void CSocketClient::CloseCommunicate()
{
	closesocket(m_SocketClient);
	m_SocketClient = INVALID_SOCKET;
}

unsigned char* CSocketClient::GetDataBuffer()
{
	//需要锁住该缓存区
	return m_totalBuffer;
}

void CSocketClient::SetTheModuleID(UINT id)
{
	if (id<0 || id >=ARM_NUM)
	{
		return;
	}

	m_uID = id;
}

UINT CSocketClient::GetTheModuleID()
{
	if (m_uID<0 || m_uID >=ARM_NUM)
	{
		m_uID = 0;
	}
	return m_uID;
}
