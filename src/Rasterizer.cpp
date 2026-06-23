#include "Rasterizer.h"

bool Rasterizer::StartUp(Buffer<std::uint32_t>& InColorBuffer, Buffer<float>& InDepthBuffer)
{
	if (mIsStarted)
	{
		return true;
	}

	if (
		InColorBuffer.GetHeight() != InDepthBuffer.GetHeight() ||
		InColorBuffer.GetWidth() != InDepthBuffer.GetWidth() ||
		InColorBuffer.GetPixelCount() != InDepthBuffer.GetPixelCount()
		)
	{
		return false;
	}

	// save point
	mColorBuffer = &InColorBuffer;
	mDepthBuffer = &InDepthBuffer;

	mIsStarted = true;
	return true;
}

void Rasterizer::ShutDown()
{
	if (!mIsStarted)
	{
		return;
	}

	mColorBuffer = nullptr;
	mDepthBuffer = nullptr;
	mIsStarted = false;

}

void Rasterizer::DrawPixel(int x, int y, float depth, std::uint32_t color)
{
	if (!mIsStarted)
	{
		return;
	}

	if (x < 0 ||
		y < 0 ||
		x >= mColorBuffer->GetWidth() ||
		y >= mColorBuffer->GetHeight())
	{
		return;
	}

	// depth
	float oldDepth = (*mDepthBuffer)(x, y);
	if (depth <= oldDepth)
	{
		return;
	}

	(*mDepthBuffer)(x, y) = depth;
	(*mColorBuffer)(x, y) = color;
}
