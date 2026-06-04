#include "Vector.h"

#include <algorithm>
#include <cassert>
#include <cmath>

// =======================
// Vector2
// =======================


// =======================
// Vector3
// =======================

Vector3::Vector3()
    : x(0.0f), y(0.0f), z(0.0f)
{
}

Vector3::Vector3(float inValue)
    : x(inValue), y(inValue), z(inValue)
{
}

Vector3::Vector3(float inX, float inY, float inZ)
    : x(inX), y(inY), z(inZ)
{
}

float Vector3::Length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::Normalized() const
{
    float length = Length();

    if (length <= 0.000001f)
    {
        return Vector3(0.0f, 0.0f, 0.0f);
    }

    return Vector3(
        x / length,
        y / length,
        z / length
    );
}

void Vector3::NormalizeSelf()
{
    float length = Length();

    if (length <= 0.000001f)
    {
        return;
    }

    x /= length;
    y /= length;
    z /= length;
}

float Vector3::Dot(const Vector3& a, const Vector3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)
{
    return Vector3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

Vector3 Vector3::GetMax(const Vector3& a, const Vector3& b, const Vector3& c)
{
    return Vector3(
        std::max(a.x, std::max(b.x, c.x)),
        std::max(a.y, std::max(b.y, c.y)),
        std::max(a.z, std::max(b.z, c.z))
    );
}

Vector3 Vector3::GetMin(const Vector3& a, const Vector3& b, const Vector3& c)
{
    return Vector3(
        std::min(a.x, std::min(b.x, c.x)),
        std::min(a.y, std::min(b.y, c.y)),
        std::min(a.z, std::min(b.z, c.z))
    );
}

Vector3 Vector3::operator+(const Vector3& rhs) const
{
    return Vector3(
        x + rhs.x,
        y + rhs.y,
        z + rhs.z
    );
}

Vector3 Vector3::operator-(const Vector3& rhs) const
{
    return Vector3(
        x - rhs.x,
        y - rhs.y,
        z - rhs.z
    );
}

Vector3 Vector3::operator*(float scalar) const
{
    return Vector3(
        x * scalar,
        y * scalar,
        z * scalar
    );
}

Vector3& Vector3::operator+=(const Vector3& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;

    return *this;
}

Vector3& Vector3::operator-=(const Vector3& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;

    return *this;
}

Vector3& Vector3::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;

    return *this;
}

float& Vector3::operator[](int index)
{
    assert(index >= 0 && index < 3);
    return v[index];
}

const float& Vector3::operator[](int index) const
{
    assert(index >= 0 && index < 3);
    return v[index];
}

// =======================
// Vector4
// =======================

Vector4::Vector4()
    : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{
}

Vector4::Vector4(float inValue)
    : x(inValue), y(inValue), z(inValue), w(inValue)
{
}

Vector4::Vector4(float inX, float inY, float inZ, float inW)
    : x(inX), y(inY), z(inZ), w(inW)
{
}

Vector4::Vector4(const float* inData)
    : x(inData[0]), y(inData[1]), z(inData[2]), w(inData[3])
{
}

Vector4::Vector4(const Vector3& xyz, float inW)
    : x(xyz.x), y(xyz.y), z(xyz.z), w(inW)
{
}

Vector3 Vector4::XYZ() const
{
    return Vector3(x, y, z);
}

Vector4 Vector4::FromPoint(const Vector3& p)
{
    return Vector4(p.x, p.y, p.z, 1.0f);
}

Vector4 Vector4::FromDirection(const Vector3& d)
{
    return Vector4(d.x, d.y, d.z, 0.0f);
}

Vector3 Vector4::GetMax(const Vector4& a, const Vector4& b, const Vector4& c)
{
    return Vector3(
        std::max(a.x, std::max(b.x, c.x)),
        std::max(a.y, std::max(b.y, c.y)),
        std::max(a.z, std::max(b.z, c.z))
    );
}

Vector3 Vector4::GetMin(const Vector4& a, const Vector4& b, const Vector4& c)
{
    return Vector3(
        std::min(a.x, std::min(b.x, c.x)),
        std::min(a.y, std::min(b.y, c.y)),
        std::min(a.z, std::min(b.z, c.z))
    );
}

Vector4 Vector4::operator+(const Vector4& rhs) const
{
    return Vector4(
        x + rhs.x,
        y + rhs.y,
        z + rhs.z,
        w + rhs.w
    );
}

Vector4 Vector4::operator-(const Vector4& rhs) const
{
    return Vector4(
        x - rhs.x,
        y - rhs.y,
        z - rhs.z,
        w - rhs.w
    );
}

Vector4 Vector4::operator*(float scalar) const
{
    return Vector4(
        x * scalar,
        y * scalar,
        z * scalar,
        w * scalar
    );
}

Vector4& Vector4::operator+=(const Vector4& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;

    return *this;
}

Vector4& Vector4::operator-=(const Vector4& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;

    return *this;
}

Vector4& Vector4::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;

    return *this;
}

float& Vector4::operator[](int index)
{
    assert(index >= 0 && index < 4);
    return v[index];
}

const float& Vector4::operator[](int index) const
{
    assert(index >= 0 && index < 4);
    return v[index];
}

std::ostream& operator<<(std::ostream& stream, const Vector4& value)
{
    stream << value.x << " "
        << value.y << " "
        << value.z << " "
        << value.w;

    return stream;
}