#include "SoftwareRenderer.h"
#include <cstdlib>

bool SoftwareRenderer::StartUp(int width, int height)
{
    if (mIsStarted)
    {
        return true;
    }

    mColorBuffer.Resize(width, height);
    mDepthBuffer.Resize(width, height);

    if (!mColorBuffer.IsValid() || !mDepthBuffer.IsValid())
    {
        return false;
    }
    
    if (!mRasterizer.StartUp(mColorBuffer, mDepthBuffer))
    {
        return false;
    }

    mIsStarted = true;
    return true;
}

void SoftwareRenderer::ShutDown()
{
    if (mIsStarted)
    {
        mIsStarted = false;
    }

    mRasterizer.ShutDown();
}

void SoftwareRenderer::BeginFrame(std::uint32_t clearColor)
{
    if (!mIsStarted)
    {
        return;
    }
    mColorBuffer.Clear(clearColor);
    mDepthBuffer.Clear(0.0f);
}

const Buffer<std::uint32_t>& SoftwareRenderer::GetColorBuffer() const
{
    assert(mIsStarted);

    return mColorBuffer;
}

void SoftwareRenderer::tempRenderTest()
{
    int tmpw = mDepthBuffer.GetWidth();
    int tmph = mDepthBuffer.GetHeight();

    for (int i = 0; i < tmph; i++) // i 对应 y (行)
    {
        for (int j = 0; j < tmpw; j++) // j 对应 x (列)
        {
            // 1. 生成 0~255 的随机 RGB 值
            std::uint32_t r = std::rand() % 256;
            std::uint32_t g = std::rand() % 256;
            std::uint32_t b = std::rand() % 256;

            // 2. 拼装成 32位 ARGB 颜色 (Alpha 固定为 0xFF 即完全不透明)
            std::uint32_t randomColor = (0xFF << 24) | (r << 16) | (g << 8) | b;

            // 3. 绘制像素！注意传入的坐标是 (x, y) 也就是 (j, i)
            mRasterizer.DrawPixel(j, i, 1.0f, randomColor);
        }
    }
}