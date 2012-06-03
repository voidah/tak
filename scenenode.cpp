#include "scenenode.h"
#include "tool.h"

UniqueIdGenerator<SceneNode::IdType> SceneNode::m_idGenerator;

SceneNode::SceneNode(const std::string& name) : m_parent(0), m_name(name), m_posX(0), m_posY(0), m_posZ(0), m_rotX(0), m_rotY(0), m_rotZ(0), m_scaleX(1.f), m_scaleY(1.f), m_scaleZ(1.f)
{
    m_id = m_idGenerator.Get();

    if(m_name == "")
        m_name = "noname";

    std::cout << "Created node with id=" << GetId() << std::endl;
}

SceneNode::~SceneNode()
{
}

SceneNode::IdType SceneNode::GetId() const
{
    return m_id;
}

const std::string& SceneNode::GetName() const
{
    return m_name;
}

void SceneNode::SetParent(SceneNode* node)
{
    m_parent = node;
}

SceneNode* SceneNode::GetParent() const
{
    return m_parent;
}

bool SceneNode::IsLeaf() const
{
    return m_childs.size() == 0;
}

void SceneNode::AddChild(SceneNode* node)
{
    node->SetParent(this);
    m_childs.insert(node);
}

void SceneNode::RemoveChild(SceneNode* node)
{
    m_childs.erase(node);
}

void SceneNode::SetPositionRelative(float x, float y, float z)
{
    m_posX += x;
    m_posY += y;
    m_posZ += z;
}

void SceneNode::SetPositionAbsolute(float x, float y, float z)
{
    m_posX = x;
    m_posY = y;
    m_posZ = z;
}

void SceneNode::SetRotationRelative(float x, float y, float z)
{
    m_rotX += x;
    m_rotY += y;
    m_rotZ += z;
}

void SceneNode::SetRotationAbsolute(float x, float y, float z)
{
    m_rotX = x;
    m_rotY = y;
    m_rotZ = z;
}

void SceneNode::SetScaleRelative(float x, float y, float z)
{
    m_scaleX += x;
    m_scaleY += y;
    m_scaleZ += z;
}

void SceneNode::SetScaleAbsolute(float x, float y, float z)
{
    m_scaleX = x;
    m_scaleY = y;
    m_scaleZ = z;
}

void SceneNode::ShowGraph(bool useGraphviz) const
{
    if(useGraphviz)
    {
        const std::string tmpFile("tmpgraphviz.gv");
        std::ofstream file(tmpFile.c_str());
        file << "digraph scenegraph {" << std::endl;
        file << "size=\"6,6\";" << std::endl;
        file << "node [color=black, shape=note];" << std::endl;
        InternalShowGraphGraphviz(file, this);
        file << "}" << std::endl;

        system(("dot -Tpng -oscenegraph.png " + tmpFile).c_str());
    }
    else
        InternalShowGraphConsole(this);
}

void SceneNode::InternalShowGraphConsole(const SceneNode* node, int level) const
{
    for(int i = 0; i < level; ++i)
        std::cout << "  ";
    std::cout << node->GetId() << std::endl;

    for(ChildNodes::const_iterator it = node->m_childs.begin(); it != node->m_childs.end(); ++it)
        InternalShowGraphConsole(*it, level + 1);
}

void SceneNode::InternalShowGraphGraphviz(std::ofstream& file, const SceneNode* node) const
{

    for(ChildNodes::const_iterator it = node->m_childs.begin(); it != node->m_childs.end(); ++it)
    {
        SceneNode* child = *it;
        file << "\"Name: " << node->GetName() << "\\nId: " << node->GetId() << "\" -> \"Name: " << child->GetName() << "\\nId: " << child->GetId() << "\";" << std::endl;
        InternalShowGraphGraphviz(file, child);
    }
}

void SceneNode::InternalUpdate(float elapsedTime)
{
    Update(elapsedTime);

    for(ChildNodes::const_iterator it = m_childs.begin(); it != m_childs.end(); ++it)
    {
        (*it)->InternalUpdate(elapsedTime);
    }
}

void SceneNode::InternalRender(Matrix4f projection, Matrix4f modelview, Shader* shader)
{
    modelview.ApplyTranslation(m_posX, m_posY, m_posZ);
    modelview.ApplyRotation(m_rotX, 1.f, 0, 0);
    modelview.ApplyRotation(m_rotY, 0, 1.f, 0);
    modelview.ApplyRotation(m_rotZ, 0, 0, 1.f);
    modelview.ApplyScale(m_scaleX, m_scaleY, m_scaleZ);
    shader->SetMat4Uniform("modelViewMatrix", modelview.GetInternalValues());
    CHECK_GL_ERROR();

    //if(!GetParent())
        //std::cout << "=============================" << std::endl;
    //std::cout << "Rendering " << GetName() << std::endl;
    Render();

    for(ChildNodes::const_iterator it = m_childs.begin(); it != m_childs.end(); ++it)
    {
        (*it)->InternalRender(projection, modelview, shader);
    }
}

