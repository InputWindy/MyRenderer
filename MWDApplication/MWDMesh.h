#pragma once
#include "headers.h"
#include "MWDShader.h"
#include "MWDMaterial.h"
#include "MWDBoneNode.h"
#define MAX_BONE_INFLUENCE 4
#define MAX_UV_NUM 8
//����ά��pos��normal��uv(8��)��tangent��bitangent����������������Ȩ��
struct Vertex {
    // position
    vec3 Position;
    // normal
    vec3 Normal;
    // texCoords
    vec2 TexCoords[MAX_UV_NUM];
    // tangent
    vec3 Tangent;
    // bitangent
    vec3 Bitangent;
    //�Ե�ǰ������Ӱ��Ķ�������
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //�Ե�ǰ������Ӱ��Ķ���������Ӧ��Ӱ��Ȩ��
    float m_Weights[MAX_BONE_INFLUENCE];
private:
    void AddBoneData(unsigned int BoneID, float Weight) {
        for (uint i = 0; i < MAX_BONE_INFLUENCE; i++) {
            if (m_Weights[i] == 0.0) {
                m_BoneIDs[i] = BoneID;
                m_Weights[i] = Weight;
                return;
            }
        }
        assert(0);
    }
};

class MWDRenderer;
class MWDMaterial;
class MWDModel;
//Meshά��IBO��VBO(Meshʵ������ʱ����Ѿ��ϴ��Դ���,�洢��VAO��)
//Mesh���Խڵ����ʽ��֯�����ģ�ÿһ��aiNodeά����һ��MeshNode���������ݣ��������֣�������
class MWDMesh
{
    friend class MWDModel;
    public:
        // ÿһ��Meshά��һ�����ʣ�ʵ����ʱ����Ĭ��PBR���ʣ�   
        MWDMaterial*            m_material;

        //��ǰMesh�Ķ������ݺ���������
        vector<Vertex>          vertices;
        vector<unsigned int>    indices;

        //Mesh������Model
        MWDModel*               m_Owner;

        //Mesh����
        string                  m_meshName;

        //Mesh���ڵ�
        MWDMesh*                m_pMeshParent;

        //Mesh�ӽڵ㼯��
        vector<MWDMesh>        m_pMeshChildren;

        // constructor
        MWDMesh();
        ~MWDMesh() {
            m_Owner = NULL;
            m_pMeshParent = NULL;
        }
        //�洢���Դ��Mesh����
        unsigned int VAO,VBO, IBO;
    private:
        void setupMesh()
        {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &IBO);

            glBindVertexArray(VAO);
            // load data into vertex buffers
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            // A great thing about structs is that their memory layout is sequential for all its items.
            // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a vec3/2 array which
            // again translates to 3/2 floats which translates to a byte array.
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

            // set the vertex attribute pointers
            // vertex Positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            // vertex normals
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
            // vertex tangent
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
            // vertex bitangent
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
            // ids
            glEnableVertexAttribArray(4);
            glVertexAttribIPointer(4, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

            // weights
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
            // vertex texture coords(location=10~17)
            for (int i = 10; i <= 17; ++i) {
                glEnableVertexAttribArray(i);
                glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords[i-10]));
            }
            glBindVertexArray(0);
        }
        void addChild(MWDMesh child_mesh) {
            m_pMeshChildren.push_back(child_mesh);
        }
    public:
        void DrawMesh(MWDRenderer* pRenderer);
    
};

