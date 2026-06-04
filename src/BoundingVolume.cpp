#include "BoundingVolume.h"

void BoundingVolume::GetBoundingVolume(const vector4& inVectorPa, const vector4& inVectorPb, const vector4& inVectorPc)
{
	vector4 min = vector4::GetMin(inVectorPa, inVectorPb, inVectorPc);
	vector4 max = vector4::GetMax(inVectorPa, inVectorPb, inVectorPc);
	minX = min.x;
	minY = min.y;
	maxX = max.x;
	maxY = max.y;
}