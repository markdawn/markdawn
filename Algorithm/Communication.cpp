#include "stdafx.h"
#include "Communication.h"

#pragma comment(lib,"WS2_32.lib")
#pragma warning(disable:4996)
int CommTime;									// 通讯计时
Communication::Communication(void)
{
	int i;
	CommTime=0;
	SetCommTime=30;
	TimeOut=10000;
	for(i=0;i<Com_Pack_Length*Com_Pack_Num/2;i++)
	{
		recData[i]=0;
	}
	for(i=0;i<Com_Pack_Num*2;i++)
		recBuff[i]="";
}

Communication::~Communication(void)
{

}

std::string Communication::Hex2String(BYTE *pszHex,int nLen)
{
	if(NULL==pszHex||0==nLen)
	{
		return "";
	}
	std::string strRet="";
	for(int i=0;i<nLen;i++)
	{
		char szTmp[4]={' '};
		sprintf_s(szTmp,"%02x",(byte)pszHex[i]);
		strRet+=szTmp;
	}
	return strRet;
}

int Communication::Comm_Init(CString ip,int port)
{
	IP=ip;
	Port=port;
	sockVersion=MAKEWORD(2,0);
	if(::WSAStartup(sockVersion,&wsaData))
	{
		CommFlag=true;
		CommErr=1;
		return 1;
	}
	s=::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(s==INVALID_SOCKET)
	{
		CommFlag=true;
		CommErr=1;
		Comm_Close();
		return 1;
	}

	if(::setsockopt(s,SOL_SOCKET,SO_SNDTIMEO,(char*)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
	{
		CommFlag=true;
		CommErr=1;
		Comm_Close();
		return 1;
	}
	if(::setsockopt(s,SOL_SOCKET,SO_RCVTIMEO,(char*)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
	{
		CommFlag=true;
		CommErr=1;
		Comm_Close();
		return 1;
	}
	CommFlag=false;
	CommErr=0;
	return 0;
}

int Communication::Comm_Conn()
{
	
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(Port);
	servAddr.sin_addr.S_un.S_addr=inet_addr(IP);
	if(::connect(s,(sockaddr*)&servAddr,sizeof(servAddr))==-1)
	{
		CommFlag=true;
		CommErr=2;
		Comm_Close();
		return 1;
	}
	CommFlag=false;
	CommErr=0;
	return 0;
}
int Communication::Comm_Sound()
{
	BYTE cmdCodeSP[8]={0x2,0xf,0xf,0xf};
	int nlen;
	CommWrite(cmdCodeSP,4);
	nlen=CommRead();
	if(nlen<=0)
		return 1;
	else
		return 0;
}
int Communication::Comm_GasCtl(int order)
{
	BYTE cmdCodeSP[8]={0x6,0x1,0xf,0xf};
	int nlen;
	if(order<=0)
	{
		cmdCodeSP[1]=0x2;
	}
	else
	{
		cmdCodeSP[1]=0x1;
	}
	CommWrite(cmdCodeSP,4);
	nlen=CommRead();
	if(nlen<=0)
		return 1;
	else
		return 0;
}
void CALLBACK ProcTime(HWND hWnd,UINT para1,UINT_PTR para2,DWORD para3)
{
	CommTime++;
}
/*
DWORD WINAPI ComRecData(Communication *comm)
{
	BYTE cmdCodeSD[8]={0x3,0xf,0xf,0xf};
	CString temp;
	int nlen,i,n,m;
	n=0;
	CommTime=0;
	SetTimer(0,0,1000,ProcTime);
	comm->CommWrite(cmdCodeSD,4);
	for(i=0;i<20;i++)
		comm->recBuff[i]="";
	do 
	{
		nlen=comm->CommRead();
		rec_len[n]=nlen;
		if(comm->Rbuff[3]!=(n+1))
		{
			KillTimer(0,0);
			comm->Comm_Disconn();
			comm->Comm_Close();
			return 1;
		}
		m=0;
		for(i=0;i<nlen;i++)
		{
			if(i<4)
			{

			}
			else
			{
				comm->tmpData[n*1024+m]=comm->Rbuff[i]&0xFF;
				m++;
			}
		}
		n++;
		if((n>10)||(CommTime>=comm->SetCommTime))
		{
			KillTimer(0,0);
			comm->Comm_Disconn();
			comm->Comm_Close();
			return 1;
		}
	} while(comm->Rbuff[3]<0xa);
	KillTimer(0,0);
	if(comm->Rbuff[0]!=0x3||comm->Rbuff[1]!=0x40||comm->Rbuff[2]!=0xa||comm->Rbuff[3]!=0xa)
	{
		return 1;
	}
	return 0;
}
int Communication::Comm_recData()
{
	::CreateThread()
}*/

int Communication::Comm_recData()
{
	BYTE cmdCodeSD[8]={0x3,0xf,0xf,0xf};
	CString temp;
	int nlen,i,n,m;
	n=0;
	CommTime=0;
	SetTimer(0,0,1000,ProcTime);
	CommWrite(cmdCodeSD,4);
	for(i=0;i<20;i++)
		recBuff[i]="";
	do 
	{
		nlen=CommRead();
		rec_len[n]=nlen;
		if(Rbuff[3]!=(n+1))
		{
			KillTimer(0,0);
			Comm_Disconn();
			Comm_Close();
			return 1;
		}
		m=0;
		for(i=0;i<nlen;i++)
		{
			if(i<4)
			{
				
			}
			else
			{
				tmpData[n*1024+m]=Rbuff[i]&0xFF;
				m++;
			}
		}
		n++;
		if((n>10)||(CommTime>=SetCommTime))
		{
			KillTimer(0,0);
			Comm_Disconn();
			Comm_Close();
			return 1;
		}
	} while(Rbuff[3]<0xa);
	KillTimer(0,0);
	if(Rbuff[0]!=0x3||Rbuff[1]!=0x40||Rbuff[2]!=0xa||Rbuff[3]!=0xa)
	{
		return 1;
	}
	return 0;
}

int Communication::CommRead()
{
	int nRecv=::recv(s,Rbuff,8192,0);
	return nRecv;
}

int Communication::CommWrite(BYTE *Cmd,int CmdLen)
{
	char Sbuff[8192];
	int i;
	for(i=0;i<CmdLen;i++)
	{
		Sbuff[i]=Cmd[i];
	}
	::send(s,Sbuff,4,0);
	return 0;
}

void Communication::Comm_Disconn()
{
	::closesocket(s);
}

void Communication::Comm_Close()
{
	::WSACleanup();
}

int string2int(CString cdata)
{
	int len,i;
	int num[4],sum;
	int coef[4]={4096,256,16,0};
	sum=0;
	len=cdata.GetLength();
	if(len>4)
	{
		return -1;
	}
	else
	{
		for(i=0;i<len;i++)
		{
			num[i]=cdata.GetAt(i);
			sum=sum+num[i]*coef[4-len];
		}
		return sum;
	}
}
int Communication::transBuff2Data()
{
	int i,len;
	len=0;
	for(i=0;i<10;i++)
	{
		len=len+rec_len[i]-Com_Pack_Header_Length;
	}
	for(i=0;i<len/2;i++)
	{
		recData[i]=tmpData[i*2]*256+tmpData[i*2+1];
	}
	return 0;
}
