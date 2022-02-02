#pragma once
#include "MWDShader.h"
#include "MWDUniform.h"
#include "MWDRenderState.h"
//Materialά��ShaderProgram��Uniform����
class MWDMaterial
{
protected:
    static inline MWDMaterial* m_screenTexture = NULL;
public:
    MWDMaterial() {
        m_shaderProgram = MWDShader::GetDefault();
    }
    MWDMaterial(string vshader,string fshader) {
        m_shaderProgram = new MWDShader(vshader.c_str(),fshader.c_str());
        *m_shaderProgram = *MWDShader::GetDefault();
    }
public:
    //��ʼ�����ʵ�ʱ�����AddUniform
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
    //�ڱ༭���е���SetUniform��ʵʱ�޸Ĳ��ʵĲ���
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

    //��ʼ�����ʵ�ʱ�����AddSampler
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
                *tmp = *new MWDTexture(nameineditor, tmp->m_tex_unit, path);;
                return true;
            }
        }
        return false;
    }
    //��ȡһ��Ĭ�ϲ��ʵ�ʵ��(new���¿ռ�)
    static MWDMaterial* GetDefault() {
        MWDMaterial* defaultMaterial = new MWDMaterial();
        if (defaultMaterial) {

            #pragma region ���ñ���
            vec3 lightpos = vec3();
            MWDVec3* _lightPos = new MWDVec3(string("lightPos"),string("lightPos"),lightpos);
            defaultMaterial->AddUniform(_lightPos);

            vec3 viewpos = vec3();
            MWDVec3* _viewpos = new MWDVec3(string("viewPos"), string("viewPos"), viewpos);
            defaultMaterial->AddUniform(_viewpos);

            mat4 model = mat4();
            MWDMat4* _model = new MWDMat4(string("model"),string("model_matrix"),model);
            defaultMaterial->AddUniform(_model);

            mat4 view = mat4();
            MWDMat4* _view = new MWDMat4(string("view"), string("view_matrix"), view);
            defaultMaterial->AddUniform(_view);

            mat4 proj = mat4();
            MWDMat4* _proj = new MWDMat4(string("projection"), string("proj_matrix"), proj);
            defaultMaterial->AddUniform(_proj);
            #pragma endregion

            #pragma region �༭�������õı���
            MWDTexture* tex = new MWDTexture(string("diffuse_map"), 0);
            defaultMaterial->AddSampler(tex);

            MWDTexture* tex1 = new MWDTexture(string("normal_map"), 1);
            defaultMaterial->AddSampler(tex1);

            MWDTexture* tex2 = new MWDTexture(string("specular_map"), 2);
            defaultMaterial->AddSampler(tex2);

            MWDTexture* tex3 = new MWDTexture(string("metallic_map"), 3);
            defaultMaterial->AddSampler(tex3);

            MWDTexture* tex4 = new MWDTexture(string("ao_map"), 4);
            defaultMaterial->AddSampler(tex4);

            MWDTexture* tex5 = new MWDTexture(string("pbr_map"), 5);
            defaultMaterial->AddSampler(tex5);
            
            #pragma endregion

        }
        return defaultMaterial;
    }
    //newһ���²���
    static MWDMaterial* GetScreenMaterial() {
        if (!m_screenTexture) {
            m_screenTexture = new MWDMaterial("C:/Users/InputWindy/Desktop/MyRenderer/MWDEngine/shaders/offScreen.vert", "C:/Users/InputWindy/Desktop/MyRenderer/MWDEngine/shaders/offScreen.frag");
            MWDTexture* tex5 = new MWDTexture(string("screenTexture"), 0);
            m_screenTexture->AddSampler(tex5);
        }
        return m_screenTexture;
    }
public:
    MWDShader* m_shaderProgram;
    MWDRenderState m_renderState;
    vector<MWDUniform*> m_uniform;
    vector<MWDTexture*> m_sampler;
};

