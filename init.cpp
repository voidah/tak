#include "init.h"
#include "game.h"
#include "tool.h"
#include "engine.h"

bool RunGame(Game* game)
{
    Tool::CheckTypes();

    Engine engine(game);
    engine.SetMaxFps(99999);
    engine.Start(game->GetName(), 640, 480, false, 0);

    return true;
}
