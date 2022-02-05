#pragma once
#include "MWDMath.h"
class MWDBoneNode
{
public:
	MWDBoneNode() {
		
	}
	~MWDBoneNode() {

	}
public:
	//Use for IK
	bool m_bIsEffector;					//��ǰ�����Ƿ���ЧӦ��
	vec3 m_TargetPosInWorld;			//��ǰ����������ռ�����

	vector<bool>  m_bAllowTranslation;	//�Ƿ�����ƽ�Ʊ任
	vector<float> m_fMinTranslation;	//��Сƽ�Ʊ任�޶�
	vector<float> m_fMaxTranslation;	//���ƽ�Ʊ任�޶�

	vector<bool>  m_bAllowRotation;		//�Ƿ�������ת�任
	vector<float> m_fMinRotation;		//��С��ת�任�޶�
	vector<float> m_fMaxRotation;		//�����ת�任�޶�

	float m_fIKWeight;					//IKȨ��

	string m_cName;						//��������

	mat4 m_OffSetMatrix;				//�������ģ�Ϳռ�ת���������ռ�
};

