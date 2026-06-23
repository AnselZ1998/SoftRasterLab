#include "Engine.h"
#include "Buffer.h"

bool Engine::StartUp(HINSTANCE hInstance)
{
    if (mIsStarted)
    {
        return true;
    }

    if (!mDisplayManager.StartUp(
        hInstance,
        720,
        480
    ))
    {
        return false;
    }

    if (!mSoftwareRenderer.StartUp(
        mDisplayManager.GetWidth(),
        mDisplayManager.GetHeight()
    ))
    {
        mDisplayManager.ShutDown();
        return false;
    }

    mIsStarted = true;
    return true;
}

void Engine::Run()
{
    if (!mIsStarted)
    {
        return;
    }

    MSG message{};
    bool shouldQuit = false;

    while (!shouldQuit)
    {
        while (PeekMessage(
            &message,
            nullptr,
            0,
            0,
            PM_REMOVE
        ))
        {
            if (message.message == WM_QUIT)
            {
                shouldQuit = true;
                break;
            }

            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        if (shouldQuit)
        {
            break;
        }

        mSoftwareRenderer.BeginFrame(0x00FF00FF);
        mSoftwareRenderer.tempRenderTest();
        mDisplayManager.Present(mSoftwareRenderer.GetColorBuffer());

        Sleep(1);
    }
    
}

void Engine::ShutDown()
{
    mSoftwareRenderer.ShutDown();
    mDisplayManager.ShutDown();
    mIsStarted = false;
}