#pragma once

#include <cstdint>
#include <vector>
#include "Buffer.h"
#include "Vector.h"
#include "Bounds.h"

class Rasterizer
{
public:
	Rasterizer() = default;
	~Rasterizer() = default;

	bool StartUp(Buffer<std::uint32_t>* InColorBuffer, Buffer<float>* InDepthBuffer);
	void ShutDown();

	void DrawPixel(int x, int y, float depth, std::uint32_t);
	// Draw Line use Bresenham
	void DrawLine(const Vector2& p0, const Vector2& p1, const std::uint32_t color);
	void DrawWireFrame(const Vector2& p0, const Vector2& p1, const Vector2& p2, const std::uint32_t color);
	void DrawTriangle(const Vector2& p0, const float depth0, 
					  const Vector2& p1, const float depth1,
				      const Vector2& p2, const float depth2,
					  const std::uint32_t color);

private:
	Buffer<std::uint32_t>* mColorBuffer = nullptr;
	Buffer<float>* mDepthBuffer = nullptr;

	std::uint32_t GetColor(bool Random);
	Bounds2 GetTriangleBounds2(const Vector2& p0, const Vector2& p1, const Vector2& p2);
	float edge(const Vector2& p, const Vector2& p0, const Vector2& p1);

	bool mIsStarted = false;
};