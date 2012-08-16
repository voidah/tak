#ifndef SCENEPARAMS_H__
#define SCENEPARAMS_H__

#include "texture.h"

class Camera;

class SceneParams
{
    public:
        SceneParams();

        void SetWidth(int v);
        int GetWidth() const;
        void SetHeight(int v);
        int GetHeight() const;

        void SetCurrentTexture(Texture* texture);
        Texture* GetCurrentTexture() const;

        void SetCamera(Camera* camera);
        Camera* GetCamera() const;

    private:
        int m_width;
        int m_height;
        Texture* m_currentTexture;
        Camera* m_camera;
};

#endif // SCENEPARAMS_H__
