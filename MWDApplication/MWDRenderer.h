#pragma once
#include "headers.h"
#include "MWDMesh.h"
#include "MWDRenderState.h"
#include "MWDCamera.h"
#include "MWDMaterial.h"
#include "MWDSkyBox.h"
#include "MWDPass.h"
static float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
};

struct FBO {
public:
	FBO() {
		glGenFramebuffers(1, &ID);
	}
	~FBO() {
		glDeleteFramebuffers(1, &ID);
	}
	unsigned int ID;
	void Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
	}
	void unBind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	//����ά����ɫ����
	void AttachColorBufferi(GLuint texture,int i) {
		Bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, texture, 0);
		unBind();
	}
	//rboά�����ģ������
	void AttachRbo(GLuint rbo) {
		Bind();
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		unBind();
	}
	bool CheckIsComplete() {
		Bind();
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
			unBind();
			return false;
		}
		else {
			unBind();
			return true;
		}
	}
};

struct ColorBuffer
{
public:
	ColorBuffer() {
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	~ColorBuffer() {
		glDeleteTextures(1, &ID);
	}
	unsigned int ID;
	void Bind() {
		glBindTexture(GL_TEXTURE_2D, ID);
	}
	void unBind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	//ÿ֡���ÿ��
	void SetWidthHeight(int wid,int hei) {
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wid, hei, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

struct RBO
{
public:
	RBO() {
		glGenRenderbuffers(1, &ID);
	}
	~RBO() {
		glDeleteRenderbuffers(1, &ID);
	}
	unsigned int ID;
	void Bind() {
		glBindRenderbuffer(GL_RENDERBUFFER, ID);
	}
	void unBind() {
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
	//ÿ֡���ÿ��
	void SetWidthHeight(int wid, int hei) {
		glBindRenderbuffer(GL_RENDERBUFFER, ID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, wid, hei);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

};

struct ScreenMesh
{
public:
	ScreenMesh() {
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	~ScreenMesh() {
		glDeleteBuffers(1, &quadVBO);
		glDeleteVertexArrays(1, &quadVAO);
	}
	unsigned int quadVAO, quadVBO;
	void Bind() {
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	}
	void unBind() {
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
};

struct RenderContext {
public:
	RenderContext() {
		m_OffScreenRender = false;

		m_curCamera = NULL;
		m_curMesh = NULL;
		m_curMaterial = NULL;

		screen_width = 600;
		screen_height = 800;

		m_modelMatrix = mat4(1.0f);

		m_FBO.AttachColorBufferi(m_ColorBuffer.ID, 0);
		m_FBO.AttachRbo(m_ZSBuffer.ID);

	}
	bool			m_OffScreenRender;		//�Ƿ���������Ⱦ

	MWDCamera*		m_curCamera;		//�����
	MWDMesh*		m_curMesh;			//���ڱ���Ⱦ��Mesh
	MWDMaterial*	m_curMaterial;		//��ȾMesh��ʹ�õĲ���

	unsigned int	screen_width;		//��ǰ�ӿڿ��
	unsigned int	screen_height;		//��ǰ�ӿڸ߶�

	mat4			m_modelMatrix;      //Model����

	FBO				m_FBO;				//������Ⱦ֡���壨fbo��
	ColorBuffer		m_ColorBuffer;		//������Ⱦ��ɫ���壨texture��
	RBO				m_ZSBuffer;			//������Ⱦ�����ģ�建�壨rbo��
	ScreenMesh		m_screenMesh;		//��Ļ���ε�Mesh

	MWDSkyBox*		m_curSkyBox;		//��ǰʹ�õ���պУ���պ�����Ⱦ���ȼ���͵�Mesh��

	vector<MWDPass*>	m_Pass;			//ά��������Ⱦ����
};

//Rendererά��һ��renderCtx,ʵ�ֻ���һ֡��������з���
class MWDRenderer
{
	friend class RenderContex;
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
	//�������е�Pass��ÿ��Pass��ִ��һ����������Ⱦ��
	//������Pass��Ҫ��дʵʱUniform������Pass��ֻ��Ҫ��д����Uniform
	static void UseMSAA() {
		glEnable(GL_MULTISAMPLE);
	}
	void DrawFrame() {
		AnalyzeRenderState();
		SetInnerUniform(ms_Ctx.m_curMaterial);
		DeliverMaterialUniform(ms_Ctx.m_curMaterial);
		Render();
	}
	void SetCamera(MWDCamera* camera) {
		assert(camera);
		ms_Ctx.m_curCamera = camera;
	}
	void SetScreenWidth(unsigned int _screen_width) {
		assert(_screen_width>0);
		ms_Ctx.screen_width = _screen_width;
	}
	void SetScreenHeight(unsigned int _screen_height) {
		assert(_screen_height>0);
		ms_Ctx.screen_height = _screen_height;
	}
	void SetSkyBox(MWDSkyBox* skyBox) {
		assert(skyBox);
		ms_Ctx.m_curSkyBox = skyBox;
	}
	//��ո��ֻ��壬��Ⱦ��պС��決������ͼ������̽�룬��Ӱ��ͼ
	static void BeginRendering() {
		glClearColor(0.1, 0.1, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
		RenderContext* ms_Ctx = &GetMainRenderer()->ms_Ctx;
		if (ms_Ctx->m_OffScreenRender) {
			ms_Ctx->m_ZSBuffer.SetWidthHeight(ms_Ctx->screen_width, ms_Ctx->screen_height);
			ms_Ctx->m_ColorBuffer.SetWidthHeight(ms_Ctx->screen_width, ms_Ctx->screen_height);
			if (!ms_Ctx->m_FBO.CheckIsComplete()) {
				return;
			};
			ms_Ctx->m_FBO.Bind();
		}
		GetMainRenderer()->DrawSkyBox();
	}
	//ִ����Ļ����/�ӳ���Ⱦ
	static void EndRendering() {
		MWDShader* screenShader = MWDShader::GetScreenShader();
		RenderContext* ms_Ctx = &GetMainRenderer()->ms_Ctx;
		ms_Ctx->m_FBO.unBind();
		if (ms_Ctx->m_OffScreenRender) {
			//Pass2��������ɫ���壩
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_STENCIL_TEST);
			glDisable(GL_BLEND);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |GL_STENCIL_BUFFER_BIT);

			screenShader->use();
			glActiveTexture(GL_TEXTURE0);
			ms_Ctx->m_ColorBuffer.Bind();
			screenShader->setInt("screenTexture", 0);

			ms_Ctx->m_screenMesh.Bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);
			ms_Ctx->m_screenMesh.unBind();
		}
	}
private:
	//������ǰMesh����Ⱦ״̬
	void AnalyzeRenderState() {
		#pragma region ��д��Ⱦ״̬
		MWDRenderState render_state = ms_Ctx.m_curMaterial->m_renderState;
		ms_Ctx.m_OffScreenRender = render_state.m_OffScreenRender;
		if (render_state.m_bEnableDepth) {
			glEnable(GL_DEPTH_TEST);
			if (render_state.m_bDepthReadOnly) {
				glDepthMask(GL_FALSE);
			}
			glDepthFunc(render_state.m_CompareMethod);
		}
		if (render_state.m_bEnableBlend) {
			glEnable(GL_BLEND);
			glBlendEquation(render_state.m_uiBlendFuncMode);
			glBlendFunc(render_state.m_uiBlendSrc, render_state.m_uiBlendDest);
		}
		if (render_state.m_bEnableStencil) {
			glEnable(GL_STENCIL);
			if (render_state.m_bStencilReadOnly) {
				glStencilFunc(render_state.m_CompareMethod, render_state.m_uiStencilRef, 0x00);
			}
			else {
				glStencilFunc(render_state.m_CompareMethod, render_state.m_uiStencilRef, 0xFF);
			}

			glStencilOp(render_state.m_uiSFailOp, render_state.m_uiSPassZFailOp, render_state.m_uiPassOp);
		}
		if (render_state.m_bEnableCull) {
			glEnable(GL_CULL_FACE);
			glCullFace(render_state.m_uiCullMode);
			glFrontFace(render_state.m_uiFrontDefinition);
		}
		#pragma endregion
	}
	//����Uniform����
	void DeliverMaterialUniform(MWDMaterial* Material) {
		//������Ĵ���
		#pragma region ����Uniform����
		//��дShader����
		MWDMaterial* cur_Material = Material;
		MWDShader* cur_Shader = Material->m_shaderProgram;
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
		#pragma endregion
	}
	//����Shader�����Դ���ͬ�������ñ�����������Ҳû��ϵ
	void SetInnerUniform(MWDMaterial* Material) {
		#pragma region ��д���ñ���
		//shader���ñ������ݣ�����Ҫeditor�ڸı��uniform��
		//�������ȡvp����
		mat4 _projection = perspective(radians(ms_Ctx.m_curCamera->Zoom), (float)ms_Ctx.screen_width / ms_Ctx.screen_height, 0.1f, 100.0f);
		mat4 _view = ms_Ctx.m_curCamera->GetViewMatrix();
		Material->SetUniform<mat4, MWDMat4>(string("proj_matrix"), _projection);
		Material->SetUniform<mat4, MWDMat4>(string("view_matrix"), _view);
		Material->SetUniform<mat4, MWDMat4>(string("model_matrix"), ms_Ctx.m_modelMatrix);

		Material->SetUniform<vec3, MWDVec3>(string("lightPos"), vec3(-10, -10, -10));
		Material->SetUniform<vec3, MWDVec3>(string("viewPos"), ms_Ctx.m_curCamera->Position);
		#pragma endregion
	}
	//�õ�ǰ����Ⱦ״̬��Shader����Ⱦ��ǰ��Mesh
	void Render() {
		glBindVertexArray(ms_Ctx.m_curMesh->VAO);
		glDrawElements(GL_TRIANGLES, ms_Ctx.m_curMesh->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	//��Ⱦ��պ�
	void DrawSkyBox() {
		if (!ms_Ctx.m_curSkyBox) {
			return;
		}
		else {
			glDepthMask(GL_FALSE);
			mat4 _projection = perspective(radians(ms_Ctx.m_curCamera->Zoom), (float)ms_Ctx.screen_width / ms_Ctx.screen_height, 0.1f, 100.0f);
			mat4 _view = glm::mat4(glm::mat3(ms_Ctx.m_curCamera->GetViewMatrix()));
			MWDShader* skyboxShader = ms_Ctx.m_curSkyBox->skyBoxShader;
			skyboxShader->use();
			glActiveTexture(GL_TEXTURE0);
			skyboxShader->setInt("skybox", 0);
			skyboxShader->setMat4("projection", _projection);
			skyboxShader->setMat4("view", _view);

			glBindVertexArray(GetMainRenderer()->ms_Ctx.m_curSkyBox->skyboxVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
		}
	}
};

