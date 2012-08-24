#include "model.h"
#include <fstream>
#include <cassert>

Model::Model() : m_vertices(0), m_indices(0), m_vertexCount(0), m_indexCount(0)
{
}

Model::~Model()
{
}

bool Model::Load(const std::string& key, const OptionList& options)
{
    std::ifstream tmd(key.c_str());
    if(!tmd.is_open())
        return false;

    tmd >> m_vertexCount >> m_indexCount;

    assert(m_vertexCount < 65535); // Mesh currently uses 16 bit for indexes

    std::cout << "vcount: " << m_vertexCount << ", icount: " << m_indexCount << std::endl;

    m_vertices = new Vertex[m_vertexCount];
    m_indices = new Index[m_indexCount];


    for(int i = 0; i < m_vertexCount; ++i)
    {
        Vertex& v = m_vertices[i];
        tmd >> v.coord.x;
        tmd >> v.coord.y;
        tmd >> v.coord.z;
        tmd >> v.normal.x;
        tmd >> v.normal.y;
        tmd >> v.normal.z;
    }

    for(int i = 0; i < m_indexCount; ++i)
    {
        tmd >> m_indices[i];
    }

    tmd.close();
    return true;
}

bool Model::Release()
{
    if(m_vertices)
        delete [] m_vertices;

    if(m_indices)
        delete [] m_indices;
    return true;
}

Mesh<VertexData3dNormal>* Model::GetMesh()
{
    assert(m_vertexCount > 0 && m_indexCount > 0);

    VertexData3dNormal* vd = new VertexData3dNormal[m_vertexCount];

    for(int i = 0; i < m_vertexCount; ++i)
    {
        Vertex& v = m_vertices[i];

        vd[i] = Mesh<VertexData3dNormal>::VertexData(v.coord.x, v.coord.y, v.coord.z, v.normal.x, v.normal.y, v.normal.z, 1.f, 1.f, 1.f, 0, 0);
    }

    Mesh<VertexData3dNormal>* mesh = new Mesh<VertexData3dNormal>(Mesh<VertexData3dNormal>::MT_TRIANGLE);
    mesh->SetMeshData(vd, m_vertexCount, m_indices, m_indexCount);
    //mesh->SetMeshData(vd, m_vertexCount);

    delete [] vd;
    return mesh;
}

