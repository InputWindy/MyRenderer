#pragma once
#include "headers.h"
#include "MWDShader.h"
#include "MWDMaterial.h"
#include "MWDBoneNode.h"
#define MAX_BONE_INFLUENCE 4
#define MAX_UV_NUM 8
//顶点维护pos，normal，uv(8套)，tangent，bitangent，骨骼索引，骨骼权重
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
    //对当前顶点有影响的顶点索引
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //对当前顶点有影响的顶点索引对应的影响权重
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
//Mesh维护IBO和VBO(Mesh实例化的时候就已经上传显存了,存储在VAO中)
//Mesh是以节点的形式组织起来的（每一个aiNode维护了一个MeshNode的所有数据，包括名字，骨骼）
class MWDMesh
{
    friend class MWDModel;
    public:
        // 每一个Mesh维护一个材质（实例化时加载默认PBR材质）   
        MWDMaterial*            m_material;

        //当前Mesh的顶点数据和索引数据
        vector<Vertex>          vertices;
        vector<unsigned int>    indices;

        //Mesh所属的Model
        MWDModel*               m_Owner;

        //Mesh名字
        string                  m_meshName;

        //Mesh父节点
        MWDMesh*                m_pMeshParent;

        //Mesh子节点集合
        vector<MWDMesh>        m_pMeshChildren;

        // constructor
        MWDMesh();
        ~MWDMesh() {
            m_Owner = NULL;
            m_pMeshParent = NULL;
        }
        //存储在显存的Mesh数据
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

