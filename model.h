#ifndef TAK_MODEL_H__
#define TAK_MODEL_H__

#include "define.h"
#include "resource.h"
#include "vector3.h"
#include "mesh.h"

class Model : public Resource<Model>
{
    public:
        Model();
        virtual ~Model();

        virtual bool Load(const std::string& key, const OptionList& options);
        virtual bool Release();

        Mesh<VertexData3dNormal>* GetMesh();

    private:
        struct Vertex
        {
            Vector3f coord;
            Vector3f normal;
        };

        typedef uint16 Index;

    private:
        Vertex* m_vertices;
        Index* m_indices;

        int m_vertexCount;
        int m_indexCount;
};

#endif // TAK_MODEL_H__
