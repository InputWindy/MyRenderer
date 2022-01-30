#include "MWDMesh.h"
#include "MWDRenderer.h"
#include "MWDMaterial.h"
#include "MWDModel.h"
MWDMesh::MWDMesh(vector<Vertex> vertices, vector<unsigned int> indices, MWDModel* owner)
{
	m_Owner = owner;
	m_material = MWDMaterial::GetDefault();
    this->vertices = vertices;
    this->indices = indices;
    //�ϴ��������ݣ��󶨶�������ָ�룬��ȡ��д������vbo��ibo��
    setupMesh();
	//cout << m_material << endl;
}

//����RenderCtxȻ��DrawFrame()
void MWDMesh::DrawMesh(MWDRenderer* pRenderer)
{
	pRenderer->ms_Ctx.m_curMaterial = m_material;
	pRenderer->ms_Ctx.m_curMesh = this;

	pRenderer->ms_Ctx.m_modelMatrix = m_Owner->m_transform.GetModelMatrix();

	pRenderer->DrawFrame();
}
