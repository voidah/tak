#include <iostream>
//#include <assimp/assimp.hpp>
//#include <assimp/aiScene.h>
//#include <assimp/aiPostProcess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <vector>
#include <fstream>

struct Vertex
{
    float x, y, z;    // vectex coord
    float nx, ny, nz; // normals
};

typedef std::vector<Vertex> VertexList;
typedef std::vector<int> IndexList;

void ProcessNode(const aiScene* scene, aiNode* node, aiMatrix4x4 transform, VertexList& vertices, IndexList& indices)
{
    std::cout << "================================================" << std::endl;
    std::cout << "Name: " << node->mName.data << std::endl;

    transform *= node->mTransformation;


    std::cout << "Num mesh: " << node->mNumMeshes << std::endl;
    for(int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[0]];

        std::cout << "Mesh name: " << mesh->mName.data << std::endl;
        std::cout << "Num faces: " << mesh->mNumFaces << std::endl;
        std::cout << "Num vertices: " << mesh->mNumVertices << std::endl;

        // Show all vertices
        std::cout << "Vertices: " << std::endl;
        for(int j = 0; j < mesh->mNumVertices; ++j)
        {
            Vertex vert;
            aiVector3D vertex = mesh->mVertices[j];
            //vertex *= transform;
            vertex = transform * vertex;

            vert.x = vertex.x;
            vert.y = vertex.y;
            vert.z = vertex.z;

            if(mesh->mNormals)
            {
                aiVector3D normal = mesh->mNormals[j];
                vert.nx = normal.x;
                vert.ny = normal.y;
                vert.nz = normal.z;
            }
            std::cout << vertex.x << " ";
            std::cout << vertex.y << " ";
            std::cout << vertex.z << " ";
            std::cout << std::endl;

            vertices.push_back(vert);
        }

        int vcount = vertices.size();

        // Show vertex index for each face
        std::cout << "\nIndexes: " << std::endl;
        for(int j = 0; j < mesh->mNumFaces; ++j)
        {
            aiFace& face = mesh->mFaces[j];

            // Process only triangles..
            if(face.mNumIndices == 3)
            {
                std::cout << vcount + face.mIndices[0] << " ";
                std::cout << vcount + face.mIndices[1] << " ";
                std::cout << vcount + face.mIndices[2] << " ";
                std::cout << std::endl;

                indices.push_back(vcount + face.mIndices[0]);
                indices.push_back(vcount + face.mIndices[1]);
                indices.push_back(vcount + face.mIndices[2]);
            }
        }
    }


    for(int i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(scene, node->mChildren[i], transform, vertices, indices);
    }
}

int main(int argc, char* argv[])
{
    std::cout << "Model importer" << std::endl;
    if(argc != 2)
    {
        std::cout << "Usage: mipm <modelfile.ext>" << std::endl;
        return 1;
    }

    std::string input(argv[1]);
    std::string output;
    std::string::size_type pos = input.rfind('.');
    if(pos != std::string::npos)
        output = input.substr(0, pos) + ".tmd";
    else
        output = input + ".tmd";

    Assimp::Importer importer;
    //const aiScene* scene = importer.ReadFile("plancher.dae", 
    const aiScene* scene = importer.ReadFile(input.c_str(), 
            aiProcess_CalcTangentSpace       | 
            aiProcess_GenNormals |
            aiProcess_Triangulate            |
            aiProcess_JoinIdenticalVertices  |
            aiProcess_SortByPType |
            aiProcess_GenUVCoords);
            //aiProcess_OptimizeMeshes |
            //aiProcess_OptimizeGraph);

    // If the import failed, report it
    if( !scene)
    {
        std::cout << "Error: " << importer.GetErrorString() << std::endl;
        return 1;
    }

    std::cout << "Has meshes: " << (scene->HasMeshes() ? "yes" : "no") << std::endl;


    std::cout << "Mesh count: " << scene->mNumMeshes << std::endl;

    // Vertex count
    int vertexCount = 0;
    for(int i = 0; i < scene->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[i];
        vertexCount += mesh->mNumVertices;
    }
    std::cout << "Vertex count: " << vertexCount << std::endl;

    VertexList vertices;
    IndexList indices;

    ProcessNode(scene, scene->mRootNode, aiMatrix4x4(), vertices, indices);

    std::ofstream tmd(output.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
    tmd << vertices.size() << " ";
    tmd << indices.size() << " ";
    for(int i = 0; i < vertices.size(); ++i)
    {
        tmd << vertices[i].x << " ";
        tmd << vertices[i].y << " ";
        tmd << vertices[i].z << " ";
        tmd << vertices[i].nx << " ";
        tmd << vertices[i].ny << " ";
        tmd << vertices[i].nz << " ";
    }

    for(int i = 0; i < indices.size(); ++i)
    {
        tmd << indices[i] << " ";
    }

    std::cout << "file size: " << tmd.tellp() << std::endl;
    tmd.close();

}
