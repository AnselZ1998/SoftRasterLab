#include <Windows.h>

#include "Engine.h"

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE,
    LPSTR,
    int
)
{
    Engine engine;

    if (!engine.StartUp(hInstance))
    {
        engine.ShutDown();
        return -1;
    }

    engine.Run();
    engine.ShutDown();

    return 0;
}