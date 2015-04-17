#include "rigidbody.h"
#include "define.h"
#include <cassert>

RigidBody::RigidBody(float weight) : m_rigidBody(0), m_weight(weight), m_node(0)
{
}

RigidBody::~RigidBody()
{
    if(m_rigidBody)
    {
        delete m_rigidBody->getMotionState();
        delete m_rigidBody;
    }
}

void RigidBody::SetPosition(const Vector3f& position)
{
    assert(m_rigidBody);

    btTransform transform = m_rigidBody->getCenterOfMassTransform();
    transform.setOrigin(btVector3(position.x, position.y, position.z));
    m_rigidBody->setCenterOfMassTransform(transform);
}

Vector3f RigidBody::GetPosition() const
{
    assert(m_rigidBody);

    btTransform trans;
    m_rigidBody->getMotionState()->getWorldTransform(trans);

    btVector3 pos = trans.getOrigin();
    return Vector3f(pos.getX(), pos.getY(), pos.getZ());
}

Vector3f RigidBody::GetRotation() const
{
    assert(m_rigidBody);

    btTransform trans;
    m_rigidBody->getMotionState()->getWorldTransform(trans);

    float x, y, z;
    trans.getBasis().getEulerZYX(x, y, z);


    Vector3f rot = Vector3f(RADTODEG(x), RADTODEG(y), RADTODEG(z));
    return rot;
}

void RigidBody::SetLinearVelocity(const Vector3f& velocity)
{
    assert(m_rigidBody);

    m_rigidBody->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
}

Vector3f RigidBody::GetLinearVelocity() const
{
    assert(m_rigidBody);

    const btVector3& velocity = m_rigidBody->getLinearVelocity();
    return Vector3f(velocity.x(), velocity.y(), velocity.z());
}

void RigidBody::SetKinematic()
{
    m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
}

void RigidBody::ApplyForce(const Vector3f& force)
{
    m_rigidBody->applyCentralImpulse(btVector3(force.x, force.y, force.z));
}

btRigidBody* RigidBody::GetInternalHandle() const
{
    return m_rigidBody;
}

void RigidBody::LinkWithNode(SceneNode* node)
{
    m_node = node;
}

SceneNode* RigidBody::GetLinkedNode() const
{
    return m_node;
}

void RigidBody::SetInternalHandle(btRigidBody* handle)
{
    m_rigidBody = handle;
    m_rigidBody->setUserPointer(this);
}

BoxRigidBody::BoxRigidBody(float weight, const Vector3f& position, const Vector3f& size) : RigidBody(weight)
{
    btCollisionShape* fallShape = new btBoxShape(btVector3(size.x / 2.f, size.y / 2.f, size.z / 2.f));

    btDefaultMotionState* fallMotionState =
        //new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,50,0)));
        //new btDefaultMotionState(btTransform(btQuaternion(btVector3(0, 0, 1), 45),btVector3(0,10,0)));
        new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(position.x, position.y, position.z)));
    btScalar mass = weight;
    btVector3 fallInertia(0,0,0);
    fallShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
    SetInternalHandle(new btRigidBody(fallRigidBodyCI));
}

BoxRigidBody::~BoxRigidBody()
{
}

CapsuleRigidBody::CapsuleRigidBody(float weight) : RigidBody(weight)
{
    // The total height is height+2*radius
    btCollisionShape* fallShape = new btCapsuleShape(.5f, 1);

    btDefaultMotionState* fallMotionState =
        //new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,50,0)));
        new btDefaultMotionState(btTransform(btQuaternion(btVector3(0, 0, 1), 0),btVector3(5,5,0)));
    btScalar mass = weight;
    btVector3 fallInertia(0,0,0);
    fallShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
    SetInternalHandle(new btRigidBody(fallRigidBodyCI));
}

CapsuleRigidBody::~CapsuleRigidBody()
{
}
