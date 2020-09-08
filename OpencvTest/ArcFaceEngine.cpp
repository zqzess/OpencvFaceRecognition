#include "pch.h"
#include "ArcFaceEngine.h"
using namespace std;

#define NSCALE 32   //识别的最小人脸比例
#define FACENUM 5	//最大需要检测的人脸个数

ArcFaceEngine::ArcFaceEngine()
{
}

ArcFaceEngine::~ArcFaceEngine()
{
}

MRESULT ArcFaceEngine::ActiveSDK(char* appID, char* sdkKey)
{
	/*
	MRESULT res = ASFActivation(appID, sdkKey);
	if (res != MOK && MERR_ASF_ALREADY_ACTIVATED != res)
		printf("ALActivation fail: %d\n", res);
	else
		printf("ALActivation sucess: %d\n", res);
	*/

	res = ASFOnlineActivation(appID, sdkKey);
	if (MOK != res && MERR_ASF_ALREADY_ACTIVATED != res)
		return res;
	return MOK;
}

MRESULT ArcFaceEngine::GetActiveFileInfo(ASF_ActiveFileInfo& activeFileInfo)
{
	MRESULT res = ASFGetActiveFileInfo(&activeFileInfo);
	return res;
}

MRESULT ArcFaceEngine::InitEngine(ASF_DetectMode detectMode)
{
	m_hEngine = NULL;
	MInt32 mask = 0;

	if (ASF_DETECT_MODE_IMAGE == detectMode)
	{
		mask = ASF_FACE_DETECT | ASF_FACERECOGNITION | ASF_AGE | ASF_GENDER | ASF_FACE3DANGLE | ASF_LIVENESS | ASF_IR_LIVENESS;
	}
	else
	{
		mask = ASF_FACE_DETECT | ASF_FACERECOGNITION | ASF_LIVENESS | ASF_IR_LIVENESS;
	}

	MRESULT res = ASFInitEngine(detectMode, ASF_OP_0_ONLY, NSCALE, FACENUM, mask, &m_hEngine);
	/*
		detectMode			VIDEO模式:处理连续帧的图像数据,IMAGE模式:处理单张的图像数据
		
		ASF_OP_0_ONLY		人脸检测角度，单一角度
		
		NSCALE				识别的最小人脸比例（图片长边与人脸框长边的比值），
							VIDEO模式取值范围[2,32]，推荐值为16，
							IMAGE模式取值范围[2,32]，推荐值为32

		FACENUM				最大需要检测的人脸个数，取值范围[1,50]

		mask				需要启用的功能组合，可多选

		&m_hEngine			引擎句柄
	*/
	return res;
}

MRESULT ArcFaceEngine::UnInitEngine()
{
	//销毁引擎
	return ASFUninitEngine(m_hEngine);
}

MRESULT ArcFaceEngine::FacePairMatching(MFloat& confidenceLevel, ASF_FaceFeature feature1, ASF_FaceFeature feature2, ASF_CompareModel compareModel)
{
	//人脸比对
	int res = ASFFaceFeatureCompare(m_hEngine, &feature1, &feature2, &confidenceLevel, compareModel);
	return res;
}
