#pragma once

#include "vector4.h"
#include <cmath>

class matrix4
{
public:
	union {
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float _data[16];
	};

	matrix4();

	static matrix4 Translation(float tx, float ty, float tz);

	matrix4& Set(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44
	);

	matrix4& GetPerspective(float inFOV, float inAspect, float inNear, float inFar);
	void Tranlate(float inX, float inY, float inZ);
	void SetScale(float inX, float inY, float inZ);

	vector4 operator*(const vector4& inVector) const;

	/*string operator<<(const matrix4& inMatrix) const;*/
};


