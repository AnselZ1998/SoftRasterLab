#pragma once

#include "DisplayManager.h"
#include "SoftwareRenderer.h"
#include "SceneManager.h"

class RenderManager
{
public:
	RenderManager() = default;
	~RenderManager() = default;

	bool StartUp(DisplayManager& inDisplayManager, SceneManager& inSceneManager);
	void ShutDown();

	void render();

private:
	bool initSoftwareRenderer();
	bool mIsStarted = false;

	DisplayManager* mDisplayManager = nullptr;
	SceneManager* mSceneManager = nullptr;
	SoftwareRenderer mSoftwareRenderer;
	Scene* mScene = nullptr;

};

