#pragma once

#include <cstdint>
#include "Buffer.h"

class Rasterizer
{
public:
	Rasterizer() = default;
	~Rasterizer() = default;

	bool StartUp(Buffer<std::uint32_t>& InColorBuffer, Buffer<float>& InDepthBuffer);
	void ShutDown();

	void DrawPixel(int x, int y, float depth, std::uint32_t);

private:
	Buffer<std::uint32_t>* mColorBuffer = nullptr;
	Buffer<float>* mDepthBuffer = nullptr;

	bool mIsStarted = false;
};