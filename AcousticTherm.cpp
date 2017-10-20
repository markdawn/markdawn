
// AcousticTherm.cpp : ����Ӧ�ó��������Ϊ��
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

#define  _DEBUG_MEMORY_LEAKS //����ڴ�й©
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
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CAcousticThermApp ����

CAcousticThermApp::CAcousticThermApp()
{
	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("AcousticTherm.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}
CAcousticThermApp::~CAcousticThermApp()
{
	if (CMFCVisualManager::GetInstance() != NULL)
	{
		delete CMFCVisualManager::GetInstance();
	}
//	_CrtDumpMemoryLeaks();
}

// Ψһ��һ�� CAcousticThermApp ����
CAcousticThermApp theApp;


// CAcousticThermApp ��ʼ��
BOOL CAcousticThermApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
#ifdef _DEBUG_MEMORY_LEAKS
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 
//	_CrtSetBreakAlloc(3885);//������λ�ڴ�й©
#endif
/*          //access���ݿ�
	if(!AfxOleInit())  //��ʼ��OLE�⻷��
	{
		AfxMessageBox(_T("OLE �����ʼ��ʧ�ܡ�\r\n��ȷ�� OLE ���������ȷ�İ汾��"));
		return FALSE;
	}

	HRESULT hr;
	try   //�����ݿ�
	{
		hr=m_pConnection.CreateInstance("ADODB.Connection");
		if(SUCCEEDED(hr))
			m_pConnection->Open("Provider=Microsoft.JET.OLEDB.4.0;Data Source=..\\Debug\\Include\\AcousticT.mdb","","",adModeUnknown);
	}
	catch(_com_error e)
	{
		CString strMsg;
		strMsg.Format(_T("���ݿ�����ʧ�ܣ�\r\n������Ϣ��%s"),e.Description());
		AfxMessageBox(strMsg);

		return FALSE;
	}
*/
	AfxEnableControlContainer();
	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CAcousticThermDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CMainDIsInfoView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	  CView* pActiveView = ((CFrameWnd*) m_pMainWnd)->GetActiveView();//test ��ͼָ��

	 CMainFrame *pMain =(CMainFrame *)AfxGetMainWnd();
	 if (pMain == NULL)
	 {
		 return FALSE;
	 }
	 pMain->m_pViews[0] = pActiveView;
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
//	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����

	//�ڴ˴��ɽ�����ӳ�ʼ��
	return TRUE;
}



int CAcousticThermApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);
	return CWinApp::ExitInstance();
}

// CAcousticThermApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CAcousticThermApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CAcousticThermApp ��Ϣ�������



