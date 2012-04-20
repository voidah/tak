#include "game.h"
#include <cassert>

Game::Game(const std::string& name, const std::string& version) : m_syncValueManager(0), m_name(name), m_version(version)
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

ParticleManager* Game::GetParticleManager()
{
    return m_particleManager;
}

const std::string& Game::GetName() const
{
    return m_name;
}

const std::string& Game::GetVersion() const
{
    return m_version;
}
