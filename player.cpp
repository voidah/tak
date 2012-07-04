#include "player.h"

// TODO
#define MOVEMENT_DIVIDER 4.000f

Player::Player(const std::string& name, const Vector3f& size, float weightKg, float eyeHeight) : m_motion(0), m_name(name), m_size(size), m_weight(weightKg), m_eyeHeight(eyeHeight), m_position(0, 0, 0), m_rotation(0, 0, 0)
{
    // Player position must be initialised before creating the motion state
    m_motion = new MotionState(this);

    m_rigidBody = new PlayerRigidBody(this, m_motion);
    GPhysicEngine.AddRigidBody(m_rigidBody);
}

Player::~Player()
{
    if(m_motion)
        delete m_motion;

    if(m_rigidBody)
        delete m_rigidBody;
}


void Player::SetName(const std::string& name)
{
    m_name = name;
}

const std::string& Player::GetName() const
{
    return m_name;
}

void Player::SetSize(const Vector3f& size)
{
    m_size = size;
}

const Vector3f& Player::GetSize() const
{
    return m_size;
}

void Player::SetWeight(float weight)
{
    m_weight = weight;
}

float Player::GetWeight() const
{
    return m_weight;
}

void Player::SetEyeHeight(float eyeHeight)
{
    m_eyeHeight = eyeHeight;
}

float Player::GetEyeHeight() const
{
    return m_eyeHeight;
}

void Player::Move(bool front, bool back, bool left, bool right, bool run, bool fly, float elapsedTime)
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

    btTransform fromTrans;
    m_motion->getWorldTransform(fromTrans);
    btTransform toTrans(fromTrans);
    toTrans.setOrigin(btVector3(m_position.x + delta.x, m_position.y + delta.y, m_position.z + delta.z));

    CollisionList collisions;
    GPhysicEngine.GetCollisionWorld()->convexSweepTest(m_rigidBody->GetCollisionShape(), fromTrans, toTrans, collisions);

    if(collisions.Count() != 0)
    {
        std::cout << "Collision!" << std::endl;
        for(CollisionList::ConstIterator it = collisions.Begin(); it != collisions.End(); ++it)
        {
            //std::cout << (*it).hitPoint << std::endl;
            Vector3f relCol = (*it).hitPoint - m_position;

            std::cout << "relCol: " << relCol << std::endl;
            std::cout << "delta avant: " << delta << std::endl;
            delta.x = std::min(delta.x, relCol.x);
            delta.y = std::min(delta.y, relCol.y);
            delta.z = std::min(delta.z, relCol.z);
            std::cout << "delta apres: " << delta << std::endl;
        }

        // TODO wrong.. freeze the player instead of allowing it to slide
        // along walls
        delta = Vector3f(0, 0, 0);
    }

    m_position += delta;
}

void Player::MoveForward(float distance)
{
}

void Player::MoveBackward(float distance)
{
}

void Player::MoveLeft(float distance)
{
}

void Player::MoveRight(float distance)
{
}

void Player::SetPosition(const Vector3f& position)
{
    m_position = position;
}

Vector3f Player::GetPosition() const
{
    return m_position;
}

Vector3f Player::GetEyePosition() const
{
    Vector3f eyePos = GetPosition();
    eyePos.y += GetEyeHeight() - (m_size.y / 2.f);

    return eyePos;
}

void Player::Rotate(const Vector3f& deltaRotation)
{
    m_rotation += deltaRotation;
    ClampRotation();
}

void Player::SetRotation(const Vector3f& rotation)
{
    m_rotation = rotation;
    ClampRotation();
}

const Vector3f& Player::GetRotation() const
{
    return m_rotation;
}

Vector3f Player::GetEyeVector() const
{
    float rx = DEGTORAD(m_rotation.x);
    float ry = DEGTORAD(m_rotation.y);
    Vector3f result(sin(ry) * cos(rx), -sin(rx), -cos(ry) * cos(rx));
    result.Normalize();
    return result;
}

void Player::ClampRotation()
{
    if(m_rotation.x >= 85.f)
        m_rotation.x = 85.f;

    if(m_rotation.x <= -85.f)
        m_rotation.x = -85.f;

}
