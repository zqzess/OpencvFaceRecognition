
// OpencvTestDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "OpencvTest.h"
#include "OpencvTestDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "MatImage.h"
using namespace cv;
//#include <opencv.hpp>


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COpencvTestDlg 对话框



COpencvTestDlg::COpencvTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPENCVTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpencvTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COpencvTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &COpencvTestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &COpencvTestDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &COpencvTestDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// COpencvTestDlg 消息处理程序

BOOL COpencvTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void COpencvTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COpencvTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COpencvTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


UINT CamThread(LPVOID pwind);
//CEvent g_event;

HANDLE g_hEvent1 = NULL;

UINT CamThread(LPVOID pwind)
{
	VideoCapture capture(0);
	cv::Mat frame;
	CImage image;
	while (1)
	//while(WaitForSingleObject(g_hEvent1, INFINITE) != WAIT_OBJECT_0)
	{
		
		capture >> frame;
		cv::waitKey(10);
		MatImage mc;
		mc.MatToCImage(frame, image);
		/*HWND hwnd2 = ::FindWindow(NULL, _T("Cam"));//获取主窗口句柄
		CWnd* pMainWnd = AfxGetMainWnd();//获取主窗口指针*/

		CWnd* pMainWnd = AfxGetMainWnd();//获取主窗口指针
		HWND hwnd2 = pMainWnd->GetSafeHwnd();//获取主窗口句柄

		//CWnd* pMainWnd = CWnd::FromHandle(hwnd2);//主窗口句柄->指针

		HWND hwnd = GetDlgItem(hwnd2, IDC_STATIC);//获得pictrue控件窗口的句柄
		CWnd* pWnd = CWnd::FromHandle(hwnd);//控件句柄->指针

		CDC* pDC = pWnd->GetDC();
		HDC hDC = pDC->GetSafeHdc();
		CRect rect;
		pMainWnd->GetClientRect(&rect);
		pWnd->GetClientRect(&rect);
		Mat dst;
		int x = rect.Width();
		int y = rect.Height();
		resize(frame, dst, Size(x, y));
		mc.MatToCImage(dst, image);
		image.Draw(pDC->m_hDC, rect);
		if (WAIT_OBJECT_0 != WaitForSingleObject(g_hEvent1, INFINITE))	// 收到激发态的消息  
		{
			//return 0;//正常退出
			break;
		}
		//WaitForSingleObject(g_hEvent1, INFINITE);
	}
	
	return 0;
}

void COpencvTestDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	
	AfxBeginThread(CamThread, NULL);
	//ResetEvent(g_hEvent1);
	g_hEvent1 = CreateEvent(NULL, TRUE, TRUE, NULL);
}


void COpencvTestDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	//CDialogEx::OnCancel();
	ResetEvent(g_hEvent1);
	//ResetEvent(g_hEvent1);
	//SetEvent(g_hEvent1);
	//::PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	
	//DestroyWindow();
}


void COpencvTestDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	DestroyWindow();
}
