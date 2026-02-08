#pragma once
#include <DirectXMath.h>

// 4成分ベクトル
class Vec4 
{
public:
    Vec4();
    ~Vec4();
    Vec4(float x, float y, float z, float w);

    Vec4 operator+(const Vec4& rhs) const;
    Vec4 operator-(const Vec4& rhs) const;
    Vec4 operator*(float s) const;
    
    float X() const;
    float Y() const;
    float Z() const;
    float W() const;
    float Length() const;
    Vec4 Normalize() const;
    Vec4 MultiplyQuaternion(const Vec4& rhs) const;
    Vec3 RotateVec3(const Vec3& rhs) const;
    void ToFloat4(float out[4]) const;

private:
    Vec4(const DirectX::XMVECTOR& v);
    DirectX::XMVECTOR vec;
};