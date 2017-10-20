
// MainFrm.h : CMainFrame 类的接口
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
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar        m_wndStatusBar;

// 生成的消息映射函数
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

	//L侧
	SoundAndReceive SAR_A;					//定义发声接收对象
	CalTime CT_A;							//定义时间计算对象
	FlightTime FT_A;						//定义飞渡时间对象
	TempField5 TF_A;							//定义区域温度场计算对象
	//R侧
	SoundAndReceive SAR_B;					//定义发声接收对象
	CalTime CT_B;							//定义时间计算对象
	FlightTime FT_B;						//定义飞渡时间对象
	TempField5 TF_B;							//定义区域温度场计算对象

	int order[5] ;                 //定义发声顺序
	int ip[5] ;					//定义IP地址最后一位
	int port[5] ;		//定义通讯端口号
	int armnum  ;							//定义控制模块数量
	bool Startflag;
	int  UpdataCommProcess(UINT uEvent , UINT uModuleID);
public:
	LRESULT StartDataProcess(WPARAM wparam, LPARAM lparam);

	CView *m_pViews[INTERFACE_NUM];
//private:
	BOOL SelView(VIEW_STATUS nView);
	// 记录当前的消息，主要用来进行文档切换
	UINT m_currentMsg;
public:
	VIEW_STATUS m_nCurViewStatus;
	UINT* GetAreaAvrgTValue();
	UINT* GetLineAvrgTValue();
	UINT* GetLineAvrgTValueB();
private:

	// 数据处理线程句柄
	HANDLE m_hWorkThread;
	private:
	// 建立PingPong缓冲区
	int**** m_fPingPongBuffer;
	// 读PingPong缓冲区指针
	float*** m_fReadBufferPointer;
	// 串口线程写ping-pong缓冲区指针
	float*** m_fWriteBufferPointer;
	// 当前buffer指针
	int*** m_pfCurBufferPointer;
	// 标记ping-pong缓冲区：0-ping，1-pong
	BOOL m_bPingPongBufferFlag;
public:
	
	// 重构温度场
	CRebuildAreaT m_RebuildAreaT;
	// 保存区域平均温度
	UINT *m_iAreaAverageT;
	UINT *m_iLineAverageT;
	UINT *m_iLineAverageTB;
	float *f_da;
	
	//保存每个模块发生五次每次发声处理后的结果
	int*** iPerArmTime;
	//bool bSoundCycleOver;
protected:
	MYSQL m_mysql;
	// 保存温度网格数据
	float** m_pCalTGridData;
public:
	// 记录区域平均温度
	
	LRESULT RecordAreaT(UINT* iAreaAvrgT);
	//记录波形数据
	LRESULT RecordCurBuffer(int isounddev, int ireceivearm);
	LRESULT RecordDA(float* f_da);

	//记录报警信息
	LRESULT RecordWarningMessage(CString strSQL);
protected:
	// 记录上一次的数据存储时间
	CTime m_tOldTime;
public:
	// 删除平均温度数据
	LRESULT DeleteAreaTData(WPARAM wparam, LPARAM lparam);
	// 得到mysql句柄
	MYSQL GetMySQLHandle(void);
	// 读取系统配置文件
	int ReadSystemIniConfig(void);

	// 初始化MySQL数据库
	ERROR_MYSQL MySQLInit(char* userID, char* password, char* dbName, unsigned int port);
protected:
	// 数据库ID
	CString m_strMySqlUserID;
	// mysql密码
	CString m_strMySqlPassword;
	// 数据库名称
	CString m_strMySqlName;
	// 数据库所用的端口
	unsigned int m_iMySqlPort;
public:
	// 系统故障标记
	bool m_bSystemFaultFlag;
	// PC与ARM通信类
	void SetIPAddressAndPort(DWORD dwIP,u_short usPort,UINT iModuleNum);
	DWORD GetIPAddress(UINT iModuleNum);
	u_short GetPortNum(UINT iModuleNum);
	int SaveIPAndPortSetting(DWORD* dwIP,u_short* usPort,UINT nModuleNum);
	int ReadCommunicateConfig(DWORD* dwIP,u_short* usPort,UINT nModuleNum);

private:
	DWORD* m_ipWord;//保存IP
	u_short* m_iPort;//保存端口号
private:
	// 网格数据信息
	GridBaiscInfo m_gridDataInfo;
public:
	// 得到网格数据
	float** GetGridData();
	// 获得栅格数据信息
//	void GetGridDataInfo(GridBaiscInfo& girdDataInfo);
	GridBaiscInfo* GetGridDataInfo();
	CDatabaseOperate m_OperateDB;
private:
	HANDLE StartThreadWork(LPVOID lpParameter,LPTHREAD_START_ROUTINE threadFunc);//TCP/IP接收数据线程
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
//	CRITICAL_SECTION	m_csOne;//临界量
//	CRITICAL_SECTION	m_csTwo;//临界量
	CCriticalSection m_cs;//临界量
private:
//	CCriticalSection m_cs;//临界量
public:
	afx_msg void OnClose();
private:
	CString m_strExitPassword;
public:
	CString GetSysExitPassword();
	void SetSysExitPassword(CString strPassword);
	CString ReadSysExitPass(MYSQL sql);
public:
	void	SetStatusBar(WPARAM wparam, LPARAM lparam);//设置状态条
public:
	//CManageClient* m_ManageClient;
	//Gas Control
	DWORD GasIP;
	DWORD GasPort;
	//CManageClient m_GasControl;
	BOOL InitSocket(); //初始化套接字环境
	BOOL StartADeviceSound(UINT iOrder);//启动一个发声装置
	void CopyDataIntoGlobalBuffer(LPBYTE ucDataIn,int* pfBuffer,int iNumOfData,int iBufferSize);
	void SwapBufferPointer();
	//BOOL OnStartReceiveDataProcess(void);
	//LRESULT OnStartReceiveDataProcess(WPARAM wparam, LPARAM lparam);
	BOOL IsAllSocketConn();
	BOOL IsAllRecvOver();
	int m_iCurrentFaultDev;
	BOOL m_bConnState;//程序与ARM的连接状态
	BOOL GetOneArmState(UINT iArm);
	BOOL GetOneArmStateB(UINT iArm);
	
protected:
	BOOL m_bPerArmConnState[ARM_NUM];//记录每个ARM的通信状态
public:
	
	DWORD* GetIPAddressPointer();
	u_short* GetPortPointer();
public:
	//读取区域平均温度阀值配置信息
	int ReadAreaTValveSettingConfig(AreaTParaSet* setAreaTLimit,UINT nAreaNum);

private:
	AreaTParaSet* m_SaveAreaTLimit;//保存平均温度阀值
public:
	int SaveAreaTLimitSetting(AreaTParaSet* setAreaTLimit,UINT nAreaNum);//保存到配置文件
	void SetIAreaTLimit(AreaTParaSet* setAreaTLimit,UINT nAreaNum);//设置全局变量
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


