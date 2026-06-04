#include "matrix4.h"

matrix4::matrix4()
{
    Set(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
}

matrix4& matrix4::Set(
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

matrix4& matrix4::GetPerspective(float inFOV, float inAspect, float inNear, float inFar)
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

vector4 matrix4::operator*(const vector4& inVector) const
{
    return vector4(
        _data[0] * inVector.x + _data[1] * inVector.y + _data[2] * inVector.z + _data[3] * inVector.w,
        _data[4] * inVector.x + _data[5] * inVector.y + _data[6] * inVector.z + _data[7] * inVector.w,
        _data[8] * inVector.x + _data[9] * inVector.y + _data[10] * inVector.z + _data[11] * inVector.w,
        _data[12] * inVector.x + _data[13] * inVector.y + _data[14] * inVector.z + _data[15] * inVector.w
    );
}

matrix4 matrix4::Translation(float tx, float ty, float tz)
{
    matrix4 result;

    result.Set(
        1.0f, 0.0f, 0.0f, tx,
        0.0f, 1.0f, 0.0f, ty,
        0.0f, 0.0f, 1.0f, tz,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    return result;
}

void matrix4::Tranlate(float inX, float inY, float inZ) {
    _41 = inX;
    _42 = inY;
    _43 = inZ;
}

void matrix4::SetScale(float inX, float inY, float inZ)
{
    _11 = inX;
    _22 = inY;
    _33 = inZ;
}
