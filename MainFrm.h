
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once


#include "rebuildareat.h"
#include "atltime.h"
#include "commonFile.h"
#include "databaseoperate.h"
#include "ManageClient.h"

#include <afxmt.h>
#include "Algorithm\SoundAndReceive.h"
#include "Algorithm\CalTime.h"
#include "Algorithm\FlightTime.h"
#include "Algorithm\TempField5.h"


typedef  enum  _VIEW_STATUS{
	MAINDIS_VIEW = 1,ISOTHERM_VIEW = 2,AREAAVRGT_VIEW,TTREND_VIEW,AVRGTINFO_VIEW,
	POSITION_VIEW,TIME_VIEW,LEAK_VIEW
}VIEW_STATUS;


DWORD WINAPI UpdateGuiThread( LPVOID lpParam ) ;
DWORD WINAPI StartWorkThread(LPVOID lpParam);



class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CStatusBar        m_wndStatusBar;

// ���ɵ���Ϣӳ�亯��
protected:
	LRESULT OnGuiDisplay(WPARAM wparam,LPARAM lparam);
	LRESULT OnCalGridData(WPARAM wparam,LPARAM lparam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	
public:
	//afx_msg void OnAvrgtBtn();
	//afx_msg void OnIsothermBtn();
	//afx_msg void OnLeakBtn();
	//afx_msg void OnPositionBtn();
	//afx_msg void OnTimeBtn();
	//afx_msg void OnTinfosBtn();
	//afx_msg void OnTtrendBtn();
	
	//bool StartDataProcess();
	void PreSoundDataProcess();

	//L��
	SoundAndReceive SAR_A;					//���巢�����ն���
	CalTime CT_A;							//����ʱ��������
	FlightTime FT_A;						//����ɶ�ʱ�����
	TempField5 TF_A;							//���������¶ȳ��������
	//R��
	SoundAndReceive SAR_B;					//���巢�����ն���
	CalTime CT_B;							//����ʱ��������
	FlightTime FT_B;						//����ɶ�ʱ�����
	TempField5 TF_B;							//���������¶ȳ��������

	int order[5] ;                 //���巢��˳��
	int ip[5] ;					//����IP��ַ���һλ
	int port[5] ;		//����ͨѶ�˿ں�
	int armnum  ;							//�������ģ������
	bool Startflag;
	int  UpdataCommProcess(UINT uEvent , UINT uModuleID);
public:
	LRESULT StartDataProcess(WPARAM wparam, LPARAM lparam);

	CView *m_pViews[INTERFACE_NUM];
//private:
	BOOL SelView(VIEW_STATUS nView);
	// ��¼��ǰ����Ϣ����Ҫ���������ĵ��л�
	UINT m_currentMsg;
public:
	VIEW_STATUS m_nCurViewStatus;
	UINT* GetAreaAvrgTValue();
	UINT* GetLineAvrgTValue();
	UINT* GetLineAvrgTValueB();
private:

	// ���ݴ����߳̾��
	HANDLE m_hWorkThread;
	private:
	// ����PingPong������
	int**** m_fPingPongBuffer;
	// ��PingPong������ָ��
	float*** m_fReadBufferPointer;
	// �����߳�дping-pong������ָ��
	float*** m_fWriteBufferPointer;
	// ��ǰbufferָ��
	int*** m_pfCurBufferPointer;
	// ���ping-pong��������0-ping��1-pong
	BOOL m_bPingPongBufferFlag;
public:
	
	// �ع��¶ȳ�
	CRebuildAreaT m_RebuildAreaT;
	// ��������ƽ���¶�
	UINT *m_iAreaAverageT;
	UINT *m_iLineAverageT;
	UINT *m_iLineAverageTB;
	float *f_da;
	
	//����ÿ��ģ�鷢�����ÿ�η��������Ľ��
	int*** iPerArmTime;
	//bool bSoundCycleOver;
protected:
	MYSQL m_mysql;
	// �����¶���������
	float** m_pCalTGridData;
public:
	// ��¼����ƽ���¶�
	
	LRESULT RecordAreaT(UINT* iAreaAvrgT);
	//��¼��������
	LRESULT RecordCurBuffer(int isounddev, int ireceivearm);
	LRESULT RecordDA(float* f_da);

	//��¼������Ϣ
	LRESULT RecordWarningMessage(CString strSQL);
protected:
	// ��¼��һ�ε����ݴ洢ʱ��
	CTime m_tOldTime;
public:
	// ɾ��ƽ���¶�����
	LRESULT DeleteAreaTData(WPARAM wparam, LPARAM lparam);
	// �õ�mysql���
	MYSQL GetMySQLHandle(void);
	// ��ȡϵͳ�����ļ�
	int ReadSystemIniConfig(void);

	// ��ʼ��MySQL���ݿ�
	ERROR_MYSQL MySQLInit(char* userID, char* password, char* dbName, unsigned int port);
protected:
	// ���ݿ�ID
	CString m_strMySqlUserID;
	// mysql����
	CString m_strMySqlPassword;
	// ���ݿ�����
	CString m_strMySqlName;
	// ���ݿ����õĶ˿�
	unsigned int m_iMySqlPort;
public:
	// ϵͳ���ϱ��
	bool m_bSystemFaultFlag;
	// PC��ARMͨ����
	void SetIPAddressAndPort(DWORD dwIP,u_short usPort,UINT iModuleNum);
	DWORD GetIPAddress(UINT iModuleNum);
	u_short GetPortNum(UINT iModuleNum);
	int SaveIPAndPortSetting(DWORD* dwIP,u_short* usPort,UINT nModuleNum);
	int ReadCommunicateConfig(DWORD* dwIP,u_short* usPort,UINT nModuleNum);

private:
	DWORD* m_ipWord;//����IP
	u_short* m_iPort;//����˿ں�
private:
	// ����������Ϣ
	GridBaiscInfo m_gridDataInfo;
public:
	// �õ���������
	float** GetGridData();
	// ���դ��������Ϣ
//	void GetGridDataInfo(GridBaiscInfo& girdDataInfo);
	GridBaiscInfo* GetGridDataInfo();
	CDatabaseOperate m_OperateDB;
private:
	HANDLE StartThreadWork(LPVOID lpParameter,LPTHREAD_START_ROUTINE threadFunc);//TCP/IP���������߳�
	void SuspendThreadWork(HANDLE hThread);
	void ResumeThreadWork(HANDLE hThread);
	void EndThreadWork(HANDLE hThread, DWORD dTimeOut);
//	HANDLE m_hReceiveThread;
	HANDLE m_hGuiThread;
	HANDLE m_hCalGridThread;
	HANDLE m_hStartWork;
public:
	int*** GetAcousticData();
public:
//	CRITICAL_SECTION	m_csOne;//�ٽ���
//	CRITICAL_SECTION	m_csTwo;//�ٽ���
	CCriticalSection m_cs;//�ٽ���
private:
//	CCriticalSection m_cs;//�ٽ���
public:
	afx_msg void OnClose();
private:
	CString m_strExitPassword;
public:
	CString GetSysExitPassword();
	void SetSysExitPassword(CString strPassword);
	CString ReadSysExitPass(MYSQL sql);
public:
	void	SetStatusBar(WPARAM wparam, LPARAM lparam);//����״̬��
public:
	//CManageClient* m_ManageClient;
	//Gas Control
	DWORD GasIP;
	DWORD GasPort;
	//CManageClient m_GasControl;
	BOOL InitSocket(); //��ʼ���׽��ֻ���
	BOOL StartADeviceSound(UINT iOrder);//����һ������װ��
	void CopyDataIntoGlobalBuffer(LPBYTE ucDataIn,int* pfBuffer,int iNumOfData,int iBufferSize);
	void SwapBufferPointer();
	//BOOL OnStartReceiveDataProcess(void);
	//LRESULT OnStartReceiveDataProcess(WPARAM wparam, LPARAM lparam);
	BOOL IsAllSocketConn();
	BOOL IsAllRecvOver();
	int m_iCurrentFaultDev;
	BOOL m_bConnState;//������ARM������״̬
	BOOL GetOneArmState(UINT iArm);
	BOOL GetOneArmStateB(UINT iArm);
	
protected:
	BOOL m_bPerArmConnState[ARM_NUM];//��¼ÿ��ARM��ͨ��״̬
public:
	
	DWORD* GetIPAddressPointer();
	u_short* GetPortPointer();
public:
	//��ȡ����ƽ���¶ȷ�ֵ������Ϣ
	int ReadAreaTValveSettingConfig(AreaTParaSet* setAreaTLimit,UINT nAreaNum);

private:
	AreaTParaSet* m_SaveAreaTLimit;//����ƽ���¶ȷ�ֵ
public:
	int SaveAreaTLimitSetting(AreaTParaSet* setAreaTLimit,UINT nAreaNum);//���浽�����ļ�
	void SetIAreaTLimit(AreaTParaSet* setAreaTLimit,UINT nAreaNum);//����ȫ�ֱ���
	AreaTParaSet* GetAreaTParaSetPointer(void);
public:
	int InitAllSocket();
	int SelectNextSoundArm(int dataIn);
	int cycleTimes;
	CString pathDirectory;
	CString old_pathDirectory;
	int falseCount ;

	BOOL travelTimeCalFlag[10];
	bool bGasControlFlag;

	BOOL SaveWaveData();
	void recordTravelTimeCalFlagError();
	int GetOneSounderState(int iSounder);
	int GetOneSounderStateB(int iSounder);
	int GetGasControlState();
	bool GetOneSersorState(int iArm);
	bool GetOneSersorStateB(int iArm);
};


