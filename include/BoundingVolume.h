#pragma once

#include"vector4.h"

class BoundingVolume
{
public:
    float minX;
    float minY;
    float maxX;
    float maxY;

	void GetBoundingVolume(const vector4& inVectorPa, const vector4& inVectorPb, const vector4& inVectorPc);

};
