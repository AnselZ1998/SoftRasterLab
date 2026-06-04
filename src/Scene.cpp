#include "Scene.h"
#include <iostream>
#include "OBJ_Loader.h"

static int gWidth = 0;
static int gHeight = 0;
static objl::Loader gObjLoader;
static bool gModelLoaded = false;
static float gModelScale = 0.01f;
static float gModelOffsetX = 0.0f;
static float gModelOffsetY = 0.0f;

void InitScene(int inViewportWidth, int inViewportHeight) {
	gWidth = inViewportWidth;
	gHeight = inViewportHeight;

    //Load model
    LoadModel();
}

Vector3 ToVector3(const objl::Vector3& p)
{
    return Vector3(p.X, p.Y, p.Z);
}

void LoadModel() {
    // Load model
    gModelLoaded = gObjLoader.LoadFile("model/human_t.obj");

    if (!gModelLoaded)
    {
        std::cout << "Failed to load obj model." << std::endl;
    }
    else
    {
        std::cout << "OBJ loaded. Mesh count: "
            << gObjLoader.LoadedMeshes.size()
            << std::endl;
    }
}

void RenderOneFrame(float inFrameTime) {
    // TODO: ClearScreen();

    // 1. Model Matrix
    Matrix4 scaleMatrix;
    scaleMatrix.SetScale(gModelScale, gModelScale, gModelScale);

    Matrix4 translateMatrix;
    translateMatrix.SetTranslation(gModelOffsetX, gModelOffsetY, 4.0f);

    Matrix4 modelMatrix = translateMatrix * scaleMatrix;

    // 2. View Matrix
    Matrix4 viewMatrix;
    viewMatrix.SetTranslation(0.0f, 0.0f, 10.0f);

    // 3. Projection Matrix
    Matrix4 projectionMatrix;

    float fov = 45.0f * 3.1415926f / 180.0f;
    float aspect = static_cast<float>(gWidth) / static_cast<float>(gHeight);

    projectionMatrix.SetPerspective(
        fov,
        aspect,
        0.1f,
        100.0f
    );

    // 4. MVP
    Matrix4 mvp = projectionMatrix * viewMatrix * modelMatrix;

    // 5. Render
    if (!gModelLoaded)
    {
        return;
    }

    int triangleCount = 0;
    const int maxTriangleCount = 50000;


    for (const objl::Mesh& mesh : gObjLoader.LoadedMeshes)
    {
        const std::vector<objl::Vertex>& vertices = mesh.Vertices;
        const std::vector<unsigned int>& indices = mesh.Indices;

        for (size_t i = 0; i + 2 < indices.size(); i += 3)
        {
            // debug max
            if (triangleCount >= maxTriangleCount)
            {
                return;
            }
            const objl::Vertex& v0 = vertices[indices[i + 0]];
            const objl::Vertex& v1 = vertices[indices[i + 1]];
            const objl::Vertex& v2 = vertices[indices[i + 2]];

            Vector3 A = ToVector3(v0.Position);
            Vector3 B = ToVector3(v1.Position);
            Vector3 C = ToVector3(v2.Position);

            DrawTriangle(A, B, C, mvp);

            triangleCount++;
        }
    }
}

void DrawTriangle(
    const Vector3& SA,
    const Vector3& SB,
    const Vector3& SC,
    const Matrix4& mvp
)
{
    Vector4 SA4 = Vector4::FromPoint(SA);
    Vector4 SB4 = Vector4::FromPoint(SB);
    Vector4 SC4 = Vector4::FromPoint(SC);

    Vector4 AClip = mvp * SA4;
    Vector4 BClip = mvp * SB4;
    Vector4 CClip = mvp * SC4;

    Vector4 ANDC = PerspectiveDivide(AClip);
    Vector4 BNDC = PerspectiveDivide(BClip);
    Vector4 CNDC = PerspectiveDivide(CClip);

    RenderTriangle(ANDC, BNDC, CNDC);
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

Vector4 PerspectiveDivide(const Vector4& v)
{
    return Vector4(
        v.x / v.w,
        v.y / v.w,
        v.z / v.w,
        1.0f
    );
}

void OnMouseWheel(int wheelDelta)
{
    if (wheelDelta > 0)
    {
        gModelScale *= 1.1f;
    }
    else
    {
        gModelScale *= 0.9f;
    }

    if (gModelScale < 0.0001f)
    {
        gModelScale = 0.0001f;
    }

    if (gModelScale > 10.0f)
    {
        gModelScale = 10.0f;
    }
}

void OnMouseDrag(int dx, int dy)
{
    float moveSpeed = 0.01f;

    gModelOffsetX += dx * moveSpeed;
    gModelOffsetY -= dy * moveSpeed;
}