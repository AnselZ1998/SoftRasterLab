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
   
}

bool IsInTriangle(int inX, int inY, const Vector4& inA, const Vector4& inB, const Vector4& inC) {
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

void RenderTriangle(const Vector4& A, const Vector4& B, const Vector4& C)
{
    Vector4 ASS(A), BSS(B), CSS(C);
    
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

void DrawTriangle(const Vector3& SA, const Vector3& SB, const Vector3& SC) {
    // vec3 -> vec4
    Vector4 SA4 = Vector4::FromPoint(SA);
    Vector4 SB4 = Vector4::FromPoint(SB);
    Vector4 SC4 = Vector4::FromPoint(SC);

    // camera
    Vector4 cameraPos = { 0.0f, 0.0f, -10.0f, 1.0f };
    float fov = 45.0f * 3.1415926f / 180.0f;

    // View Matrix
    Matrix4 m4View;
    m4View.SetTranslation(
        -cameraPos.x,
        -cameraPos.y,
        -cameraPos.z
    );

    // 1. World Space -> View Space
    Vector4 AView = m4View * SA4;
    Vector4 BView = m4View * SB4;
    Vector4 CView = m4View * SC4;

    // Projection Matrix
    Matrix4 m4Pro;
    m4Pro.SetPerspective(
        fov,
        1.0f,
        0.1f,
        100.0f
    );

    // 2. View Space -> Clip Space
    Vector4 AClip = m4Pro * AView;
    Vector4 BClip = m4Pro * BView;
    Vector4 CClip = m4Pro * CView;

    // 3. Perspective Divide: Clip -> NDC
    Vector4 ANDC = PerspectiveDivide(AClip);
    Vector4 BNDC = PerspectiveDivide(BClip);
    Vector4 CNDC = PerspectiveDivide(CClip);

    // 4. NDC -> Screen + Rasterization
    RenderTriangle(ANDC, BNDC, CNDC);
}

void RenderOneFrame(float inFrameTime) {
    // Origin Triangle Point
    Vector3 SA = { -1.0f, -0.8f, 4.0f};
    Vector3 SB = { 1.2f, -0.6f, 6.0f};
    Vector3 SC = { 0.0f,  1.0f, 13.0f};

    DrawTriangle(SA, SB, SC);
}

Vector4 PerspectiveDivide(const Vector4& v)
{
    return Vector4(
        v.x / v.w,
        v.y / v.w,
        v.z / v.w,
        1.0f
    );
}