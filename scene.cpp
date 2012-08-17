#include "scene.h"
#include <cassert>

Scene::Scene() : m_root(0), m_camera(0)
{
    InitDefaultPerspective(1, 1);
}

Scene::~Scene()
{
}

void Scene::Update(float elapsedTime)
{
    if(m_root)
    {
        m_params.SetCamera(m_camera);
        m_root->InternalUpdate(elapsedTime, m_params);

        if(m_camera)
            m_camera->Update(elapsedTime);
    }
}

bool Scene::Render(Shader* defaultShader, Texture* defaultTexture)
{
    assert(defaultShader);

    if(m_root)
    {

        // Prepare render list, sorting objects to render
        InitSceneParams(defaultShader, defaultTexture);
        SceneNode::RenderList m_renderList;
        m_root->InternalPrepareRender(m_renderList, m_projection, m_camera ? m_camera->GetMatrix() : Matrix4f::IDENTITY, m_params);

        // Render!
        InitSceneParams(defaultShader, defaultTexture);
        for(SceneNode::RenderList::const_iterator it = m_renderList.begin(); it != m_renderList.end(); ++it)
        {
            SceneNode* node = it->second->GetNode();
            node->InternalRender(it->second, m_params);
        }
        Shader::Disable();
    }

    return true;
}

void Scene::SetRoot(SceneNode* node)
{
    m_root = node;
}

SceneNode* Scene::GetRoot() const
{
    return m_root;
}

void Scene::KeyPressEvent(unsigned char key)
{
    if(m_camera)
        m_camera->KeyPressEvent(key);
}

void Scene::KeyReleaseEvent(unsigned char key)
{
    if(m_camera)
        m_camera->KeyReleaseEvent(key);
}

void Scene::MouseMoveEvent(int x, int y)
{
    if(m_camera)
        m_camera->MouseMoveEvent(x, y);
}

void Scene::MousePressEvent(const MOUSE_BUTTON &button, int x, int y)
{
    if(m_camera)
        m_camera->MousePressEvent(button, x, y);
}

void Scene::MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y)
{
    if(m_camera)
        m_camera->MouseReleaseEvent(button, x, y);
}

void Scene::WindowFocusEvent(bool hasFocus)
{
    if(m_camera)
        m_camera->WindowFocusEvent(hasFocus);
}

void Scene::WindowResizeEvent(int width, int height)
{
    if(m_camera)
        m_camera->WindowResizeEvent(width, height);

    m_params.SetWidth(width);
    m_params.SetHeight(height);
    InitDefaultPerspective(width, height);
}

SceneParams& Scene::GetParams()
{
    return m_params;
}

void Scene::SetCamera(Camera* camera)
{
    m_camera = camera;
}

Camera* Scene::GetCamera()
{
    return m_camera;
}

const Matrix4f& Scene::GetDefaultPerspective() const
{
    return m_projection;
}

bool Scene::InitDefaultPerspective(int width, int height)
{
    m_projection.SetPerspectiveProjection(90.f, (float)width / (float)height, 0.01f, 5000.0f);
    return true;
}

void Scene::InitSceneParams(Shader* defaultShader, Texture* defaultTexture)
{
    m_params.SetDefaultTexture(defaultTexture);
    m_params.SetCurrentTexture(0);
    m_params.SetDefaultShader(defaultShader);
    m_params.SetCurrentShader(0);
    m_params.SetCamera(m_camera);
}
