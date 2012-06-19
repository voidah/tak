#ifndef TAK_CAMERA_H__
#define TAK_CAMERA_H__

#include "matrix4.h"

class Camera
{
    public:
        Camera();
        virtual ~Camera();

        virtual void Update(float elapsedTime) = 0;
        const Matrix4f& GetMatrix() const;

    protected:
        Matrix4f m_view;
};

#endif // TAK_CAMERA_H__
