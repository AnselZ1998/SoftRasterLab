#pragma once

#include "Scene.h"
#include <string>

class SceneManager
{
public:
	SceneManager() = default;
	~SceneManager() = default;

	bool StartUp();
	void ShutDown();
	Scene* GetCurrentScene();

private:
	bool LoadScene(std::string sceneID);

	std::string currentSceneID;
	bool mIsStarted = false;
	Scene* mScene;
};

