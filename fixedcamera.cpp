#include "fixedcamera.h"

FixedCamera::FixedCamera(const Vector3f& lookAtPosition, const Vector3f& distance)
{
    m_view.SetLookAt(lookAtPosition + distance, lookAtPosition);
}

FixedCamera::~FixedCamera()
{
}

void FixedCamera::Update(float elapsedTime)
{
}
