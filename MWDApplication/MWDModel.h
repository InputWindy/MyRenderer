#pragma once
#include <../assimp/Importer.hpp>
#include <../assimp/scene.h>
#include <../assimp/postprocess.h>
#include "MWDMesh.h"
#include "MWDMaterial.h"
#include "MWDTransform.h"
#include "MWDSkeleton.h"
#include "MWDAnimation.h"
//Modelά��Mesh�����һ������
class MWDRenderer;
class MWDModel
{
public:
	// constructor, expects a filepath to a 3D model.
	MWDModel(string name,string const& path, bool gamma = false) : gammaCorrection(gamma)
	{
        m_rootMesh = NULL;
        m_rootSkeleton = NULL;
        m_name = name;
		loadModel(path);
	}
	~MWDModel() {
        int num = m_meshes.size();
        for (int i = 0; i < num; ++i) {
            glDeleteVertexArrays(1,&(m_meshes[i].VAO));
            glDeleteBuffers(1, &(m_meshes[i].VBO));
            glDeleteBuffers(1, &(m_meshes[i].IBO));
        }
	}
public:
    //ģ������
    string                      m_name;
    
    MWDTransform                m_transform;

    //Mesh��˳�������ʽ
	vector<MWDMesh>		        m_meshes;

    //Mesh�����α�����ʽ
    MWDMesh*                    m_rootMesh;

    //������˳�������ʽ
    vector<MWDSkeleton>         m_SkeletonNode;

    //���������α�����ʽ
    MWDSkeleton*                m_rootSkeleton;

    //��ǰģ�͵����ж���
    vector<MWDAnimation>        m_animations;

	string directory;
	bool gammaCorrection;

    void Draw();
    void SetWorldPosition(float x, float y, float z) {
        m_transform.SetWorldPosition(x,y,z);
    }
    void SetWorldEulers(float yaw, float pitch, float roll) {
        m_transform.SetWorldEulers(yaw,pitch,roll);
    }
    void SetWorldScale(float x, float y, float z) {
        m_transform.SetWorldScale(x,y,z);
    }
private:
    void loadModel(string const& path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // ������ʾ
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));
        LoadAnimation(scene);                           //���������ģ�Ͱ��������ж���
        processNode(NULL,scene->mRootNode, scene);      //������������е�Mesh�͹���
        SetUpAllMeshes();                               //��������������Mesh��VAO
    }
    //ÿ��Node��Ӧģ�͵�һ����Mesh
    void processNode(aiNode* parent_node, aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_meshes.push_back(processMesh(parent_node,node,mesh, scene));
            m_SkeletonNode.push_back(processSkeleton(parent_node, node, mesh, scene));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node,node->mChildren[i], scene);
        }
    }
    //��д�����ƣ��̣���Owner���̣���vertices��indices�����ڵ㣬�ӽڵ�
    MWDMesh processMesh(aiNode* parent_node, aiNode*node, aiMesh* mesh, const aiScene* scene)
    {
        MWDMesh ret = MWDMesh();
        ret.m_meshName = string(node->mName.data);
        ret.m_Owner = this;

        //��Ҫ��д������
        vector<Vertex> vertices;
        vector<unsigned int> indices;

        // ����vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;

            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;

            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            //bones
            if (mesh->HasBones()) {
                int bone_num = mesh->mNumBones;
                for (int i = 0; i < bone_num; ++i) {

                }
            }

            // texture coordinates
            if (mesh->mTextureCoords[0])
            {
                int uv_num = mesh->GetNumUVChannels();
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                for (int j = 0; j < uv_num; ++j) {
                    vec.x = mesh->mTextureCoords[j][i].x;
                    vec.y = mesh->mTextureCoords[j][i].y;
                    vertex.TexCoords[j] = vec;
                }
            }
            else {
                for (int j = 0; j < 8; ++j) {
                    vertex.TexCoords[j] = glm::vec2(0.0f, 0.0f);
                }
            }

            vertices.push_back(vertex);
        }

        //����indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        
        ret.indices = indices;
        ret.vertices = vertices;
        return ret;
    }
    MWDSkeleton processSkeleton(aiNode* parent_node, aiNode* node, aiMesh* mesh, const aiScene* scene) {
        MWDSkeleton ret = MWDSkeleton();
        /*for (int i = 0; i < mesh->mNumBones; ++i) {
            string bone_name(mesh->mBones[i]->mName.data);
            cout << mesh->mBones[i]->mNumWeights << endl;
        }*/
        return ret;
    }
    private:
        void LoadAnimation(const aiScene* scene) {
            if (scene->HasAnimations()) {
                cout << "loadModel:�ж�����" << endl;
            };
        }
        void SetUpAllMeshes() {
            for (int i = 0; i < m_meshes.size(); ++i) {
                m_meshes[i].setupMesh();
            }
        }

};

