#ifndef TAK_CAMERA_H__
#define TAK_CAMERA_H__

#include "matrix4.h"

class Camera
{
    public:
        Camera();
        virtual ~Camera();

        virtual void KeyPressEvent(unsigned char key);
        virtual void KeyReleaseEvent(unsigned char key);
        virtual void MouseMoveEvent(int x, int y);
        virtual void MousePressEvent(const MOUSE_BUTTON &button, int x, int y);
        virtual void MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y);
        virtual void WindowFocusEvent(bool hasFocus);
        virtual void WindowResizeEvent(int width, int height);

        virtual void Update(float elapsedTime) = 0;
        const Matrix4f& GetMatrix() const;

        Vector3f GetWorldPosition() const;

    protected:
        Matrix4f m_view;
};

#endif // TAK_CAMERA_H__
