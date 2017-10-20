#include "StdAfx.h"
#include "SerialPort.h"
#include "..\Include\CRC16.H"
#include "MainFrm.h"
#include "commonFile.h"

extern volatile BOOL gbUartReceiveDataFlag;
extern volatile BOOL gbDataProcessFlag;

CSerialPort::CSerialPort(void)
	: m_nCommPort(3)
	, m_BitRate(9600)
	, m_nCommTimeOut(0)
{
	m_hUartCommThread = INVALID_HANDLE_VALUE;
	m_hPortHandle = INVALID_HANDLE_VALUE;
	memset(&m_PortEvent,0,sizeof(OVERLAPPED));
	memset(&m_WriteEvent,0,sizeof(OVERLAPPED));
	
}


CSerialPort::~CSerialPort(void)
{
}


HANDLE CSerialPort::OpenPortHandle(LPCTSTR ComPort, DWORD BitRate, DWORD ReadTimeOut)
{
	HANDLE hComm;
	DCB dcb = {0};
	COMMTIMEOUTS CommTmOut = {0};

	// Get a handle to the port.
	hComm = CreateFile( ComPort,  
		GENERIC_READ | GENERIC_WRITE, 
		0, 
		NULL, 
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,//异步
		0);
	if (hComm == INVALID_HANDLE_VALUE)
		return INVALID_HANDLE_VALUE;

//	m_PortEvent.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
//	m_WriteEvent.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
//	if (m_PortEvent.hEvent == NULL || m_WriteEvent.hEvent == NULL)
//	{
//#ifdef _DEBUG_PROJECT
//		CString strError;
//		strError.Format("CreateEvent failed with error %d.\n",GetLastError());
//		AfxMessageBox(strError);
//#endif
//		return NULL;
//	}

	if (!SetupComm(hComm,1024,1024))//输入缓冲区和输出缓冲区的大小都是1024
	{
#ifdef _DEBUG_PROJECT
		CString strError;
	//	strError.Format("SetupComm failed with error %d.\n",GetLastError());
	//	AfxMessageBox(strError);
#endif
	}
	 
	// Get the current state of the port
	FillMemory(&dcb, sizeof(dcb), 0);

	if (!GetCommState(hComm, &dcb))
	{
#ifdef _DEBUG_PROJECT
		CString strError;
//		strError.Format("GetCommState failed with error %d.\n",GetLastError());
//		AfxMessageBox(strError);
#endif
		return NULL;
	}
	else{
		dcb.BaudRate = BitRate;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.fDtrControl = DTR_CONTROL_DISABLE;
		dcb.fRtsControl = RTS_CONTROL_DISABLE;
		dcb.fOutX = FALSE;
		dcb.fInX = FALSE;
		dcb.fOutxCtsFlow = FALSE;
		dcb.fOutxDsrFlow = FALSE;
		dcb.ByteSize = 8;
	}

	if (!SetCommState(hComm, &dcb))
	{
#ifdef _DEBUG_PROJECT
		CString strError;
		//strError.Format("SetCommState failed with error %d.\n",GetLastError());
		//AfxMessageBox(strError);
#endif
		return NULL;
	}


	//Set the timeout conditions
	if (!GetCommTimeouts(hComm, &CommTmOut)){
#ifdef _DEBUG_PROJECT
		CString strError;
		//strError.Format("GetCommTimeouts failed with error %d.\n",GetLastError());
		//AfxMessageBox(strError);
#endif
		return NULL;
	}
	else{
		CommTmOut.ReadIntervalTimeout = MAXDWORD;
		CommTmOut.ReadTotalTimeoutMultiplier = MAXDWORD;
		CommTmOut.ReadTotalTimeoutConstant = ReadTimeOut;
	}

	if (!SetCommTimeouts(hComm, &CommTmOut))
	{
#ifdef _DEBUG_PROJECT
		CString strError;
		//strError.Format("SetCommTimeouts failed with error %d.\n",GetLastError());
		//AfxMessageBox(strError);
#endif
		return NULL;
	}
		

	if (!PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT))
	{
#ifdef _DEBUG_PROJECT
		CString strError;
		//strError.Format("PurgeComm failed with error %d.\n",GetLastError());
		//AfxMessageBox(strError);
#endif
		return NULL;
	}

	DWORD   dwStoredFlags;   
	dwStoredFlags = EV_BREAK | EV_CTS | EV_DSR | EV_ERR | EV_RING |\
		EV_RLSD | EV_RXCHAR | EV_RXFLAG | EV_TXEMPTY ;
	if (!SetCommMask(hComm,dwStoredFlags))
	{
#ifdef _DEBUG_PROJECT
		CString strError;
		//strError.Format("SetCommMask failed with error %d.\n",GetLastError());
		//AfxMessageBox(strError);
#endif
		return NULL;
	}
	return hComm;
}


BOOL CSerialPort::OpenPort(void)
{
	if (m_hPortHandle == INVALID_HANDLE_VALUE)
	{
		CString cstr;
		cstr.Format(_T("COM%d"), m_nCommPort);
#ifdef _UNICODE
		m_hPortHandle = OpenPortHandle(cstr, m_BitRate, m_nCommTimeOut);
#else
		LPSTR lpStr = cstr.GetBuffer(cstr.GetLength());
	//	m_hPortHandle = OpenComPort(lpStr, m_BitRate, m_nCommTimeOut);
		m_hPortHandle = OpenPortHandle(cstr, m_BitRate, m_nCommTimeOut);
#endif
		if (m_hPortHandle == INVALID_HANDLE_VALUE)
		{
			return FALSE;
		}
		else if (m_hPortHandle == NULL)
		{
#ifdef _DEBUG_PROJECT
			AfxMessageBox(_T("串口设置失败"));
#endif
			return FALSE;
		}
	}
	return TRUE;
}


BOOL CSerialPort::ClosePortHandle(HANDLE hPortHandle)
{
	BOOL bCloseFlag = FALSE;
	::PurgeComm(m_hPortHandle,PURGE_TXABORT|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_RXCLEAR);
	if (CloseHandle(m_hPortHandle))
	{
		bCloseFlag = TRUE;
	}
	else
	{
		bCloseFlag = FALSE;
	}

	m_hPortHandle = INVALID_HANDLE_VALUE;
	return bCloseFlag;
}


BOOL CSerialPort::ClosePort(void)
{
	if (ClosePortHandle(m_hPortHandle))
	{
		return TRUE;
	}
	else
		return FALSE;
	
}


void CSerialPort::SetUartPara(UartConfig uartPara)
{
	m_BitRate = uartPara.bitRate;
	m_nCommPort = uartPara.nCommPort;
	m_nCommTimeOut = uartPara.nCommTimeOut;
}


void CSerialPort::SetPortNum(int nPortNum)
{
	m_nCommPort = nPortNum;
}


void CSerialPort::SetBuadRate(UINT iBuadRate)
{
	m_BitRate = iBuadRate;
}


void CSerialPort::SetTimeOut(int nTimeOut)
{
	m_nCommTimeOut = nTimeOut;
}


SERIAL_ERROR CSerialPort::SendPacket(BYTE* PacketData, UINT NumOfBytes)
{
	BOOL bWriteState=TRUE;
	BOOL bWrite=TRUE;
	DWORD dwBytesWritten=0;
	DWORD dwErrorFlags = 0;
	COMSTAT ComStat;

	m_WriteEvent.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	if (m_WriteEvent.hEvent == NULL)
	{
#ifdef _DEBUG_PROJECT
		CString strError;
	/*	strError.Format("CreateEvent failed with error %d.\n",GetLastError());
		AfxMessageBox(strError);*/
#endif
		return SERIAL_UNKNOWN_ERROR;
	}

	::PurgeComm(m_hPortHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT); //清除缓冲区

	bWriteState=::WriteFile(m_hPortHandle,PacketData,NumOfBytes,&dwBytesWritten,&m_WriteEvent);//&m_oPort=TRUE
	CString strMsg;

	if(!bWriteState)
	{
		DWORD dwError=::GetLastError();
		if(dwError==ERROR_IO_PENDING)
		{
			while(!::GetOverlappedResult(m_hPortHandle,&m_PortEvent,&dwBytesWritten,TRUE))
			{
				dwError = GetLastError();
				if(dwError==ERROR_IO_INCOMPLETE)
					continue;
				else
				{
					::ClearCommError(m_hPortHandle,&dwErrorFlags,&ComStat);
	//				::LeaveCriticalSection(&m_csOne);
					return SERIAL_UNKNOWN_ERROR;
				}
			}
		}
	}
	return SERIAL_NO_ERROR;
}


SERIAL_ERROR CSerialPort::GetPacket(BYTE* PacketData, UINT NumOfBytes)
{
	BOOL bReaded=TRUE;
	BOOL bResult=TRUE;

	DWORD dwBytesRead;
	DWORD dwBytes;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	memset(&m_PortEvent,0,sizeof(OVERLAPPED));
	m_PortEvent.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	if (m_PortEvent.hEvent == NULL)
	{
#ifdef _DEBUG_PROJECT
		CString strError;
		//strError.Format("CreateEvent failed with error %d.\n",GetLastError());
		//AfxMessageBox(strError);
#endif
		return SERIAL_ERROR_READ_PACKARTFLAG;
	}
	::ClearCommError(m_hPortHandle,&dwErrorFlags,&ComStat);//清除硬件的通讯错误,获取输入输出缓冲区大小
	dwBytesRead=min(NumOfBytes,ComStat.cbInQue); 

	if(dwBytesRead==NumOfBytes)
	{
		bResult=::ReadFile(m_hPortHandle,PacketData,dwBytesRead,&dwBytes,&m_PortEvent);//读取数据

		if(!bResult)
		{
			DWORD dwError=::GetLastError();
			if(dwError==ERROR_IO_PENDING)
			{
				while(!::GetOverlappedResult(m_hPortHandle,&m_PortEvent,&dwBytes,TRUE))
				{
					dwError = GetLastError();
					if(dwError==ERROR_IO_INCOMPLETE)
						continue;
					else
					{
						::ClearCommError(m_hPortHandle,&dwErrorFlags,&ComStat);
						break;
					}
				}
			}
		}

	}
	else
	{
	//	::LeaveCriticalSection(&m_csOne);
		return SERIAL_ERROR_PACKET_TOO_BIG;
	}

	return SERIAL_NO_ERROR;
}


SERIAL_ERROR CSerialPort::SendDataPCToARM(int iAddr)
{
	SERIAL_ERROR RetStatus = SERIAL_NO_ERROR;

	SendCmdPacket sendPacketToArm;

	sendPacketToArm.sendCmdPacket.iAddr = iAddr;
	sendPacketToArm.sendCmdPacket.iFunc  = 3;
	sendPacketToArm.sendCmdPacket.iHiDataNum =0x03 ; //1000
	sendPacketToArm.sendCmdPacket.iLoDataNum = 0xE8;

	unsigned short iCRC16 =CRC16((unsigned char *)&sendPacketToArm.sendCmdPacket,sizeof(CmdPacket));
	sendPacketToArm.crc16Verify.iHiCRCValue=(iCRC16>>8) && 0xFF;
	sendPacketToArm.crc16Verify.iLoCRCValue=(BYTE)iCRC16;

	sendPacketToArm.cmdHead.cStartCmdFlag[0] = 0xF4;
	sendPacketToArm.cmdHead.cStartCmdFlag[1] = 0xAD;
	sendPacketToArm.cmdHead.cStartCmdFlag[2] = 0xDA;
	sendPacketToArm.cmdHead.cStartCmdFlag[3] = 0x4F;

	sendPacketToArm.cmdTail.cEndCmdFlag[0] = 0xE4;
	sendPacketToArm.cmdTail.cEndCmdFlag[1] = 0xAD;
	sendPacketToArm.cmdTail.cEndCmdFlag[2] = 0xDA;
	sendPacketToArm.cmdTail.cEndCmdFlag[3] = 0x4E;

#ifdef _DEBUG_PROJECT

	OVERLAPPED m_osWrite;
	memset(&m_osWrite,0,sizeof(OVERLAPPED));
	m_osWrite.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	DWORD dwBytesWrite=sizeof(SendCmdPacket);
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	ClearCommError(m_hPortHandle,&dwErrorFlags,&ComStat);
	bWriteStat=WriteFile(m_hPortHandle,&sendPacketToArm,
		dwBytesWrite,& dwBytesWrite,&m_osWrite);

	if(!bWriteStat)
	{
		if(GetLastError()==ERROR_IO_PENDING)
		{
			WaitForSingleObject(m_osWrite.hEvent,1000);
		}
	}
#else
	RetStatus = SendPacket((BYTE*)&sendPacketToArm,sizeof(SendCmdPacket));
	
#endif
	return RetStatus;
}


SERIAL_ERROR CSerialPort::GetDataPCFromARM(BYTE* pBuff,UINT NumOfBytes,int iAddr)
{
	GetCmdPacket getData;
	unsigned short iCRC16 = 0;

	SERIAL_ERROR RetStatus = SERIAL_NO_ERROR;
	RetStatus = GetPacket((BYTE*) &getData, sizeof(GetCmdPacket));
	if(SERIAL_NO_ERROR == RetStatus)
	{
		DWORD headCmdFlag = 0;
		DWORD tailCmdFlag = 0;
		for (int i = 0; i<4;i++)
		{
			headCmdFlag += getData.cmdHead.cStartCmdFlag[i]<< (8*(3-i)); 
			tailCmdFlag += getData.cmdTail.cEndCmdFlag[i]<< (8*(3-i)); 
		}

		if (((headCmdFlag&&0xFFFFFFFF) == 0xF4ADDA4F) &&((tailCmdFlag&&0xFFFFFFFF)== 0xE4ADDA4E))//标记校验
		{
			iCRC16=CRC16((unsigned char *)&getData.getCmdPacket,sizeof(CmdGetData));
			if(iCRC16==((getData.crc16Verify.iHiCRCValue<<8)|getData.crc16Verify.iLoCRCValue))//CRC校验
			{
				if (iAddr == getData.getCmdPacket.cmdGetPacket.iAddr)
				{
					memcpy(pBuff,(BYTE*)&getData.getCmdPacket,sizeof(getData.getCmdPacket));
				}
				else
					RetStatus =  SERIAL_ERROR_IADDR	;
			}
			else
				RetStatus = SERIAL_ERROR_CRC16;
		}
		else
			RetStatus =  SERIAL_ERROR_READ_PACKARTFLAG;

	}

	return RetStatus;
}


SERIAL_ERROR CSerialPort::SendGetDataPCToARM(BYTE* pBuff, UINT NumofData, int iAddr)
{
	SERIAL_ERROR RetStatus = SERIAL_NO_ERROR;
	RetStatus = SendDataPCToARM(iAddr);
	if (RetStatus != SERIAL_NO_ERROR)
	{
		return RetStatus;
	}

	RetStatus = GetDataPCFromARM(pBuff,NumofData,iAddr);

	return RetStatus;
}


void CSerialPort::StartUartCommThread(LPVOID lpParameter)
{
	if(INVALID_HANDLE_VALUE != m_hUartCommThread)
		CloseHandle(m_hUartCommThread);

	m_hUartCommThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UARTCommThreadFunc, 
		lpParameter, CREATE_SUSPENDED, NULL);

	if(m_hUartCommThread)
	{
		::SetThreadPriority(m_hUartCommThread, THREAD_PRIORITY_BELOW_NORMAL);//设置线程优先级
		::ResumeThread(m_hUartCommThread);  //恢复线程
	}
}


//串口线程，接收ARM传来的数据
DWORD WINAPI UARTCommThreadFunc( LPVOID lpParam ) 
{

	CMainFrame *pWnd=(CMainFrame *)lpParam;

	::PurgeComm(pWnd->m_serialPort.GetPortHandle(), PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
/*
	DWORD dwEventMask=0;
	DWORD dwError=0;
	BOOL bResult=FALSE;
	UINT nCount = 0;


//	CString strError(_T(""));

	OVERLAPPED osStatus ;
	memset(&osStatus,0,sizeof(OVERLAPPED));

	osStatus.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (osStatus.hEvent == NULL)
		return 0;
*/
#ifdef _DEBUG_PROJECT
	DWORD      dwRes;
	DWORD      dwCommEvent;
	DWORD      dwStoredFlags;
	BOOL      fWaitingOnStat = FALSE;
	BOOL bWaitingOnStatusHandle = FALSE;
#endif

	while(1)
	{

#if 0
		if (!fWaitingOnStat) {
			if (!WaitCommEvent(pWnd->m_serialPort.GetPortHandle(), &dwCommEvent, &osStatus)) {
				if((dwCommEvent&EV_RXCHAR)==EV_RXCHAR)
				{
					::SendMessage(pWnd->GetSafeHwnd(),WM_DATA_MSG,1,22);
				}
				if (GetLastError() == ERROR_IO_PENDING)
					bWaitingOnStatusHandle = TRUE;
				else
					// error in WaitCommEvent; abort
					continue;
			}
			else
				// WaitCommEvent returned immediately.
				// Deal with status event as appropriate.
				//ReportStatusEvent(dwCommEvent); 
				continue;
		}

#else
		if (gbDataProcessFlag==TRUE)
		{
			gbDataProcessFlag = FALSE;
			::SendMessage(pWnd->GetSafeHwnd(),WM_DATA_MSG,1,22);//测试
		}
/*
		//	::EnterCriticalSection(&pWnd->m_csOne);
	//	SetCommMask();
		bResult=::WaitCommEvent(pWnd->m_serialPort.GetPortHandle(),&dwEventMask,&osStatus);
		//	::LeaveCriticalSection(&pWnd->m_csOne);
		if(!bResult)
		{
			dwError=::GetLastError();
			switch(dwError)
			{
			case ERROR_IO_PENDING:
				break ;
			case ERROR_INVALID_PARAMETER:
				break ;
			default :
				CString strError;
				strError.Format(_T("在等待串口事件时发生错误。\r\n错误码：%d"),dwError);
	//			AfxMessageBox(strError,MB_ICONSTOP);
				break;
			}
		}	
		if((dwEventMask&EV_RXCHAR)==EV_RXCHAR)
			::SendMessage(pWnd->GetSafeHwnd(),WM_DATA_MSG,1,22);//测试

	//	::SendMessage(pWnd->GetSafeHwnd(),WM_GUIDISPLAY_MSG,1,22);
*/
#endif
		::Sleep(100);
	}

	return 1;
}


HANDLE CSerialPort::GetPortHandle(void)
{
	return m_hPortHandle;
}


OVERLAPPED CSerialPort::GetEventHandle(void)
{
	return m_PortEvent;
}



// 判断是否串口有数据
BOOL CSerialPort::UartBufferIsDataComing(OVERLAPPED osStatus,HANDLE handle)
{
	DWORD dwError=0;
	DWORD dwEventMask=0;
	BOOL bRet = FALSE;
	BOOL bResult=::WaitCommEvent(handle,&dwEventMask,&osStatus);
	//	::LeaveCriticalSection(&pWnd->m_csOne);
	if(!bResult)
	{
		dwError=::GetLastError();
		switch(dwError)
		{
		case ERROR_IO_PENDING:
			break ;
		case ERROR_INVALID_PARAMETER:
			break ;
		default :
			CString strError;
			strError.Format(_T("在等待串口事件时发生错误。\r\n错误码：%d"),dwError);
			//			AfxMessageBox(strError,MB_ICONSTOP);
			break;
		}
	}	
	if((dwEventMask&EV_RXCHAR)==EV_RXCHAR)
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}
	return bRet;
}


// PC向ARM发送命令
SERIAL_ERROR CSerialPort::PCSendCMDToARM(int iArmAddr, int iModule)
{
	return SERIAL_ERROR();
}
