#include "MWDMesh.h"
#include "MWDRenderer.h"
#include "MWDMaterial.h"
#include "MWDModel.h"
MWDMesh::MWDMesh()
{
	m_pMeshParent = NULL;
	m_Owner = NULL;
	m_material = MWDMaterial::GetDefault();
}

//����RenderCtxȻ��DrawFrame()
void MWDMesh::DrawMesh(MWDRenderer* pRenderer)
{
	pRenderer->ms_Ctx.m_curMaterial = m_material;
	pRenderer->ms_Ctx.m_curMesh = this;

	pRenderer->ms_Ctx.m_modelMatrix = m_Owner->m_transform.GetModelMatrix();

	pRenderer->DrawFrame();
}
