#pragma once

#include"Vector.h"

class BoundingVolume
{
public:
    float minX;
    float minY;
    float maxX;
    float maxY;

	void GetBoundingVolume(const Vector4& inVectorPa, const Vector4& inVectorPb, const Vector4& inVectorPc);

};
