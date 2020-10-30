#include "player.h"

// TODO
#define MOVEMENT_DIVIDER 2.000f

Player::Player(const std::string& name, const Vector3f& size, float weightKg, float eyeHeight) : m_name(name), m_size(size), m_weight(weightKg), m_eyeHeight(eyeHeight), m_position(0, 0, 0), m_rotation(0, 0, 0)
{
}

Player::~Player()
{
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

Vector3f Player::SimulateMove(bool front, bool back, bool left, bool right, bool run, bool fly, float elapsedTime)
{
    Vector3f delta(0, 0, 0);

    float divider = MOVEMENT_DIVIDER;
    if(run)
        divider *= 4;
    if(fly)
        divider *= 20;

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


    return delta;
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
