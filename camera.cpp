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
