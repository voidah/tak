#include "thirdpersoncamera.h"

ThirdPersonCamera::ThirdPersonCamera(Player* player, float distance) : m_player(player), m_distance(distance), m_lookAt(0, 0, 0), m_cameraDistance(0, 0, 0)
{
    UpdateViewMatrix();
}

ThirdPersonCamera::~ThirdPersonCamera()
{
}

void ThirdPersonCamera::Update(float elapsedTime)
{
    UpdateViewMatrix();
}

void ThirdPersonCamera::UpdateViewMatrix()
{
    if(m_player)
    {
        Vector3f lookAt = m_player->GetEyePosition();
        Vector3f cameraDistance = -m_player->GetEyeVector() * m_distance;

        if(cameraDistance != m_cameraDistance || lookAt != m_lookAt)
        {
            m_lookAt = lookAt;
            m_cameraDistance = cameraDistance;
            m_view.SetLookAt(m_lookAt + m_cameraDistance, m_lookAt);
        }
    }
}
