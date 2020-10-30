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
#include "material.h"


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

                void Set(SceneNode* node, const Matrix4f& projection, const Matrix4f& modelview, Texture* texture, Shader* shader, bool depthTest)
                {
                    m_node = node;
                    m_projection = projection;
                    m_modelview = modelview;
                    m_texture = texture;
                    m_shader = shader;
                    m_depthTest = depthTest;
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

                Texture* GetTexture() const
                {
                    return m_texture;
                }

                Shader* GetShader() const
                {
                    return m_shader;
                }

                bool GetDepthTest() const
                {
                    return m_depthTest;
                }

            private:
                static UniqueIdGenerator<uint64> m_idGenerator;
                uint64 m_id;

                SceneNode* m_node;
                Matrix4f m_projection;
                Matrix4f m_modelview;

                Texture* m_texture;
                Shader* m_shader;

                bool m_depthTest;
        };

    public:
        class SortKey
        {
            public:
                typedef uint64 Type;

            public:
                // Order is important, the item with the higher priority is first...
                // LAST _must_ be the last item in the enum
                // When a new field is added, it must be added to m_fieldOption
                enum Field { HUD, DEPTH_TEST, SHADER, TEXTURE, LAST };

            public:
                SortKey() : m_value(0)
            {

                // Perform some validation check... only once as it's static data
                // This code probably should be enclosed in #ifdef DEBUG ...
                static bool checked = false;

                if(!checked)
                {
                    // Check that the item ordering in the enum is the same that m_fieldOption
                    for(int i = 0; i < LAST; ++i)
                    {
                        assert((Field)i == m_fieldOption[i].field);
                    }

                    // Check that total bit count doesn't overflow Type size
                    int bitCount = 0;
                    for(int i = 0; i < LAST; ++i)
                        bitCount += m_fieldOption[i].len;

                    //std::cout << "SortKey used bits: " << bitCount << "/" << sizeof(Type) * 8 << std::endl;
                    assert(bitCount < sizeof(Type) * 8);

                    checked = true;
                }
            }

                void SetValue(Field field, Type value)
                {
                    assert((int)field < LAST);

                    // Make sure the value we receive fits in the bit count that
                    // is reserved for that field
                    assert(value < (Type(1) << m_fieldOption[field].len));

                    int pos = sizeof(Type) * 8;
                    for(int i = 0; i < field; ++i)
                        pos -= m_fieldOption[i].len;

                    m_value |= (value << Type(pos - m_fieldOption[field].len));
                }

                Type GetValue() const
                {
                    return m_value;
                }

                bool operator<(const SortKey& v) const
                {
                    return m_value < v.m_value;
                }

            private:
                Type m_value;

            private:
                struct FieldOption
                {
                    Field field;
                    int len;
                };

                static FieldOption m_fieldOption[LAST];
        };

    public:
        typedef uint64 IdType;
        typedef std::multimap<SortKey, RenderBlock*> RenderList;

    public:
        SceneNode(const std::string& name);
        virtual ~SceneNode();

        IdType GetId() const;
        const std::string& GetName() const;

        Material& GetMaterial();
        void SetTexture(Texture* texture);
        Texture* GetTexture() const;
        void SetShader(Shader* shader);
        Shader* GetShader() const;

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

        const Vector3f& GetPosition() const;

        float GetRotX() const;
        float GetRotY() const;
        float GetRotZ() const;

#ifdef TAK_USE_BULLET_PHYSICS
        virtual RigidBody* AddToPhysic(float weight, const Vector3f& position);
        RigidBody* GetBoundRigidBody();
        bool IsBoundToRigidBody() const;
#endif

        void ShowGraph(bool useGraphviz = false) const;

    protected:
        typedef std::set<SceneNode*> ChildNodes;

        enum FLAGS
        {
            FLAG_NONE           = 0x0000,
            FLAG_HUD            = 0x0001,
            FLAG_NO_DEPTH_TEST  = 0x0002
        };

    protected:
#ifdef TAK_USE_BULLET_PHYSICS
        void BindToRigidBody(RigidBody* rigidBody);
#endif

        virtual bool Update(float elapsedTime, SceneParams& params) = 0;
        virtual void Render(const Matrix4f& projection, const Matrix4f& modelview, SceneParams& params) = 0;

        virtual void UpdateProjectionMatrix(Matrix4f& projection, Camera* camera);
        virtual void UpdateModelviewMatrix(Matrix4f& modelview, Camera* camera);

        void SetFlag(FLAGS flag);
        void UnsetFlag(FLAGS flag);
        bool IsFlagSet(FLAGS flag);

    private:
        void InternalShowGraphConsole(const SceneNode* node, int level = 0) const;
        void InternalShowGraphGraphviz(std::ofstream& file, const SceneNode* node) const;

        bool InternalUpdate(float elapsedTime, SceneParams& params);
        void InternalPrepareRender(RenderList& renderList, Matrix4f projection, Matrix4f modelview, SceneParams& params);
        void InternalRender(const RenderBlock* renderBlock, SceneParams& params);

        void DeleteRecursively(SceneNode* node);

    private:
        SceneNode* m_parent;
        ChildNodes m_childs;

        static UniqueIdGenerator<IdType> m_idGenerator;
        IdType m_id;

        std::string m_name;

        uint m_flags;

        Material m_material;
#ifdef TAK_USE_BULLET_PHYSICS
        RigidBody* m_rigidBody;
#endif

        bool m_active;
        bool m_visible;

        Vector3f m_position;
        Vector3f m_rotation;
        Vector3f m_scale;

        RenderBlock m_renderBlock;

        friend class Scene;
};

#endif // SCENENODE_H__
