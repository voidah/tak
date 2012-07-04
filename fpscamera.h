#ifndef TAK_FPSCAMERA_H__
#define TAK_FPSCAMERA_H__

#include "camera.h"
#include "player.h"

class FpsCamera : public Camera
{
    public:
        FpsCamera(Player* player);
        virtual ~FpsCamera();

        virtual void Update(float elapsedTime);
        
    private:
        void UpdateViewMatrix();

    private:
        Player* m_player;
};

#endif // TAK_FPSCAMERA_H__
