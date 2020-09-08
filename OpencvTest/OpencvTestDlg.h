
// OpencvTestDlg.h: 头文件
//

#pragma once
#include "ArcFaceEngine.h"

// COpencvTestDlg 对话框
class COpencvTestDlg : public CDialogEx
{
// 构造
public:
	COpencvTestDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENCVTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg
		
	void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	void OnPictureSave(UINT ID);
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	void FaceWatch(cv::Mat newframe);
	void EditOut(CString str, bool add_endl);
	void FaceDetect(cv::Mat FDframe);
	void PicDetect(cv::Mat picFrame);
public:
	ArcFaceEngine m_imageFaceEngine;
	ArcFaceEngine m_videoFaceEngine;
	ASF_SingleFaceInfo m_curFaceInfo;
	CEdit m_editlog;

	IplImage* m_curVideoImage;
	IplImage* m_curIrVideoImage;
	IplImage* m_curStaticImage;					//当前选中的图片
	
	CFont* m_Font;
	CString m_curVideoShowString;
	CString m_curIRVideoShowString;
	CString m_curStaticShowAgeGenderString;

	Gdiplus::Rect m_curFaceShowRect;
	Gdiplus::Rect m_curImageShowRect;
	Gdiplus::PointF m_curStringShowPosition;	//当前字符串显示的位置

private:
	CRect m_windowViewRect;						//展示控件的尺寸
	
public:
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton7();
};
