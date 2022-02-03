#pragma once
#include <../assimp/Importer.hpp>
#include <../assimp/scene.h>
#include <../assimp/postprocess.h>
#include "MWDMesh.h"
#include "MWDMaterial.h"
#include "MWDTransform.h"
//Model维护Mesh数组和一个材质
class MWDRenderer;
class MWDModel
{
public:
	// constructor, expects a filepath to a 3D model.
	MWDModel(string name,string const& path, bool gamma = false) : gammaCorrection(gamma)
	{
        m_name = name;
		loadModel(path);
	}
	~MWDModel() {
        int num = meshes.size();
        for (int i = 0; i < num; ++i) {
            glDeleteVertexArrays(1,&(meshes[i].VAO));
            glDeleteBuffers(1, &(meshes[i].VBO));
            glDeleteBuffers(1, &(meshes[i].IBO));
        }
	}
public:
    string              m_name;
    MWDTransform        m_transform;
	vector<MWDMesh>		meshes;
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
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }
    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene)
    {
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }
    MWDMesh& processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;

        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
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
            //cout << "(" << vertex.Bitangent.x << "," << vertex.Bitangent.y << "," << vertex.Bitangent.z << ")" << endl;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            // texture coordinates
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
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

        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        MWDMesh& ret =*new MWDMesh(vertices, indices,this);
        return ret;
    }
};

