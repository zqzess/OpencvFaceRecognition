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
	*��飺
	*	OpenCV��MatתATL/MFC��CImage����֧�ֵ�ͨ���ҶȻ���ͨ����ɫ
	*������
	*	mat��OpenCV��Mat
	*	cimage��ATL/MFC��CImage
	*/
	void MatToCImage(cv::Mat& mat, CImage& cimage);


	/*CImageToMat
	*��飺
	*	ATL/MFC��CImageתOpenCV��Mat����֧�ֵ�ͨ���ҶȻ���ͨ����ɫ
	*������
	*	cimage��ATL/MFC��CImage
	*	mat��OpenCV��Mat
	*/
	void CImageToMat(CImage& cimage, cv::Mat& mat);
};