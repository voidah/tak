#ifndef TAK_LOOKATCAMERA_H__
#define TAK_LOOKATCAMERA_H__

#include "camera.h"
#include "vector3.h"

class LookAtCamera : public Camera
{
    public:
        LookAtCamera(const Vector3f& lookAtPosition, float distance, float initialRotationX = -45.f, float initialRotationY = 0);
        virtual ~LookAtCamera();

        virtual void Update(float elapsedTime);

        virtual void MouseMoveEvent(int x, int y);
        virtual void MousePressEvent(const MOUSE_BUTTON &button, int x, int y);
        virtual void MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y);

        float GetRotationX() const;
        float GetRotationY() const;

    private:
        Vector3f m_lookAt;
        float m_distance;
        float m_rotationX;
        float m_rotationY;

        bool m_rotate;
        bool m_zoom;
        int m_lastMousePosX;
        int m_lastMousePosY;
};

#endif // TAK_LOOKATCAMERA_H__
