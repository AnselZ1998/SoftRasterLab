#include "Rasterizer.h"

#include<cmath>     
#include<algorithm>  
#include<cstdlib>    

bool Rasterizer::StartUp(Buffer<std::uint32_t>* InColorBuffer, Buffer<float>* InDepthBuffer)
{
	if (mIsStarted)
	{
		return true;
	}

	if (
		InColorBuffer -> GetHeight() != InDepthBuffer -> GetHeight() ||
		InColorBuffer -> GetWidth() != InDepthBuffer -> GetWidth() ||
		InColorBuffer -> GetPixelCount() != InDepthBuffer -> GetPixelCount()
		)
	{
		return false;
	}

	// save point
	mColorBuffer = InColorBuffer;
	mDepthBuffer = InDepthBuffer;

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

	if (mColorBuffer == nullptr || mDepthBuffer == nullptr)
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

	// depth small -> far / big -> near
	float oldDepth = (*mDepthBuffer)(x, y);
	if (depth <= oldDepth)
	{
		return;
	}

	(*mDepthBuffer)(x, y) = depth;
	(*mColorBuffer)(x, y) = color;
}

void Rasterizer::DrawLine(const Vector2& p0, const Vector2& p1, const std::uint32_t color)
{
	int x0 = static_cast<int>(std::round(p0.x));
	int y0 = static_cast<int>(std::round(p0.y));
	int x1 = static_cast<int>(std::round(p1.x));
	int y1 = static_cast<int>(std::round(p1.y));

	bool steep = false;

	if (std::abs(x1 - x0) < std::abs(y1 - y0))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	const int dx = x1 - x0;
	const int rawDy = y1 - y0;
	const int absDy = std::abs(rawDy);

	const int yStep = (rawDy >= 0) ? 1 : -1;
	const int dError2 = absDy * 2;

	int error2 = -dx;
	int y = y0;

	for (int x = x0; x <= x1; ++x)
	{
		if (steep)
		{
			DrawPixel(y, x, 1.0f, color);
		}
		else
		{
			DrawPixel(x, y, 1.0f, color);
		}

		error2 += dError2;

		if (error2 > 0)
		{
			y += yStep;
			error2 -= 2 * dx;
		}
	}
}

void Rasterizer::DrawWireFrame(const Vector2& p0, const Vector2& p1, const Vector2& p2, const std::uint32_t color)
{

	DrawLine(p0, p1, color);
	DrawLine(p1, p2, color);
	DrawLine(p2, p0, color);
}

void Rasterizer::DrawTriangle(
	const Vector2& p0, 
	const float depth0, 
	const Vector2& p1, 
	const float depth1, 
	const Vector2& p2, 
	const float depth2, 
	const std::uint32_t color)
{
	if (!mIsStarted)
	{
		return;
	}

	if (mColorBuffer == nullptr || mDepthBuffer == nullptr)
	{
		return;
	}

	Bounds2 trianglesBounds = GetTriangleBounds2(p0, p1, p2);

	int minX = static_cast<int>(std::ceil(trianglesBounds.minPoint.x));
	int minY = static_cast<int>(std::ceil(trianglesBounds.minPoint.y));
	int maxX = static_cast<int>(std::floor(trianglesBounds.maxPoint.x));
	int maxY = static_cast<int>(std::floor(trianglesBounds.maxPoint.y));

	float A1 = -(p1.y - p0.y), B1 = (p1.x - p0.x); 
	float A2 = -(p2.y - p1.y), B2 = (p2.x - p1.x);
	float A3 = -(p0.y - p2.y), B3 = (p0.x - p2.x);

	Vector2 startP(static_cast<float>(minX) + 0.5f, static_cast<float>(minY) + 0.5f);

	float e1_row = edge(startP, p0, p1);
	float e2_row = edge(startP, p1, p2);
	float e3_row = edge(startP, p2, p0);

	float totalArea = edge(p0, p1, p2);

	constexpr float AreaEpsilon = 1e-6f;
	if (std::abs(totalArea) < AreaEpsilon)
	{
		return;
	}

	float invArea = 1 / totalArea;

	float sign = (invArea > 0.0) ? 1.0 : -1.0;

	for (int j = minY; j <= maxY; ++j)
	{
		float v1 = e1_row;
		float v2 = e2_row;
		float v3 = e3_row;

		for (int i = minX; i <= maxX; ++i)
		{

			if (sign * v1 >= 0 && sign * v2 >=0 && sign * v3 >= 0)
			{
				float lambda2 = v1 * invArea;
				float lambda0 = v2 * invArea;
				float lambda1 = v3 * invArea;

				float depth = depth0 * lambda0 + depth1 * lambda1 + depth2 * lambda2;

				DrawPixel(i, j, depth, color);
			}
			v1 += A1;
			v2 += A2;
			v3 += A3;
		}

		e1_row += B1;
		e2_row += B2;
		e3_row += B3;
	}
}

std::uint32_t Rasterizer::GetColor(bool Random)
{
	int random = 1 ? Random : 0;
	
	std::uint32_t r = (std::rand()) * random * 256;
	std::uint32_t g = (std::rand()) * random * 256;
	std::uint32_t b = (std::rand()) * random * 256;
	
	std::uint32_t Color = (0xFF << 24) | (r << 16) | (g << 8) | b;
	return Color;
}

Bounds2 Rasterizer::GetTriangleBounds2(const Vector2& p0, const Vector2& p1, const Vector2& p2)
{

	float xMin = std::min({ p0.x, p1.x, p2.x });
	float yMin = std::min({ p0.y, p1.y, p2.y });
	float xMax = std::max({ p0.x, p1.x, p2.x });
	float yMax = std::max({ p0.y, p1.y, p2.y });

	float widthF = static_cast<float>(mDepthBuffer -> GetWidth() - 1);
	float heightF = static_cast<float>(mDepthBuffer -> GetHeight() - 1);

	xMin = std::max(0.0f, xMin);
	yMin = std::max(0.0f, yMin);
	xMax = std::min(widthF, xMax);
	yMax = std::min(heightF, yMax);

	return Bounds2(Vector2(xMin, yMin), Vector2(xMax, yMax));
}

float Rasterizer::edge(const Vector2& p, const Vector2& p0, const Vector2& p1)
{
	return (p1.x - p0.x) * (p.y - p0.y) - (p1.y - p0.y) * (p.x - p0.x);
}


