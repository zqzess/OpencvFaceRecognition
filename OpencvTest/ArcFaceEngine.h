#pragma once
class ArcFaceEngine
{
public:
	ArcFaceEngine();
	~ArcFaceEngine();

	//����
	MRESULT ActiveSDK(char* appID, char* sdkKey);
	//��ȡ�����ļ���Ϣ�����Ի�ȡ����Ч�ڣ�
	MRESULT GetActiveFileInfo(ASF_ActiveFileInfo& activeFileInfo);
	//��ʼ������
	MRESULT InitEngine(ASF_DetectMode detectMode);
	//�ͷ�����
	MRESULT UnInitEngine();
	//�����ȶ�
	MRESULT FacePairMatching(MFloat& confidenceLevel, ASF_FaceFeature feature1, ASF_FaceFeature feature2,
		ASF_CompareModel compareModel = ASF_LIFE_PHOTO);


private:
	MHandle m_hEngine;
	MRESULT res;
};

