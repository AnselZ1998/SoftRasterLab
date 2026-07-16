#include "Camera.h"

Camera::Camera()
{
	// degress to radians
	fovYDegrees = PI * fovYDegrees / 180.0f;
	viewMatrix = Matrix4::LookAtRH(position, target, up);
	projectionMatrix = Matrix4::PerspectiveRH(fovYDegrees, aspect, nearZ, farZ);
}