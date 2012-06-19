#ifndef SCENE_H__
#define SCENE_H__

#include "scenenode.h"
#include "shader.h"
#include "matrix4.h"
#include "sceneparams.h"
#include "camera.h"

class Scene
{
    public:
        Scene();
        ~Scene();

        void Update(float elapsedTime);
        bool Render();

        void SetRoot(SceneNode* node);
        SceneNode* GetRoot() const;

        void WindowResizeEvent(int width, int height);

        SceneParams& GetParams();

        void SetCamera(Camera* camera);
        Camera* GetCamera();

    private:
        bool InitDefaultShaderIfNeeded();
        bool InitDefaultPerspective(int width, int height);

    private:
        SceneNode* m_root;
        Shader* m_defaultShader;

        SceneParams m_params;

        Matrix4f m_projection;

        Camera* m_camera;
};

#endif // SCENE_H__
