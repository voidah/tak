#include "game.h"
#include <cassert>

Game::Game(const std::string& name, const std::string& version) : m_syncValueManager(0), m_name(name), m_version(version), m_engine(0), m_scene(0)
{
}

Game::~Game()
{
}

void Game::AddSyncValue(SyncValue* sv)
{
    assert(m_syncValueManager);
    m_syncValueManager->Add(sv);
}

const std::string& Game::GetName() const
{
    return m_name;
}

const std::string& Game::GetVersion() const
{
    return m_version;
}

int Game::GetFps() const
{
    assert(m_engine);
    return m_engine->GetFps();
}

Scene& Game::GetScene() const
{
    return *m_scene;
}
