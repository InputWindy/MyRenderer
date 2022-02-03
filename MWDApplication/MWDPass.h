#pragma once
#include "headers.h"
#include "MWDMaterial.h"
//Pass维护的都是固定管线：光照贴图/阴影贴图/预计算
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

//阴影Pass需要维护一个光源
class MWDShadowPass :public MWDPass {
protected:
	
public:
	MWDShadowPass() {
		
	}
	~MWDShadowPass() {

	}
};

