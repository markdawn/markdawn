
// AcousticTherm.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "AcousticTherm.h"
#include "MainFrm.h"

#include "AcousticThermDoc.h"
#include "AcousticThermView.h"
#include "IsothermView.h"
#include "MainDIsInfoView.h"
#include "LeakView.h"

#define  _DEBUG_MEMORY_LEAKS //检测内存泄漏
#ifdef _DEBUG_MEMORY_LEAKS

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h> 
#include <crtdbg.h> 
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAcousticThermApp

BEGIN_MESSAGE_MAP(CAcousticThermApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CAcousticThermApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CAcousticThermApp 构造

CAcousticThermApp::CAcousticThermApp()
{
	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("AcousticTherm.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}
CAcousticThermApp::~CAcousticThermApp()
{
	if (CMFCVisualManager::GetInstance() != NULL)
	{
		delete CMFCVisualManager::GetInstance();
	}
//	_CrtDumpMemoryLeaks();
}

// 唯一的一个 CAcousticThermApp 对象
CAcousticThermApp theApp;


// CAcousticThermApp 初始化
BOOL CAcousticThermApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
#ifdef _DEBUG_MEMORY_LEAKS
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 
//	_CrtSetBreakAlloc(3885);//用来定位内存泄漏
#endif
/*          //access数据库
	if(!AfxOleInit())  //初始化OLE库环境
	{
		AfxMessageBox(_T("OLE 程序初始化失败。\r\n请确认 OLE 库程序是正确的版本。"));
		return FALSE;
	}

	HRESULT hr;
	try   //打开数据库
	{
		hr=m_pConnection.CreateInstance("ADODB.Connection");
		if(SUCCEEDED(hr))
			m_pConnection->Open("Provider=Microsoft.JET.OLEDB.4.0;Data Source=..\\Debug\\Include\\AcousticT.mdb","","",adModeUnknown);
	}
	catch(_com_error e)
	{
		CString strMsg;
		strMsg.Format(_T("数据库连接失败！\r\n错误信息：%s"),e.Description());
		AfxMessageBox(strMsg);

		return FALSE;
	}
*/
	AfxEnableControlContainer();
	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CAcousticThermDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CMainDIsInfoView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	  CView* pActiveView = ((CFrameWnd*) m_pMainWnd)->GetActiveView();//test 视图指针

	 CMainFrame *pMain =(CMainFrame *)AfxGetMainWnd();
	 if (pMain == NULL)
	 {
		 return FALSE;
	 }
	 pMain->m_pViews[0] = pActiveView;
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
//	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生

	//在此处可进行添加初始化
	return TRUE;
}



int CAcousticThermApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);
	return CWinApp::ExitInstance();
}

// CAcousticThermApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CAcousticThermApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CAcousticThermApp 消息处理程序



