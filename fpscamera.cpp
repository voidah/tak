#include "fpscamera.h"

FpsCamera::FpsCamera(Player* player) : m_player(player)
{
    UpdateViewMatrix();
}

FpsCamera::~FpsCamera()
{
}

void FpsCamera::Update(float elapsedTime)
{
    UpdateViewMatrix();
}

void FpsCamera::UpdateViewMatrix()
{
    if(m_player)
    {
        Vector3f eyePos = m_player->GetEyePosition();
        m_view.SetLookAt(eyePos, eyePos + m_player->GetEyeVector());
    }
}

