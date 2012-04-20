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
        virtual void KeyPressEvent(unsigned char key) = 0;
        virtual void KeyReleaseEvent(unsigned char key) = 0;
        virtual void MouseMoveEvent(int x, int y) = 0;
        virtual void MousePressEvent(const MOUSE_BUTTON &button, int x, int y) = 0;
        virtual void MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y) = 0;
        virtual void WindowFocusEvent(bool hasFocus) = 0;

        const std::string& GetName() const;
        const std::string& GetVersion() const;

    protected:
        void AddSyncValue(SyncValue* sv);

    private:
        SyncValueManager* m_syncValueManager;
        std::string m_name;
        std::string m_version;

        friend class Engine;
};

#endif // GAME_H__
