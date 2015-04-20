#ifndef TOPDOWNCAMERA_H__
#define TOPDOWNCAMERA_H__

#include "camera.h"
#include "vector3.h"

class TopDownCamera : public Camera
{
    public:
        TopDownCamera(const Vector3f& xzPosition, float distance);
        virtual ~TopDownCamera();

        virtual void Update(float elapsedTime);

        virtual void MouseMoveEvent(int x, int y);
        virtual void MousePressEvent(const MOUSE_BUTTON &button, int x, int y);
        virtual void MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y);

        float GetDistance() const;
        void SetDistance(float distance);

        const Vector3f& GetPosition() const;
        void SetPosition(const Vector3f& position);

    private:
        Vector3f m_position;
        float m_distance;

        bool m_move;
        int m_lastMousePosX;
        int m_lastMousePosY;
};

#endif // TOPDOWNCAMERA_H__
