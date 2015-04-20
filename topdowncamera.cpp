#include "topdowncamera.h"

TopDownCamera::TopDownCamera(const Vector3f& xzPosition, float distance) : m_move(false), m_lastMousePosX(0), m_lastMousePosY(0)
{
    SetPosition(xzPosition);
    SetDistance(distance);
}

TopDownCamera::~TopDownCamera()
{
}

void TopDownCamera::Update(float elapsedTime)
{
    static float angle = 0;
    angle += elapsedTime * 20;

    m_view.SetIdentity();
    m_view.SetLookAt(Vector3f(m_position.x, m_distance, m_position.z), m_position, Vector3f(0, 0, -1));
}

void TopDownCamera::MouseMoveEvent(int x, int y)
{
    if(m_move)
    {
        m_position.x -= (x - m_lastMousePosX) * (0.0025f * m_distance);
        m_position.z -= (y - m_lastMousePosY) * (0.0025f * m_distance);
    }

    m_lastMousePosX = x;
    m_lastMousePosY = y;
}

void TopDownCamera::MousePressEvent(const MOUSE_BUTTON &button, int x, int y)
{
    switch(button)
    {
        case MOUSE_BUTTON_RIGHT:
            m_lastMousePosX = x;
            m_lastMousePosY = y;
            m_move = true;
            break;
        case MOUSE_BUTTON_WHEEL_UP:
            SetDistance(m_distance - (m_distance / 5.f));
            break;
        case MOUSE_BUTTON_WHEEL_DOWN:
            SetDistance(m_distance + (m_distance / 5.f));
            break;
    }
}

void TopDownCamera::MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y)
{
    switch(button)
    {
        case MOUSE_BUTTON_RIGHT:
            m_move = false;
            break;
    }
}

float TopDownCamera::GetDistance() const
{
    return m_distance;
}

void TopDownCamera::SetDistance(float distance)
{
    m_distance = distance;

    if(m_distance < EPSILON)
        m_distance = EPSILON;
}

const Vector3f& TopDownCamera::GetPosition() const
{
    return m_position;
}

void TopDownCamera::SetPosition(const Vector3f& position)
{
    m_position.x = position.x;
    m_position.z = position.z;
}

