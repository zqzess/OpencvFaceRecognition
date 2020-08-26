
// OpencvTestDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "OpencvTest.h"
#include "OpencvTestDlg.h"
#include "afxdialogex.h"

#define APPID "D74VSBHHxiSzKKUiT9pN5P1krT6Mu2b1LMDxa1NLibcR"
#define SDKKEY "SDK KEY:FAwNxRfK2jMFmmQR4qgmRCHJeRJe7KjcBGriU38Ndt8f"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "MatImage.h"
using namespace std;
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
	ON_BN_CLICKED(IDC_BUTTON2, &COpencvTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &COpencvTestDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &COpencvTestDlg::OnBnClickedButton6)
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
HANDLE g_hEvent1 = NULL;
VideoCapture capture;
BOOL SFCZ = FALSE;
cv::Mat frame;

UINT CamThread(LPVOID pwind)
{
	//开启摄像头线程

	//VideoCapture capture(0);
	if (capture.isOpened())
	{
		AfxMessageBox(_T("摄像头已开启"));
		return 0;
	}
	capture.open(0);
	//cv::Mat frame;
	CImage image;
	//while (1)
	while(WaitForSingleObject(g_hEvent1, 1) != WAIT_OBJECT_0)
	{
		
		capture >> frame;
		//cv::waitKey(1);
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
		//WaitForSingleObject(g_hEvent1, INFINITE);
		//if (WAIT_OBJECT_0 == WaitForSingleObject(g_hEvent1, INFINITE))	// 收到激发态的消息  
		//{
		//	return 0;//正常退出
		//	//capture.release();
		//	//break;
		//}
	}
	//资源释放
	capture.release();//关闭摄像头
	image.ReleaseDC();
	return 0;
}

void COpencvTestDlg::OnBnClickedOk()
{
	// 开启摄像头按钮

	AfxBeginThread(CamThread, NULL);
	g_hEvent1=CreateEvent(NULL, TRUE, FALSE, NULL);//创建事件
	//SetEvent(g_hEvent1);
	//ResetEvent(g_hEvent1);
}


void COpencvTestDlg::OnBnClickedCancel()
{
	//关闭摄像头

	
	//ResetEvent(g_hEvent1);
	SetEvent(g_hEvent1);
	
	//::PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	
	//DestroyWindow();


	/*CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC);
	CRect IRect;
	pStatic->GetClientRect(&IRect);
	pStatic->GetDC()->FillSolidRect(IRect.left, IRect.top, IRect.Width(), IRect.Height(), RGB(240, 240, 240));*/

	Sleep(50);
	GetDlgItem(IDC_STATIC)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC)->ShowWindow(TRUE);
	SFCZ = FALSE;
	//Sleep(30);
	//Invalidate();//使整个窗体无效，全部重绘
	//UpdateWindow();

}


void COpencvTestDlg::OnBnClickedButton1()
{
	// 退出窗口
	CloseHandle(g_hEvent1);
	DestroyWindow();
	SFCZ = FALSE;
}




void COpencvTestDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!capture.isOpened())
	{
		AfxMessageBox(_T("请先开启摄像头"));
		return;
	}

	SetEvent(g_hEvent1);//设置事件有信号
	

	/*CString str;
	int a = AfxMessageBox(_T("是否保存？"), MB_OKCANCEL | MB_ICONQUESTION);
	str.Format(_T("不同选择，不同输出%d"), a);
	AfxMessageBox(str);*/

	SFCZ = TRUE;
}

void COpencvTestDlg::OnPictureSave(UINT ID)
{
	//CWnd* pWnd = GetDlgItem(ID);//获取控件指针
	//CDC* pDC = pWnd->GetDC();//获取控件DC
	////CImage imag;
	//CRect rect;
	//CImage image;
	//GetClientRect(&rect);//获取画布大小
	//pWnd->GetWindowRect(&rect);
	//image.Create(rect.Width(), rect.Height(),32);
	//::BitBlt(image.GetDC(), 0, 0, rect.Width(), rect.Height(), pDC->m_hDC, 0, 0, SRCCOPY);
	//TCHAR szFilter[] = _T("jpg file(*.jpg)|*.jpg|bmp file(*.bmp)|*.bmp|所有文件(*.*)|*.*||");  //文件格式过滤
	//CFileDialog fileDlg(FALSE, _T("jpg"), _T("1.jpg"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	//fileDlg.m_ofn.lpstrTitle = _T("保存直方图");  //保存对话窗口标题名
	//CString picturePath;
	//if (IDOK == fileDlg.DoModal())
	//{
	//	picturePath = fileDlg.GetPathName();
	//}
	//HRESULT hResult = image.Save(picturePath);
	//ReleaseDC(pDC);
	//image.ReleaseDC();

	if (!frame.empty())
	{
		CString FilePath;
		FilePath = "./人脸采集";
		if (!PathFileExists(FilePath))
		{
			CreateDirectory((LPCTSTR)FilePath, NULL);
		}

		//图片名随机数
		const int N = 100;
		int result = rand() % N;
		//str.Format(_T("./人脸采集/%d.jpg"), result);//可以把保存路径加到引号内。

		
		//保存图像的名字
		string strSaveName;
		char buffer[256];
		sprintf(buffer, "D%04d", result);
		strSaveName = buffer;

		//保存图像的完整路径
		string strImgSavePath = FilePath + "\\" + strSaveName;
		//保存图像的格式
		strImgSavePath += ".jpg";
		//strImgSavePath += ".png";


		bool sfcz = imwrite(strImgSavePath.c_str(), frame);
		if (sfcz == true)
		{
			AfxMessageBox(_T("保存成功"));
		}
		else {
			AfxMessageBox(_T("保存失败，请重新拍照"));
			return;
		}
		Sleep(200);
		//清理画面
		GetDlgItem(IDC_STATIC)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC)->ShowWindow(TRUE);
	}
	else
	{
		AfxMessageBox(_T("缓存器无图片"));
		return;
	}
	
}




void COpencvTestDlg::OnBnClickedButton5()
{
	//保存图片按钮

	if (SFCZ == FALSE)
	{
		AfxMessageBox(_T("请先打开摄像头拍照"));
		return;
	}
	OnPictureSave(IDC_STATIC_FACE);
	SFCZ =FALSE;
}


void COpencvTestDlg::OnBnClickedButton6()
{
	// 取消按钮
	SFCZ = FALSE;
	GetDlgItem(IDC_STATIC)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC)->ShowWindow(TRUE);
}
