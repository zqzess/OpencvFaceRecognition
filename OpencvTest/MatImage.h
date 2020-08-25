#pragma once
#include "opencv/build/include/opencv2/opencv.hpp"
#include "opencv/build/include/opencv2/objdetect/objdetect.hpp"
#include "opencv/build/include/opencv2/highgui/highgui.hpp"
#include "opencv/build/include/opencv2/imgproc/imgproc.hpp"
#include "opencv/build/include/opencv2/imgproc/types_c.h"
class MatImage
{
public:
	/*MatToCImage
	*简介：
	*	OpenCV的Mat转ATL/MFC的CImage，仅支持单通道灰度或三通道彩色
	*参数：
	*	mat：OpenCV的Mat
	*	cimage：ATL/MFC的CImage
	*/
	void MatToCImage(cv::Mat& mat, CImage& cimage);


	/*CImageToMat
	*简介：
	*	ATL/MFC的CImage转OpenCV的Mat，仅支持单通道灰度或三通道彩色
	*参数：
	*	cimage：ATL/MFC的CImage
	*	mat：OpenCV的Mat
	*/
	void CImageToMat(CImage& cimage, cv::Mat& mat);
};