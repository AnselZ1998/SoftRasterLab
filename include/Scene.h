#pragma once
#include "vector4.h"
#include "BoundingVolume.h"
#include "matrix4.h"
#include <iostream>

static void TestVector4();
void InitScene(int inViewportWidth, int inViewportHeight);
void RenderOneFrame(float inFrameTime = 0.0f);
void SetPixelColor(int inX, int inY, unsigned char inR, unsigned char inG, unsigned char inB, unsigned char inA = 255);
bool IsInTriangle(int inX, int inY, const vector4& inA, const vector4& inB, const vector4& inC);
void RenderTriangle(const vector4& A, const vector4& B, const vector4& C);
void DrawTriangle(const vector4& A, const vector4& B, const vector4& C);
vector4 PerspectiveDivide(const vector4& v);

struct Vector2 {
    float x;
    float y;
};