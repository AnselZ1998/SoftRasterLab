#pragma once

#include <cstdint>
#include "Buffer.h"
#include "Matrix.h"
#include "Rasterizer.h"


struct ScreenVertex
{
	Vector2 position;
	float depth;
};

class SoftwareRenderer
{
public:
	
	SoftwareRenderer() = default;
	~SoftwareRenderer() = default;

	bool StartUp(int width, int height);
	void ShutDown();
	void BeginFrame(std::uint32_t clearColor = 0x000000);
	const Buffer<std::uint32_t>* GetColorBuffer() const;
	void drawTriangularMesh();
	ScreenVertex TransformToScreen(const Vector3& localPos, const Matrix4& mvp);

private:
	Buffer<std::uint32_t> mColorBuffer;
	Buffer<float> mDepthBuffer;
	Rasterizer mRasterizer;

	int swWidth, swHeight;
	bool mIsStarted = false;

};