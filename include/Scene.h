#pragma once
#include "Vector.h"
#include "BoundingVolume.h"
#include "Matrix.h"
#include <iostream>

void OnMouseWheel(int wheelDelta);
void OnMouseDrag(int dx, int dy);
void LoadModel();
void InitScene(int inViewportWidth, int inViewportHeight);
void RenderOneFrame(float inFrameTime = 0.0f);
void SetPixelColor(int inX, int inY, unsigned char inR, unsigned char inG, unsigned char inB, unsigned char inA = 255);
bool IsInTriangle(int inX, int inY, const Vector4& inA, const Vector4& inB, const Vector4& inC);
void RenderTriangle(const Vector4& A, const Vector4& B, const Vector4& C);
void DrawTriangle(const Vector3& SA, const Vector3& SB, const Vector3& SC, const Matrix4& mvp);
Vector4 PerspectiveDivide(const Vector4& v);