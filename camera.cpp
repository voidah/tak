#include "camera.h"

Camera::Camera() : m_view(Matrix4f::IDENTITY)
{
}

Camera::~Camera()
{
}

const Matrix4f& Camera::GetMatrix() const
{
    return m_view;
}

void Camera::KeyPressEvent(unsigned char key)
{
    // Default camera behavior, do nothing
}

void Camera::KeyReleaseEvent(unsigned char key)
{
    // Default camera behavior, do nothing
}

void Camera::MouseMoveEvent(int x, int y)
{
    // Default camera behavior, do nothing
}

void Camera::MousePressEvent(const MOUSE_BUTTON &button, int x, int y)
{
    // Default camera behavior, do nothing
}

void Camera::MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y)
{
    // Default camera behavior, do nothing
}

void Camera::WindowFocusEvent(bool hasFocus)
{
    // Default camera behavior, do nothing
}

void Camera::WindowResizeEvent(int width, int height)
{
    // Default camera behavior, do nothing
}

