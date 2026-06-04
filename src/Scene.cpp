#include "Scene.h"
#include <iostream>

static int gWidth = 0;
static int gHeight = 0;

void InitScene(int inViewportWidth, int inViewportHeight) {
	gWidth = inViewportWidth;
	gHeight = inViewportHeight;
    #ifdef _DEBUG
        TestVector4();
    #endif
}

static void TestVector4() {
    matrix4 m1;
    //std::cout << "m1: " << m1 << std::endl;
}

bool IsInTriangle(int inX, int inY, const vector4& inA, const vector4& inB, const vector4& inC) {
    Vector2 A = { inA.x, inA.y };
    Vector2 B = { inB.x, inB.y };
    Vector2 C = { inC.x, inC.y };
    Vector2 P = { inX, inY };

    Vector2 AB = { B.x - A.x, B.y - A.y };
    Vector2 BC = { C.x - B.x, C.y - B.y };
    Vector2 CA = { A.x - C.x, A.y - C.y };

    Vector2 AP = { P.x - A.x, P.y - A.y };
    Vector2 BP = { P.x - B.x, P.y - B.y };
    Vector2 CP = { P.x - C.x, P.y - C.y };

    int cross1 = AB.x * AP.y - AB.y * AP.x;
    int cross2 = BC.x * BP.y - BC.y * BP.x;
    int cross3 = CA.x * CP.y - CA.y * CP.x;

    bool sameSide =
        (cross1 >= 0 && cross2 >= 0 && cross3 >= 0) ||
        (cross1 <= 0 && cross2 <= 0 && cross3 <= 0);

    return sameSide;
}

void RenderTriangle(const vector4& A, const vector4& B, const vector4& C)
{
    vector4 ASS(A), BSS(B), CSS(C);
    
    // NDC -> Screen Space
    // NDC *= 0.5f -> += 0.5f -> *gCanvas -1

    ASS.x = (ASS.x + 1.0f) * 0.5f * gWidth - 1;
    ASS.y = (ASS.y + 1.0f) * 0.5f * gHeight - 1;

    BSS.x = (BSS.x + 1.0f) * 0.5f * gWidth - 1;
    BSS.y = (BSS.y + 1.0f) * 0.5f * gHeight - 1;

    CSS.x = (CSS.x + 1.0f) * 0.5f * gWidth - 1;
    CSS.y = (CSS.y + 1.0f) * 0.5f * gHeight - 1;

    BoundingVolume BV = BoundingVolume();
    BV.GetBoundingVolume(ASS, BSS, CSS);
    
    int minX_i = floor(BV.minX);
    int minY_i = floor(BV.minY);
    int maxX_i = ceil(BV.maxX);
    int maxY_i = ceil(BV.maxY);

    for (int x = minX_i; x < maxX_i; x++)
    {
        for (int y = minY_i; y < maxY_i; y++)
        {
            if (IsInTriangle(x, y, ASS, BSS, CSS)) {
                SetPixelColor(x, y, 255, 200, 80, 255);
            }

        }
    }
}

void DrawTriangle(const vector4& AWorld, const vector4& BWorld, const vector4& CWorld) {
    // camera
    vector4 cameraPos = { 0.0f, 0.0f, -10.0f, 1.0f };
    matrix4 m4;
    float fov = 45.0f * 3.1415926f / 180.0f;

    matrix4 ViewMatrix = matrix4::Translation(
        -cameraPos.x,
        -cameraPos.y,
        -cameraPos.z
    );

    // 1. World Space -> View Space
    vector4 AView = ViewMatrix * AWorld;
    vector4 BView = ViewMatrix * BWorld;
    vector4 CView = ViewMatrix * CWorld;

    matrix4 ProjectionMatrix = m4.GetPerspective(
        fov,
        1.0f,
        0.1f,
        100.0f
    );

    // 2. View Space -> Clip Space
    vector4 AClip = ProjectionMatrix * AView;
    vector4 BClip = ProjectionMatrix * BView;
    vector4 CClip = ProjectionMatrix * CView;

    // 3. Perspective Divide: Clip -> NDC
    vector4 ANDC = PerspectiveDivide(AClip);
    vector4 BNDC = PerspectiveDivide(BClip);
    vector4 CNDC = PerspectiveDivide(CClip);

    // 4. NDC -> Screen + Rasterization
    RenderTriangle(ANDC, BNDC, CNDC);
}

void RenderOneFrame(float inFrameTime) {
    // World Space
    vector4 A = { -1.0f, -0.8f, 4.0f, 1.0f };
    vector4 B = { 1.2f, -0.6f, 6.0f, 1.0f };
    vector4 C = { 0.0f,  1.0f, 13.0f, 1.0f };

    DrawTriangle(A, B, C);
	
}

vector4 PerspectiveDivide(const vector4& v)
{
    return vector4(
        v.x / v.w,
        v.y / v.w,
        v.z / v.w,
        1.0f
    );
}