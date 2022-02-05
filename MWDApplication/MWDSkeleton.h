#pragma once
#include "headers.h"
#include "MWDBoneNode.h"
#include "MWDAABB3.h"
using namespace math;
class MWDModel;
//����ά��һ�������ڵ���Ϣ��һ��aabb3��Χ�У��������˽ṹ����������
class MWDSkeleton
{
public:
	//��������
	MWDBoneNode			m_bone;
	//��Χ��
	AABB3				m_aabb3;

	MWDSkeleton*		m_pParent;
	vector<MWDSkeleton> m_skeletonChildren;

	//��ǰ����������ģ��
	MWDModel*			m_Owner;

	//��������
	string				m_skeletonName;
};

