#include "StdAfx.h"
#include "TcpipCommunication.h"


CTcpipCommunication::CTcpipCommunication(void)
{
	m_SocketClient = INVALID_SOCKET;
}


CTcpipCommunication::~CTcpipCommunication(void)
{

}


// 加载套接字库
void CTcpipCommunication::LoadSocketBase(void)
{
	WORD wVersionRequested;
	WSADATA wsadata;
	int err;
	wVersionRequested = MAKEWORD(1,1);

	err = WSAStartup(wVersionRequested,&wsadata);
	if (err!=0)
	{
		return;
	}

	if (LOBYTE(wsadata.wVersion)!=1 || HIBYTE(wsadata.wVersion)!=1)
	{
		WSACleanup();
	}
}


// 创建客户端套接字
SOCKET CTcpipCommunication::CreateClientSocket(void)
{
	return socket(AF_INET,SOCK_STREAM,0);
}


// 设置TCPIP为非阻塞模式
BOOL CTcpipCommunication::SetTcpipNoBlockMode(SOCKET socket)
{
	int imode=1;  
	int retVal=ioctlsocket(socket,FIONBIO,(u_long *)&imode);  
	if(retVal == SOCKET_ERROR)  
	{  
		TcpipClose(socket);
		return FALSE;  
	}
	return TRUE;
}



// TCP/IP连接  --------------------该函数还得改进
BOOL CTcpipCommunication::TcpipConnect(SOCKET socket , SOCKADDR_IN addrSrv, int len, int iRetryTimes)
{
	int retVal=connect(socket,(sockaddr *)&addrSrv,len); //连接到某一个具体的服务器  
	UINT iTimes = 0;
	for (iTimes = 0;iTimes<iRetryTimes;iTimes++)
	{
		if(retVal == INVALID_SOCKET)  
		{  
			int errcode=WSAGetLastError();  
			if(errcode==WSAEWOULDBLOCK || errcode==WSAEINVAL)  //表示服务器端未准备好，继续循环  
			{  
				Sleep(20);  
				continue;  
			}  
			else 
			{  
				if(errcode == WSAEISCONN) //连接成功，则退出  
				{  
					return TRUE;   
				}  
				else                      //否则连接失败，关闭客户端套接字并释放套接字库  
				{  
		//			printf("connect failed!");  
					TcpipClose(socket);
					return FALSE;  
				}  
			}  
		}  
	}
	if (iTimes == iRetryTimes)
	{
		return FALSE;
	}
	return TRUE;
}

// 接受套接字数据
int CTcpipCommunication::TcpipReCeiveData(SOCKET socket , char* revBuf, int len)
{
	int bytesRecv = SOCKET_ERROR;

	while( bytesRecv != SOCKET_ERROR ) {
		bytesRecv = recv( socket, revBuf, len, 0 );
		if ( bytesRecv == 0 || bytesRecv == WSAECONNRESET ) {//无连接即返回
	//		printf(_T( "Connection Closed.\n"));
			break;
		}
	//	printf( _T("Bytes Recv: %ld\n"), bytesRecv );
	}
	return bytesRecv;
}


// 发送套接字数据
int CTcpipCommunication::TcpipSendData(SOCKET socket, char* sendBuf, int len)
{
	int bytesSent;
	bytesSent = send( socket, sendBuf, len, 0 );
	return bytesSent;
}


// 关闭套接字
void CTcpipCommunication::TcpipClose(SOCKET socket)
{
	closesocket(socket);
	WSACleanup();
}
