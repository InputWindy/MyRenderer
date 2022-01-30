#pragma once
#include "MWDShader.h"
#include "MWDUniform.h"
#include "MWDRenderState.h"
#include "MWDMesh.h"
//Material维护ShaderProgram的Uniform变量
class MWDMaterial
{
protected:
    static inline MWDMaterial* defaultMaterial = NULL;
public:
    MWDMaterial() {
        m_shaderProgram = MWDShader::GetDefault();
        m_renderState = MWDRenderState::GetDefault();
    }
public:
    //初始化材质的时候调用AddUniform
    bool AddUniform(MWDUniform* uniform) {
        if (!uniform) {
            return false;
        }
        for (vector<MWDUniform*>::iterator iter = m_uniform.begin(); iter < m_uniform.end(); ++iter) {
            MWDUniform* tmp = *iter;
            if (tmp==uniform) {
                return false;
            }
        }
        m_uniform.push_back(uniform);
        return true;
    }
    //在编辑器中调用SetUniform来实时修改材质的参数
    template<typename T,typename N>
    bool SetUniform(string nameineditor,T data) {
        for (vector<MWDUniform*>::iterator iter = m_uniform.begin(); iter < m_uniform.end(); ++iter) {
            MWDUniform* tmp = *iter;
            if (tmp->nameInEditor == nameineditor) {
                ((N*)tmp)->m_data = data;
                return true;
            }
        }
        return false;
    }

    //初始化材质的时候调用AddSampler
    bool AddSampler(MWDTexture*texture) {
        if (!texture) {
            return false;
        }
        for (vector<MWDTexture*>::iterator iter = m_sampler.begin(); iter < m_sampler.end(); ++iter) {
            MWDTexture* tmp = *iter;
            if (tmp == texture) {
                return false;
            }
        }
        m_sampler.push_back(texture);
        return true;
    }
    bool SetSampler(string nameineditor,string path) {
        for (vector<MWDTexture*>::iterator iter = m_sampler.begin(); iter < m_sampler.end(); ++iter) {
            MWDTexture* tmp = *iter;
            if (tmp->nameInEditor == nameineditor) {
                MWDTexture tex = MWDTexture(nameineditor, tmp->m_tex_unit,path);
                *tmp = tex;
                return true;
            }
        }
        return false;
    }
    static MWDMaterial* GetDefault() {
        if (!defaultMaterial) {
            defaultMaterial = new MWDMaterial();

            mat4 model = mat4();
            MWDMat4* _model = new MWDMat4(string("model"),string("model_matrix"),model);
            defaultMaterial->AddUniform(_model);

            mat4 view = mat4();
            MWDMat4* _view = new MWDMat4(string("view"), string("view_matrix"), view);
            defaultMaterial->AddUniform(_view);

            mat4 proj = mat4();
            MWDMat4* _proj = new MWDMat4(string("projection"), string("proj_matrix"), proj);
            defaultMaterial->AddUniform(_proj);

            MWDTexture* tex = new MWDTexture(string("diffuse_map"), 0);
            defaultMaterial->AddSampler(tex);

        }
        return defaultMaterial;
    }
public:
    MWDShader* m_shaderProgram;
    MWDRenderState* m_renderState;
    vector<MWDUniform*> m_uniform;
    vector<MWDTexture*> m_sampler;
};

