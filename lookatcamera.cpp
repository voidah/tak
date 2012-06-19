#include "lookatcamera.h"

LookAtCamera::LookAtCamera(const Vector3f& lookAtPosition, float distance, float initialRotationX, float initialRotationY) : m_lookAt(lookAtPosition), m_distance(distance), m_rotationX(initialRotationX), m_rotationY(initialRotationY), m_rotate(false), m_zoom(false), m_lastMousePosX(0), m_lastMousePosY(0)
{
}

LookAtCamera::~LookAtCamera()
{
}

void LookAtCamera::Update(float elapsedTime)
{
    m_view.SetIdentity();
    m_view.ApplyTranslation(m_lookAt.x, m_lookAt.y, m_lookAt.z -m_distance);
    m_view.ApplyRotation(m_rotationX, 1, 0, 0);
    m_view.ApplyRotation(m_rotationY, 0, 1, 0);
}

void LookAtCamera::MouseMoveEvent(int x, int y)
{
    if(m_rotate)
    {
        m_rotationX += (m_lastMousePosY - y) * 0.5f;
        m_rotationY += (m_lastMousePosX - x) * 0.5f;
    }
    else if(m_zoom)
    {
        m_distance += (y - m_lastMousePosY) * 0.15f;
    }

    m_lastMousePosX = x;
    m_lastMousePosY = y;
}

void LookAtCamera::MousePressEvent(const MOUSE_BUTTON &button, int x, int y)
{
    switch(button)
    {
        case MOUSE_BUTTON_LEFT:
            m_lastMousePosX = x;
            m_lastMousePosY = y;
            m_rotate = true;
            break;

        case MOUSE_BUTTON_RIGHT:
            std::cout << "move" << std::endl;
            m_lastMousePosX = x;
            m_lastMousePosY = y;
            m_zoom = true;
            break;
    }
}

void LookAtCamera::MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y)
{
    switch(button)
    {
        case MOUSE_BUTTON_LEFT:
            m_rotate = false;
            break;

        case MOUSE_BUTTON_RIGHT:
            m_zoom = false;
            break;
    }
}

