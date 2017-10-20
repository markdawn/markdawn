
// MainFrm.cpp : CMainFrame 类的实现

#include "stdafx.h"
#include "AcousticTherm.h"
#include "MainFrm.h"
#include "TTrendView.h"
#include "AreaAvrgTemperatureView.h"
#include "DisAreaAvrgTInfoView.h"
#include "PositionView.h"
#include "LeakView.h"
#include "SignalTimeView.h"
#include "MainDIsInfoView.h"
#include "IsothermView.h"
#include "AFX.h"
#include "ExitDlg.h"
#include "SysSetDlg.h"


#include "Algorithm/CalTime.h"
#include "Algorithm/FlightTime.h"
#include "Algorithm/TempField.h"
#include "Algorithm/SoundAndReceive.h"

#include <fstream>
using namespace std;
#pragma warning(disable:4018)
#pragma warning(disable:4996)
#pragma warning(disable:4305)
#pragma warning(disable:4715)
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define NOTCPIC   

volatile BOOL gbMainWorkThreadFlag = TRUE;

volatile BOOL gbReceiveOnceDataOverFlag = TRUE;         //发声一次时数据是否接收完
volatile UINT g_bCurrentSoundDevice = 0;                //标记当前发声装置


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()

	ON_MESSAGE(WM_GUIDISPLAY_MSG, &CMainFrame::OnGuiDisplay)
	ON_MESSAGE(WM_STARTWORK_MSG, &CMainFrame::StartDataProcess)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_NAME,
	ID_INDICATOR_ALARM_STATUS,//报警
	ID_INDICATOR_COMMUNICATE_STATUS,//通信故障
	ID_INDICATOR_DATE,
	ID_INDICATOR_WEEK,
	ID_INDICATOR_TIME,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
	: m_fPingPongBuffer(NULL)
	, m_fReadBufferPointer(NULL)
	, m_bPingPongBufferFlag(FALSE)
	, m_fWriteBufferPointer(NULL)
	, m_iAreaAverageT(NULL)
	, m_tOldTime(0)
	, m_strMySqlUserID(_T(""))
	, m_strMySqlPassword(_T(""))
	, m_strMySqlName(_T(""))
	, m_iMySqlPort(0)
	, m_bSystemFaultFlag(false)
	, m_pCalTGridData(NULL)
	, m_strExitPassword(_T(""))
{
	cycleTimes = 0;
	// 在此添加成员初始化代码
	gbMainWorkThreadFlag = TRUE;
	int i = 0;
	for (i = 0; i < 5; i++)
		travelTimeCalFlag[i] = 1;//飞渡时间计算结果状态。1,计算结果正常，0,计算结果异常
	bGasControlFlag = 1;//气通信状态
	Startflag=true;    //启动标志

	for (i = 0; i < INTERFACE_NUM; i++)
	{
		m_pViews[i] = NULL;
	}
	m_nCurViewStatus = MAINDIS_VIEW;
	m_currentMsg = WM_MAINDIS_MSG;
	m_hWorkThread = INVALID_HANDLE_VALUE;
	m_hGuiThread = INVALID_HANDLE_VALUE;
	m_hCalGridThread = INVALID_HANDLE_VALUE;
	m_hStartWork = INVALID_HANDLE_VALUE;
	//创建Ping-Pong缓冲区
	int j = 0;
	int k = 0;
	int m = 0;
	m_fPingPongBuffer = new int***[PINGPONG];
	for (i = 0; i < PINGPONG; i++)
	{
		m_fPingPongBuffer[i] = new int**[SOUNDDEV_NUM];
		for (j = 0; j < SOUNDDEV_NUM; j++)
		{
			m_fPingPongBuffer[i][j] = new int*[ARM_NUM];
			for (k = 0; k < ARM_NUM; k++)
			{
				m_fPingPongBuffer[i][j][k] = new int[PERMODULE_DATA_LENTH];
			}
		}
	}
	//初始化缓冲区
	for (i = 0; i < PINGPONG; i++)
	{
		for (j = 0; j < SOUNDDEV_NUM; j++)
		{
			for (k = 0; k < ARM_NUM; k++)
			{
				for (m = 0; m < PERMODULE_DATA_LENTH; m++)
				{
					m_fPingPongBuffer[i][j][k][m] = 0.0f;
				}
			}
		}
	}
	//初始化网格数据缓冲区
	m_pCalTGridData = new float*[GRID_ROW_NUM];
	for (i = 0; i < GRID_ROW_NUM; i++)
	{
		m_pCalTGridData[i] = new float[GRID_COLUMN_NUM];
	}
	for (i = 0; i < GRID_ROW_NUM; i++)
	{
		for (j = 0; j < GRID_COLUMN_NUM; j++)
		{
			m_pCalTGridData[i][j] = 0.0f;
		}
	}

	m_pfCurBufferPointer = NULL;

	//初始化区域平均温度缓冲区
	m_iAreaAverageT = new  UINT[AREA_BLOCK_NUM];
	memset(m_iAreaAverageT, 0, AREA_BLOCK_NUM*sizeof(UINT));
	//初始化路径平均温度
	m_iLineAverageT = new  UINT[PATH_NUM];
	memset(m_iLineAverageT, 0, PATH_NUM*sizeof(UINT));
	m_iLineAverageTB = new  UINT[PATH_NUM];
	memset(m_iLineAverageTB, 0, PATH_NUM*sizeof(UINT));

	f_da = new float[10];
	memset(f_da, 0.0f, 10 * sizeof(float));

	m_ipWord = new DWORD[ARM_NUM];
	memset(m_ipWord, 0, (ARM_NUM)*sizeof(DWORD));

	m_iPort = new u_short[ARM_NUM];
	memset(m_iPort, 0, (ARM_NUM)*sizeof(u_short));

	memset(&m_gridDataInfo, 0, sizeof(GridBaiscInfo));



	m_iCurrentFaultDev = 0;
	m_SaveAreaTLimit = new AreaTParaSet[AREA_BLOCK_NUM];
	memset(m_SaveAreaTLimit, 100, AREA_BLOCK_NUM*sizeof(AreaTParaSet));

	for (int i = 0; i < ARM_NUM; i++)
	{
		m_bPerArmConnState[i] = TRUE;
	}
	//初始化每个模块发生五次每次发声处理后的结果

	iPerArmTime = new int**[SOUNDDEV_NUM];
	for (i = 0; i < SOUNDDEV_NUM; i++)
	{
		iPerArmTime[i] = new int*[ARM_NUM];
		for (j = 0; j < ARM_NUM; j++)
		{
			iPerArmTime[i][j] = new int[SOUND_TIMES];
		}
	}

	for (i = 0; i < SOUNDDEV_NUM; i++)
	{
		for (j = 0; j < ARM_NUM; j++)
		{
			for (k = 0; k < SOUND_TIMES; k++)
			{
				iPerArmTime[i][j][k] = 0;
			}
		}

	}

	GasIP = htonl(inet_addr("192.168.0.25"));
	GasPort = 8989;
	//GasIP = htonl(inet_addr("127.0.0.6"));
	//GasPort = 6000;
	//bSoundCycleOver = 0;
	falseCount = 0;

	order[0] = 1;
	order[1] = 2;
	order[2] = 3;
	order[3] = 4;
	order[4] = 5;
	order[5] = 6;
	order[6] = 7;
	order[7] = 8;
	order[8] = 9;
	order[9] = 10;
	//order[4] = { 1, 2, 4, 3 };


	/*CString ip[5] = { "192.168.0.11", "192.168.0.12", "192.168.0.13", "192.168.0.14", "192.168.0.15" };
	
	for (int i = 0; i < 5; i++)
	{
		port[i] = 8989;
	}*/
	//A侧初始化
	CString ip_A[5] = { "192.168.184.137", "192.168.184.137", "192.168.184.137", "192.168.184.137", "192.168.184.137" ,};
	for (int i = 0; i < 5; i++)
	{
		port[i] = 6000 + i;
	}
	armnum = ARM_NUM;

	SAR_A.SnR_Init(order, ip_A, port, armnum, 5, 10);	//初始化发声接收对象，最后一个5指每个发声接收装置连续发声5次
	CT_A.Init(200, 800, 1200, 19700, 0.05);		//初始化时间计算对象
	//B侧初始化
	CString ip_B[5] = { "192.168.184.137", "192.168.184.137", "192.168.184.137", "192.168.184.137", "192.168.184.137" };
	for (int i = 5; i < 10; i++)
	{
		port[i-5] = 6000 + i;
	}
	armnum = ARM_NUM;

	SAR_B.SnR_Init(order, ip_B, port, armnum, 5, 10);	//初始化发声接收对象，最后一个5指每个发声接收装置连续发声5次
	CT_B.Init(200, 800, 1200, 19700, 0.05);		//初始化时间计算对象
}

CMainFrame::~CMainFrame()
{
	Sleep(200);
	EndThreadWork(m_hGuiThread, 1000);
	EndThreadWork(m_hStartWork, 1000);
	Sleep(500);

	m_hWorkThread = INVALID_HANDLE_VALUE;
	m_hGuiThread = INVALID_HANDLE_VALUE;
	m_hCalGridThread = INVALID_HANDLE_VALUE;
	m_hStartWork = INVALID_HANDLE_VALUE;

	int i = 0;
	int j = 0;
	int k = 0;
	for (i = 0; i < PINGPONG; i++)
	{
		for (j = 0; j < SOUNDDEV_NUM; j++)
		{
			for (k = 0; k < ARM_NUM; k++)
			{
				delete m_fPingPongBuffer[i][j][k];
			}
			delete m_fPingPongBuffer[i][j];
		}
		delete m_fPingPongBuffer[i];
	}
	delete[] m_fPingPongBuffer;

	delete[] m_iAreaAverageT;

	for (int i = 0; i < GRID_ROW_NUM; i++)
	{
		delete[] m_pCalTGridData[i];
	}
	delete[] m_pCalTGridData;
	m_pCalTGridData = NULL;
	m_fPingPongBuffer = NULL;
	m_fPingPongBuffer = NULL;

	if (m_ipWord)
	{
		delete[] m_ipWord;
		m_ipWord = NULL;
	}
	if (m_iPort)
	{
		delete[] m_iPort;
		m_iPort = NULL;
	}

	if (m_SaveAreaTLimit)
	{
		delete[] m_SaveAreaTLimit;
		m_SaveAreaTLimit = NULL;
	}

	for (int i = 0; i < SOUNDDEV_NUM; i++)
	{
		for (j = 0; j < ARM_NUM; j++)
		{
			delete iPerArmTime[i][j];
		}

		delete iPerArmTime[i];
	}
	delete[] iPerArmTime;

	WSACleanup(); //清除套接字资源
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	////////////////////////////读ini配置文件:系统环境配置////////////////////////////////////////////////////////////

	if (ReadSystemIniConfig() == ERROR_READINI)
	{
		//记录下来，并显示
		AfxMessageBox(_T("读取系统配置文件失败,请重新启动程序!"));
	}
	////////////////////////////初始化并连MySQL接数据库////////////////////////////////////////////////////////////
	//初始化mysql
	if (!m_OperateDB.InitMysql(&m_mysql))
	{

		AfxMessageBox(_T("初始化数据库错误!"));
	}
	//连接mysql
	if (!m_OperateDB.ConnectMysql(&m_mysql, (LPSTR)(LPCTSTR)m_strMySqlUserID, (LPSTR)(LPCTSTR)m_strMySqlPassword, (LPSTR)(LPCTSTR)m_strMySqlName, m_iMySqlPort))
	{
		AfxMessageBox(_T("连接数据库错误!"));
	}
	//读取系统退出密码
	/*m_strExitPassword = ReadSysExitPass(m_mysql);
	if (m_strExitPassword == _T("error"))
	{
	AfxMessageBox(_T("读取系统密码错误！"));
	}*/


	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	/*if (!m_toolDlgBar.Create(this,IDD_TOOL_DLGBAR,WS_CHILD | WS_VISIBLE | CBRS_TOP,IDD_TOOL_DLGBAR))
	{
	TRACE0("未能创建工具栏\n");
	return -1;      // fail to create
	}*/

	//状态栏设置
	/*	if(!m_wndStatusBar.Create(this) ||!m_wndStatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT)))
	{
	TRACE0("Failed to create status bar\n");
	return -1;      // fail to create
	}

	m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_NAME, SBPS_POPOUT|SBPS_NOBORDERS, 150);
	m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_ALARM_STATUS, SBPS_POPOUT|SBPS_NOBORDERS, 150);
	m_wndStatusBar.SetPaneInfo(3, ID_INDICATOR_COMMUNICATE_STATUS, SBPS_POPOUT|SBPS_NOBORDERS, 150);
	m_wndStatusBar.SetPaneInfo(4, ID_INDICATOR_DATE, SBPS_POPOUT|SBPS_NOBORDERS, 80);
	m_wndStatusBar.SetPaneInfo(5, ID_INDICATOR_WEEK, SBPS_POPOUT|SBPS_NOBORDERS, 80);
	m_wndStatusBar.SetPaneInfo(6, ID_INDICATOR_TIME, SBPS_POPOUT|SBPS_NOBORDERS, 50);


	//	m_wndStatusBar.GetStatusBarCtrl().SetBkColor(RGB(20,128,190)); //设置状态栏背景
	m_wndStatusBar.GetStatusBarCtrl().SetMinHeight(40*YCOFF); //设置状态栏高度
	m_wndStatusBar.GetStatusBarCtrl().SetSimple(FALSE);
	*/
	CRect rect(0, 0, 1360, 1000);
	RecalcLayout();

	//	m_pViews[0] = pActiveView;
	m_pViews[1] = (CView*) new CIsothermView;
	m_pViews[2] = (CView*) new CAreaAvrgTemperatureView;
	m_pViews[3] = (CView*) new CTTrendView;
	m_pViews[4] = (CView*) new CDisAreaAvrgTInfoView;
	m_pViews[5] = (CView*) new CPositionView;
	m_pViews[6] = (CView*) new CSignalTimeView;
	m_pViews[7] = (CView*) new CLeakView;

	UINT viewID[INTERFACE_NUM] = { 0 };
	CRect rectP(0, 0, 0, 0); // gets resized later
	viewID[0] = AFX_IDW_PANE_FIRST;

	for (int nView = 1; nView < INTERFACE_NUM; nView++)
	{
		viewID[nView] = AFX_IDW_PANE_FIRST + nView;
		m_pViews[nView]->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW& ~WS_VISIBLE, rect, this, viewID[nView], NULL);
	}

	RECT rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
	MoveWindow(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
	SetMenu(NULL);

	//创建界面数据更新线程
	m_hGuiThread = StartThreadWork(this, UpdateGuiThread);
	if (m_hGuiThread == INVALID_HANDLE_VALUE)
	{
		RecordWarningMessage("start UpdateGuiThread failed");
		AfxMessageBox(_T("开启界面更新线程失败"));
	}

	//创建开始工作线程
	m_hStartWork = StartThreadWork(this, StartWorkThread);
	if (m_hStartWork == INVALID_HANDLE_VALUE)
	{
		RecordWarningMessage("start StartWorkThread failed");
		AfxMessageBox(_T("开启开始工作线程失败！"));
	}

	ResumeThreadWork(m_hGuiThread);//启动界面更新线程
	ResumeThreadWork(m_hStartWork); //启动开始工作线程

	SetThreadPriority(m_hGuiThread, THREAD_PRIORITY_ABOVE_NORMAL);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	cs.style &= ~WS_MAXIMIZEBOX;
	//	cs.style &= ~WS_MINIMIZEBOX;	
	//	cs.style=cs.style&~WS_SYSMENU;
	cs.style &= ~WS_SIZEBOX;//去掉窗口右下角的三根斜杠
	return TRUE;
}

//界面切换
BOOL CMainFrame::SelView(VIEW_STATUS nView)
{
	CView* pViewAdd;
	CView* pViewRemove;
	CDocument* pDoc = GetActiveDocument();

	if (nView == m_nCurViewStatus)
		return FALSE;
	switch (nView)
	{
	case MAINDIS_VIEW:
		pViewAdd = m_pViews[0];
		pViewRemove = m_pViews[m_nCurViewStatus - 1];
		m_nCurViewStatus = MAINDIS_VIEW;
		break;
	case ISOTHERM_VIEW:
		pViewAdd = m_pViews[1];
		pViewRemove = m_pViews[m_nCurViewStatus - 1];
		m_nCurViewStatus = ISOTHERM_VIEW;
		break;
	case AREAAVRGT_VIEW:
		pViewAdd = m_pViews[2];
		pViewRemove = m_pViews[m_nCurViewStatus - 1];
		m_nCurViewStatus = AREAAVRGT_VIEW;
		break;
	case TTREND_VIEW:
		pViewAdd = m_pViews[3];
		pViewRemove = m_pViews[m_nCurViewStatus - 1];
		m_nCurViewStatus = TTREND_VIEW;
		break;
	case AVRGTINFO_VIEW:
		pViewAdd = m_pViews[4];
		pViewRemove = m_pViews[m_nCurViewStatus - 1];
		m_nCurViewStatus = AVRGTINFO_VIEW;
		break;
	case POSITION_VIEW:
		pViewAdd = m_pViews[5];
		pViewRemove = m_pViews[m_nCurViewStatus - 1];
		m_nCurViewStatus = POSITION_VIEW;
		break;
	case TIME_VIEW:
		pViewAdd = m_pViews[6];
		pViewRemove = m_pViews[m_nCurViewStatus - 1];
		m_nCurViewStatus = TIME_VIEW;
		break;
	case LEAK_VIEW:
		pViewAdd = m_pViews[7];
		pViewRemove = m_pViews[m_nCurViewStatus - 1];
		m_nCurViewStatus = LEAK_VIEW;
		break;
	default:
		break;
	}

	// 将活动视的child id设置为AFX_IDW_PANE_FIRST
	// 将其它视设置为AFX_IDW_PANE_FIRST以外的值，
	// 这样当调用CFrameWnd::RecalcLayout重新布局窗口时，
	// 才会得到正确的视
	int nSwitchChildID = pViewAdd->GetDlgCtrlID();
	pViewAdd->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
	pViewRemove->SetDlgCtrlID(nSwitchChildID);
	// 显示活动视而隐藏非活动视
	pViewAdd->ShowWindow(SW_SHOW);
	pViewRemove->ShowWindow(SW_HIDE);
	// 将新的活动视连接到文档，并断开原来的视与文档的连接
	pDoc->AddView(pViewAdd);
	pDoc->RemoveView(pViewRemove);
	SetActiveView(pViewAdd);
	RecalcLayout();

	return TRUE;
}
// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

//////////////////////////////////////////////////线程函数/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//界面更新线程
DWORD WINAPI UpdateGuiThread(LPVOID lpParam)
{
	CMainFrame *pWnd = (CMainFrame *)lpParam;
	if (!pWnd)
	{
		return -1;
	}
	/*CTime stoptime = CTime::GetCurrentTime();
	if (stoptime.GetHour() >= 9 && stoptime.GetHour() < 11)
	{
	return 0;
	}*/
	while (gbMainWorkThreadFlag)
	{
		//获取系统时间
		time_t msgTime;
		time(&msgTime);
		::SendMessage(pWnd->GetSafeHwnd(), WM_GUIDISPLAY_MSG, (WPARAM)pWnd, (LPARAM)msgTime);//界面显示
		::Sleep(100);
	}
	return 0;
}

//开始工作线程
DWORD WINAPI StartWorkThread(LPVOID lpParam)
{
	CMainFrame *pWnd = (CMainFrame *)lpParam;
	if (!pWnd)
	{
		return -1;
	}

	/*CTime stoptime = CTime::GetCurrentTime();
	if (stoptime.GetHour() >= 9 && stoptime.GetHour() < 11)
	{
	return 0;
	}*/
	while (gbMainWorkThreadFlag)
	{
		time_t msgTime;
		time(&msgTime);
		pWnd->StartDataProcess((WPARAM)pWnd, (LPARAM)msgTime);
		Sleep(2000);
	}
	return 0;
}

/////////////////////////////////////////////////线程处理函数////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//界面显示及更新  (WPARAM)pWnd,(LPARAM)msgTime
LRESULT CMainFrame::OnGuiDisplay(WPARAM wparam, LPARAM lparam)
{
	//TRACE0(_T("OnGuiDisplay\n"));

	CMainFrame *pWnd = (CMainFrame *)wparam;
	if (!pWnd)
	{
		return -1;
	}

	switch (m_currentMsg)
	{
		TRACE0(_T("SWITCH\n"));
	case WM_MAINDIS_MSG://主界面
		if (m_pViews[0])
		{
			m_pViews[0]->SendMessage(m_currentMsg, wparam, lparam);
		}
		break;
	case WM_ISOTHERM_MSG:// 温度场:温度网格数据、网格数据信息
		if (m_pViews[1])
		{
			m_pViews[1]->SendMessage(m_currentMsg, WPARAM(pWnd->GetGridData()), (LPARAM)pWnd->GetGridDataInfo());
		}
		break;
	case WM_AREAARVGT_MSG://区域平均温度：区域平均温度
		if (m_pViews[2])
		{
			m_pViews[2]->SendMessage(m_currentMsg, WPARAM(pWnd->GetAreaAvrgTValue()), LPARAM(pWnd->GetAreaTParaSetPointer()));
		}
		break;
	case WM_TTREND_MSG://温度趋势：数据库句柄
		if (m_pViews[3])
		{
			m_pViews[3]->SendMessage(m_currentMsg, (WPARAM)&(pWnd->GetMySQLHandle()), lparam);
		}
		break;
	case WM_AREATINFO_MSG://温度信息统计
		if (m_pViews[4])
		{
			m_pViews[4]->SendMessage(m_currentMsg, wparam, lparam);
		}
		break;
	case WM_POSITION_MSG://测点位置
		if (m_pViews[5])
		{
			m_pViews[5]->SendMessage(m_currentMsg, wparam, lparam);
		}
		break;
	case WM_TIME_MSG://时域信号：原始数据
		if (m_pViews[6])
		{
			m_pViews[6]->SendMessage(m_currentMsg, WPARAM(pWnd->GetAcousticData()), lparam);
		}
		break;
	case WM_LEAK_MSG://泄漏
		if (m_pViews[7])
		{
			m_pViews[7]->SendMessage(m_currentMsg, wparam, lparam);
		}
		break;
	}
	//	SetStatusBar(wparam,lparam);  //状态栏
	//TRACE0(_T("RETURN\n"));
	return 0;
}

//数据处理
LRESULT CMainFrame::StartDataProcess(WPARAM wparam, LPARAM lparam)
{

	int i, n, m, j, ArmSound;					//定义中间过程变量
	j = 0;
	CString debugstr;						//调试对象使用输出字符串
	ofstream fout;
	string FilePath="E:\\pcwavedata\\vcout\\1.txt";
	unsigned int CircleNum=0;
	while (1)
	{
		//L侧
		for (i = 0; i < armnum; i++)				//发声循环
		{
			ArmSound = order[i] - 1;            //编号从1开始，程序从0开始
			/*****************开始发声***************/
			SAR_A.SnR_Circle(ArmSound);     //发声循环函数

			for (n = 0; n < armnum; n++)
			{
				if (SAR_A.SnRModule[n].SensorFlag)
				{
					//AfxMessageBox(_T("Sensor err"));//探头报错
					OutputDebugString(_T("Sensor err\n"));
				}
			}
			/****************发声结束****************/
			/****************开始飞渡计算************/
			CT_A.MainSound = ArmSound;
			CT_A.ReadData(SAR_A.ArmSoundData, 5, 5);
			
			CT_A.CalcTime();
			FT_A.iSound = ArmSound;
			for (n = 0; n < armnum; n++)  //接收计算循环
			{
				for (m = 0; m < 5; m++)
				{
					FT_A.iTime[n][m] = CT_A.Time[n][m];
				}
			}
			FT_A.CalcFlightTime(2);
			/***************飞渡计算完成************/
		}
		/***************发声轮转结束*************/

		//SaveWaveData();  //保存波形数据到txt

		/***************开始区域温度计算********/
		for (i = 0; i < armnum; i++)
		{
			for (n = 0; n < armnum; n++)
			{
				TF_A.MyTraveTime[i][n] = FT_A.FTime[i][n];
			}
		}
		TF_A.BuildAreaT_LSM(Startflag);

		//for (i = 0; i < AREA_BLOCK_NUM; i++)
		for (i = 0; i < PATH_NUM; i++)
		{
			m_iLineAverageT[i] = TF_A.aver_T_P[i];
		}
		for (i = 0; i < AREA_BLOCK_NUM-6; i++)
		{
			m_iAreaAverageT[i] = TF_A.AreaT[i];
		}
		//RecordAreaT(m_iAreaAverageT);
		//计算网格数据
		BOOL bRet = FALSE;
		bRet = m_RebuildAreaT.CalTGridData(m_iAreaAverageT, m_pCalTGridData, AREA_BLOCK_NUM, &m_gridDataInfo);
		if (bRet == FALSE)
		{

			RecordWarningMessage("计算温度场数据失败");
			//return 0;
		}

		//R侧
		
		for (i = 0; i < armnum; i++)				//发声循环
		{
			ArmSound = order[i] - 1;            //编号从1开始，程序从0开始
			/////////////////////开始发声/////////////////////
			SAR_B.SnR_Circle(ArmSound);     //发声循环函数

			for (n = 0; n < armnum; n++)
			{
				if (SAR_B.SnRModule[n].SensorFlag)
				{
					//AfxMessageBox(_T("Sensor err"));//探头报错
					OutputDebugString(_T("Sensor err\n"));
				}
			}
			/////////////////////发声结束/////////////////////
			/////////////////////开始飞渡计算/////////////////////
			CT_B.MainSound = ArmSound;
			CT_B.ReadData(SAR_B.ArmSoundData, 5, 5);

			CT_B.CalcTime();
			FT_B.iSound = ArmSound;
			for (n = 0; n < armnum; n++)  //接收计算循环
			{
				for (m = 0; m < 5; m++)
				{
					FT_B.iTime[n][m] = CT_B.Time[n][m];
				}
			}
			FT_B.CalcFlightTime(2);
			/////////////////////飞渡计算完成/////////////////////
		}
		/////////////////////发声轮转结束/////////////////////

		//SaveWaveData();  //保存波形数据到txt

		/////////////////////开始区域温度计算/////////////////////
		for (i = 0; i < armnum; i++)
		{
			for (n = 0; n < armnum; n++)
			{
				TF_B.MyTraveTime[i][n] = FT_B.FTime[i][n];
			}
		}
		TF_B.BuildAreaT_LSM(Startflag);
		
		
		//for (i = 0; i < AREA_BLOCK_NUM; i++)
		for (i = 0; i < PATH_NUM; i++)
		{
			m_iLineAverageTB[i] = TF_B.aver_T_P[i];
		}
		for (i = 6; i < AREA_BLOCK_NUM; i++)
		{
			m_iAreaAverageT[i] = TF_B.AreaT[i-6];
		}
		
		//RecordAreaT(m_iAreaAverageT);
		//计算网格数据
		
		BOOL bRet = FALSE;
		bRet = m_RebuildAreaT.CalTGridData(m_iAreaAverageT, m_pCalTGridData, AREA_BLOCK_NUM, &m_gridDataInfo);
		if (bRet == FALSE)
		{

			RecordWarningMessage("计算温度场数据失败");
			return 0;
		}

		/***************区域温度计算完成************/
		//j++;
		//debugstr.Format("J=%d\n", j);
		//OutputDebugString(debugstr);
		/* 
		for (i = 0; i < 10; i++)
		{
			for (int j = 0; j < 60; j++)
			{
				Sleep(1000);
			}
		}*/

		Startflag=false;
		if(CircleNum<365*24*3600*10)
		{
			CircleNum++;
		}
		else
		{
			CircleNum=0;
		}
		fout.open(FilePath.c_str(),ios::app|ios::_Nocreate);
		fout<<"No."<<CircleNum<<"  A:"<<" "<<TF_A.AreaT[0]<<","<<TF_A.AreaT[1]<<","<<TF_A.AreaT[2]<<","<<TF_A.AreaT[3]<<","<<TF_A.AreaT[4]<<","<<TF_A.AreaT[5];
		fout<<"    B:"<<" "<<TF_B.AreaT[0]<<","<<TF_B.AreaT[1]<<","<<TF_B.AreaT[2]<<","<<TF_B.AreaT[3]<<","<<TF_B.AreaT[4]<<","<<TF_B.AreaT[5]<<endl;
		fout.close();
	}
	return true;
}

///////////////////////////////////////////记录数据/////////////////////////////////////////////////////

// 初始化MySQL数据库
ERROR_MYSQL CMainFrame::MySQLInit(char* userID, char* password, char* dbName, unsigned int port)
{
	/*初始化MySql数据库*/
	if (mysql_init(&m_mysql) == NULL)
	{
		return ERROR_MYSQL_INIT;
	}

	if (!mysql_real_connect(&m_mysql, _T("localhost"), userID, password, dbName, port, NULL, 0))
	{
		return  ERROR_MYSQL_CONNECT;
	}
	return NO_MYSQL_ERROR;
}

//保存波形数据到txt
BOOL CMainFrame::SaveWaveData()
{

	FILE      *stream;
	int i, j, k, n;
	CTime timeTmp = CTime::GetCurrentTime();;
	CString m_timestr;
	m_timestr = timeTmp.Format("%Y%m%d");

	CString strDirec, strCyc;
	CString pathName;
	pathName = _T("E:\\WaveData");
	if (!PathIsDirectory(pathName))
	{
		if (!CreateDirectory(pathName, NULL))//here
		{
			return 0;
		}
	}

	pathName = pathName + _T("\\") + m_timestr;

	if (!PathIsDirectory(pathName))
	{
		if (!CreateDirectory(pathName, NULL))//here
		{
			return 0;
		}
	}

	strCyc = timeTmp.Format("%H-%M");
	strCyc = _T("\\") + strCyc;
	//strCyc.Format(_T("\\%d-%d"), timeTmp.GetHour(), timeTmp.GetMinute());	
	pathName = pathName + strCyc;

	if (!PathIsDirectory(pathName))
	{
		if (!CreateDirectory(pathName, NULL))//here
		{
			return 0;
		}
	}

	old_pathDirectory = pathName;

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			for (n = 0; n < 5; n++)
			{
				strDirec.Format(_T("\\%d-%d-%d.txt"), i + 1, j + 1, n + 1);
				pathDirectory = old_pathDirectory + strDirec;

				if ((stream = fopen((LPSTR)(LPCTSTR)pathDirectory, "w+ ")) != NULL)
				{
					for (k = 0; k < 5120; k++)
					{						
						fprintf(stream, "%f    ", SAR_A.ArmSoundData[i][j][n][k]);
						fprintf(stream, "\n");
					}
					fclose(stream);
				}
			}
		}
	}


	return TRUE;
}

//记录错误信息
LRESULT CMainFrame::RecordWarningMessage(CString s_strSQL)
{
	CTime timeTmp = CTime::GetCurrentTime();
	CString m_timestr;
	m_timestr = timeTmp.Format("%Y%m%d %H:%M:%S");

	CString strSQL, strTemp;

	strTemp = s_strSQL;
	strSQL.Format(_T("insert into warnmessage(TIME,WARNMESS)values(\'%s\',\'%s\')"), m_timestr, strTemp);

	if (mysql_real_query(&m_mysql, (char*)(LPCTSTR)strSQL, (UINT)strSQL.GetLength()) != 0)
	{
		//AfxMessageBox(_T("记录WARNING MESSSGE失败！"));
	}

	return 0;
}

//记录f_da到mysql
LRESULT CMainFrame::RecordDA(float* f_da)
{
	CTime timeTmp = CTime::GetCurrentTime();
	CString m_timestr;
	m_timestr = timeTmp.Format("%Y%m%d %H:%M:%S");

	int i = 0;
	CString strSQL;

	strSQL.Format(_T("insert into DA(TIME,DA0,DA1,DA2,DA3,DA4,DA5,DA6,DA7,DA8,DA9)values(\'%s\',\'%f\',\'%f\',\'%f\',\'%f\',\'%f\',\'%f\',\'%f\',\'%f\',\'%f\',\'%f\')"), m_timestr, f_da[0], f_da[1], f_da[2], f_da[3], f_da[4], f_da[5], f_da[6], f_da[7], f_da[8], f_da[9]);

	if (mysql_real_query(&m_mysql, (char*)(LPCTSTR)strSQL, (UINT)strSQL.GetLength()) != 0)
	{
		AfxMessageBox(_T("记录DA数据失败！"));
	}

	return 0;
}

//记录区域平均温度数据，计算一次保存一次
LRESULT CMainFrame::RecordAreaT(UINT* iAreaAvrgT)
{
	CTime timeTmp = CTime::GetCurrentTime();;
	UINT* pBuffer = iAreaAvrgT;
	CString m_timestr;
	m_timestr = timeTmp.Format("%Y%m%d %H:%M:%S");
	CString strSQL;
	strSQL.Format(_T("insert into areaavrgtvalue(TimeList,Area1AvrgTValue,Area2AvrgTValue,Area3AvrgTValue,Area4AvrgTValue,Area5AvrgTValue) \
					 					 					 					 					 values(\'%s\',\'%d\',\'%d\',\'%d\',\'%d\',\'%d\')"), m_timestr, pBuffer[0], pBuffer[1], pBuffer[2], pBuffer[3], pBuffer[4]);

	if (mysql_real_query(&m_mysql, (char*)(LPCTSTR)strSQL, (UINT)strSQL.GetLength()) != 0)
	{
		//AfxMessageBox(_T("记录区域平均温度失败"));
	}
	return 0;
}

// 得到mysql句柄
MYSQL CMainFrame::GetMySQLHandle(void)
{
	MYSQL mysql;
	if (m_cs.Lock())
	{
		mysql = m_mysql;
	}
	m_cs.Unlock();
	return mysql;
}

//记录travelTimeCalFlag错误
void CMainFrame::recordTravelTimeCalFlagError()
{
	if (travelTimeCalFlag[0] == 0)
	{
		RecordWarningMessage("TraveTimes[2][0] of this time ERROR!");
	}
	if (travelTimeCalFlag[1] == 0)
	{
		RecordWarningMessage("TraveTimes[3][0] of this time ERROR!");
	}
	if (travelTimeCalFlag[2] == 0)
	{
		RecordWarningMessage("TraveTimes[2][3] of this time ERROR!");
	}
	if (travelTimeCalFlag[3] == 0)
	{
		RecordWarningMessage("TraveTimes[1][2] of this time ERROR!");
	}
	if (travelTimeCalFlag[4] == 0)
	{
		RecordWarningMessage("TraveTimes[1][3] of this time ERROR!");
	}
	if (travelTimeCalFlag[5] == 0)
	{
		RecordWarningMessage("TraveTimes[6][4] of this time ERROR!");
	}
	if (travelTimeCalFlag[6] == 0)
	{
		RecordWarningMessage("TraveTimes[7][4] of this time ERROR!");
	}
	if (travelTimeCalFlag[7] == 0)
	{
		RecordWarningMessage("TraveTimes[6][7] of this time ERROR!");
	}
	if (travelTimeCalFlag[8] == 0)
	{
		RecordWarningMessage("TraveTimes[5][6] of this time ERROR!");
	}
	if (travelTimeCalFlag[9] == 0)
	{
		RecordWarningMessage("TraveTimes[5][7] of this time ERROR!");
	}
}

//////////////////////////////////////////////////////////系统配置/////////////////////////////////////////////////////////////////////////////////////////////////////////

// 读取系统配置文件
int CMainFrame::ReadSystemIniConfig(void)
{
	char str[MAX_PATH];
	CString strIniFilePath;
	strIniFilePath = _T("\\Include\\Config.ini");
	//读取数据库配置信息
	::GetPrivateProfileString(_T("MySQL"), _T("UseID"), _T("root"), str, MAX_PATH - 1, strIniFilePath.GetBuffer(0));
	strIniFilePath.ReleaseBuffer();
	m_strMySqlUserID = str;
	::GetPrivateProfileString(_T("MySQL"), _T("Password"), _T("123321"), str, MAX_PATH - 1, strIniFilePath.GetBuffer(0));
	strIniFilePath.ReleaseBuffer();
	m_strMySqlPassword = str;
	::GetPrivateProfileString(_T("MySQL"), _T("DBName"), _T("acoustictherm"), str, MAX_PATH - 1, strIniFilePath.GetBuffer(0));
	strIniFilePath.ReleaseBuffer();
	m_strMySqlName = str;
	m_iMySqlPort = ::GetPrivateProfileInt(_T("MySQL"), _T("Port"), 3306, strIniFilePath.GetBuffer(0));
	strIniFilePath.ReleaseBuffer();

	ReadCommunicateConfig(m_ipWord, m_iPort, ARM_NUM);
	ReadAreaTValveSettingConfig(m_SaveAreaTLimit, AREA_BLOCK_NUM);
	return 0;
}

// 读取通信配置信息
int CMainFrame::ReadCommunicateConfig(DWORD* dwIP, u_short* usPort, UINT nModuleNum)
{
	if (nModuleNum > ARM_NUM || nModuleNum < 0)
	{
		AfxMessageBox(_T("读取IP设置错误"));

		return -1;
	}

	char cValue[MAX_PATH];
	CString strIniFilePath;
	strIniFilePath = _T("\\Include\\Config.ini");
	CString strIP;

	UINT i = 0;
	for (i = 0; i < nModuleNum; i++)
	{
		strIP.Format(_T("IP%d"), i + 1);
		::GetPrivateProfileString(_T("Communicate"), strIP, _T("192.168.1.10"), cValue, MAX_PATH - 1, strIniFilePath.GetBuffer(0));
		strIniFilePath.ReleaseBuffer();
		strIP = cValue;
		dwIP[i] = htonl(inet_addr(strIP));

		strIP.Format(_T("Port%d"), i + 1);
		usPort[i] = ::GetPrivateProfileInt(_T("Communicate"), strIP, 6000, strIniFilePath.GetBuffer(0));
		strIniFilePath.ReleaseBuffer();
	}
	return 0;
}

int CMainFrame::ReadAreaTValveSettingConfig(AreaTParaSet* setAreaTLimit, UINT nAreaNum)
{
	if (nAreaNum > AREA_BLOCK_NUM || nAreaNum < 0)
	{
		CString strError;
		strError.Format(_T("读取区域平均温度阀值错误"));
		RecordWarningMessage(strError);
		AfxMessageBox(_T("读取区域平均温度阀值错误"));
		return -1;
	}

	//char cValue[MAX_PATH];
	CString strIniFilePath;
	strIniFilePath = _T("\\Include\\Config.ini");
	CString strLowLimit, strHighLimit;

	UINT i = 0;
	for (i = 0; i < nAreaNum; i++)
	{
		strLowLimit.Format(_T("AreaTLowLimit%d"), i + 1);
		setAreaTLimit[i].iLowTLimit = ::GetPrivateProfileInt(_T("AreaTLimit"), strLowLimit, 1, strIniFilePath.GetBuffer(0));
		strIniFilePath.ReleaseBuffer();

		strHighLimit.Format(_T("AreaTHighLimit%d"), i + 1);
		setAreaTLimit[i].iHighTLimit = ::GetPrivateProfileInt(_T("AreaTLimit"), strHighLimit, 1600, strIniFilePath.GetBuffer(0));
		strIniFilePath.ReleaseBuffer();
	}
	return 0;
}

///////////////////////////////////////////////////////////获取数据////////////////////////////////////////////////////////////////////////////////////////////////

//得到声波信号数据
int*** CMainFrame::GetAcousticData()
{
	int*** fTemp;
	if (m_cs.Lock())
		fTemp = m_pfCurBufferPointer;
	//	//fTemp = & (SAR_A.ArmSoundData[0][0][0][0]);
	m_cs.Unlock();
	return fTemp;
}

//获取区域平均温度
UINT* CMainFrame::GetAreaAvrgTValue()
{
	UINT* pTemp;

	if (m_cs.Lock())
	{
		pTemp = m_iAreaAverageT;
	}
	m_cs.Unlock();
	return pTemp;
}

UINT* CMainFrame::GetLineAvrgTValue()
{
	UINT* pTemp;
	if (m_cs.Lock())
	{
		pTemp = m_iLineAverageT;
	}
	m_cs.Unlock();
	return pTemp;
}
UINT* CMainFrame::GetLineAvrgTValueB()
{
	UINT* pTemp;
	if (m_cs.Lock())
	{
		pTemp = m_iLineAverageTB;
	}
	m_cs.Unlock();
	return pTemp;
}
// 获得网格信息
GridBaiscInfo* CMainFrame::GetGridDataInfo()
{
	m_gridDataInfo.col = GRID_COLUMN_NUM;
	m_gridDataInfo.row = GRID_ROW_NUM;
	m_gridDataInfo.width = GRID_COLUMN_NUM - 1;
	m_gridDataInfo.hight = GRID_ROW_NUM - 1;
	m_gridDataInfo.maxValue = 1400;
	m_gridDataInfo.minValue = 500;

	GridBaiscInfo* girdDataInfo;
	if (m_cs.Lock())
		girdDataInfo = &m_gridDataInfo;
	m_cs.Unlock();
	return girdDataInfo;
}

// 得到网格数据
float** CMainFrame::GetGridData()
{
	float** fTemp;
	if (m_cs.Lock())
	{
		fTemp = m_pCalTGridData;
	}

	m_cs.Unlock();
	return fTemp;
}

/////////////////////////////////////////////////////////////线程操作///////////////////////////////////////////////////////////////////////////////////////////////////////////
HANDLE CMainFrame::StartThreadWork(LPVOID lpParameter, LPTHREAD_START_ROUTINE threadFunc)
{
	HANDLE hThreadHandle = INVALID_HANDLE_VALUE;

	hThreadHandle = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadFunc,
		lpParameter, CREATE_SUSPENDED, NULL);
	return hThreadHandle;
	/*
	if(hThreadHandle)
	{
	::SetThreadPriority(hThreadHandle, THREAD_PRIORITY_BELOW_NORMAL);//设置线程优先级
	::ResumeThread(hThreadHandle);  //恢复线程
	}
	CloseHandle(hThreadHandle);
	*/
}

//暂停线程工作
void CMainFrame::SuspendThreadWork(HANDLE hThread)
{
	if (hThread != INVALID_HANDLE_VALUE)
	{
		SuspendThread(hThread);
	}
	else
	{
		CString strError;
		strError.Format(_T("暂停线程失败"));
		RecordWarningMessage(strError);
		AfxMessageBox(_T("暂停线程失败"));
	}


}

//恢复线程工作
void CMainFrame::ResumeThreadWork(HANDLE hThread)
{
	if (hThread != INVALID_HANDLE_VALUE)
	{
		::ResumeThread(hThread);  //恢复线程
	}
	else
	{
		CString strError;
		strError.Format(_T("恢复线程失败"));
		RecordWarningMessage(strError);
		AfxMessageBox(_T("恢复线程失败"));
	}


}

//结束线程
void CMainFrame::EndThreadWork(HANDLE hThread, DWORD dTimeOut)
{
	if (hThread != INVALID_HANDLE_VALUE)
	{
		DWORD dwRet = WaitForSingleObject(hThread, dTimeOut);
		if (dwRet != WAIT_OBJECT_0)
		{
			DWORD   dwExitCode = 0;
			GetExitCodeThread(hThread, &dwExitCode);
			if (dwExitCode == STILL_ACTIVE)
			{
				TerminateThread(hThread, dwExitCode);
			}
		}
		CloseHandle(hThread);
		//		delete hThread;
	}
}

/////////////////////////////////////////////////////////////系统密码///////////////////////////////////////////////////////////////////////////
CString CMainFrame::GetSysExitPassword()
{
	return m_strExitPassword;
}

void CMainFrame::SetSysExitPassword(CString strPassword)
{
	m_strExitPassword = strPassword;
}

CString CMainFrame::ReadSysExitPass(MYSQL hMysql)
{
	CString strPass(_T("error"));
	char *ch_query;
	ch_query = "select ExitPass from Password limit 0,1";  //查询第一行
	if (mysql_real_query(&hMysql, ch_query, (UINT)strlen(ch_query)) != 0) //执行查询
	{
		return strPass;
	}

	MYSQL_RES *result;
	MYSQL_ROW row;
	if (!(result = mysql_use_result(&hMysql)))//初始化检索
	{
		AfxMessageBox(_T("读取数据集失败"));
		return strPass;
	}

	if (row = mysql_fetch_row(result)){  //获取每一行行指针    
		strPass = row[0];
	}
	mysql_free_result(result);

	return strPass;
}

void CMainFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CExitDlg dlg;
	CString strTemp;
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_strPassword == m_strExitPassword || dlg.m_strPassword == _T(" "))
		{

			gbMainWorkThreadFlag = FALSE;
			CFrameWnd::OnClose();
		}
		else
			AfxMessageBox(_T("密码错误!\r\n请输入密码!"), MB_ICONSTOP);

	}
	gbMainWorkThreadFlag = FALSE;
	CFrameWnd::OnClose();
}

/////////////////////////////////////////////////////////TCP/IP通信配置//////////////////////////////////////////////////////////////////////

int CMainFrame::SaveIPAndPortSetting(DWORD* dwIP, u_short* usPort, UINT nModuleNum)
{
	if (nModuleNum > ARM_NUM || nModuleNum < 0)
	{
		AfxMessageBox(_T("保存IP设置错误"));
		return -1;
	}
	CString strIniFilePath;
	strIniFilePath = _T("\\Include\\Config.ini");

	CString strIP;
	CString strPort;
	unsigned   char   *pIP;
	CString strKeyName;

	//int i = 0;
	UINT i = 0;
	for (i = 0; i < nModuleNum; i++)
	{
		pIP = (unsigned   char*)&dwIP[i];
		strIP.Format("%u.%u.%u.%u", *(pIP + 3), *(pIP + 2), *(pIP + 1), *pIP);
		strKeyName.Format(_T("IP%d"), i + 1);
		::WritePrivateProfileString("Communicate", strKeyName, strIP.GetBuffer(0), strIniFilePath.GetBuffer(0));
		strIP.ReleaseBuffer();
		strIniFilePath.ReleaseBuffer();

		strKeyName.Format(_T("Port%d"), i + 1);
		strPort.Format(_T("%d"), usPort[i]);
		::WritePrivateProfileString("Communicate", strKeyName, strPort.GetBuffer(0), strIniFilePath.GetBuffer(0));
		strPort.ReleaseBuffer();
		strIniFilePath.ReleaseBuffer();
	}

	return 0;
}

void CMainFrame::SetIPAddressAndPort(DWORD dwIP, u_short usPort, UINT iModuleNum)
{
	if (iModuleNum > ARM_NUM || iModuleNum < 0)
	{
		AfxMessageBox(_T("模块IP设置错误"));
		return;
	}
	m_ipWord[iModuleNum] = dwIP;
	m_iPort[iModuleNum] = usPort;
}

DWORD CMainFrame::GetIPAddress(UINT iModuleNum)
{
	if (iModuleNum > ARM_NUM || iModuleNum < 0)
	{
		AfxMessageBox(_T("获取模块IP错误"));
		return 0;
	}

	return m_ipWord[iModuleNum];
}

u_short CMainFrame::GetPortNum(UINT iModuleNum)
{
	if (iModuleNum > ARM_NUM || iModuleNum < 0)
	{
		AfxMessageBox(_T("获取模块IP错误"));
		return 6000;
	}
	return m_iPort[iModuleNum];
}

BOOL CMainFrame::GetOneArmState(UINT iArm)
{
	if (iArm < 0 || iArm >= ARM_NUM)
	{
		return TRUE;
	}

	return SAR_A.SnRModule[iArm].ArmFlag;
	//return m_bPerArmConnState[iArm];
}
BOOL CMainFrame::GetOneArmStateB(UINT iArm)
{
	if (iArm < 0 || iArm >= ARM_NUM)
	{
		return TRUE;
	}

	return SAR_B.SnRModule[iArm].ArmFlag;
	//return m_bPerArmConnState[iArm];
}

DWORD* CMainFrame::GetIPAddressPointer()
{
	return m_ipWord;
}

u_short* CMainFrame::GetPortPointer()
{
	return m_iPort;
}

int CMainFrame::SaveAreaTLimitSetting(AreaTParaSet* setAreaTLimit, UINT nAreaNum)
{
	if (nAreaNum > AREA_BLOCK_NUM || nAreaNum < 0)
	{
		AfxMessageBox(_T("保存区域平均温度阀值设置错误"));
		return -1;
	}
	CString strIniFilePath;
	strIniFilePath = _T("\\Include\\Config.ini");

	CString strTLowLimit;
	CString strTHighLimit;

	CString strKeyName;

	int i = 0;
	for (i = 0; i < nAreaNum; i++)
	{
		strKeyName.Format(_T("AreaTLowLimit%d"), i + 1);
		strTLowLimit.Format(_T("%d"), setAreaTLimit[i].iLowTLimit);
		::WritePrivateProfileString("AreaTLimit", strKeyName, strTLowLimit.GetBuffer(0), strIniFilePath.GetBuffer(0));
		strTLowLimit.ReleaseBuffer();
		strIniFilePath.ReleaseBuffer();

		strKeyName.Format(_T("AreaTHighLimit%d"), i + 1);
		strTHighLimit.Format(_T("%d"), setAreaTLimit[i].iHighTLimit);
		::WritePrivateProfileString("AreaTLimit", strKeyName, strTHighLimit.GetBuffer(0), strIniFilePath.GetBuffer(0));
		strTHighLimit.ReleaseBuffer();
		strIniFilePath.ReleaseBuffer();
	}

	return 0;
}

void CMainFrame::SetIAreaTLimit(AreaTParaSet* setAreaTLimit, UINT nAreaNum)
{
	if (nAreaNum > AREA_BLOCK_NUM || nAreaNum < 0)
	{
		AfxMessageBox(_T("保存区域平均温度阀值设置错误"));
		return;
	}

	for (UINT i = 0; i < nAreaNum; i++)
	{
		m_SaveAreaTLimit[i].iLowTLimit = setAreaTLimit[i].iLowTLimit;
		m_SaveAreaTLimit[i].iHighTLimit = setAreaTLimit[i].iHighTLimit;
	}

}

AreaTParaSet* CMainFrame::GetAreaTParaSetPointer(void)
{
	ASSERT(m_SaveAreaTLimit);
	return m_SaveAreaTLimit;
}

int CMainFrame::GetOneSounderState(int iSounder)
{
	if (iSounder < 0 || iSounder >= ARM_NUM)
	{
		return FALSE;
	}
	CString str;
	//str.Format("Module [%d] status %d \n", iSounder, SAR_A.SnRModule[iSounder].ArmStatus);
	OutputDebugString(str);
	return SAR_A.SnRModule[iSounder].ArmStatus;
}
int CMainFrame::GetOneSounderStateB(int iSounder)
{
	if (iSounder < 0 || iSounder >= ARM_NUM)
	{
		return FALSE;
	}
	CString str;
	//str.Format("Module [%d] status %d \n", iSounder, SAR_A.SnRModule[iSounder].ArmStatus);
	OutputDebugString(str);
	return SAR_B.SnRModule[iSounder].ArmStatus;
}

int CMainFrame::GetGasControlState()
{
	//OutputDebugString("GET GAS STATE!\n");
	if (SAR_A.SnRModule[0].GasStatus  && SAR_A.SnRModule[1].GasStatus  && SAR_A.SnRModule[2].GasStatus  && SAR_A.SnRModule[3].GasStatus /*&& SAR_A.SnRModule[4].GasStatus == 0 && SAR_A.SnRModule[5].GasStatus == 0 && SAR_A.SnRModule[6].GasStatus == 0 && SAR_A.SnRModule[7].GasStatus == 0*/)
	{
		return 0;
	}
	if (SAR_A.SnRModule[0].GasStatus || SAR_A.SnRModule[1].GasStatus || SAR_A.SnRModule[2].GasStatus || SAR_A.SnRModule[3].GasStatus  /*|| SAR_A.SnRModule[4].GasStatus == 1 || SAR_A.SnRModule[5].GasStatus == 1 || SAR_A.SnRModule[6].GasStatus == 1 || SAR_A.SnRModule[7].GasStatus == 1*/)
	{
		return 1;
	}
}

bool CMainFrame::GetOneSersorState(int iArm)
{
	if (iArm < 0 || iArm >= ARM_NUM)
	{
		return TRUE;
	}
	return SAR_A.SnRModule[iArm].SensorFlag;
}
bool CMainFrame::GetOneSersorStateB(int iArm)
{
	if (iArm < 0 || iArm >= ARM_NUM)
	{
		return TRUE;
	}
	return SAR_A.SnRModule[iArm].SensorFlag;
}


