#ifndef TAK__GAME_H__
#define TAK__GAME_H__

#include "engine.h"
#include <string>

class Game
{
    public:
        typedef Engine::MOUSE_BUTTON MOUSE_BUTTON;

    public:
        Game(const std::string& name, const std::string& version);
        virtual ~Game();

        virtual bool LoadResource() = 0;
        virtual bool UnloadResource() = 0;
        virtual void Render3d(float elapsedTime) = 0;
        virtual void Render2d(float elapsedTime) = 0;
        virtual void RenderText(float elapsedTime) = 0;
        virtual void KeyPressEvent(unsigned char key) = 0;
        virtual void KeyReleaseEvent(unsigned char key) = 0;
        virtual void MouseMoveEvent(int x, int y) = 0;
        virtual void MousePressEvent(const MOUSE_BUTTON &button, int x, int y) = 0;
        virtual void MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y) = 0;
        virtual void WindowFocusEvent(bool hasFocus) = 0;
        virtual void WindowResizeEvent(int width, int height) = 0;

        const std::string& GetName() const;
        const std::string& GetVersion() const;

    public:
        struct ConfigInfo
        {
            uint InitialWidth;
            uint InitialHeight;
            uint AntialiasingFactor;

            bool ShowFps;
            bool ShowVersion;

            Vector3f ClearColor;

            ConfigInfo()
            {
                InitialWidth = 640;
                InitialHeight = 480;

                AntialiasingFactor = 0;

                ShowFps = true;
                ShowVersion = true;

                ClearColor = Vector3f(0, 0, 0);
            };
        } Config;

    protected:
        void AddSyncValue(SyncValue* sv);
        ParticleManager* GetParticleManager();

        void PrintText(int x, int y, const std::string& text);

        Scene& GetScene() const;

    private:
        SyncValueManager* m_syncValueManager;
        ParticleManager* m_particleManager;
        std::string m_name;
        std::string m_version;

        Engine* m_engine;
        Scene* m_scene;

        friend class Engine;
};

#endif // GAME_H__
