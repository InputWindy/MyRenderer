#pragma once
#include "headers.h"
#include "MWDShader.h"
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
};

//Meshά��IBO��VBO(Meshʵ������ʱ����Ѿ��ϴ��Դ���,�洢��VAO��)
class MWDRenderer;
class MWDMaterial;
class MWDMesh
{
public:
    // mesh Data    
    MWDMaterial*            m_material;
    vector<Vertex>          vertices;
    vector<unsigned int>    indices;
    // constructor
    MWDMesh(vector<Vertex> vertices, vector<unsigned int> indices);
    
    unsigned int VAO;
    private:
    //�洢���Դ��Mesh����
    unsigned int VBO, IBO;
    
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
public:
    void DrawMesh(MWDRenderer* pRenderer);
};

