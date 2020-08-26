
// OpencvTestDlg.h: 头文件
//

#pragma once
#include "opencv/build/include/opencv2/opencv.hpp"
#include "opencv/build/include/opencv2/objdetect/objdetect.hpp"
#include "opencv/build/include/opencv2/highgui/highgui.hpp"
#include "opencv/build/include/opencv2/imgproc/imgproc.hpp"
#include "opencv/build/include/opencv2/imgproc/types_c.h"

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
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	void OnPictureSave(UINT ID);
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
