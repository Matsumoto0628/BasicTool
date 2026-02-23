#pragma once
#include <DirectXMath.h>

// 2成分ベクトル
class Vec2 
{
public:
    Vec2();
    ~Vec2();
    Vec2(float x, float y);

    const Vec2 operator+(const Vec2& rhs) const;
    const Vec2 operator-(const Vec2& rhs) const;
    const Vec2 operator*(float s) const;

    float X() const;
    float Y() const;
    float Length() const;
    Vec2 Normalize() const;
    void ToFloat2(float out[2]) const;

private:
    Vec2(const DirectX::XMVECTOR& v);
    DirectX::XMVECTOR vec;
};