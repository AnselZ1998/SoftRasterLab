#pragma once

#include <Windows.h>

#include "DisplayManager.h"
#include "SoftwareRenderer.h"

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
    SoftwareRenderer mSoftwareRenderer;

    bool mIsStarted = false;

};