#ifndef SCENE_H__
#define SCENE_H__

#include "scenenode.h"
#include "shader.h"
#include "matrix4.h"

class Scene
{
    public:
        Scene();
        ~Scene();

        void Update(float elapsedTime);
        bool Render();

        void SetRoot(SceneNode* node);
        SceneNode* GetRoot() const;

    private:
        bool InitDefaultShaderIfNeeded();

    private:
        SceneNode* m_root;
        Shader* m_defaultShader;

        Matrix4f m_projection;
};

#endif // SCENE_H__
