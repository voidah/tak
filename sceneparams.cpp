#include "sceneparams.h"
#include <cassert>

SceneParams::SceneParams() : m_width(0), m_height(0), m_currentTexture(0), m_camera(0)
{
}

void SceneParams::SetWidth(int v)
{
    m_width = v;
}

int SceneParams::GetWidth() const
{
    assert(m_width != 0);
    return m_width;
}

void SceneParams::SetHeight(int v)
{
    m_height = v;
}

int SceneParams::GetHeight() const
{
    assert(m_height != 0);
    return m_height;
}

void SceneParams::SetCurrentTexture(Texture* texture)
{
    m_currentTexture = texture;
}

Texture* SceneParams::GetCurrentTexture() const
{
    return m_currentTexture;
}

void SceneParams::SetCamera(Camera* camera)
{
    m_camera = camera;
}

Camera* SceneParams::GetCamera() const
{
    return m_camera;
}
