#pragma once
class ArcFaceEngine
{
public:
	ArcFaceEngine();
	~ArcFaceEngine();

	//激活
	MRESULT ActiveSDK(char* appID, char* sdkKey);
	//获取激活文件信息（可以获取到有效期）
	MRESULT GetActiveFileInfo(ASF_ActiveFileInfo& activeFileInfo);
	//初始化引擎
	MRESULT InitEngine(ASF_DetectMode detectMode);
	//释放引擎
	MRESULT UnInitEngine();
	//人脸比对
	MRESULT FacePairMatching(MFloat& confidenceLevel, ASF_FaceFeature feature1, ASF_FaceFeature feature2,
		ASF_CompareModel compareModel = ASF_LIFE_PHOTO);


private:
	MHandle m_hEngine;
	MRESULT res;
};

