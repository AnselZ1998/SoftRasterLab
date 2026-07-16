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
    ) || !mSceneManager.StartUp())
    {
        return false;
    }

    if (!mRenderManager.StartUp(mDisplayManager, mSceneManager))
    {
        mDisplayManager.ShutDown();
        mSceneManager.ShutDown();
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

        mRenderManager.render();

        Sleep(1);
    }
    
}

void Engine::ShutDown()
{
    mRenderManager.ShutDown();
    mSceneManager.ShutDown();
    mDisplayManager.ShutDown();
    mIsStarted = false;
}