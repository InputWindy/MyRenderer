#pragma once
#include "headers.h"
#include "MWDMaterial.h"
//Passά���Ķ��ǹ̶����ߣ�������ͼ/��Ӱ��ͼ/Ԥ����
class MWDPass
{
public:
	enum
	{
		ShadowPass,
		LightPass,
		PreComputePass
	};
protected:
	MWDMaterial* m_material;
public:
	MWDPass() {
		
	}
	~MWDPass() {
		delete m_material;
	}
};

//��ӰPass��Ҫά��һ����Դ
class MWDShadowPass :public MWDPass {
protected:
	
public:
	MWDShadowPass() {
		
	}
	~MWDShadowPass() {

	}
};

