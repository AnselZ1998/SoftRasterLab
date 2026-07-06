#pragma once

#include "DisplayManager.h"
#include "SoftwareRenderer.h"

class RenderManager
{
public:
	RenderManager() = default;
	~RenderManager() = default;

	bool StartUp(DisplayManager& inDisplayManager);
	void ShutDown();

	void render();

private:
	bool initSoftwareRenderer();
	bool mIsStarted = false;

	DisplayManager* mDisplayManager = nullptr;
	SoftwareRenderer mSoftwareRenderer;
};

