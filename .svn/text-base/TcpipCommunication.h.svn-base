#pragma once
#include <Winsock2.h >

class CTcpipCommunication
{
public:
	CTcpipCommunication(void);
	~CTcpipCommunication(void);
	// 加载套接字库
	void LoadSocketBase(void);
	// 客户端套接字对象
	SOCKET m_SocketClient;
	// 创建客户端套接字
	SOCKET CreateClientSocket(void);
	// 设置TCPIP为非阻塞模式
	BOOL SetTcpipNoBlockMode(SOCKET socket);
	// TCP/IP连接
	BOOL TcpipConnect(SOCKET socket , SOCKADDR_IN addrSrv, int len, int iRetryTimes);
	// 接受套接字数据
	int TcpipReCeiveData(SOCKET socket , char* revBuf, int len);
	// 发送套接字数据
	int TcpipSendData(SOCKET socket, char* sendBuf, int len);
	// 关闭套接字
	void TcpipClose(SOCKET socket);
};

