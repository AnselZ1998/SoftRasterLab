#include "Matrix.h"

// =======================
// Matrix4
// =======================

Matrix4::Matrix4()
{
    Set(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
}

Matrix4& Matrix4::Set(
    float m11, float m12, float m13, float m14,
    float m21, float m22, float m23, float m24,
    float m31, float m32, float m33, float m34,
    float m41, float m42, float m43, float m44
)
{
    _data[0] = m11; _data[1] = m12; _data[2] = m13; _data[3] = m14;
    _data[4] = m21; _data[5] = m22; _data[6] = m23; _data[7] = m24;
    _data[8] = m31; _data[9] = m32; _data[10] = m33; _data[11] = m34;
    _data[12] = m41; _data[13] = m42; _data[14] = m43; _data[15] = m44;

    return *this;
}

Matrix4& Matrix4::SetPerspective(float inFOV, float inAspect, float inNear, float inFar)
{
    float tanHalfFOV = std::tan(inFOV * 0.5f);

    float yScale = 1.0f / tanHalfFOV;
    float xScale = yScale / inAspect;

    float n = inNear;
    float f = inFar;

    return Set(
        xScale, 0.0f, 0.0f, 0.0f,
        0.0f, yScale, 0.0f, 0.0f,
        0.0f, 0.0f, (f + n) / (n - f), (2.0f * f * n) / (f - n),
        0.0f, 0.0f, 1.0f, 0.0f
    );
}

Vector4 Matrix4::operator*(const Vector4& inVector) const
{
    return Vector4(
        _data[0] * inVector.x + _data[1] * inVector.y + _data[2] * inVector.z + _data[3] * inVector.w,
        _data[4] * inVector.x + _data[5] * inVector.y + _data[6] * inVector.z + _data[7] * inVector.w,
        _data[8] * inVector.x + _data[9] * inVector.y + _data[10] * inVector.z + _data[11] * inVector.w,
        _data[12] * inVector.x + _data[13] * inVector.y + _data[14] * inVector.z + _data[15] * inVector.w
    );
}

Matrix4 Matrix4::operator*(const Matrix4& rhs) const
{
    Matrix4 result;

    result._11 = _11 * rhs._11 + _12 * rhs._21 + _13 * rhs._31 + _14 * rhs._41;
    result._12 = _11 * rhs._12 + _12 * rhs._22 + _13 * rhs._32 + _14 * rhs._42;
    result._13 = _11 * rhs._13 + _12 * rhs._23 + _13 * rhs._33 + _14 * rhs._43;
    result._14 = _11 * rhs._14 + _12 * rhs._24 + _13 * rhs._34 + _14 * rhs._44;

    result._21 = _21 * rhs._11 + _22 * rhs._21 + _23 * rhs._31 + _24 * rhs._41;
    result._22 = _21 * rhs._12 + _22 * rhs._22 + _23 * rhs._32 + _24 * rhs._42;
    result._23 = _21 * rhs._13 + _22 * rhs._23 + _23 * rhs._33 + _24 * rhs._43;
    result._24 = _21 * rhs._14 + _22 * rhs._24 + _23 * rhs._34 + _24 * rhs._44;

    result._31 = _31 * rhs._11 + _32 * rhs._21 + _33 * rhs._31 + _34 * rhs._41;
    result._32 = _31 * rhs._12 + _32 * rhs._22 + _33 * rhs._32 + _34 * rhs._42;
    result._33 = _31 * rhs._13 + _32 * rhs._23 + _33 * rhs._33 + _34 * rhs._43;
    result._34 = _31 * rhs._14 + _32 * rhs._24 + _33 * rhs._34 + _34 * rhs._44;

    result._41 = _41 * rhs._11 + _42 * rhs._21 + _43 * rhs._31 + _44 * rhs._41;
    result._42 = _41 * rhs._12 + _42 * rhs._22 + _43 * rhs._32 + _44 * rhs._42;
    result._43 = _41 * rhs._13 + _42 * rhs._23 + _43 * rhs._33 + _44 * rhs._43;
    result._44 = _41 * rhs._14 + _42 * rhs._24 + _43 * rhs._34 + _44 * rhs._44;

    return result;
}

Matrix4& Matrix4::SetTranslation(float tx, float ty, float tz)
{
    Set(
        1.0f, 0.0f, 0.0f, tx,
        0.0f, 1.0f, 0.0f, ty,
        0.0f, 0.0f, 1.0f, tz,
        0.0f, 0.0f, 0.0f, 1.0f
    );
    return *this;
}

Matrix4& Matrix4::SetScale(float inX, float inY, float inZ)
{
    Set(
        inX, 0.0f, 0.0f, 0.0f,
        0.0f, inY, 0.0f, 0.0f,
        0.0f, 0.0f, inZ, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
    return *this;
}
