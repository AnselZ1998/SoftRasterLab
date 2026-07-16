#pragma once

#include "Vector.h"
#include "Matrix.h"

static const float PI = 3.1415926535f;

struct Camera
{
	Camera();

	Matrix4 viewMatrix;
	Matrix4 projectionMatrix;
	
	// input degrees
	float fovYDegrees = 60.0;
	// temp input and we need
	float aspect = 1.77;
	float nearZ = 0.1f;
	float farZ = 100.0f;
	
	Vector3 position = Vector3(0.0, 0.0, 3.0);
	Vector3 target = Vector3(0.0, 0.0, 0.0);
	Vector3 up = Vector3(0.0, 1.0, 0.0);

};

