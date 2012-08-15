#include "scenenode.h"
#include "tool.h"
#include <cassert>

UniqueIdGenerator<SceneNode::IdType> SceneNode::m_idGenerator;
UniqueIdGenerator<uint64> SceneNode::RenderBlock::m_idGenerator;

SceneNode::SceneNode(const std::string& name) : m_parent(0), m_name(name), m_flags(FLAG_NONE), m_rigidBody(0), m_active(true), m_visible(true), m_posX(0), m_posY(0), m_posZ(0), m_rotX(0), m_rotY(0), m_rotZ(0), m_scaleX(1.f), m_scaleY(1.f), m_scaleZ(1.f)
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

SceneNode::Material& SceneNode::GetMaterial()
{
    return m_material;
}

void SceneNode::SetTexture(Texture* texture)
{
    m_material.texture = texture;
}

Texture* SceneNode::GetTexture() const
{
    return m_material.texture;
}

void SceneNode::SetActive(bool v)
{
    m_active = v;
}

bool SceneNode::IsActive() const
{
    return m_active;
}

void SceneNode::SetVisible(bool v)
{
    m_visible = v;
}

bool SceneNode::IsVisible() const
{
    return m_visible;
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

void SceneNode::SetPositionRelative(const Vector3f& pos)
{
    SetPositionRelative(pos.x, pos.y, pos.z);
}

void SceneNode::SetPositionAbsolute(float x, float y, float z)
{
    m_posX = x;
    m_posY = y;
    m_posZ = z;
}

void SceneNode::SetPositionAbsolute(const Vector3f& pos)
{
    SetPositionAbsolute(pos.x, pos.y, pos.z);
}

void SceneNode::SetRotationRelative(float x, float y, float z)
{
    m_rotX += x;
    m_rotY += y;
    m_rotZ += z;
}

void SceneNode::SetRotationRelative(const Vector3f& rot)
{
    SetRotationRelative(rot.x, rot.y, rot.z);
}

void SceneNode::SetRotationAbsolute(float x, float y, float z)
{
    m_rotX = x;
    m_rotY = y;
    m_rotZ = z;
}

void SceneNode::SetRotationAbsolute(const Vector3f& rot)
{
    SetRotationAbsolute(rot.x, rot.y, rot.z);
}

void SceneNode::SetScaleRelative(float x, float y, float z)
{
    m_scaleX += x;
    m_scaleY += y;
    m_scaleZ += z;
}

void SceneNode::SetScaleRelative(const Vector3f& scale)
{
    SetScaleRelative(scale.x, scale.y, scale.z);
}

void SceneNode::SetScaleAbsolute(float x, float y, float z)
{
    m_scaleX = x;
    m_scaleY = y;
    m_scaleZ = z;
}

void SceneNode::SetScaleAbsolute(const Vector3f& scale)
{
    SetScaleAbsolute(scale.x, scale.y, scale.z);
}

float SceneNode::GetRotX() const
{
    return m_rotX;
}

float SceneNode::GetRotY() const
{
    return m_rotY;
}

float SceneNode::GetRotZ() const
{
    return m_rotZ;
}

RigidBody* SceneNode::AddToPhysic(float weight, const Vector3f& position)
{
    // Nodes that can be added to physic should implement this method.
    // Simply assert if it's not the case.
    assert(false);
    return 0;
}

RigidBody* SceneNode::GetBoundRigidBody()
{
    return m_rigidBody;
}

bool SceneNode::IsBoundToRigidBody() const
{
    return (m_rigidBody != 0);
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

void SceneNode::BindToRigidBody(RigidBody* rigidBody)
{
    m_rigidBody = rigidBody;
}

void SceneNode::UpdateProjectionMatrix(Matrix4f& /*projection*/)
{
    // Bu default do nothing, but a derived node coule
    // override this method to change the projection matrix
}

void SceneNode::UpdateModelviewMatrix(Matrix4f& /*modelview*/)
{
    // Bu default do nothing, but a derived node coule
    // override this method to change the model view matrix
}

void SceneNode::SetFlag(FLAGS flag)
{
    m_flags |= flag;
}

void SceneNode::UnsetFlag(FLAGS flag)
{
    m_flags &= ~flag;
}

bool SceneNode::IsFlagSet(FLAGS flag)
{
    return ((m_flags & flag) == flag);
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

void SceneNode::InternalUpdate(float elapsedTime, SceneParams& params)
{
    if(!IsActive())
        return;

    if(IsBoundToRigidBody())
    {
        //std::cout << "SceneNode::InternalUpdate: " << m_rigidBody->GetRotation() << std::endl;
        SetPositionAbsolute(m_rigidBody->GetPosition());

        //Vector3f rotRad = m_rigidBody->GetRotation();
        //SetRotationAbsolute(RADTODEG(rotRad.x), RADTODEG(rotRad.y), RADTODEG(rotRad.z));
        SetRotationAbsolute(m_rigidBody->GetRotation());
    }

    Update(elapsedTime, params);

    for(ChildNodes::const_iterator it = m_childs.begin(); it != m_childs.end(); ++it)
    {
        (*it)->InternalUpdate(elapsedTime, params);
    }
}

void SceneNode::InternalPrepareRender(RenderList& renderList, Matrix4f projection, Matrix4f modelview, SceneParams& params)
{
    if(!IsVisible())
        return;

    modelview.ApplyTranslation(m_posX, m_posY, m_posZ);
    modelview.ApplyRotation(m_rotX, 1.f, 0, 0);
    modelview.ApplyRotation(m_rotY, 0, 1.f, 0);
    modelview.ApplyRotation(m_rotZ, 0, 0, 1.f);
    modelview.ApplyScale(m_scaleX, m_scaleY, m_scaleZ);

    UpdateProjectionMatrix(projection);
    UpdateModelviewMatrix(modelview);

    SortKeyType key = 0;

    if(IsFlagSet(FLAG_HUD))
        key = 1;

    m_renderBlock.Set(this, projection, modelview);
    renderList.insert(std::make_pair(key, &m_renderBlock));

    for(ChildNodes::const_iterator it = m_childs.begin(); it != m_childs.end(); ++it)
    {
        (*it)->InternalPrepareRender(renderList, projection, modelview, params);
    }
}

void SceneNode::InternalRender(const RenderBlock* renderBlock, Shader* shader, SceneParams& params)
{
    Matrix4f modelview = renderBlock->GetModelviewMatrix();
    Matrix4f projection = renderBlock->GetProjectionMatrix();

    shader->SetMat4Uniform("projectionMatrix", projection.GetInternalValues());
    shader->SetMat4Uniform("modelViewMatrix", modelview.GetInternalValues());
    CHECK_GL_ERROR();

    Texture* texture = GetTexture();
    if(texture && texture != params.GetCurrentTexture())
    {
        //std::cout << "Binding texture name=" << texture->GetTextureName() << std::endl;
        texture->Bind();

        params.SetCurrentTexture(texture);
    }
    else
    {
        //std::cout << "no bind, same texture" << std::endl;
    }

    //if(!GetParent())
    //std::cout << "=============================" << std::endl;
    //std::cout << "Rendering " << GetName() << std::endl;
    Render(projection, modelview, params);
}

