#include "RenderManager.h"

bool RenderManager::StartUp(DisplayManager& inDisplayManager)
{
	mDisplayManager = &inDisplayManager;

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
	mDisplayManager = nullptr;
}

void RenderManager::render()
{
	if (!mIsStarted || mDisplayManager == nullptr)
	{
		return;
	}

	// Clear Buffer(Color and Depth) in Buffer.h of SoftwareRender.h
	mSoftwareRenderer.BeginFrame();

	// renderInstance
	mSoftwareRenderer.drawTriangularMesh();

	// Draw base color Buffer
	mDisplayManager->Present(mSoftwareRenderer.GetColorBuffer());
}

bool RenderManager::initSoftwareRenderer()
{
	return mSoftwareRenderer.StartUp(mDisplayManager->GetWidth(), mDisplayManager->GetHeight());
}
