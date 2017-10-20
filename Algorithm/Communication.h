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
	bool CommFlag;									// ͨѶ��־
	int CommErr;									// ͨѶ�����									
	CString IP;										// IP��ַ
	int Port;										// IP�˿�
	CString recBuff[Com_Pack_Num*2];				// ���ջ�����
	UINT tmpData[Com_Pack_Length*Com_Pack_Num*2];	// ��ʱ���ݻ�����
	UINT recHead[Com_Pack_Num*2];					// ����ͷ
	int recData[Com_Pack_Length*Com_Pack_Num/2];	// ��������
public:
	int Comm_Init(CString ip,int Port);				// ��ʼ��ͨѶ
	int Comm_Conn();								// ͨѶ����
	int Comm_recData();								// ͨѶ��ȡ����
	void Comm_Close();								// �ر�ͨѶ
	void Comm_Disconn();							// �Ͽ�ͨѶ����
	int Comm_Sound();								// ͨѶ����
	int Comm_GasCtl(int order);						// ͨѶ��������
	int transBuff2Data();							// ת��ͨѶ����
	int TimeOut;									// ͨѶ��ʱ
private:
	char Rbuff[8192];								// ���ջ�����
	int rec_len[20];
	sockaddr_in servAddr;
	WSADATA wsaData;
	SOCKET s;
	WORD sockVersion;
	std::string Hex2String(BYTE *pszHex,int nLen);
	int SetCommTime;								// ͨѶ��ʱ���룩
	int CommRead();									// ͨѶ��
	int CommWrite(BYTE *Cmd,int CmdLe);				// ͨѶд
};

