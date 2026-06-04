#pragma once

#include <ostream>

class Vector2
{
public:
    union {
        struct
        {
            float x, y;
        };

        float v[2];
    };

    explicit Vector2(float inValue);
    Vector2(float inX, float inY): x(inX), y(inY) {};
    Vector2(int inX, int inY) : x(float(inX)), y(float(inY)) {};

};

class Vector3
{
public:
    union {
        struct
        {
            float x, y, z;
        };

        float v[3];
    };

public:
    Vector3();
    explicit Vector3(float inValue);
    Vector3(float inX, float inY, float inZ);

    Vector3(const Vector3& inVector) = default;
    Vector3& operator=(const Vector3& inVector) = default;

    float Length() const;
    Vector3 Normalized() const;
    void NormalizeSelf();

    static float Dot(const Vector3& a, const Vector3& b);
    static Vector3 Cross(const Vector3& a, const Vector3& b);

    static Vector3 GetMax(const Vector3& a, const Vector3& b, const Vector3& c);
    static Vector3 GetMin(const Vector3& a, const Vector3& b, const Vector3& c);

    Vector3 operator+(const Vector3& rhs) const;
    Vector3 operator-(const Vector3& rhs) const;
    Vector3 operator*(float scalar) const;

    Vector3& operator+=(const Vector3& rhs);
    Vector3& operator-=(const Vector3& rhs);
    Vector3& operator*=(float scalar);

    float& operator[](int index);
    const float& operator[](int index) const;
};

class Vector4
{
public:
    union {
        struct
        {
            float x, y, z, w;
        };

        float v[4];
    };

public:
    Vector4();
    explicit Vector4(float inValue);
    Vector4(float inX, float inY, float inZ, float inW = 1.0f);
    Vector4(const float* inData);
    Vector4(const Vector3& xyz, float inW = 1.0f);

    Vector4(const Vector4& inVector) = default;
    Vector4& operator=(const Vector4& inVector) = default;

    Vector3 XYZ() const;

    static Vector4 FromPoint(const Vector3& p);
    static Vector4 FromDirection(const Vector3& d);

    static Vector3 GetMax(const Vector4& a, const Vector4& b, const Vector4& c);
    static Vector3 GetMin(const Vector4& a, const Vector4& b, const Vector4& c);

    Vector4 operator+(const Vector4& rhs) const;
    Vector4 operator-(const Vector4& rhs) const;
    Vector4 operator*(float scalar) const;

    Vector4& operator+=(const Vector4& rhs);
    Vector4& operator-=(const Vector4& rhs);
    Vector4& operator*=(float scalar);

    float& operator[](int index);
    const float& operator[](int index) const;

    friend std::ostream& operator<<(std::ostream& stream, const Vector4& value);
};