#pragma once
#include "headers.h"
#include "MWDBoneNode.h"
#include "MWDAABB3.h"
using namespace math;
class MWDModel;
//骨骼维护一个骨骼节点信息，一个aabb3包围盒，树形拓扑结构，骨骼名称
class MWDSkeleton
{
public:
	//骨骼数据
	MWDBoneNode			m_bone;
	//包围盒
	AABB3				m_aabb3;

	MWDSkeleton*		m_pParent;
	vector<MWDSkeleton> m_skeletonChildren;

	//当前骨骼所属的模型
	MWDModel*			m_Owner;

	//骨骼名称
	string				m_skeletonName;
};

