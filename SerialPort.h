#pragma once

//Error Code
typedef enum _SERIAL_ERROR{
	SERIAL_NO_ERROR				=	0,	
	SERIAL_ERROR_READ_PACKARTFLAG	=	-1,
	SERIAL_ERROR_READ_TIMEOUT	=	-2,
	SERIAL_ERROR_READ_LIMIT		=	-3,
	SERIAL_ERROR_PACKET_TOO_BIG	=	-4,
	SERIAL_ERROR_CRC16		=	-5,
	SERIAL_ERROR_RETRY_LIMIT		=	-6,
	SERIAL_ERROR_IADDR	=	-7,
	SERIAL_ERROR_CMD_INCOMPLETE	=	-8,
	SERIAL_ERROR_INVALID_HANDLE = -9,
	SERIAL_UNKNOWN_ERROR = -100,
}SERIAL_ERROR;

#define UART_DATA_LENTH  2
typedef struct _CmdPacket{
	BYTE iAddr;//��վ��ַ
	BYTE iFunc;//������
	BYTE iHiDataNum;//���ݸ������ֽ�
	BYTE iLoDataNum;//���ݸ������ֽ�
}CmdPacket;

typedef struct _CmdTail{
	BYTE cEndCmdFlag[4];//���ݰ��������
}CmdTail;

typedef struct _CmdHead{
	BYTE cStartCmdFlag[4];//���ݰ���ʼ���
}CmdHead;

typedef struct _CRC16Verify{
	BYTE iHiCRCValue;//CRCֵ���ֽ�
	BYTE iLoCRCValue;//CRCֵ���ֽ�
}CRC16Verify;

typedef struct _CmdGetData{
	CmdPacket cmdGetPacket;
	BYTE iData[DATA_LENTH];
}CmdGetData;

typedef struct _GetCmdPacket{
	CmdHead cmdHead;
	CmdGetData getCmdPacket;
	CRC16Verify crc16Verify;
	CmdTail  cmdTail;
}GetCmdPacket;

typedef struct _SendCmdPacket{
	CmdHead cmdHead;
	CmdPacket sendCmdPacket;
	CRC16Verify crc16Verify;
	CmdTail  cmdTail;
}SendCmdPacket;

typedef struct _UartConfig{
	int nCommPort;
	UINT bitRate;
	int nCommTimeOut;
}UartConfig;

extern DWORD WINAPI UARTCommThreadFunc( LPVOID lpParam );



class CSerialPort
{
public:
	CSerialPort(void);
	~CSerialPort(void);
	
	BOOL OpenPort(void);
private:
	HANDLE m_hPortHandle;
	int m_nCommPort;
	UINT m_BitRate;
	int m_nCommTimeOut;

private:
	HANDLE OpenPortHandle(LPCTSTR ComPort, DWORD BitRate, DWORD ReadTimeOut);
	BOOL ClosePortHandle(HANDLE hPortHandle);

public:
	BOOL ClosePort(void);
	void SetUartPara(UartConfig uartPara);
	void SetPortNum(int nPortNum);
	void SetBuadRate(UINT iBuadRate);
	void SetTimeOut(int nTimeOut);
private:
	OVERLAPPED m_PortEvent;
public:
	SERIAL_ERROR SendPacket(BYTE* PacketData, UINT NumOfBytes);
	SERIAL_ERROR GetPacket(BYTE* PacketData, UINT NumOfBytes);
	SERIAL_ERROR SendDataPCToARM(int iAddr);
	SERIAL_ERROR GetDataPCFromARM(BYTE* pBuff,UINT NumOfBytes,int iAddr);
	SERIAL_ERROR SendGetDataPCToARM(BYTE* pBuff, UINT NumofData, int iAddr);
	void StartUartCommThread(LPVOID lpParameter);
private:
	HANDLE m_hUartCommThread;
public:
	HANDLE GetPortHandle(void);
	OVERLAPPED GetEventHandle(void);
private:
	OVERLAPPED m_WriteEvent;
public:
	// �ж��Ƿ񴮿�������
	BOOL UartBufferIsDataComing(OVERLAPPED osStatus,HANDLE handle);
	// PC��ARM��������
	SERIAL_ERROR PCSendCMDToARM(int iArmAddr, int iModule);
};

