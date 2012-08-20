#include "sceneparams.h"
#include <cassert>

SceneParams::SceneParams() : m_width(0), m_height(0), m_defaultTexture(0), m_currentTexture(0), m_defaultShader(0), m_currentShader(0), m_camera(0), m_depthTest(NOT_SET)
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

void SceneParams::SetDefaultTexture(Texture* texture)
{
    m_defaultTexture = texture;
}

Texture* SceneParams::GetDefaultTexture() const
{
    return m_defaultTexture;
}

void SceneParams::SetCurrentTexture(Texture* texture)
{
    m_currentTexture = texture;
}

Texture* SceneParams::GetCurrentTexture() const
{
    return m_currentTexture;
}

void SceneParams::SetDefaultShader(Shader* shader)
{
    m_defaultShader = shader;
}

Shader* SceneParams::GetDefaultShader() const
{
    return m_defaultShader;
}

void SceneParams::SetCurrentShader(Shader* shader)
{
    m_currentShader = shader;
}

Shader* SceneParams::GetCurrentShader() const
{
    return m_currentShader;
}

void SceneParams::SetCamera(Camera* camera)
{
    m_camera = camera;
}

Camera* SceneParams::GetCamera() const
{
    return m_camera;
}

void SceneParams::SetCurrentDepthTest(SceneParams::PARAM_VALUE depthTest)
{
    m_depthTest = depthTest;
}

SceneParams::PARAM_VALUE SceneParams::GetCurrentDepthTest() const
{
    return m_depthTest;
}

