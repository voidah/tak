#ifndef TAK__MESH_H__
#define TAK__MESH_H__

#include "define.h"

// Make sure the size of the struct is a multiple of 32 for maximum
// performance
struct VertexData3dNormal
{
    static const bool HAS_NORMAL = true;
    static const bool HAS_COLOR = true;
    static const bool HAS_TCOORD = true;

    typedef float VCOORD_TYPE;
    typedef float NORMAL_TYPE;
    typedef float COLOR_TYPE;
    typedef float TCOORD_TYPE;

    enum A { VCOORD_COUNT = 3 };
    VertexData3dNormal::VCOORD_TYPE x, y, z;

    enum B { NORMAL_COUNT = 3 };
    VertexData3dNormal::NORMAL_TYPE nx, ny, nz;

    enum C { COLOR_COUNT = 3 };
    VertexData3dNormal::COLOR_TYPE r, g, b;

    enum D { TCOORD_COUNT = 2 };
    VertexData3dNormal::TCOORD_TYPE u, v;

    float padding[5];

    VertexData3dNormal() {}
    VertexData3dNormal(float _x, float _y, float _z, float _nx, float _ny, float _nz, float _r, float _g, float _b, float _u, float _v) : x(_x), y(_y), z(_z), nx(_nx), ny(_ny), nz(_nz), r(_r), g(_g), b(_b), u(_u), v(_v) {}
};

// Make sure the size of the struct is a multiple of 32 for maximum
// performance
struct VertexData3d
{
    static const bool HAS_NORMAL = false;
    static const bool HAS_COLOR = true;
    static const bool HAS_TCOORD = true;

    typedef float VCOORD_TYPE;
    typedef float NORMAL_TYPE;
    typedef float COLOR_TYPE;
    typedef float TCOORD_TYPE;

    enum  A{ VCOORD_COUNT = 3 };
    VertexData3d::VCOORD_TYPE x, y, z;

    enum B { NORMAL_COUNT = 0 };

    enum C { COLOR_COUNT = 3 };
    VertexData3d::COLOR_TYPE r, g, b;

    enum D { TCOORD_COUNT = 2 };
    VertexData3d::TCOORD_TYPE u, v;

    VertexData3d() {}
    VertexData3d(float _x, float _y, float _z, float _r, float _g, float _b, float _u, float _v) : x(_x), y(_y), z(_z), r(_r), g(_g), b(_b), u(_u), v(_v) {}
};

// Make sure the size of the struct is a multiple of 32 for maximum
// performance
struct VertexData2d
{
    static const bool HAS_NORMAL = false;
    static const bool HAS_COLOR = true;
    static const bool HAS_TCOORD = true;

    typedef float VCOORD_TYPE;
    typedef float NORMAL_TYPE;
    typedef float COLOR_TYPE;
    typedef float TCOORD_TYPE;

    enum  A{ VCOORD_COUNT = 2 };
    VertexData3d::VCOORD_TYPE x, y;

    enum B { NORMAL_COUNT = 0 };

    enum C { COLOR_COUNT = 3 };
    VertexData3d::COLOR_TYPE r, g, b;

    enum D { TCOORD_COUNT = 2 };
    VertexData3d::TCOORD_TYPE u, v;

    float padding;

    VertexData2d() {}
    VertexData2d(float _x, float _y, float _r, float _g, float _b, float _u, float _v) : x(_x), y(_y), r(_r), g(_g), b(_b), u(_u), v(_v) {}
};

template <class T>
class Mesh
{
    public:
        typedef T VertexData;
        enum MESH_TYPE { MT_QUAD, MT_TRIANGLE };

    public:
        Mesh(MESH_TYPE meshType);
        ~Mesh();

        bool IsValid() const;
        void SetMeshData(T* vd, int vertexCount, uint16* indices, int indexCount, int transparentStart = -1);
        void SetMeshData(T* vd, int vertexCount, int transparentStart = -1);
        void Render() const;
        void RenderOpaque() const;
        void RenderTransparent() const;

        int Count() const;


    private:
        void RenderRange(int begin, int count) const;

    private:
        MESH_TYPE m_meshType;
        bool m_isValid;
        int m_vertexCount;
        GLuint m_vertexVboId;
        GLuint m_indexVboId;
        int m_transparentStart;
};

    template <class T>
Mesh<T>::Mesh(MESH_TYPE meshType) : m_meshType(meshType), m_isValid(false), m_transparentStart(-1)
{
    // For maximum performance T's size must be a multiple of 32
    assert(sizeof(T) % 32 == 0);
}

    template <class T>
Mesh<T>::~Mesh()
{
    if(m_isValid)
    {
        glDeleteBuffers(1, &m_vertexVboId);
        glDeleteBuffers(1, &m_indexVboId);
    }
}

template <class T>
bool Mesh<T>::IsValid() const
{
    return m_isValid;
}

    template <class T>
void Mesh<T>::SetMeshData(T* vd, int vertexCount, uint16* indices, int indexCount, int transparentStart)
{
    assert(vertexCount <= USHRT_MAX);
    if(vertexCount == 0)
        return;

    if(!m_isValid)
    {
        glGenBuffers(1, &m_vertexVboId);
        glGenBuffers(1, &m_indexVboId);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertexCount, vd, GL_STATIC_DRAW);

    // Pour le moment, generer le index array pour inclure tout les vertex, sans 
    // optimisation pour reduire le nombre de vertex envoyes a la carte
    // Idealement cet array devrait etre utiliser pour reutiliser les vertex et ainsi
    // sauver du temps en envoyant moins de donnees a la carte (il devrait etre construit
    // en meme temps que le buffer vd est rempli..)
    bool generateIndices = false;
    if(!indices)
    {
        generateIndices = true;
        indexCount = vertexCount;

        indices = new uint16[vertexCount];
        for(int i = 0; i < vertexCount; ++i)
            indices[i] = i;
    }

    m_vertexCount = indexCount;
    m_transparentStart = transparentStart;


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16) * indexCount, indices, GL_STATIC_DRAW);

    if(generateIndices)
        delete [] indices;

    CHECK_GL_ERROR();

    m_isValid = true;
}

    template <class T>
void Mesh<T>::SetMeshData(T* vd, int vertexCount, int transparentStart)
{
    SetMeshData(vd, vertexCount, 0, 0, transparentStart);
}

template <class T>
void Mesh<T>::Render() const
{
    if(IsValid())
        RenderRange(0, m_vertexCount);
}

template <class T>
void Mesh<T>::RenderOpaque() const
{
    int end = m_transparentStart;
    if(end == -1)
        end = m_vertexCount;

    RenderRange(0, end);
}

template <class T>
void Mesh<T>::RenderTransparent() const
{
    if(m_transparentStart == -1)
        return;

    RenderRange(m_transparentStart, m_vertexCount - m_transparentStart);
}

template <class T>
int Mesh<T>::Count() const
{
    return m_vertexCount;
}

template <class T>
void Mesh<T>::RenderRange(int begin, int count) const
{
    if(!IsValid() || count == 0)
        return;

    glClientActiveTexture(GL_TEXTURE0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVboId);

    char* dataOffset = 0;

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(T::VCOORD_COUNT, GL_FLOAT, sizeof(VertexData), dataOffset);
    dataOffset += T::VCOORD_COUNT * sizeof(typename T::VCOORD_TYPE);

    if(T::HAS_NORMAL)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, sizeof(VertexData), dataOffset);
        dataOffset += T::NORMAL_COUNT * sizeof(typename T::NORMAL_TYPE);
    }

    if(T::HAS_COLOR)
    {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(T::COLOR_COUNT, GL_FLOAT, sizeof(VertexData), dataOffset);
        dataOffset += T::COLOR_COUNT * sizeof(typename T::COLOR_TYPE);
    }

    if(T::HAS_TCOORD)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(T::TCOORD_COUNT, GL_FLOAT, sizeof(VertexData), dataOffset);
        dataOffset += T::TCOORD_COUNT * sizeof(typename T::TCOORD_TYPE);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVboId);
    //glDrawElements(GL_QUADS, m_vertexCount, GL_UNSIGNED_SHORT, (char*)0);

    if(m_meshType == MT_QUAD)
        glDrawRangeElements(GL_QUADS, begin, m_vertexCount, begin + count, GL_UNSIGNED_SHORT, (char*)0);
    else if(m_meshType == MT_TRIANGLE)
        glDrawRangeElements(GL_TRIANGLES, begin, m_vertexCount, begin + count, GL_UNSIGNED_SHORT, (char*)0);
    else
    {
        assert(false); // Format not supported yet
    }

    if(T::HAS_TCOORD)
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    if(T::HAS_COLOR)
        glDisableClientState(GL_COLOR_ARRAY);

    if(T::HAS_NORMAL)
        glDisableClientState(GL_NORMAL_ARRAY);

    glDisableClientState(GL_VERTEX_ARRAY);
}

#endif // MESH_H__
