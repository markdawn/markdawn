#pragma once
#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#endif

#include <string>
#include <WinSock2.h>
#include "alcommon.h"
class Communication
{
public:
	Communication(void);
	~Communication(void);
public:
	//static const int Com_Pack_Length=1024;
	//static const int Com_Pack_Num=10;
	//static const int Com_Pack_Header_Length=4;
	bool CommFlag;									// 通讯标志
	int CommErr;									// 通讯错误号									
	CString IP;										// IP地址
	int Port;										// IP端口
	CString recBuff[Com_Pack_Num*2];				// 接收缓冲区
	UINT tmpData[Com_Pack_Length*Com_Pack_Num*2];	// 临时数据缓冲区
	UINT recHead[Com_Pack_Num*2];					// 接收头
	int recData[Com_Pack_Length*Com_Pack_Num/2];	// 接收数据
public:
	int Comm_Init(CString ip,int Port);				// 初始化通讯
	int Comm_Conn();								// 通讯连接
	int Comm_recData();								// 通讯读取数据
	void Comm_Close();								// 关闭通讯
	void Comm_Disconn();							// 断开通讯连接
	int Comm_Sound();								// 通讯发声
	int Comm_GasCtl(int order);						// 通讯气阀控制
	int transBuff2Data();							// 转换通讯数据
	int TimeOut;									// 通讯超时
private:
	char Rbuff[8192];								// 接收缓冲区
	int rec_len[20];
	sockaddr_in servAddr;
	WSADATA wsaData;
	SOCKET s;
	WORD sockVersion;
	std::string Hex2String(BYTE *pszHex,int nLen);
	int SetCommTime;								// 通讯超时（秒）
	int CommRead();									// 通讯读
	int CommWrite(BYTE *Cmd,int CmdLe);				// 通讯写
};

