#pragma once

#include <Windows.h>

#include "DisplayManager.h"
#include "RenderManager.h"

class Engine
{
public:
    Engine() = default;
    ~Engine() = default;

    bool StartUp(HINSTANCE hInstance);
    void Run();
    void ShutDown();

private:
    DisplayManager mDisplayManager;
    RenderManager mRenderManager;

    bool mIsStarted = false;

};