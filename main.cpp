#include <iostream>
#include "engine.h"
#include "tool.h"

int main()
{
    Tool::CheckTypes();

    Engine engine;
    engine.SetMaxFps(99999);
    engine.Start(PROJECT_NAME, 640, 480, false, 0);

    return 0;
}
