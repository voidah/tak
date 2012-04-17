#include "mesh.h"
#include <cassert>
#include <climits>

Mesh::Mesh() : m_isValid(false), m_transparentStart(-1)
{
}

Mesh::~Mesh()
{
    if(m_isValid)
    {
        glDeleteBuffers(1, &m_vertexVboId);
        glDeleteBuffers(1, &m_indexVboId);
    }
}

bool Mesh::IsValid() const
{
    return m_isValid;
}

void Mesh::SetMeshData(VertexData* vd, int vertexCount, int transparentStart)
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

void Mesh::Render() const
{
    if(IsValid())
        RenderRange(0, m_vertexCount);
}

void Mesh::RenderOpaque() const
{
    int end = m_transparentStart;
    if(end == -1)
        end = m_vertexCount;

    RenderRange(0, end);
}

void Mesh::RenderTransparent() const
{
    if(m_transparentStart == -1)
        return;

    RenderRange(m_transparentStart, m_vertexCount - m_transparentStart);
}

int Mesh::Count() const
{
    return m_vertexCount;
}

void Mesh::RenderRange(int begin, int count) const
{
    if(!IsValid() || count == 0)
        return;

    glClientActiveTexture(GL_TEXTURE0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVboId);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(VertexData), (char*)0);
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, sizeof(VertexData), (char*)12);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(VertexData), (char*)24);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVboId);
    //glDrawElements(GL_QUADS, m_vertexCount, GL_UNSIGNED_SHORT, (char*)0);
    // TODO
    glDrawRangeElements(GL_QUADS, begin, m_vertexCount, begin + count, GL_UNSIGNED_SHORT, (char*)0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
