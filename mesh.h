#ifndef TAK__MESH_H__
#define TAK__MESH_H__

#include "define.h"

// Make sure the size of the struct is a multiple of 32 for maximum
// performance
struct VertexData3d
{
    typedef float VCOORD_TYPE;
    typedef float COLOR_TYPE;
    typedef float TCOORD_TYPE;

    enum  A{ VCOORD_COUNT = 3 };
    VertexData3d::VCOORD_TYPE x, y, z;

    enum B { COLOR_COUNT = 3 };
    VertexData3d::COLOR_TYPE r, g, b;

    enum C { TCOORD_COUNT = 2 };
    VertexData3d::TCOORD_TYPE u, v;

    VertexData3d() {}
    VertexData3d(float _x, float _y, float _z, float _r, float _g, float _b, float _u, float _v) : x(_x), y(_y), z(_z), r(_r), g(_g), b(_b), u(_u), v(_v) {}
};

// Make sure the size of the struct is a multiple of 32 for maximum
// performance
struct VertexData2d
{
    typedef float VCOORD_TYPE;
    typedef float COLOR_TYPE;
    typedef float TCOORD_TYPE;

    enum  A{ VCOORD_COUNT = 2 };
    VertexData3d::VCOORD_TYPE x, y;

    enum B { COLOR_COUNT = 3 };
    VertexData3d::COLOR_TYPE r, g, b;

    enum C { TCOORD_COUNT = 2 };
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

    public:
        Mesh();
        ~Mesh();

        bool IsValid() const;
        void SetMeshData(T* vd, int vertexCount, int transparentStart = -1);
        void Render() const;
        void RenderOpaque() const;
        void RenderTransparent() const;

        int Count() const;


    private:
        void RenderRange(int begin, int count) const;

    private:
        bool m_isValid;
        int m_vertexCount;
        GLuint m_vertexVboId;
        GLuint m_indexVboId;
        int m_transparentStart;
};

template <class T>
Mesh<T>::Mesh() : m_isValid(false), m_transparentStart(-1)
{
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
void Mesh<T>::SetMeshData(T* vd, int vertexCount, int transparentStart)
{
    assert(vertexCount <= USHRT_MAX);
    if(vertexCount == 0)
        return;

    if(!m_isValid)
    {
        glGenBuffers(1, &m_vertexVboId);
        glGenBuffers(1, &m_indexVboId);
    }

    m_vertexCount = vertexCount;
    m_transparentStart = transparentStart;

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertexCount, vd, GL_STATIC_DRAW);

    // Pour le moment, generer le index array pour inclure tout les vertex, sans 
    // optimisation pour reduire le nombre de vertex envoyes a la carte
    // Idealement cet array devrait etre utiliser pour reutiliser les vertex et ainsi
    // sauver du temps en envoyant moins de donnees a la carte (il devrait etre construit
    // en meme temps que le buffer vd est rempli..)
    uint16* idx = new uint16[vertexCount];
    for(int i = 0; i < vertexCount; ++i)
        idx[i] = i;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16) * vertexCount, idx, GL_STATIC_DRAW);
    delete [] idx;

    m_isValid = true;
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
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(T::VCOORD_COUNT, GL_FLOAT, sizeof(VertexData), (char*)0);
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(T::COLOR_COUNT, GL_FLOAT, sizeof(VertexData), (char*)(T::VCOORD_COUNT * sizeof(typename T::VCOORD_TYPE)));
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(T::TCOORD_COUNT, GL_FLOAT, sizeof(VertexData), (char*)(T::VCOORD_COUNT * sizeof(typename T::VCOORD_TYPE)) + (T::COLOR_COUNT * sizeof(typename T::COLOR_TYPE)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVboId);
    //glDrawElements(GL_QUADS, m_vertexCount, GL_UNSIGNED_SHORT, (char*)0);
    // TODO
    glDrawRangeElements(GL_QUADS, begin, m_vertexCount, begin + count, GL_UNSIGNED_SHORT, (char*)0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

#endif // MESH_H__
