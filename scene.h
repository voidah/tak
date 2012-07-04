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

        void KeyPressEvent(unsigned char key);
        void KeyReleaseEvent(unsigned char key);
        void MouseMoveEvent(int x, int y);
        void MousePressEvent(const MOUSE_BUTTON &button, int x, int y);
        void MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y);
        void WindowFocusEvent(bool hasFocus);
        void WindowResizeEvent(int width, int height);

        SceneParams& GetParams();

        void SetCamera(Camera* camera);
        Camera* GetCamera();

        const Matrix4f& GetDefaultPerspective() const;

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
