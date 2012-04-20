#include "init.h"
#include "game.h"
#include "tool.h"
#include "engine.h"

bool RunGame(Game* game)
{
    Tool::CheckTypes();

    Engine engine(game);
    engine.SetMaxFps(99999);
    engine.Start(game->GetName(), game->Config.InitialWidth, game->Config.InitialHeight, false, 0);

    return true;
}
