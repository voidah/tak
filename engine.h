#ifndef TAK__ENGINE_H__
#define TAK__ENGINE_H__
#include "define.h"
#include "tool.h"
#include "openglcontext.h"
#include "shader.h"
//#include "texturebank.h"
#include "array2d.h"
#include "syncvaluemanager.h"
#include "particlemanager.h"
#include "scene.h"

// TODO
#include "texture.h"
#include "mesh.h"

class Game;

class Engine : public OpenglContext
{
    public:
        Engine(Game* game);
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
        virtual void WindowFocusEvent(bool hasFocus);
        virtual void WindowResizeEvent(int width, int height);

        void PrintText(int x, int y, const std::string& t);

    private:
        void Render2d(float elapsedTime);



    private:
        Game* m_game;
        bool m_wireframe;
        bool m_takeScreenshot;

        SyncValueManager m_syncValueManager;
        ParticleManager m_particleManager;

        Scene m_scene;
};

#endif // ENGINE_H__
