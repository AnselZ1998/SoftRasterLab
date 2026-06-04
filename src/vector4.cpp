#include "vector4.h"

float& vector4::operator[](int inIndex) {
	return v[inIndex];
}

float vector4::Length3() const
{
	return std::sqrt(x * x + y * y + z * z);
}

vector4 vector4::GetMax(const vector4& inVectorPa, const vector4& inVectorPb, const vector4& inVectorPc)
{
	float PxMax = std::max(inVectorPa.x, std::max(inVectorPb.x, inVectorPc.x));
	float PyMax = std::max(inVectorPa.y, std::max(inVectorPb.y, inVectorPc.y));
	float PzMax = std::max(inVectorPa.z, std::max(inVectorPb.z, inVectorPc.z));
	return vector4(PxMax, PyMax, PzMax);
}

vector4 vector4::GetMin(const vector4& inVectorPa, const vector4& inVectorPb, const vector4& inVectorPc)
{
	float PxMin = std::min(inVectorPa.x, std::min(inVectorPb.x, inVectorPc.x));
	float PyMin = std::min(inVectorPa.y, std::min(inVectorPb.y, inVectorPc.y));
	float PzMin = std::min(inVectorPa.z, std::min(inVectorPb.z, inVectorPc.z));
	return vector4(PxMin, PyMin, PzMin);
}

float vector4::Dot3(const vector4& a, const vector4& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

vector4 vector4::Cross3(const vector4& a, const vector4& b)
{
	return vector4(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x,
		0.0f
	);
}

vector4 vector4::Normalize3(const vector4& inVector)
{
	float length = inVector.Length3();
	if (length <= 0.000001f)
	{
		return vector4(0.0f, 0.0f, 0.0f, inVector.w);
	}
	else
	{
		return vector4(inVector.x / length, inVector.y / length, inVector.z / length, inVector.w);
	}
	
}

vector4 vector4::operator+(const vector4& inVector)
{
	return vector4(
		x + inVector.x,
		y + inVector.y,
		z + inVector.z,
		w + inVector.w);
}

vector4 vector4::operator-(const vector4& inVector)
{
	return vector4(
		x - inVector.x,
		y - inVector.y,
		z - inVector.z,
		w - inVector.w);
}

vector4 vector4::operator*(const float& inScalar)
{
	return vector4(
		x * inScalar,
		y * inScalar,
		z * inScalar,
		w * inScalar
	);
}