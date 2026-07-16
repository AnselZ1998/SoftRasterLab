#include "SceneManager.h"

bool SceneManager::StartUp()
{
    currentSceneID = "defaultScene";
    if (!LoadScene(currentSceneID))
    {
        return false;
    }
    mIsStarted = true;
    return true;
}

void SceneManager::ShutDown()
{
    delete mScene;
}

Scene* SceneManager::GetCurrentScene()
{
    return mScene;
}

bool SceneManager::LoadScene(std::string sceneID)
{
    mScene = new Scene(sceneID);
    return mScene->CheckIfEmpty();
}
