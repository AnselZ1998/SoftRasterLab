#pragma once

#include <iostream>
#include <algorithm>

class vector4
{
private:
    
public:
    union{
        struct 
        {
            float x, y, z, w;
        };
        float v[4];
    };

    vector4(const float inValue) 
        :x(inValue), y(inValue), z(inValue), w(inValue) {};
    vector4(float inX, float inY, float inZ, float inW = 1.0f) 
        :x(inX), y(inY), z(inZ), w(inW) {};
    vector4(const float* inData) 
        :x(inData[0]), y(inData[1]), z(inData[2]), w(inData[3]) {};
    vector4(const vector4 &inVector)
        :x(inVector.x), y(inVector.y), z(inVector.z), w(inVector.w) {};

    float Length3() const;

    static vector4 GetMax(const vector4& inVectorPa, const vector4& inVectorPb, const vector4& inVectorPc);
    static vector4 GetMin(const vector4& inVectorPa, const vector4& inVectorPb, const vector4& inVectorPc);
    static float Dot3(const vector4& a, const vector4& b);
    static vector4 Cross3(const vector4& a, const vector4& b);
    static vector4 Normalize3(const vector4& v);
    
    vector4 operator+(const vector4& inVector);
    vector4 operator-(const vector4& inVector);
    vector4 operator*(const float& inScalar);
    vector4& operator=(const vector4& inVector) = default;

    float& operator[](int inIndex);
    
};
