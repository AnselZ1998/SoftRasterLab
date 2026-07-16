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
    swWidth = width;
    swHeight = height;

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
    mCamera = nullptr;
}

void SoftwareRenderer::BeginFrame(std::uint32_t clearColor)
{
    if (!mIsStarted)
    {
        return;
    }
    mColorBuffer.Clear(clearColor);
    mDepthBuffer.Clear(1.0f);
}

void SoftwareRenderer::SetCameraToRender(Camera* camera)
{
    mCamera = camera;
}

const Buffer<std::uint32_t>* SoftwareRenderer::GetColorBuffer() const
{
    assert(mIsStarted);
    return &mColorBuffer;
}

void SoftwareRenderer::drawTriangularMesh()
{

    std::uint32_t red = (0xFF << 24) | (255 << 16) | (0 << 8) | 0;
    std::uint32_t green = (0xFF << 24) | (0 << 16) | (255 << 8) | 0;

    Vector3 p0(-0.5f, -0.5f, 0.0f);
    Vector3 p1(0.5f, -0.5f, 0.0f);
    Vector3 p2(0.0f, 0.5f, 0.0f);

    float b_d0 = 0.3f, b_d1 = 0.3f, b_d2 = 0.3f;
    
    static float time = 0.0f;
    time += 0.02f;

  // 1.Build Matrix mvp
    
    // Model (local -> world)
    Matrix4 transform = Matrix4::Translation(0.0, 0.0, 0.0);
    Matrix4 scale = Matrix4::Scale(1.0, 1.0, 1.0);
    Matrix4 rotation = Matrix4::RotationFromEuler(time * 0.5f, time, time * 0.25f);
    Matrix4 model = transform * rotation * scale;

    // View (world -> view)
    Matrix4 view = mCamera->viewMatrix;
    Matrix4 projection = mCamera->projectionMatrix;

    // Projection (View -> Clip)
    Matrix4 mvp = projection * view * model;

    // Clip -> NDC -> Screen
    ScreenVertex screenP0 = TransformToScreen(p0, mvp);
    ScreenVertex screenP1 = TransformToScreen(p1, mvp);
    ScreenVertex screenP2 = TransformToScreen(p2, mvp);

    mRasterizer.DrawTriangle(screenP0.position, screenP0.depth, screenP1.position, screenP1.depth, screenP2.position, screenP2.depth, red);

}

ScreenVertex SoftwareRenderer::TransformToScreen(const Vector3& localPos, const Matrix4& mvp)
{

    Vector4 clipPos = mvp * Vector4::FromPoint(localPos);
    Vector3 ndc = clipPos.PerspectiveDivide();
    float screenX = (ndc.x + 1.0f) * 0.5f * swWidth;
    float screenY = (1.0f - ndc.y) * 0.5f * swHeight;

    ScreenVertex out;
    out.position = Vector2(screenX, screenY);
    out.depth = ndc.z;

    return out;
}

