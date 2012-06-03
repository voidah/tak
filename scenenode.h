#ifndef SCENENODE_H__
#define SCENENODE_H__

#include <set>
#include <fstream>
#include "uniqueidgenerator.h"
#include "define.h"
#include "shader.h"
#include "matrix4.h"
#include "sceneparams.h"


class SceneNode
{
    public:
        typedef uint64 IdType;

    public:
        SceneNode(const std::string& name);
        virtual ~SceneNode();

        IdType GetId() const;
        const std::string& GetName() const;

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
        void SetPositionAbsolute(float x, float y, float z);
        void SetRotationRelative(float x, float y, float z);
        void SetRotationAbsolute(float x, float y, float z);
        void SetScaleRelative(float x, float y, float z);
        void SetScaleAbsolute(float x, float y, float z);

        void ShowGraph(bool useGraphviz = false) const;

    protected:
        typedef std::set<SceneNode*> ChildNodes;

    protected:
        virtual void Update(float elapsedTime, SceneParams& params) = 0;
        virtual void Render(SceneParams& params) = 0;

    private:
        void InternalShowGraphConsole(const SceneNode* node, int level = 0) const;
        void InternalShowGraphGraphviz(std::ofstream& file, const SceneNode* node) const;

        void InternalUpdate(float elapsedTime, SceneParams& params);
        void InternalRender(Matrix4f projection, Matrix4f modelview, Shader* shader, SceneParams& params);

    private:
        SceneNode* m_parent;
        ChildNodes m_childs;

        static UniqueIdGenerator<IdType> m_idGenerator;
        IdType m_id;

        std::string m_name;

        bool m_active;
        bool m_visible;

        float m_posX, m_posY, m_posZ;
        float m_rotX, m_rotY, m_rotZ;
        float m_scaleX, m_scaleY, m_scaleZ;

        friend class Scene;
};

#endif // SCENENODE_H__
