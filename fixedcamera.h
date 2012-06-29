#ifndef TAK_FIXEDCAMERA_H__
#define TAK_FIXEDCAMERA_H__

#include "camera.h"

class FixedCamera : public Camera
{
    public:
        FixedCamera(const Vector3f& lookAtPosition, const Vector3f& distance);
        virtual ~FixedCamera();

        virtual void Update(float elapsedTime);
};

#endif // TAK_FIXEDCAMERA_H__
