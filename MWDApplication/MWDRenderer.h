#pragma once
#include "headers.h"
#include "MWDMesh.h"
#include "MWDRenderState.h"
#include "MWDCamera.h"
#include "MWDMaterial.h"

struct RenderContext {
	MWDCamera*		m_curCamera;		//�����
	MWDMesh*		m_curMesh;			//���ڱ���Ⱦ��Mesh
	MWDMaterial*	m_curMaterial;		//��ȾMesh��ʹ�õĲ���

	unsigned int	screen_width;		//��ǰ�ӿڿ��
	unsigned int	screen_height;		//��ǰ�ӿڸ߶�

	mat4			m_modelMatrix;      //Model����


};

//Rendererά��һ��renderCtx,ʵ�ֻ���һ֡��������з���
class MWDRenderer
{
	friend class MWDMesh;
	friend class MWDMaterial;
protected:
	RenderContext ms_Ctx;
	static inline MWDRenderer* ms_Renderer = NULL;

	void PrintMat4(mat4& mat) {
		cout << endl;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				cout << mat[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
public:
	MWDRenderer() {
		
	}
	~MWDRenderer() {

	}

	//��ȡ����Ⱦ��
	static MWDRenderer* GetMainRenderer() {
		if (!ms_Renderer) {
			ms_Renderer = new MWDRenderer();
		}
		return ms_Renderer;
	}

	void DrawFrame() {
		//RenderState
		MWDRenderState* render_state = ms_Ctx.m_curMaterial->m_renderState;
		/*
			�������RenderState
		*/
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		//shader���ñ������ݣ�����Ҫeditor�ڸı��uniform��
		//�������ȡvp����
		mat4 _projection = perspective(radians(ms_Ctx.m_curCamera->Zoom), (float)ms_Ctx.screen_width/ms_Ctx.screen_height, 0.1f, 100.0f);
		mat4 _view = ms_Ctx.m_curCamera->GetViewMatrix();
		ms_Ctx.m_curMaterial->SetUniform<mat4, MWDMat4>(string("proj_matrix"), _projection);
		ms_Ctx.m_curMaterial->SetUniform<mat4, MWDMat4>(string("view_matrix"), _view);
		ms_Ctx.m_curMaterial->SetUniform<mat4, MWDMat4>(string("model_matrix"), ms_Ctx.m_modelMatrix);

		ms_Ctx.m_curMaterial->SetUniform<vec3, MWDVec3>(string("lightPos"),vec3(10,10,10));
		ms_Ctx.m_curMaterial->SetUniform<vec3, MWDVec3>(string("viewPos"),ms_Ctx.m_curCamera->Position );
		
		//��дShader����
		MWDMaterial* cur_Material = ms_Ctx.m_curMaterial;
		MWDShader* cur_Shader = ms_Ctx.m_curMaterial->m_shaderProgram;
		cur_Shader->use();
		for (vector<MWDUniform*>::iterator iter = cur_Material->m_uniform.begin(); iter < cur_Material->m_uniform.end(); ++iter) {
			MWDUniform* tmp = *iter;
			MWDUniform::UT type = (MWDUniform::UT)tmp->GetUniformType();
			switch (type)
			{
			case MWDUniform::INT:cur_Shader->setInt(tmp->nameInShader, ((MWDInt*)tmp)->m_data); break;
			case MWDUniform::FLOAT:cur_Shader->setFloat(tmp->nameInShader, ((MWDFloat*)tmp)->m_data); break;
			case MWDUniform::VEC2:cur_Shader->setVec2(tmp->nameInShader, ((MWDVec2*)tmp)->m_data); break;
			case MWDUniform::VEC3:cur_Shader->setVec3(tmp->nameInShader, ((MWDVec3*)tmp)->m_data); break;
			case MWDUniform::VEC4:cur_Shader->setVec4(tmp->nameInShader, ((MWDVec4*)tmp)->m_data); break;
			case MWDUniform::MAT2:cur_Shader->setMat2(tmp->nameInShader, ((MWDMat2*)tmp)->m_data); break;
			case MWDUniform::MAT3:cur_Shader->setMat3(tmp->nameInShader, ((MWDMat3*)tmp)->m_data); break;
			case MWDUniform::MAT4:cur_Shader->setMat4(tmp->nameInShader, ((MWDMat4*)tmp)->m_data); break;
			case MWDUniform::UT_MAX:
				break;
			default:
				break;
			}

		}
		for (vector<MWDTexture*>::iterator iter = cur_Material->m_sampler.begin(); iter < cur_Material->m_sampler.end(); ++iter) {
			MWDTexture* tmp = *iter;
			tmp->Bind();
			cur_Material->m_shaderProgram->setSampler(tmp->nameInEditor, tmp->m_tex_unit);
		}
		glBindVertexArray(ms_Ctx.m_curMesh->VAO);
		glDrawElements(GL_TRIANGLES,ms_Ctx.m_curMesh->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void SetCamera(MWDCamera* camera) {
		ms_Ctx.m_curCamera = camera;
	}
	void SetScreenWidth(unsigned int _screen_width) {
		ms_Ctx.screen_width = _screen_width;
	}
	void SetScreenHeight(unsigned int _screen_height) {
		ms_Ctx.screen_height = _screen_height;
	}
};

