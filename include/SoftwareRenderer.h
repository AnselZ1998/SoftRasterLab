#pragma once

#include <cstdint>
#include "Buffer.h"
#include "Rasterizer.h"

class SoftwareRenderer
{
public:
	
	SoftwareRenderer() = default;
	~SoftwareRenderer() = default;

	bool StartUp(int width, int height);
	void ShutDown();
	void BeginFrame(std::uint32_t clearColor);

	const Buffer<std::uint32_t>& GetColorBuffer() const;

	void tempRenderTest();

private:
	Buffer<std::uint32_t> mColorBuffer;
	Buffer<float> mDepthBuffer;

	Rasterizer mRasterizer;

	bool mIsStarted = false;
};