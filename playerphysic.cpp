#include "playerphysic.h"

#ifdef TAK_USE_BULLET_PHYSICS

#include <BulletCollision/CollisionDispatch/btGhostObject.h>

// TODO
#define MOVEMENT_DIVIDER 6.000f

PlayerPhysic::PlayerPhysic(const std::string& name, const Vector3f& size, float weightKg, float eyeHeight) : m_motion(0), m_name(name), m_size(size), m_weight(weightKg), m_eyeHeight(eyeHeight), m_position(0, 0, 0), m_rotation(0, 0, 0)
{
    // Old code to manually implement a character controller
    // PlayerPhysic position must be initialised before creating the motion state
    //m_motion = new MotionState(this);

    //m_rigidBody = new PlayerRigidBody(this, m_motion);
    //GPhysicEngine.AddRigidBody(m_rigidBody);




    // Reference: Bbullet Character demo
    btTransform startTransform;
    startTransform.setIdentity ();
    // TODO
    startTransform.setOrigin (btVector3(5.f, 20.f + size.y / 2.f, 0.0));


    m_ghostObject = new btPairCachingGhostObject();
    m_ghostObject->setWorldTransform(startTransform);

    GPhysicEngine.GetBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
    btConvexShape* capsule = new btCapsuleShape(.25f, size.y - .5f);
    m_ghostObject->setCollisionShape (capsule);
    m_ghostObject->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);

    //btScalar stepHeight = btScalar(0.35);
    btScalar stepHeight = btScalar(0.55);
    m_character = new btKinematicCharacterController (m_ghostObject,capsule,stepHeight);


    // TODO
    GPhysicEngine.GetDynamicsWorld()->addCollisionObject(m_ghostObject,btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);

    GPhysicEngine.AddAction(m_character);

    m_character->setWalkDirection(btVector3(0, 0, 0));

}

PlayerPhysic::~PlayerPhysic()
{
    if(m_motion)
        delete m_motion;

    if(m_rigidBody)
        delete m_rigidBody;
}


void PlayerPhysic::SetName(const std::string& name)
{
    m_name = name;
}

const std::string& PlayerPhysic::GetName() const
{
    return m_name;
}

void PlayerPhysic::SetSize(const Vector3f& size)
{
    m_size = size;
}

const Vector3f& PlayerPhysic::GetSize() const
{
    return m_size;
}

void PlayerPhysic::SetWeight(float weight)
{
    m_weight = weight;
}

float PlayerPhysic::GetWeight() const
{
    return m_weight;
}

void PlayerPhysic::SetEyeHeight(float eyeHeight)
{
    m_eyeHeight = eyeHeight;
}

float PlayerPhysic::GetEyeHeight() const
{
    return m_eyeHeight;
}

void PlayerPhysic::Move(bool front, bool back, bool left, bool right, bool run, bool fly, float elapsedTime)
{
    Vector3f delta(0, 0, 0);

    float divider = MOVEMENT_DIVIDER;
    if(run)
        divider *= 2;
    if(fly && run)
        divider *= 7;

    float yrotrad = DEGTORAD(m_rotation.y);
    if(front)
    {
        delta.x += float(sin(yrotrad)) * divider * elapsedTime;
        delta.z -= float(cos(yrotrad)) * divider * elapsedTime;
    }

    if(left)
    {
        delta.x -= float(cos(yrotrad)) * divider * elapsedTime;
        delta.z -= float(sin(yrotrad)) * divider * elapsedTime;
    }

    if(back)
    {
        delta.x -= float(sin(yrotrad)) * divider * elapsedTime;
        delta.z += float(cos(yrotrad)) * divider * elapsedTime;
    }

    if(right)
    {
        delta.x += float(cos(yrotrad)) * divider * elapsedTime;
        delta.z += float(sin(yrotrad)) * divider * elapsedTime;
    }


    btTransform& curTrans = m_ghostObject->getWorldTransform();
    btVector3 pos = curTrans.getOrigin();
    Vector3f myVec(pos.getX(), pos.getY(), pos.getZ());
    //std::cout << "ghost pos: " << myVec << std::endl;

    SetPosition(myVec);


    delta.Normalize();
    Vector3f walkDirection = delta * divider * elapsedTime;
    m_character->setWalkDirection(btVector3(walkDirection.x, walkDirection.y, walkDirection.z));


    // Old code to manually implement a character controller
    //btTransform fromTrans;
    //m_motion->getWorldTransform(fromTrans);
    //btTransform toTrans(fromTrans);
    //toTrans.setOrigin(btVector3(m_position.x + delta.x, m_position.y + delta.y, m_position.z + delta.z));

    //CollisionList collisions;
    //GPhysicEngine.GetCollisionWorld()->convexSweepTest(m_rigidBody->GetCollisionShape(), fromTrans, toTrans, collisions);

    //if(collisions.Count() != 0)
    //{
    //std::cout << "Collision!" << std::endl;
    //for(CollisionList::ConstIterator it = collisions.Begin(); it != collisions.End(); ++it)
    //{
    ////std::cout << (*it).hitPoint << std::endl;
    //Vector3f relCol = (*it).hitPoint - m_position;

    //std::cout << "relCol: " << relCol << std::endl;
    //std::cout << "delta avant: " << delta << std::endl;
    //delta.x = std::min(delta.x, relCol.x);
    //delta.y = std::min(delta.y, relCol.y);
    //delta.z = std::min(delta.z, relCol.z);
    //std::cout << "delta apres: " << delta << std::endl;
    //}

    //// TODO wrong.. freeze the player instead of allowing it to slide
    //// along walls
    //delta = Vector3f(0, 0, 0);
    //}

    //m_position += delta;
}

bool PlayerPhysic::IsOnGround() const
{
    return m_character->canJump();
}

void PlayerPhysic::Jump()
{
    m_character->jump();
}

void PlayerPhysic::SetPosition(const Vector3f& position)
{
    m_position = position;
}

Vector3f PlayerPhysic::GetPosition() const
{
    return m_position;
}

Vector3f PlayerPhysic::GetEyePosition() const
{
    Vector3f eyePos = GetPosition();
    eyePos.y += GetEyeHeight() - (m_size.y / 2.f);

    return eyePos;
}

void PlayerPhysic::Rotate(const Vector3f& deltaRotation)
{
    m_rotation += deltaRotation;
    ClampRotation();
}

void PlayerPhysic::SetRotation(const Vector3f& rotation)
{
    m_rotation = rotation;
    ClampRotation();
}

const Vector3f& PlayerPhysic::GetRotation() const
{
    return m_rotation;
}

Vector3f PlayerPhysic::GetEyeVector() const
{
    float rx = DEGTORAD(m_rotation.x);
    float ry = DEGTORAD(m_rotation.y);
    Vector3f result(sin(ry) * cos(rx), -sin(rx), -cos(ry) * cos(rx));
    result.Normalize();
    return result;
}

void PlayerPhysic::ClampRotation()
{
    if(m_rotation.x >= 85.f)
        m_rotation.x = 85.f;

    if(m_rotation.x <= -85.f)
        m_rotation.x = -85.f;

}

#endif

