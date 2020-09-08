#include "pch.h"
#include "ArcFaceEngine.h"
using namespace std;

#define NSCALE 32   //ʶ�����С��������
#define FACENUM 5	//�����Ҫ������������

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
		detectMode			VIDEOģʽ:��������֡��ͼ������,IMAGEģʽ:�����ŵ�ͼ������
		
		ASF_OP_0_ONLY		�������Ƕȣ���һ�Ƕ�
		
		NSCALE				ʶ�����С����������ͼƬ�����������򳤱ߵı�ֵ����
							VIDEOģʽȡֵ��Χ[2,32]���Ƽ�ֵΪ16��
							IMAGEģʽȡֵ��Χ[2,32]���Ƽ�ֵΪ32

		FACENUM				�����Ҫ��������������ȡֵ��Χ[1,50]

		mask				��Ҫ���õĹ�����ϣ��ɶ�ѡ

		&m_hEngine			������
	*/
	return res;
}

MRESULT ArcFaceEngine::UnInitEngine()
{
	//��������
	return ASFUninitEngine(m_hEngine);
}

MRESULT ArcFaceEngine::FacePairMatching(MFloat& confidenceLevel, ASF_FaceFeature feature1, ASF_FaceFeature feature2, ASF_CompareModel compareModel)
{
	//�����ȶ�
	int res = ASFFaceFeatureCompare(m_hEngine, &feature1, &feature2, &confidenceLevel, compareModel);
	return res;
}
