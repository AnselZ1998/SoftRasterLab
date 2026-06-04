#pragma once
#include "Vector.h"
#include <cmath>

class Matrix4
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

	Matrix4();

	Matrix4& SetPerspective(float inFOV, float inAspect, float inNear, float inFar);
	Matrix4& SetScale(float inX, float inY, float inZ);
	Matrix4& SetTranslation(float tx, float ty, float tz);

	Matrix4& Set(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44
	);

	Vector4 operator*(const Vector4& inVector) const;

	Matrix4 operator*(const Matrix4& rhs) const;
};


