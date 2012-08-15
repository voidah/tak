#ifndef SCENENODE_H__
#define SCENENODE_H__

#include <set>
#include <fstream>
#include <map>
#include "uniqueidgenerator.h"
#include "define.h"
#include "shader.h"
#include "matrix4.h"
#include "sceneparams.h"
#include "texture.h"
#include "rigidbody.h"
#include "vector3.h"


class SceneNode
{
    protected:
        class RenderBlock
        {
            public:
                RenderBlock()
                {
                    m_id = m_idGenerator.Get();
                    std::cout << "Created RenderBlock with id=" << m_id << std::endl;
                }

                void Set(SceneNode* node, const Matrix4f& projection, const Matrix4f& modelview)
                {
                    m_node = node;
                    m_projection = projection;
                    m_modelview = modelview;
                }

                virtual ~RenderBlock()
                {
                    std::cout << "Destroyed RenderBlock with id=" << m_id << std::endl;
                }

                uint64 GetId() const
                {
                    return m_id;
                }

                SceneNode* GetNode() const
                {
                    return m_node;
                }

                const Matrix4f& GetProjectionMatrix() const
                {
                    return m_projection;
                }

                const Matrix4f& GetModelviewMatrix() const
                {
                    return m_modelview;
                }

            private:
                static UniqueIdGenerator<uint64> m_idGenerator;
                uint64 m_id;

                SceneNode* m_node;
                Matrix4f m_projection;
                Matrix4f m_modelview;
        };

    public:
        typedef uint64 IdType;
        typedef uint64 SortKeyType;
        typedef std::multimap<SortKeyType, RenderBlock*> RenderList;

    public:
        struct Material
        {
            Texture* texture;

            Material() : texture(0) {}
        };

    public:
        SceneNode(const std::string& name);
        virtual ~SceneNode();

        IdType GetId() const;
        const std::string& GetName() const;

        Material& GetMaterial();
        void SetTexture(Texture* texture);
        Texture* GetTexture() const;

        void SetActive(bool v);
        bool IsActive() const;
        void SetVisible(bool v);
        bool IsVisible() const;

        void SetParent(SceneNode* node);
        SceneNode* GetParent() const;

        bool IsLeaf() const;
        void AddChild(SceneNode* node);
        void RemoveChild(SceneNode* node);

        void SetPositionRelative(float x, float y, float z);
        void SetPositionRelative(const Vector3f& pos);
        void SetPositionAbsolute(float x, float y, float z);
        void SetPositionAbsolute(const Vector3f& pos);

        void SetRotationRelative(float x, float y, float z);
        void SetRotationRelative(const Vector3f& rot);
        void SetRotationAbsolute(float x, float y, float z);
        void SetRotationAbsolute(const Vector3f& rot);

        void SetScaleRelative(float x, float y, float z);
        void SetScaleRelative(const Vector3f& scale);
        void SetScaleAbsolute(float x, float y, float z);
        void SetScaleAbsolute(const Vector3f& scale);

        float GetRotX() const;
        float GetRotY() const;
        float GetRotZ() const;

        virtual RigidBody* AddToPhysic(float weight, const Vector3f& position);
        RigidBody* GetBoundRigidBody();
        bool IsBoundToRigidBody() const;

        void ShowGraph(bool useGraphviz = false) const;

    protected:
        typedef std::set<SceneNode*> ChildNodes;

        enum FLAGS
        {
            FLAG_NONE       = 0x0000,
            FLAG_HUD         = 0x0001
        };

    protected:
        void BindToRigidBody(RigidBody* rigidBody);

        virtual void Update(float elapsedTime, SceneParams& params) = 0;
        virtual void Render(const Matrix4f& projection, const Matrix4f& modelview, SceneParams& params) = 0;

        virtual void UpdateProjectionMatrix(Matrix4f& projection);
        virtual void UpdateModelviewMatrix(Matrix4f& modelview);

        void SetFlag(FLAGS flag);
        void UnsetFlag(FLAGS flag);
        bool IsFlagSet(FLAGS flag);

    private:
        void InternalShowGraphConsole(const SceneNode* node, int level = 0) const;
        void InternalShowGraphGraphviz(std::ofstream& file, const SceneNode* node) const;

        void InternalUpdate(float elapsedTime, SceneParams& params);
        void InternalPrepareRender(RenderList& renderList, Matrix4f projection, Matrix4f modelview, SceneParams& params);
        void InternalRender(const RenderBlock* renderBlock, Shader* shader, SceneParams& params);

    private:
        SceneNode* m_parent;
        ChildNodes m_childs;

        static UniqueIdGenerator<IdType> m_idGenerator;
        IdType m_id;

        std::string m_name;

        uint m_flags;

        Material m_material;
        RigidBody* m_rigidBody;

        bool m_active;
        bool m_visible;

        float m_posX, m_posY, m_posZ;
        float m_rotX, m_rotY, m_rotZ;
        float m_scaleX, m_scaleY, m_scaleZ;

        RenderBlock m_renderBlock;

        friend class Scene;
};

#endif // SCENENODE_H__
