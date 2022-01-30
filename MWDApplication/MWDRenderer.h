#pragma once
#include "headers.h"
#include "MWDMesh.h"
#include "MWDRenderState.h"
#include "MWDCamera.h"
#include "MWDMaterial.h"

struct RenderContext {
	MWDCamera*		m_curCamera;		//主相机
	MWDMesh*		m_curMesh;			//正在被渲染的Mesh
	MWDMaterial*	m_curMaterial;		//渲染Mesh所使用的材质

	unsigned int	screen_width;		//当前视口宽度
	unsigned int	screen_height;		//当前视口高度

	vec3			m_meshPosition;     //当前Mesh的世界坐标
	vec3			m_meshScale;		//当前Mesh的缩放
};

//Renderer维护一个renderCtx,实现绘制一帧画面的所有方法
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

	//获取主渲染器
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
			在这解析RenderState
		*/
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//从相机读取vp矩阵。
		mat4 _projection = perspective(radians(ms_Ctx.m_curCamera->Zoom), (float)ms_Ctx.screen_width/ms_Ctx.screen_height, 0.1f, 100.0f);
		mat4 _view = ms_Ctx.m_curCamera->GetViewMatrix();

		//m矩阵由mesh自身的位置决定
		mat4 _model = mat4(1.0f);
		_model = translate(_model,ms_Ctx.m_meshPosition); // translate it down so it's at the center of the scene
		_model = scale(_model,ms_Ctx.m_meshScale);	// it's a bit too big for our scene, so scale it down

		//shader内置变量传递（不需要editor内改变的uniform）
		ms_Ctx.m_curMaterial->SetUniform<mat4, MWDMat4>(string("proj_matrix"), _projection);
		ms_Ctx.m_curMaterial->SetUniform<mat4, MWDMat4>(string("view_matrix"), _view);
		ms_Ctx.m_curMaterial->SetUniform<mat4, MWDMat4>(string("model_matrix"), _model);
		
		
		//填写Shader参数
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

