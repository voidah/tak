#include "scene.h"
#include <cassert>

Scene::Scene() : m_root(0), m_defaultShader(0), m_camera(0)
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
        m_root->InternalUpdate(elapsedTime, m_params);

        if(m_camera)
            m_camera->Update(elapsedTime);
    }
}

bool Scene::Render()
{
    if(!InitDefaultShaderIfNeeded())
        return false;

    if(m_root)
    {

        // Preparer render list, sorting objects to render
        SceneNode::RenderList m_renderList;
        m_root->InternalPrepareRender(m_renderList, m_projection, m_camera ? m_camera->GetMatrix() : Matrix4f::IDENTITY, m_params);

        // Render!
        m_defaultShader->Use();
        m_params.SetCurrentTexture(0);
        for(SceneNode::RenderList::const_iterator it = m_renderList.begin(); it != m_renderList.end(); ++it)
        {
            SceneNode* node = it->second->GetNode();
            node->InternalRender(it->second, m_defaultShader, m_params);
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

bool Scene::InitDefaultShaderIfNeeded()
{
    // Default scene shader, can be changed by a scenenode
    // This one will be used otherwise
    static const char* defaultVertexShader = 
        "varying vec4 light;\n"
        "uniform mat4 projectionMatrix;\n"
        "uniform mat4 modelViewMatrix;\n"
        "void main()\n"
        "{\n"
        "    light = gl_Color;\n"
        "    gl_TexCoord[0] = gl_MultiTexCoord0;\n"
        //"    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;\n"
        "    gl_Position = projectionMatrix * modelViewMatrix * gl_Vertex;\n"
        "}\n";

    static const char* defaultFragmentShader = 
        "uniform sampler2D tex;\n"
        "varying vec4 light;\n"
        "void main()\n"
        "{\n"
        "    vec4 texel;\n"
        "    texel = texture2D(tex,gl_TexCoord[0].st);\n"
        "    texel *= light;\n"
        "    gl_FragColor = texel;\n"
        "}\n";

    if(!m_defaultShader)
    {
        Shader* shader = new Shader();
        if(!shader->LoadFromMemory(defaultVertexShader, defaultFragmentShader, true))
        {
            std::cout << "[Scene::InitDefaultShaderIfNeeded] Failed to load default shader, fatal" << std::endl;
            return false;
        }

        if(!shader->IsValid())
            return false;

        m_defaultShader = shader;
        return true;
    }

    return true;
}

bool Scene::InitDefaultPerspective(int width, int height)
{
    m_projection.SetPerspectiveProjection(90.f, (float)width / (float)height, 0.01f, 5000.0f);
	return true;
}
