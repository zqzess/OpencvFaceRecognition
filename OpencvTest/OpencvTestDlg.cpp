
// OpencvTestDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "OpencvTest.h"
#include "OpencvTestDlg.h"
#include "afxdialogex.h"
//#include "ArcFaceEngine.h"

#define APPID "xxxx"
#define SDKKEY "xxxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "MatImage.h"
#include <mutex>
#include "ArcFaceEngine.h"
using namespace cv::face;
using namespace std;
using namespace cv;

#define VIDEO_FRAME_DEFAULT_WIDTH 640
#define VIDEO_FRAME_DEFAULT_HEIGHT 480


HANDLE g_hEvent1 = NULL;
VideoCapture capture;
BOOL SFCZ = FALSE;
cv::Mat frame;
cv::Mat PicFrame;
cv::Mat FaceFrame;
mutex g_mutex;
MHandle handle = NULL;

CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

CascadeClassifier face_cascade2;
CascadeClassifier eyes_cascade2;

CascadeClassifier face_cascade3;
CascadeClassifier face_cascade4;

CascadeClassifier cascade;

string face_cascade_name = "";
string eyes_cascade_name = "";

Ptr<FaceRecognizer> modelPCA = createEigenFaceRecognizer();
Ptr<FaceRecognizer> modelPic = createEigenFaceRecognizer();


UINT CamThread(LPVOID pwind);
void timestampToTime(char* timeStamp, char* dateTime, int dateTimeSize);
void activesdk();
void PicCutOut(IplImage* src, IplImage* dst, int x, int y);
CBitmap* IplImage2CBitmap(const IplImage* img);


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
	DDX_Control(pDX, IDC_EDIT_LOG, m_editlog);
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
	ON_BN_CLICKED(IDC_BUTTON4, &COpencvTestDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &COpencvTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON7, &COpencvTestDlg::OnBnClickedButton7)
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
	

	face_cascade_name = ".//opencv//build//haarcascade_frontalface_alt2.xml";
	eyes_cascade_name = ".//opencv//build//haarcascade_eye_tree_eyeglasses.xml";
	if (!face_cascade.load(face_cascade_name)) { AfxMessageBox(_T("加载失败")); };
	if (!eyes_cascade.load(eyes_cascade_name)) { AfxMessageBox(_T("加载失败")); };

	if (!face_cascade2.load(face_cascade_name)) { AfxMessageBox(_T("加载失败")); };
	if (!eyes_cascade2.load(eyes_cascade_name)) { AfxMessageBox(_T("加载失败")); };

	if (!face_cascade3.load(face_cascade_name)) { AfxMessageBox(_T("加载失败")); };
	if (!face_cascade4.load(face_cascade_name)) { AfxMessageBox(_T("加载失败")); };

	if (!cascade.load(".//opencv//build//haarcascade_frontalface_alt2.xml")) { AfxMessageBox(_T("加载失败")); };
	modelPCA->load(".//FacesData//MyFacePCAModel6.xml");

	//开启控制台

	
	AllocConsole();//为当前的窗口程序申请一个Console窗口
	freopen("CONOUT$", "a+", stdout);
	
	//activesdk();
	

	/*首次使用激活*/
	CString resStr;
	MRESULT faceRes = m_imageFaceEngine.ActiveSDK(APPID, SDKKEY);
	resStr.Format(_T("激活结果: %d\n"), faceRes);
	EditOut(resStr, TRUE);

	//获取激活文件信息
	ASF_ActiveFileInfo activeFileInfo = { 0 };
	m_imageFaceEngine.GetActiveFileInfo(activeFileInfo);
	/*初始化接口*/
	if (faceRes == MOK)
	{
		resStr = "";
		faceRes = m_imageFaceEngine.InitEngine(ASF_DETECT_MODE_IMAGE);//Image
		resStr.Format(_T("IMAGE模式下初始化结果: %d"), faceRes);
		EditOut(resStr, TRUE);

		//resStr = "";
		//faceRes = m_videoFaceEngine.InitEngine(ASF_DETECT_MODE_VIDEO);//Video
		//resStr.Format(_T("VIDEO模式下初始化结果: %d"), faceRes);
		//EditOut(resStr, TRUE);
	}
	


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void activesdk()
{
	//激活与初始化，控制台版
	MRESULT res = MOK;
	ASF_ActiveFileInfo activeFileInfo = { 0 };
	res = ASFGetActiveFileInfo(&activeFileInfo);
	if (res != MOK)
	{
		printf("ASFGetActiveFileInfo fail: %d\n", res);
	}
	else
	{
		char startDateTime[32];
		timestampToTime(activeFileInfo.startTime, startDateTime, 32);
		printf("startTime: %s\n", startDateTime);
		char endDateTime[32];
		timestampToTime(activeFileInfo.endTime, endDateTime, 32);
		printf("endTime: %s\n", endDateTime);
	}

	const ASF_VERSION version = ASFGetVersion();
	printf("\nVersion:%s\n", version.Version);
	printf("BuildDate:%s\n", version.BuildDate);
	printf("CopyRight:%s\n", version.CopyRight);

	printf("\n************* Face Recognition *****************\n");

	//激活接口,首次激活需联网
	res = ASFOnlineActivation(APPID, SDKKEY);
	if (MOK != res && MERR_ASF_ALREADY_ACTIVATED != res)
		printf("ASFActivation fail: %d\n", res);
	else
		printf("ASFActivation sucess: %d\n", res);

	//初始化接口
	
	MInt32 mask = ASF_FACE_DETECT | ASF_FACERECOGNITION | ASF_AGE | ASF_GENDER | ASF_FACE3DANGLE |
		ASF_LIVENESS | ASF_IR_LIVENESS;
	res = ASFInitEngine(ASF_DETECT_MODE_IMAGE, ASF_OP_0_ONLY, 30, 5, mask, &handle);
	if (res != MOK)
		printf("ASFInitEngine fail: %d\n", res);
	else
		printf("ASFInitEngine sucess: %d\n", res);
}
void timestampToTime(char* timeStamp, char* dateTime, int dateTimeSize)
{
	time_t tTimeStamp = atoll(timeStamp);
	struct tm* pTm = gmtime(&tTimeStamp);
	strftime(dateTime, dateTimeSize, "%Y-%m-%d %H:%M:%S", pTm);
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
		/*
		if (capture.isOpened())
		{
			lock_guard<mutex> lock(g_mutex);
			//文字显示框
			CRect rect(m_curFaceInfo.faceRect.left - 10, m_curFaceInfo.faceRect.top - 50,
				m_curFaceInfo.faceRect.right, m_curFaceInfo.faceRect.bottom);
			IplDrawToHDC(TRUE, m_curVideoImage, rect, IDC_STATIC);
		}
		else
		{
			if (m_curStaticImage)
			{
				CRect rect((int)m_curStringShowPosition.X + 10, (int)m_curStringShowPosition.Y + 10, 40, 40);
				IplDrawToHDC(FALSE, m_curStaticImage, rect, IDC_STATIC);
			}
		}
		*/
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COpencvTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CutIplImage(IplImage* src, IplImage* dst, int x, int y)
{
	CvSize size = cvSize(dst->width, dst->height);//区域大小
	cvSetImageROI(src, cvRect(x, y, size.width, size.height));//设置源图像ROI
	cvCopy(src, dst); //复制图像
	cvResetImageROI(src);//源图像用完后，清空ROI
}





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
		/*COpencvTestDlg* _this = (COpencvTestDlg*)pwind;
		((COpencvTestDlg*)CWnd::FromHandle(_this->m_hWnd))->FaceWatch();*/
		capture >> frame;
		capture >> PicFrame;
		COpencvTestDlg A;
		//A.FaceWatch(frame);
		A.FaceDetect(frame);
		//A.ArcFaceWatch(frame);
			
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





void COpencvTestDlg::FaceWatch(cv::Mat newframe)
{

	/*OpenCv人脸检测*/
	
	std::vector<Rect> faces;
	Mat frame_gray;
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);//转换成灰度图像
	equalizeHist(frame_gray, frame_gray);//直方图均衡化
	//1.1表示每次图像尺寸减小的比例为1.1，2表示每一个目标至少要被检测到3次才算是真的目标(因为周围的像素和不同的窗口大小都可以检测到人脸),CV_HAAR_SCALE_IMAGE表示不是缩放分类器来检测，而是缩放图像，Size(30, 30)为目标的最小最大尺寸
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++)
	{
		/*
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 2, 8, 0);//画椭圆
		*/

		rectangle(frame, Rect(faces[0].x, faces[0].y, faces[0].width, faces[0].height), Scalar(0, 0, 255), 2);
		/*
		Mat faceROI = frame_gray(faces[i]);
		std::vector<Rect> eyes;

		//-- In each face, detect eyes
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

		for (size_t j = 0; j < eyes.size(); j++)//检测眼睛
		{
			Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
			int radius = cvRound((eyes[j].width + eyes[j].height) * 0.25);
			circle(frame, eye_center, radius, Scalar(255, 0, 0), 3, 8, 0);
		}
		*/
	}


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

	Sleep(100);
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
	if (capture.isOpened())
	{
		AfxMessageBox(_T("请先关闭摄像头"));
		return;
	}
	SFCZ = FALSE;
	CloseHandle(g_hEvent1);
	m_imageFaceEngine.UnInitEngine();
	DestroyWindow();
	
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
		FilePath = "./照片";
		if (!PathFileExists(FilePath))
		{
			bool fsf=CreateDirectory((LPCTSTR)FilePath, NULL);
			if (fsf == false)
			{
				AfxMessageBox(_T("创建路径失败"));
			}
		}

		//图片名随机数
		const int N = 100;
		int num1 = rand() % N;
		//str.Format(_T("./人脸采集/%d.jpg"), result);//可以把保存路径加到引号内。

		
		//保存图像的名字
		string strSaveName;
		char buffer[256];
		sprintf(buffer, "D%04d", num1);
		strSaveName = buffer;

		//保存图像的完整路径
		string strImgSavePath = FilePath + "\\" + strSaveName;
		//保存图像的格式
		strImgSavePath += ".jpg";
		//strImgSavePath += ".png";


		bool sfcz = imwrite(strImgSavePath.c_str(), PicFrame);
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

void COpencvTestDlg::EditOut(CString str, bool add_endl)
{
	if (add_endl)
		str += "\r\n";
	int iLen = m_editlog.GetWindowTextLength();
	m_editlog.SetSel(iLen, iLen, TRUE);
	m_editlog.ReplaceSel(str, FALSE);
}


void PicCutOut(IplImage* src, IplImage* dst, int x, int y)
{
	if (!src || !dst)
	{
		return;
	}

	CvSize size = cvSize(dst->width, dst->height);//区域大小
	cvSetImageROI(src, cvRect(x, y, size.width, size.height));//设置源图像ROI
	cvCopy(src, dst); //复制图像
	cvResetImageROI(src);//源图像用完后，清空ROI
}

CBitmap* IplImage2CBitmap(const IplImage* img)
{
	if (!img)
	{
		return NULL;
	}

	CBitmap* bitmap = new CBitmap;//new一个CWnd对象
	BITMAPINFO bmpInfo;  //创建位图        
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = img->width;
	bmpInfo.bmiHeader.biHeight = img->origin ? abs(img->height) : -abs(img->height);//img->height;//高度
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 24;
	bmpInfo.bmiHeader.biCompression = BI_RGB;

	void* pArray = NULL;
	HBITMAP hbmp = CreateDIBSection(NULL, &bmpInfo, DIB_RGB_COLORS, &pArray, NULL, 0);//创建DIB，可直接写入、与设备无关，相当于创建一个位图窗口
	ASSERT(hbmp != NULL);
	UINT uiTotalBytes = img->width * img->height * 3;
	memcpy(pArray, img->imageData, uiTotalBytes);

	bitmap->Attach(hbmp);// 一个CWnd对象的HWND成员指向这个窗口句柄

	return bitmap;
}


void COpencvTestDlg::OnBnClickedButton4()
{
	// 加载图片


	

	TCHAR szFilter[] = _T("jpg file(*.jpg)|*.jpg|bmp file(*.bmp)|*.bmp|所有文件(*.*)|*.*||");  //文件格式过滤
	CString fileName = L"";//默认打开的文件名
	CString defaultDir = L"./";	//默认打开的文件路径
	CFileDialog fileDlg(TRUE, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, szFilter, this);
	fileDlg.m_ofn.lpstrTitle = _T("加载图片");  //保存对话窗口标题名
	CString picturePath;
	if (IDOK == fileDlg.DoModal())
	{
		picturePath = fileDlg.GetPathName();
	}
	GetDlgItem(IDC_STATIC_FACE)->ShowWindow(TRUE);
	string strImgSavePath = picturePath+"";
	Mat img = imread(strImgSavePath,1);
	Mat dstImg, grayImg;
	dstImg = img.clone();
	grayImg.create(img.size(), img.type());
	cvtColor(img, grayImg, CV_BGR2GRAY);//生成灰度图，提高检测效率
	equalizeHist(grayImg, grayImg);
	vector<Rect> faces;
	/*模型训练，特征提取*/
	vector<Mat> images;
	images.push_back(imread(strImgSavePath, CV_LOAD_IMAGE_GRAYSCALE));

	//vector<int> labels;
	//vector<int> labels;
	//labels.push_back(1);
	///*for (int i = 1; i <= 1; i++)
	//	labels[i - 1] = i;*/
	//modelPic->train(images, labels);//训练


	//人脸识别，框出
	//face_cascade.detectMultiScale(grayImg, faces, 1.1, 3, 0);
	face_cascade2.detectMultiScale(grayImg, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	for (int i = 0; i < faces.size(); i++)
	{
		rectangle(img, Rect(faces[0].x, faces[0].y, faces[0].width, faces[0].height), Scalar(0, 0, 255), 2);
		
	}
	

	CImage image;
	MatImage mc;
	mc.MatToCImage(img, image);
	CWnd* pWnd = GetDlgItem(IDC_STATIC_FACE);//获得pictrue控件窗口的句柄   
	CDC* pDC = pWnd->GetDC();//获得pictrue控件的DC  
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetClientRect(&rect);
	GetDlgItem(IDC_STATIC_FACE)->GetClientRect(&rect);
	//rect.MoveToX(rect.Width() / 2 -image.GetWidth() / 2);
	//rect.MoveToY(rect.Height() / 2 - image.GetHeight() / 2);    
	//GetDlgItem(IDC_STATIC)->MoveWindow(rect);
	Mat dst;
	int x = rect.Width();
	int y = rect.Height();
	resize(img, dst, Size(x, y));
	mc.MatToCImage(dst, image);
	image.Draw(pDC->m_hDC, rect);

	ReleaseDC(pDC);
	image.ReleaseDC();
	

}

void COpencvTestDlg::FaceDetect(cv::Mat FDframe)
{
	//Mat frame;
	//Mat edges;
	Mat gray;

	
	bool stop = false;
	//训练好的文件名称，放置在可执行文件同目录下
	
	
	
	

	while (!stop)
	{
		//cap >> frame;

		//建立用于存放人脸的向量容器
		vector<Rect> faces(0);

		cvtColor(FDframe, gray, CV_BGR2GRAY);
		//改变图像大小，使用双线性差值
		//resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
		//变换后的图像进行直方图均值化处理
		equalizeHist(gray, gray);

		cascade.detectMultiScale(gray, faces,1.1, 2, 0
			//|CV_HAAR_FIND_BIGGEST_OBJECT
			//|CV_HAAR_DO_ROUGH_SEARCH
			| CV_HAAR_SCALE_IMAGE,
			Size(30, 30));

		Mat face;
		Point text_lb;

		for (size_t i = 0; i < faces.size(); i++)
		{
			if (faces[i].height > 0 && faces[i].width > 0)
			{
				face = gray(faces[i]);
				text_lb = Point(faces[i].x, faces[i].y);

				rectangle(FDframe, faces[i], Scalar(255, 0, 0), 2, 8, 0);
			}
		}

		Mat face_test;

		//开始人脸匹配
		int predictPCA = -1;
		if (face.rows >= 120)
		{
			resize(face, face_test, Size(92, 112));

		}
		//Mat face_test_gray;
		//cvtColor(face_test, face_test_gray, CV_BGR2GRAY);
		//double confidence=0.0;
		if (!face_test.empty())
		{
			//测试图像应该是灰度图

			
			//int predictPCA;
			//modelPCA->predict(face_test, predictPCA, confidence);

			//modelPCA->setThreshold(123.0);
			predictPCA = modelPCA->predict(face_test);
			cout << predictPCA;
		}

		//cout << predictPCA << endl;
		
		if (predictPCA == 35)
		{
			string name = "zqz";
			putText(FDframe, name, text_lb, FONT_HERSHEY_COMPLEX, 2, Scalar(0, 0, 255));
			cout << "人脸匹配成功:"<<predictPCA<<"\n";
		}

		//imshow("face", FDframe);
		if (waitKey(50) >= 0)
			stop = true;
	}
}
void COpencvTestDlg::PicDetect(cv::Mat picFrame)
{
	//图片比对
	std::vector<Rect> faces;
	Mat frame_gray;

	cvtColor(picFrame, frame_gray, COLOR_BGR2GRAY);  //BGR 转化为灰度图
	equalizeHist(frame_gray, frame_gray);   //直方图均衡化
	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(60, 60));

	for (size_t i = 0; i < faces.size(); i++)
	{
		rectangle(picFrame, Rect(faces[0].x, faces[0].y, faces[0].width, faces[0].height), Scalar(255, 0, 0), 2);

		Mat faceROI = frame_gray(faces[i]);
		Mat face_test;
		// 调整大小为92*112  
		resize(faceROI, face_test, Size(92, 112));
		imwrite("lisa.pgm", face_test);
		double confidence;
		int predictPCA;
		modelPic->predict(face_test, predictPCA, confidence);
		cout << "the predict result is " << predictPCA << endl << "confidence is " << confidence << endl;
		if (predictPCA == 1)
		{
			putText(picFrame, "审核通过", Point(faces[i].x, faces[i].y), FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 255), 2);

		}
		else
		{
			putText(picFrame, "审核失败", Point(faces[i].x, faces[i].y), FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 255), 2);
		}
		CImage image;
		MatImage mc;
		mc.MatToCImage(picFrame, image);
		CWnd* pWnd = GetDlgItem(IDC_STATIC_FACE);//获得pictrue控件窗口的句柄   
		CDC* pDC = pWnd->GetDC();//获得pictrue控件的DC  
		HDC hDC = pDC->GetSafeHdc();
		CRect rect;
		GetClientRect(&rect);
		GetDlgItem(IDC_STATIC_FACE)->GetClientRect(&rect);
		//rect.MoveToX(rect.Width() / 2 -image.GetWidth() / 2);
		//rect.MoveToY(rect.Height() / 2 - image.GetHeight() / 2);    
		//GetDlgItem(IDC_STATIC)->MoveWindow(rect);
		Mat dst;
		int x = rect.Width();
		int y = rect.Height();
		resize(picFrame, dst, Size(x, y));
		mc.MatToCImage(dst, image);
		image.Draw(pDC->m_hDC, rect);

		ReleaseDC(pDC);
		image.ReleaseDC();

	}

}
void COpencvTestDlg::OnBnClickedButton3()
{
	//// 人脸比对
	//SetEvent(g_hEvent1);
	//Sleep(100);
	//VideoCapture cam;
	//Mat camFrame;
	//capture.open(0);
	//while (capture.read(camFrame)) //读取帧
	//{
	//	if (camFrame.empty())
	//	{
	//		printf(" --(!) No captured frame -- Break!");
	//		break;
	//	}

	//	//-- 3. Apply the classifier to the frame
	//	PicDetect(camFrame);

	//	if (waitKey(10) == 'k') { break; } // escape  
	//}
	TCHAR szFilter[] = _T("jpg file(*.jpg)|*.jpg|bmp file(*.bmp)|*.bmp|所有文件(*.*)|*.*||");  //文件格式过滤
	CString fileName = L"";//默认打开的文件名
	CString defaultDir = L"./";	//默认打开的文件路径
	CFileDialog fileDlg(TRUE, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, szFilter, this);
	fileDlg.m_ofn.lpstrTitle = _T("加载图片");  //保存对话窗口标题名
	CString picturePath;
	if (IDOK == fileDlg.DoModal())
	{
		picturePath = fileDlg.GetPathName();
	}
	GetDlgItem(IDC_STATIC_FACE)->ShowWindow(TRUE);
	string strImgSavePath = picturePath + "";
	MRESULT res = MOK;
	
	Mat imageframe;
	imageframe=imread(strImgSavePath);
	ASF_MultiFaceInfo detectedFaces1 = { 0 };
	ASF_SingleFaceInfo SingleDetectedFaces1 = { 0 };
	ASF_FaceFeature feature1 = { 0 };
	ASF_FaceFeature copyfeature1 = { 0 };

	res = ASFDetectFaces(handle, imageframe.cols, imageframe.rows, ASVL_PAF_RGB24_B8G8R8,
		(MUInt8*)imageframe.ptr<uchar>(0), &detectedFaces1);
	if (MOK == res && detectedFaces1.faceNum >= 1)
	{
		SingleDetectedFaces1.faceRect.left = detectedFaces1.faceRect[0].left;
		SingleDetectedFaces1.faceRect.top = detectedFaces1.faceRect[0].top;
		SingleDetectedFaces1.faceRect.right = detectedFaces1.faceRect[0].right;
		SingleDetectedFaces1.faceRect.bottom = detectedFaces1.faceRect[0].bottom;
		SingleDetectedFaces1.faceOrient = detectedFaces1.faceOrient[0];

		res = ASFFaceFeatureExtract(handle, imageframe.cols, imageframe.rows, ASVL_PAF_RGB24_B8G8R8,
			(MUInt8*)imageframe.ptr<uchar>(0), &SingleDetectedFaces1, &feature1);
		if (res == MOK)
		{
			//拷贝feature
			copyfeature1.featureSize = feature1.featureSize;
			copyfeature1.feature = (MByte*)malloc(feature1.featureSize);
			memset(copyfeature1.feature, 0, feature1.featureSize);
			memcpy(copyfeature1.feature, feature1.feature, feature1.featureSize);
		}
		else
			printf("ASFFaceFeatureExtract 1 fail: %d\n", res);
	}
	else
		printf("ASFDetectFaces 1 fail: %d\n", res);


}


void COpencvTestDlg::OnBnClickedButton7()
{
	int i = 0;
	//人脸采集
	VideoCapture cap;
	cap.open(0);
	Mat frame2;

	std::vector<Rect> faces;
	Mat img_gray;

	while (1)
	{
		
		char key = waitKey(100);	
		cap >> FaceFrame;
		cap >> frame2;

		cvtColor(FaceFrame, img_gray, COLOR_BGR2GRAY);
		equalizeHist(img_gray, img_gray);
		//-- Detect faces
		face_cascade3.detectMultiScale(img_gray, faces, 1.1, 3, CV_HAAR_DO_ROUGH_SEARCH, Size(50, 50));
		rectangle(FaceFrame, Rect(faces[0].x, faces[0].y, faces[0].width, faces[0].height), Scalar(0, 0, 255), 2);

		imshow("frame", FaceFrame);


		CString FilePath;
		FilePath = "./人脸采集";
		if (!PathFileExists(FilePath))
		{
			bool fsf = CreateDirectory((LPCTSTR)FilePath, NULL);
			if (fsf == false)
			{
				AfxMessageBox(_T("创建路径失败"));
			}
		}
		
		//str.Format(_T("./人脸采集/%d.jpg"), result);//可以把保存路径加到引号内。
		switch (key)
		{
			case'p':
					i++;			
					cvtColor(frame2, img_gray, COLOR_BGR2GRAY);
					equalizeHist(img_gray, img_gray);
					//-- Detect faces
					face_cascade4.detectMultiScale(img_gray, faces, 1.1, 3, CV_HAAR_DO_ROUGH_SEARCH, Size(50, 50));
					for (size_t j = 0; j < faces.size(); j++)
					{	
						
						Mat faceROI = frame2(faces[j]);
						Mat MyFace;
						if (faceROI.cols > 100)
						{	
							resize(faceROI, MyFace, Size(92, 112));

							//保存图像的名字
							string strSaveName;
							char buffer[256];
							//sprintf(buffer, "MyFcae%d", i);
							sprintf(buffer, "%d", i);
							strSaveName = buffer;

							//保存图像的完整路径
							string strImgSavePath = FilePath + "\\" + strSaveName;
							//保存图像的格式
							strImgSavePath += ".pgm";

							//string  str = format("D:\\MyFaces\\MyFcae%d.jpg", i);
							imwrite(strImgSavePath, MyFace);
							imshow("ii", MyFace);
						}
						waitKey(10);
					}
					imshow("photo", frame2);
					waitKey(500);
					destroyWindow("photo");
					break;
			default:
				break;
		}
	}
}
