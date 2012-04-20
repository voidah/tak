#ifndef ENGINE_H__
#define ENGINE_H__
#include "define.h"
#include "tool.h"
#include "openglcontext.h"
#include "shader.h"
//#include "texturebank.h"
#include "array2d.h"
#include "syncvaluemanager.h"

// TODO
#include "texture.h"



class Engine : public OpenglContext
{
    public:
        Engine();
        virtual ~Engine();
        virtual void Init();
        virtual void DeInit();
        virtual void LoadResource();
        virtual void UnloadResource();
        virtual void Render(float elapsedTime);
        virtual void KeyPressEvent(unsigned char key);
        virtual void KeyReleaseEvent(unsigned char key);
        virtual void MouseMoveEvent(int x, int y);
        virtual void MousePressEvent(const MOUSE_BUTTON &button, int x, int y);
        virtual void MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y);
        virtual void WindowsFocusEvent(bool hasFocus);

    private:
        bool LoadTexture(Texture& texture, const std::string& filename, bool stopOnError = true);

        void PrintText(unsigned int x, unsigned int y, const std::string& t);
        void DrawHud();



    private:
        bool m_wireframe;
        bool m_takeScreenshot;

        Texture m_textureFont;
        Shader m_shader01;

        SyncValueManager m_syncValueManager;
};

#endif // ENGINE_H__