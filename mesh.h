#ifndef MESH_H__
#define MESH_H__

#include "define.h"

class Mesh
{
    public:
    // Make sure the size of the struct is a multiple of 32 for maximum
    // performance
    struct VertexData
    {
        float x, y, z;
        float r, g, b;
        float u, v;

        VertexData() {}
        VertexData(float _x, float _y, float _z, float _r, float _g, float _b, float _u, float _v) : x(_x), y(_y), z(_z), r(_r), g(_g), b(_b), u(_u), v(_v) {}
    };

    public:
    Mesh();
    ~Mesh();
    
    bool IsValid() const;
    void SetMeshData(VertexData* vd, int vertexCount, int transparentStart = -1);
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

#endif // MESH_H__
