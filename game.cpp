#include "game.h"
#include <cassert>

Game::Game() : m_syncValueManager(0)
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
