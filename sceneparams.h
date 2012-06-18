#ifndef SCENEPARAMS_H__
#define SCENEPARAMS_H__

#include "texture.h"

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

    private:
        int m_width;
        int m_height;
        Texture* m_currentTexture;
};

#endif // SCENEPARAMS_H__
