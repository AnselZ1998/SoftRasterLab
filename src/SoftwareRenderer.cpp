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
    
    if (!mRasterizer.StartUp(&mColorBuffer, &mDepthBuffer))
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

const Buffer<std::uint32_t>* SoftwareRenderer::GetColorBuffer() const
{
    assert(mIsStarted);

    return &mColorBuffer;
}

void SoftwareRenderer::Render()
{
    BeginFrame(0x00FF00FF);
    int tmpw = mDepthBuffer.GetWidth();
    int tmph = mDepthBuffer.GetHeight();

    std::uint32_t red = (0xFF << 24) | (255 << 16) | (0 << 8) | 0;
    std::uint32_t green = (0xFF << 24) | (0 << 16) | (255 << 8) | 0;


    // ==========================================
    // 三角形 A (红色) - 整体设定在较远位置 (Depth = 0.7)
    // ==========================================
    Vector2 a_p0(150.0f, 120.0f);
    Vector2 a_p1(100.0f, 50.0f);
    Vector2 a_p2(200.0f, 50.0f);
    float a_d0 = 0.7f, a_d1 = 0.7f, a_d2 = 0.7f;

    // ==========================================
    // 三角形 B (绿色) - 整体设定在较近位置 (Depth = 0.3)
    // 它与三角形 A 在右侧有重叠区域
    // ==========================================
    Vector2 b_p0(200.0f, 120.0f);
    Vector2 b_p1(150.0f, 50.0f);
    Vector2 b_p2(250.0f, 50.0f);
    float b_d0 = 0.3f, b_d1 = 0.3f, b_d2 = 0.3f;


    mRasterizer.DrawTriangle(a_p0, a_d0, a_p1, a_d1, a_p2, a_d2, red);
    mRasterizer.DrawTriangle(b_p0, b_d0, b_p1, b_d1, b_p2, b_d2, green);

}

