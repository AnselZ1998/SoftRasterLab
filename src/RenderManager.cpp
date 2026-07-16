#include "RenderManager.h"

bool RenderManager::StartUp(DisplayManager& inDisplayManager, SceneManager& inSceneManager)
{
	mDisplayManager = &inDisplayManager;
	mSceneManager = &inSceneManager;

	if (!initSoftwareRenderer())
	{
		return false;
	}
	mIsStarted = true;
	return true;
}

void RenderManager::ShutDown()
{
	mSoftwareRenderer.ShutDown();
	mSceneManager = nullptr;
	mDisplayManager = nullptr;
	mScene = nullptr;
	mIsStarted = false;
}

void RenderManager::render()
{
	if (!mIsStarted || mDisplayManager == nullptr)
	{
		return;
	}

	Scene* mScene = mSceneManager->GetCurrentScene();

	// Clear Buffer(Color and Depth) in Buffer.h of SoftwareRender.h
	mSoftwareRenderer.BeginFrame();
	
	mSoftwareRenderer.SetCameraToRender(mScene->getCurrentCamera());
	// renderInstance
	mSoftwareRenderer.drawTriangularMesh();

	// Draw base color Buffer
	mDisplayManager->Present(mSoftwareRenderer.GetColorBuffer());
}

bool RenderManager::initSoftwareRenderer()
{
	return mSoftwareRenderer.StartUp(mDisplayManager->GetWidth(), mDisplayManager->GetHeight());
}
