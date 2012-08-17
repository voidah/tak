#include "scenenode.h"
#include "tool.h"
#include <cassert>

UniqueIdGenerator<SceneNode::IdType> SceneNode::m_idGenerator;
UniqueIdGenerator<uint64> SceneNode::RenderBlock::m_idGenerator;

// The order of these fields must match the order found in the SortKey::Field enum
SceneNode::SortKey::FieldOption SceneNode::SortKey::m_fieldOption[SceneNode::SortKey::LAST] = {
    {SceneNode::SortKey::HUD, 1},
    {SceneNode::SortKey::SHADER, 4},
    {SceneNode::SortKey::TEXTURE, 5}
};

SceneNode::SceneNode(const std::string& name) : m_parent(0), m_name(name), m_flags(FLAG_NONE), m_rigidBody(0), m_active(true), m_visible(true), m_position(0, 0, 0), m_rotation(0, 0, 0), m_scale(1.f, 1.f, 1.f)
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

Material& SceneNode::GetMaterial()
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

void SceneNode::SetShader(Shader* shader)
{
    m_material.shader = shader;
}

Shader* SceneNode::GetShader() const
{
    return m_material.shader;
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
    m_position.x += x;
    m_position.y += y;
    m_position.z += z;
}

void SceneNode::SetPositionRelative(const Vector3f& pos)
{
    SetPositionRelative(pos.x, pos.y, pos.z);
}

void SceneNode::SetPositionAbsolute(float x, float y, float z)
{
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;
}

void SceneNode::SetPositionAbsolute(const Vector3f& pos)
{
    SetPositionAbsolute(pos.x, pos.y, pos.z);
}

void SceneNode::SetRotationRelative(float x, float y, float z)
{
    m_rotation.x += x;
    m_rotation.y += y;
    m_rotation.z += z;
}

void SceneNode::SetRotationRelative(const Vector3f& rot)
{
    SetRotationRelative(rot.x, rot.y, rot.z);
}

void SceneNode::SetRotationAbsolute(float x, float y, float z)
{
    m_rotation.x = x;
    m_rotation.y = y;
    m_rotation.z = z;
}

void SceneNode::SetRotationAbsolute(const Vector3f& rot)
{
    SetRotationAbsolute(rot.x, rot.y, rot.z);
}

void SceneNode::SetScaleRelative(float x, float y, float z)
{
    m_scale.x += x;
    m_scale.y += y;
    m_scale.z += z;
}

void SceneNode::SetScaleRelative(const Vector3f& scale)
{
    SetScaleRelative(scale.x, scale.y, scale.z);
}

void SceneNode::SetScaleAbsolute(float x, float y, float z)
{
    m_scale.x = x;
    m_scale.y = y;
    m_scale.z = z;
}

void SceneNode::SetScaleAbsolute(const Vector3f& scale)
{
    SetScaleAbsolute(scale.x, scale.y, scale.z);
}

const Vector3f& SceneNode::GetPosition() const
{
    return m_position;
}

float SceneNode::GetRotX() const
{
    return m_rotation.x;
}

float SceneNode::GetRotY() const
{
    return m_rotation.y;
}

float SceneNode::GetRotZ() const
{
    return m_rotation.z;
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
    m_rigidBody->LinkWithNode(this);
}

void SceneNode::UpdateProjectionMatrix(Matrix4f& /*projection*/, Camera* /*camera*/)
{
    // Bu default do nothing, but a derived node coule
    // override this method to change the projection matrix
}

void SceneNode::UpdateModelviewMatrix(Matrix4f& /*modelview*/, Camera* /*camera*/)
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

bool SceneNode::InternalUpdate(float elapsedTime, SceneParams& params)
{
    if(!IsActive())
        return true;

    if(IsBoundToRigidBody())
    {
        //std::cout << "SceneNode::InternalUpdate: " << m_rigidBody->GetRotation() << std::endl;
        SetPositionAbsolute(m_rigidBody->GetPosition());

        //Vector3f rotRad = m_rigidBody->GetRotation();
        //SetRotationAbsolute(RADTODEG(rotRad.x), RADTODEG(rotRad.y), RADTODEG(rotRad.z));
        SetRotationAbsolute(m_rigidBody->GetRotation());
    }

    if(!Update(elapsedTime, params))
        return false;;

    std::vector<SceneNode*> nodesToDelete;
    for(ChildNodes::const_iterator it = m_childs.begin(); it != m_childs.end(); ++it)
    {
        if(!(*it)->InternalUpdate(elapsedTime, params))
            nodesToDelete.push_back(*it);
    }

    for(int i = 0; i < nodesToDelete.size(); ++i)
    {
        SceneNode* node = nodesToDelete[i];
        std::cout << "Automatically removing node and it's childs (id=" << node->GetId() << ")" << std::endl;

        // Delete all childs recursively:
        for(ChildNodes::const_iterator it = node->m_childs.begin(); it != node->m_childs.end(); ++it)
            DeleteRecursively(*it);

        RemoveChild(node);
        // TODO all nodes should be allocated/freed using a factory
        delete node;

    }

    return true;
}

void SceneNode::InternalPrepareRender(RenderList& renderList, Matrix4f projection, Matrix4f modelview, SceneParams& params)
{
    if(!IsVisible())
        return;

    assert(params.GetDefaultShader());
    assert(params.GetDefaultTexture());

    modelview.ApplyTranslation(m_position.x, m_position.y, m_position.z);
    modelview.ApplyRotation(m_rotation.x, 1.f, 0, 0);
    modelview.ApplyRotation(m_rotation.y, 0, 1.f, 0);
    modelview.ApplyRotation(m_rotation.z, 0, 0, 1.f);
    modelview.ApplyScale(m_scale.x, m_scale.y, m_scale.z);

    UpdateProjectionMatrix(projection, params.GetCamera());
    UpdateModelviewMatrix(modelview, params.GetCamera());

    Texture* texture = GetTexture();
    Shader* shader = GetShader();

    if(!texture)
        texture = params.GetDefaultTexture();
    assert(texture);

    if(!shader)
        shader = params.GetDefaultShader();
    assert(shader);


    SortKey key;

    if(IsFlagSet(FLAG_HUD))
        key.SetValue(SortKey::HUD, 1);

    Resource<Shader>::SequentialKeyType shaderKey = shader->GetSequentialKey();
    Resource<Texture>::SequentialKeyType textureKey = texture->GetSequentialKey();

    if(shaderKey == Resource<Shader>::INVALID_KEY)
        shaderKey = 0;

    if(textureKey == Resource<Texture>::INVALID_KEY)
        textureKey = 0;

    // We add +1 to the key because INVALID_KEY are assigned the value 0
    key.SetValue(SortKey::SHADER, shaderKey + 1);
    key.SetValue(SortKey::TEXTURE, textureKey + 1);

    m_renderBlock.Set(this, projection, modelview, texture, shader);
    renderList.insert(std::make_pair(key, &m_renderBlock));

    for(ChildNodes::const_iterator it = m_childs.begin(); it != m_childs.end(); ++it)
    {
        (*it)->InternalPrepareRender(renderList, projection, modelview, params);
    }
}

void SceneNode::InternalRender(const RenderBlock* renderBlock, SceneParams& params)
{
    assert(params.GetDefaultShader());
    assert(params.GetDefaultTexture());

    Matrix4f modelview = renderBlock->GetModelviewMatrix();
    Matrix4f projection = renderBlock->GetProjectionMatrix();

    Shader* shader = renderBlock->GetShader();
    assert(shader);
    bool newShaderBound = false;
    if(shader != params.GetCurrentShader())
    {
        newShaderBound = true;
        shader->Use();
    }

    if(newShaderBound)
    {
        //std::cout << "Binding new shader (seqKey=" << shader->GetSequentialKey() << ")";
        //std::cout << " (old shader was: " << (params.GetCurrentShader() ? params.GetCurrentShader()->GetSequentialKey() : -1) << ")";
        //std::cout << std::endl;
    }

    params.SetCurrentShader(shader);


    shader->SetMat4Uniform("projectionMatrix", projection.GetInternalValues());
    shader->SetMat4Uniform("modelViewMatrix", modelview.GetInternalValues());
    CHECK_GL_ERROR();

    Texture* texture = renderBlock->GetTexture();
    assert(texture);
    bool newTextureBound = false;
    if(texture != params.GetCurrentTexture())
    {
        newTextureBound = true;
        texture->Bind();
    }

    if(newTextureBound)
    {
        //std::cout << "Binding new texture (seqKey=" << texture->GetSequentialKey() << ")";
        //std::cout << " (old texture was: " << (params.GetCurrentTexture() ? params.GetCurrentTexture()->GetSequentialKey() : -1) << ")";
        //std::cout << std::endl;
    }

    params.SetCurrentTexture(texture);

    Render(projection, modelview, params);
}

void SceneNode::DeleteRecursively(SceneNode* node)
{
    // TODO all nodes should be allocated/freed using a factory
    for(ChildNodes::const_iterator it = node->m_childs.begin(); it != node->m_childs.end(); ++it)
    {
        DeleteRecursively(*it);
    }

    delete node;
}

