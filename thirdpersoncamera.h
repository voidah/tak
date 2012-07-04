#ifndef TAK_THIRDPERSONCAMERA_H__
#define TAK_THIRDPERSONCAMERA_H__

#include "camera.h"
#include "player.h"

class ThirdPersonCamera : public Camera
{
    public:
        ThirdPersonCamera(Player* player, float distance);
        virtual ~ThirdPersonCamera();

        virtual void Update(float elapsedTime);

    private:
        void UpdateViewMatrix();

    private:
        Player* m_player;
        float m_distance;
        Vector3f m_lookAt;
        Vector3f m_cameraDistance;
};

#endif // TAK_THIRDPERSONCAMERA_H__
